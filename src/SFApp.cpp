#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) :
 						fire(0), is_running(true), sf_window(window),
						P_UP(false), P_DOWN(false), P_LEFT(false),
						P_RIGHT(false), P_SHOOT(false), P_FIRE(false),
						fire_timer(20),power_timer(20), score(0),
						gun_power(false), wall_power(2), wave(0), bossWave(false)
						{

  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);
	//making application bounding box
  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);
	//make level creator
	wave_get = make_shared<SFWave>(sf_window,canvas_w,canvas_h);
  //make pattern creator
  pattern_get = make_shared<SFPattern>(sf_window);

  //creating the player.
	player  = make_shared<SFPlayer>(sf_window);
  auto player_pos = Point2(canvas_w/2, 22);
  player->SetPosition(player_pos);


	//creating the scoreText
	scoreText = make_shared<SFGUI>(SFGUI_TEXTBOX,sf_window);
	AddToScore(0);
	auto scoreText_pos = Point2((canvas_w+50-scoreText->GetWidth()),canvas_h - 20);
	scoreText->SetPosition(scoreText_pos);
  GUI.push_back(scoreText);

  //create healthText
  healthText = make_shared<SFGUI>(SFGUI_TEXTBOX,sf_window);
  healthText->SetText("Health : 100%");
  auto healthText_pos = Point2(10+(healthText->GetWidth()/2), canvas_h - 20);
  healthText->SetPosition(healthText_pos);
  GUI.push_back(healthText);

  //create wallsText
  wallsText = make_shared<SFGUI>(SFGUI_TEXTBOX,sf_window);
  wallsText->SetText("Walls : 2");
  auto wallsText_pos = Point2(20+healthText->GetWidth()*1.5,canvas_h - 20);
  wallsText->SetPosition(wallsText_pos);
  GUI.push_back(wallsText);



  //create boss, reposition it when it gets to the boss stage
  boss = make_shared<SFBoss>(sf_window,pattern_get,player,canvas_h);
  auto boss_pos = Point2(canvas_w/2,canvas_h+15);
  boss->SetPosition(boss_pos);
  //same with boss health, because its a unique entity might aswell make it up here.
  bossHealth = make_shared<SFGUI>(SFGUI_BAR,sf_window);
  auto bossHealth_pos = Point2(canvas_w/2,canvas_h-50);
  bossHealth->SetWidth(boss->GetHealth()*4);
  bossHealth->SetPosition(bossHealth_pos);

  //starts the waves up
  CheckWave();
}

