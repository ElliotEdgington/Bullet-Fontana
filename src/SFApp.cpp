#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) :
 						fire(0), is_running(true), sf_window(window),
						P_UP(false), P_DOWN(false), P_LEFT(false),
						P_RIGHT(false), P_SHOOT(false), P_FIRE(false),
						fire_timer(20),power_timer(20), score(0),
						gun_power(false), wall_power(2)
						{

  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

	//making application bounding box
  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);

	//creating the player.
	player  = make_shared<SFPlayer>(sf_window);
  auto player_pos = Point2(canvas_w/2, 22);
  player->SetPosition(player_pos);

	//creating the scoreText
	scoreText = make_shared<SFGUI>(SFGUI_TEXTBOX,sf_window);
	AddToScore(0);
	auto scoreText_pos = Point2((canvas_w+50-scoreText->GetWidth()),canvas_h - 20);
	scoreText->SetPosition(scoreText_pos);

//turn this into waves in the future. ----------------------------


	auto alien = make_shared<SFBasic_Enemy>(E_WAVE,sf_window);
	auto pos = Point2(canvas_w/2, canvas_h);
	alien->SetPosition(pos);
	aliens.push_back(alien);

  // auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
  // auto pos  = Point2((canvas_w/4), 100);
  // coin->SetPosition(pos);
  // coins.push_back(coin);

	//gui texture test.
}

SFApp::~SFApp() {
}

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

  // Detect collisions
  for(auto p : p_projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
      }
    }
	}
	//enemy projectile -> player / wall collision
	for(auto p : e_projectiles) {
		if(p->CollidesWith(player)){
      p->HandleCollision();
      player->HandleCollision();
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

  // remove dead aliens (the long way)
	// Also add score on death / death projectiles
  list<shared_ptr<SFBasic_Enemy>> tmp_a;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp_a.push_back(a);
    }else{
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
			}
			CreateExplosion(a->GetPosition());
			FireAt(a->GetPosition(),player->GetPosition());
			DropPowerUp(a->GetPosition(),POWER_WALL);
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
					break;
				case POWER_WALL:
					wall_power +=1;
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

void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

	//SINGLE Entities (not in list)
  // draw the player
  player->OnRender();
	scoreText->OnRender();


	//LIST Entities (in list)
	for(auto g: GUI){
		g->OnRender();
	}

  for(auto p: e_projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }
	for(auto p: p_projectiles) {
		if(p->IsAlive()) {p->OnRender();}
	}

  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
  }

  for(auto c: power_ups) {
    c->OnRender();
  }

	for(auto w: walls){
		w->OnRender();
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

void SFApp::AddToScore(int s){
	score += s;
	string text = to_string(s);
	string zeros("");
	for(int i = text.size(); i < 8 ; i++){
		zeros += "0";
	}
	scoreText->SetText(zeros+text);
}


void SFApp::DropPowerUp(Point2 pos,POWERTYPE t){
	auto c = make_shared<SFPower_Up>(t,sf_window);
	c->SetPosition(pos);
	power_ups.push_back(c);
}



// some projectile paths for when enemies shooting or die.
// ideally would have liked this to be its own class but time constrains yo.
void SFApp::CreateExplosion(Point2 point){
	auto rotation = Vector2(1,0);
	//list<make_shared<Vector2>>
	for(int i = 0; i < 8; i++){
		auto p = make_shared<SFProjectile>(P_ENEMY_ROUND,sf_window);
		p->SetPosition(point);
		p->SetDirection(rotation);
		e_projectiles.push_back(p);
		//rotating the vector 45 degrees
		rotation = Vector2(
			rotation.getX()*cos(M_PI_4)-rotation.getY()*sin(M_PI_4),
			rotation.getX()*sin(M_PI_4)+rotation.getY()*cos(M_PI_4));
	}
}

void SFApp::FireAt(Point2 pos1, Point2 pos2){
	//needed to normalise the vector
	float length = sqrt((pow(pos1.getX(),2)-pow(pos2.getX(),2))
											+(pow(pos1.getY(),2)-pow(pos2.getY(),2)));
	// normalised vector
	auto dir = Vector2((pos2.getX()-pos1.getX())/length,(pos2.getY()-pos1.getY())/length);
	auto p = make_shared<SFProjectile>(P_ENEMY_ROUND,sf_window);
	p->SetPosition(pos1);
	p->SetDirection(dir);
	e_projectiles.push_back(p);
}
