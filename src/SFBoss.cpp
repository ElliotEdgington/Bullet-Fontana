#include "SFBoss.h"


SFBoss::SFBoss(shared_ptr<SFWindow> window,shared_ptr<SFPattern> pattern_get, shared_ptr<SFPlayer> player_get, int h) :
                    SFAsset(window), w_height(h), pattern(pattern_get), player(player_get),
                    phase(0), sub_phase(0), p_dir(0){
  health = 100;
  move_speed = 1.0f;
  sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien_boss.png");
  LoadSprite();

}

SFBoss::~SFBoss(){}


list <shared_ptr<SFProjectile>> SFBoss::UpdateBoss(){
  list<shared_ptr<SFProjectile>> bullethell;
  phase_timer++;
  //get boss on screen
  if(GetPosition().getY() > w_height - sprite_height - 100) {
    GoSouth();
    phase_timer = 0;
  }

  switch(phase){
    case 1:
      bullethell.splice(bullethell.end(),Phase1());
      break;
    case 2:
      bullethell.splice(bullethell.end(),Phase2());
      break;
    case 3:
      bullethell.splice(bullethell.end(),Phase3());
      break;
  }
  return bullethell;
}

int SFBoss::GetHealth(){
  return health;
}


//sets health, and resets alive
void SFBoss::SetHealth(int h){
  health = h;
  alive = true;
}

//sets boss phase
void SFBoss::SetPhase(int p){
  phase = p;
}


//each phase the boss creates different bullet patterns.
//phase increments after each boss kill.

list <shared_ptr<SFProjectile>> SFBoss::Phase1(){
  list<shared_ptr<SFProjectile>> bullets;
  //every 1 seconds
  if(phase_timer%(60*1) == 0){
    bullets.splice(bullets.end(),pattern->FireAt(GetPosition(),player->GetPosition()));
  }
  //every 4 seconds
  if(phase_timer%(60*4) == 0){
    bullets.splice(bullets.end(),pattern->CreateExplosion(GetPosition(), 0));
  }
  //every 4 seconds starting at 2s
  if(phase_timer%(60*4) == 2*60){
    bullets.splice(bullets.end(),pattern->CreateExplosion(GetPosition(), 23));
  }
  return bullets;
}

list <shared_ptr<SFProjectile>> SFBoss::Phase2(){
  list<shared_ptr<SFProjectile>> bullets;
  //every 2 seconds
  if(phase_timer%(60*2) == 0){
    bullets.splice(bullets.end(),pattern->SprayAt(GetPosition(),player->GetPosition()));
  }
  //every half second
  if(phase_timer%(30*1) == 0){
    bullets.splice(bullets.end(),pattern->FireAngle(GetPosition(),p_dir));
    bullets.splice(bullets.end(),pattern->FireAngle(GetPosition(),p_dir+180));
    bullets.splice(bullets.end(),pattern->FireAngle(GetPosition(),p_dir+90));
    bullets.splice(bullets.end(),pattern->FireAngle(GetPosition(),p_dir+270));
    AddRotation(20);
  }
  //every 4 seconds starting at 2s
  if(phase_timer%(60*8) == 4*60){
    bullets.splice(bullets.end(),pattern->CreateExplosion(GetPosition(),0));
  }
  return bullets;
}
list <shared_ptr<SFProjectile>> SFBoss::Phase3(){
  list<shared_ptr<SFProjectile>> bullets;
  //every second
  if(phase_timer%(60*1) == 0){
    bullets.splice(bullets.end(),pattern->CreateExplosion(GetPosition(),p_dir));
    AddRotation(30);
  }
  //
  if(phase_timer%(45*2) == 0){
    bullets.splice(bullets.end(),pattern->CreateExplosion(GetPosition(),0));
  }
  //
  if(phase_timer%(45*2) == 0){
    bullets.splice(bullets.end(),pattern->SprayAt(GetPosition(),player->GetPosition()));
  }

  return bullets;
}

//used for the rotating bullets during the boss phase.
void SFBoss::AddRotation(int a){
  p_dir += a;
  if(p_dir >= 360) p_dir -=360;
}
