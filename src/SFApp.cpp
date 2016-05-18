#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) :
 						fire(0), is_running(true), sf_window(window),
						P_UP(false), P_DOWN(false), P_LEFT(false),
						P_RIGHT(false), P_SHOOT(false), P_FIRE(false),
						fire_timer(20),power_timer(20), score(0),
						gun_power(false), wall_power(2), wave(0)
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


	//getting a starting wave
	//aliens = list<shared_ptr<SFBasic_Enemy>>(wave_get->GetWave(1));
  //testing boss
  wave = 9;
  bossWave = false;
  boss = make_shared<SFBoss>(sf_window,pattern_get,player,canvas_h);
  auto boss_pos = Point2(canvas_w/2,canvas_h+10);
  boss->SetPosition(boss_pos);

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

		//could maybe do this better with
		//!SFEVENT_PLAYER_UP_KEYDOWN sorta thing
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
  PlayerMovement();

	//increment timers;
	fire_timer++;
	//12 second power duration
	if(gun_power && power_timer < 60*12){
		power_timer++;
	}else{
		gun_power = false;
		power_timer = 0;
	}

  // Update projectile positions
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
  if(bossWave) AddProjectiles(boss->UpdateBoss());

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
      if(wave == 14){
        bossWave = false;
        boss.reset();
        WinGame();
      }
    }
  }


  // remove dead aliens (the long way)
	// Also add score on death / death projectiles
  list<shared_ptr<SFBasic_Enemy>> tmp_a;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp_a.push_back(a);
    }else if(a->GetKilled()){
			switch(a->GetType()){
				case E_STRAIGHT:
					AddToScore(500);
					break;
				case E_WAVE:
					AddToScore(1000);
					break;
				case E_DIAGONAL_LEFT:
				case E_DIAGONAL_RIGHT:
					AddToScore(750);
					break;
        case E_LARGE:
          AddToScore(1500);
          //AddProjectiles(pattern_get->CreateExplosion(a->GetPosition()));
          break;
			}
			//CreateExplosion(a->GetPosition());
			//AddProjectiles(pattern_get->CreateExplosion(a->GetPosition()));
			//DropPowerUp(a->GetPosition(),POWER_WALL);
		}
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

  if(bossWave){
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

	Point2 bulletPos1 = Point2(playerX+25.0f,playerY+25.0f);
	Point2 bulletPos2 = Point2(playerX-25.0f,playerY+25.0f);
  pb1->SetPosition(bulletPos1);
	pb2->SetPosition(bulletPos2);
  p_projectiles.push_back(pb1);
	p_projectiles.push_back(pb2);
	//If under gun_power mode, add more projectiles!
	if(gun_power){
		auto pb3 = make_shared<SFProjectile>(P_PLAYER,sf_window);
		auto pb4 = make_shared<SFProjectile>(P_PLAYER,sf_window);
		Point2 bulletPos3 = Point2(playerX+40.0f,playerY+25.0f);
		Point2 bulletPos4 = Point2(playerX-40.0f,playerY+25.0f);
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

void SFApp::DisplayStatus(){
  healthText->SetText("Health : " + to_string(player->GetHealth()) + "%");
  wallsText->SetText("Walls : " + to_string(wall_power));
}

//dont have another param here because player doesnt have
//cool projectile patterns so only need to append e_projectiles
void SFApp::AddProjectiles(list<shared_ptr<SFProjectile>> add){
  e_projectiles.splice(e_projectiles.end(),add);
}

void SFApp::DropPowerUp(Point2 pos,POWERTYPE t){
	auto c = make_shared<SFPower_Up>(t,sf_window);
	c->SetPosition(pos);
	power_ups.push_back(c);
}

void SFApp::CheckWave(){
  cout<<wave<<endl;
  if(!bossWave){
    if(aliens.empty()){
        wave++;
      if(wave == 10) {
        bossWave = true;
        boss->SetPhase(0);
        boss->SetHealth(12);
        bossHealth->SetWidth(boss->GetHealth()*4);
      }else{
        aliens = wave_get->GetWave(wave);
      }
    }

  }else{
    switch(wave){
      case 11:
        boss->SetPhase(1);
        boss->SetHealth(100);
        break;
      case 12:
        boss->SetPhase(2);
        boss->SetHealth(100);
        break;
      case 13:
        boss->SetPhase(3);
        boss->SetHealth(50);
        break;
    }
    bossHealth->SetWidth(boss->GetHealth()*4);
  }
}


void SFApp::WinGame(){
  GUI.clear();
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  auto winscr = make_shared<SFGUI>(SFGUI_WIN,sf_window);
  auto winscr_pos = Point2(canvas_w/2,canvas_h/2);
  winscr->SetPosition(winscr_pos);
  GUI.push_back(winscr);
  auto finalScore = make_shared<SFGUI>(SFGUI_TEXTBOX,sf_window);
  auto finalScore_pos = Point2(canvas_w/2,(canvas_h/2) - 30);
  finalScore->SetPosition(finalScore_pos);
  finalScore->SetText("Final Score : " + to_string(score));
  GUI.push_back(finalScore);
}

void SFApp::LoseGame(){
  wave = -1;
  GUI.clear();
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);
  auto losescr = make_shared<SFGUI>(SFGUI_WIN,sf_window);
  auto losescr_pos = Point2(canvas_w/2,canvas_h/2);
  losescr->SetPosition(losescr_pos);
  GUI.push_back(losescr);

  auto finalScore = make_shared<SFGUI>(SFGUI_TEXTBOX,sf_window);
  auto finalScore_pos = Point2(canvas_w/2,(canvas_h/2) - 30);
  finalScore->SetPosition(finalScore_pos);
  finalScore->SetText("Final Score : " + to_string(score));
  GUI.push_back(finalScore);
}