SFApp::~SFApp() {}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;

  //Movement code
  //Using booleans to make the character move smoothly across the screen.
  //Movement is now updated in OnUpdateWorld()
  case SFEVENT_PLAYER_UP_KEYDOWN:
    P_UP = true;
    break;
  case SFEVENT_PLAYER_UP_KEYUP:
    P_UP = false;
    break;
  case SFEVENT_PLAYER_DOWN_KEYDOWN:
    P_DOWN = true;
    break;
	case SFEVENT_PLAYER_DOWN_KEYUP:
	  P_DOWN = false;
	  break;
  case SFEVENT_PLAYER_LEFT_KEYDOWN:
    P_LEFT = true;
    break;
	case SFEVENT_PLAYER_LEFT_KEYUP:
	  P_LEFT = false;
	  break;
  case SFEVENT_PLAYER_RIGHT_KEYDOWN:
		P_RIGHT = true;
    break;
	case SFEVENT_PLAYER_RIGHT_KEYUP:
		P_RIGHT = false;
		break;
  case SFEVENT_FIRE_KEYDOWN:
		P_FIRE = true;
    break;
	case SFEVENT_FIRE_KEYUP:
		P_FIRE = false;
		break;
	case SFEVENT_WALL:
		PlaceWall();
		break;
  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // wrap an SDL_Event with our SFEvent
    SFEvent sfevent((const SDL_Event) event);
    // handle our SFEvent
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {
  //Have to make sure player is alive because despawns after death but can still move.
  if (player->IsAlive()) PlayerMovement();

  //cheats uncomment and build if youre stuck.
  //player->AddHealth(100);

	//increment timers;
  //fire_timer is used to create a delay while firing
	fire_timer++;
	//12 second power duration
	if(gun_power && power_timer < 60*12){
		power_timer++;
	}else{
		gun_power = false;
		power_timer = 0;
	}

  // Update entity positions
  for(auto p: e_projectiles) {
    p->UpdateMovement();
  }

	for(auto p: p_projectiles) {
		p->UpdateMovement();
	}


  for(auto c: power_ups) {
    c->GoSouth();
  }

  // Update enemy positions
  for(auto a : aliens) {
    a->UpdateMovement();
  }

  //Update boss on boss wave
  if(bossWave) {
    AddProjectiles(boss->UpdateBoss());
    if(rand()%(8*60) == 1) DropPowerUp(boss->GetPosition(),RandomPowerUp(3));
  }

  // Detect collisions
  for(auto p : p_projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
        AddToScore(25);
      }
    }
    if(bossWave){
      if(p->CollidesWith(boss)){
        p->HandleCollision();
        boss->HandleCollision();
        bossHealth->SetWidth(bossHealth->GetWidth()-4);
        AddToScore(35);
      }
    }

	}
	//enemy projectile -> player / wall collision
	for(auto p : e_projectiles) {
		if(p->CollidesWith(player)){
      p->HandleCollision();
      player->AddHealth(-5);
      DisplayStatus();
    }
		//wall collision /w enemy projectile
		for(auto w : walls){
			if(p->CollidesWith(w)){
				p->HandleCollision();
				w->HandleCollision();
			}
		}
  }

	//Power_Up -> player collision;
	for(auto c : power_ups){
		if(c->CollidesWith(player)){
			c->HandleCollision();
		}
	}

  // alien -> player collision
  // alien -> wall collision
  for(auto a : aliens){
    if(a->CollidesWith(player)){
      a->SetNotAlive();
      player->AddHealth(-20);
      DisplayStatus();
    }
    for(auto w : walls){
      if(a->CollidesWith(w)){
        a->SetNotAlive();
        w->HandleCollision();
      }
    }
  }


  //remove dead player
  if(!player->IsAlive()){
    LoseGame();
  }


  //remove dead boss and win Game!!!
  if(bossWave){
    if(!boss->IsAlive()){
      AddToScore(10000);
      wave++;
      DropPowerUp(boss->GetPosition(),POWER_WALL);
      CheckWave();
      if(wave == 9){
        bossWave = false;
        boss->SetPhase(0);
        WinGame();
      }
    }
  }


  // remove dead aliens (the long way)
	// Also add score on death / death projectiles
  // This is also my work around for making enemies fire.
  // The enemies now fire on a random timer, meaning that every game is different!!
  // (and really unfair).
  list<shared_ptr<SFBasic_Enemy>> tmp_a;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp_a.push_back(a);
      switch(a->GetType()){
        case E_WAVE:
          if(rand()%600 == 1) AddProjectiles(pattern_get->FireAt(a->GetPosition(),player->GetPosition()));
          break;
        case E_LARGE:
          if(rand()%300 == 1) AddProjectiles(pattern_get->SprayAt(a->GetPosition(),player->GetPosition()));
          break;
      }
    }else if(a->GetKilled()){
      CheckWave();
			switch(a->GetType()){
				case E_STRAIGHT:
					AddToScore(500);
          AddProjectiles(pattern_get->FireAngle(a->GetPosition(),rand()%360));
          if(rand()%10 == 1) DropPowerUp(a->GetPosition(),POWER_HEALTH);
					break;
        case E_STRAIGHT_SLOW:
          AddToScore(625);
          AddProjectiles(pattern_get->FireAt(a->GetPosition(),player->GetPosition()));
          DropPowerUp(a->GetPosition(),RandomPowerUp(30));
          break;
				case E_WAVE:
					AddToScore(1000);
          AddProjectiles(pattern_get->SprayAt(a->GetPosition(),player->GetPosition()));
          DropPowerUp(a->GetPosition(),RandomPowerUp(15));
					break;
				case E_DIAGONAL_LEFT:
				case E_DIAGONAL_RIGHT:
					AddToScore(750);
          DropPowerUp(a->GetPosition(),RandomPowerUp(12));
					break;
        case E_LARGE:
          AddToScore(1500);
          AddProjectiles(pattern_get->CreateExplosion(a->GetPosition(), 0));
          DropPowerUp(a->GetPosition(),RandomPowerUp(3));
          break;
			}
		}else{ CheckWave(); }
  }
  aliens.clear();
  aliens = list<shared_ptr<SFBasic_Enemy>>(tmp_a);

	// remove used power_ups
	// also addscore / give player powerup.
	list<shared_ptr<SFPower_Up>> tmp_c;
	for(auto c : power_ups) {
		if(c->IsAlive()) {
			tmp_c.push_back(c);
		}else{
			switch(c->GetType()){
				case POWER_HEALTH:
					player->AddHealth(10);
          DisplayStatus();
					break;
				case POWER_WALL:
					wall_power +=1;
          DisplayStatus();
					break;
				case POWER_BULLETS:
					gun_power = true;
					break;
			}
		}
	}
	power_ups.clear();
	power_ups = list<shared_ptr<SFPower_Up>>(tmp_c);




	//remove dead walls
	list<shared_ptr<SFWall>> tmp_w;
	for(auto w : walls) {
		if(w->IsAlive()) tmp_w.push_back(w);
	}
	walls.clear();
	walls = list<shared_ptr<SFWall>>(tmp_w);

	//remove projectiles that have collided
	list<shared_ptr<SFProjectile>> tmp_p;
  for(auto p : e_projectiles) {
    if(p->IsAlive()) {
      tmp_p.push_back(p);
    }
  }
  e_projectiles.clear();
  e_projectiles = list<shared_ptr<SFProjectile>>(tmp_p);

	tmp_p.clear();
	for(auto p : p_projectiles) {
		if(p->IsAlive()) {
			tmp_p.push_back(p);
		}
	}
	p_projectiles.clear();
	p_projectiles = list<shared_ptr<SFProjectile>>(tmp_p);

}

//renders from back layer to front layer
void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

	//SINGLE Entities (not in list)
  // draw the player
  if(player->IsAlive()) player->OnRender();

  if(bossWave && boss->IsAlive()){
     boss->OnRender();
     bossHealth->OnRender();
   }

	//LIST Entities (in list)
  for(auto w: walls){
    w->OnRender();
  }

  for(auto p: p_projectiles) {
		if(p->IsAlive()) {p->OnRender();}
	}

  for(auto p: e_projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }

  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
  }

  for(auto c: power_ups) {
    c->OnRender();
  }

  for(auto g: GUI){
    g->OnRender();
  }

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::PlayerMovement(){
	// priority on up
  // This also doubles as my collision detection
	if(P_UP){
    player->GoNorth();
		if(HitWall()) player->GoSouth();
  }else if(P_DOWN){
		player->GoSouth();
		if(HitWall()) player->GoNorth();
	}
	//priority on right
	if(P_RIGHT){
		player->GoEast();
		if(HitWall()) player->GoWest();
	}else if(P_LEFT){
		player->GoWest();
		if(HitWall()) player->GoEast();
	}

	if(P_FIRE){
		//every 8/60 second
		if(fire_timer > 8){
			fire ++;
			FireProjectile();
			fire_timer = 0;
		}
	}

}

//fires 2 projectiles instead of 1
void SFApp::FireProjectile() {
	float playerX  = player->GetPosition().getX();
	float playerY = player->GetPosition().getY();

  auto pb1 = make_shared<SFProjectile>(P_PLAYER,sf_window);
	auto pb2 = make_shared<SFProjectile>(P_PLAYER,sf_window);

	Point2 bulletPos1 = Point2(playerX+20.0f,playerY+18.0f);
	Point2 bulletPos2 = Point2(playerX-20.0f,playerY+18.0f);
  pb1->SetPosition(bulletPos1);
	pb2->SetPosition(bulletPos2);
  p_projectiles.push_back(pb1);
	p_projectiles.push_back(pb2);
	//If under gun_power mode, add more projectiles!
	if(gun_power){
		auto pb3 = make_shared<SFProjectile>(P_PLAYER,sf_window);
		auto pb4 = make_shared<SFProjectile>(P_PLAYER,sf_window);
		Point2 bulletPos3 = Point2(playerX+10.0f,playerY+18.0f);
		Point2 bulletPos4 = Point2(playerX-10.0f,playerY+18.0f);
	  pb3->SetPosition(bulletPos3);
		pb4->SetPosition(bulletPos4);
	  p_projectiles.push_back(pb3);
		p_projectiles.push_back(pb4);
	}
}

// limit this to a powerup to place limited walls
void SFApp::PlaceWall() {
	if(wall_power > 0){
		auto wall = make_shared<SFWall>(sf_window);
		auto pos_wall = Point2(player->GetPosition().getX(),player->GetPosition().getY()+50);
		wall->SetPosition(pos_wall);
		walls.push_back(wall);
		wall_power--;
    DisplayStatus();
	}
}


// tests if player hits wall. for collision.
bool SFApp::HitWall(){
	for(auto w : walls){
		if(player->CollidesWith(w)){
			return true;
		}
	}
	return false;
}

// display to GUI functions
void SFApp::AddToScore(int s){
	score += s;
	string text = to_string(score);
	string zeros("");
	for(int i = text.size(); i < 8 ; i++){
		zeros += "0";
	}
	scoreText->SetText(zeros+text);
}

// Resets Health and walls putting new values on screen.
void SFApp::DisplayStatus(){
  healthText->SetText("Health : " + to_string(player->GetHealth()) + "%");
  wallsText->SetText("Walls : " + to_string(wall_power));
}

//dont have another param here because player doesnt have
//cool projectile patterns so only need to append e_projectiles
void SFApp::AddProjectiles(list<shared_ptr<SFProjectile>> add){
  e_projectiles.splice(e_projectiles.end(),add);
}

//Creates a powerup of type at pos position.
void SFApp::DropPowerUp(Point2 pos,POWERTYPE t){
  if(t !=POWER_NONE){
  	auto c = make_shared<SFPower_Up>(t,sf_window);
  	c->SetPosition(pos);
  	power_ups.push_back(c);
  }
}

//returns powerup dropped with variable chance (higher chance less drop rate)
POWERTYPE SFApp::RandomPowerUp(int chance){
  POWERTYPE p;
  int r = rand()%chance;
  switch(r){
    case 0:
      p = POWER_WALL;
      break;
    case 1:
      p = POWER_HEALTH;
      break;
    case 2:
      p = POWER_BULLETS;
      break;
    default:
      p = POWER_NONE;
      break;
  }
  return p;
}

//checks if the wave is completed/ almost completed and then increments wave
// when at wave 5 the boss spawns and the boss waves start.
void SFApp::CheckWave(){
  if(!bossWave){
    if(aliens.size() < 4){
        wave++;
      if(wave == 5) {
        bossWave = true;
        boss->SetPhase(0);
        boss->SetHealth(12);
        bossHealth->SetWidth(boss->GetHealth()*4);
      }else{
        aliens.splice(aliens.end(),wave_get->GetWave(wave));
      }
    }

  }else{
    switch(wave){
      case 6:
        boss->SetPhase(1);
        boss->SetHealth(100);
        break;
      case 7:
        boss->SetPhase(2);
        boss->SetHealth(100);
        break;
      case 8:
        boss->SetPhase(3);
        boss->SetHealth(50);
        break;
    }
    //sets the new width for the boss health bar
    if(boss->GetHealth() > 0) bossHealth->SetWidth(boss->GetHealth()*4);
  }
}


// when game is won display final score
void SFApp::WinGame(){

  GUI.clear();
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  auto winscr = make_shared<SFGUI>(SFGUI_WIN,sf_window);
  auto winscr_pos = Point2(canvas_w/2,canvas_h/2);
  winscr->SetPosition(winscr_pos);
  GUI.push_back(winscr);
  auto finalScore = make_shared<SFGUI>(SFGUI_WIN,sf_window);
  auto finalScore_pos = Point2(canvas_w/2,(canvas_h/2) - 30);
  finalScore->SetPosition(finalScore_pos);
  finalScore->SetText("Final Score : " + to_string(score));
  GUI.push_back(finalScore);
}


// when game is lost show final score.
void SFApp::LoseGame(){
  wave = -1;
  GUI.clear();
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);
  auto losescr = make_shared<SFGUI>(SFGUI_LOSE,sf_window);
  auto losescr_pos = Point2(canvas_w/2,canvas_h/2);
  losescr->SetPosition(losescr_pos);
  GUI.push_back(losescr);

  auto finalScore = make_shared<SFGUI>(SFGUI_TEXTBOX,sf_window);
  auto finalScore_pos = Point2(canvas_w/2,(canvas_h/2) - 30);
  finalScore->SetPosition(finalScore_pos);
  finalScore->SetText("Final Score : " + to_string(score));
  GUI.push_back(finalScore);
}
