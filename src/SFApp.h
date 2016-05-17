#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>
#include <cmath>
#include <string>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFAsset.h"
#include "SFGUI.h"
#include "SFWave.h"
//including entities
#include "SFPlayer.h"
#include "SFPower_Up.h"
#include "SFBasic_Enemy.h"
#include "SFProjectile.h"
#include "SFWall.h"

/**
 * Represents the StarshipFontana application.  It has responsibilities for
 * * Creating and destroying the app window
 * * Processing game events
 */
class SFApp {
public:
  SFApp(std::shared_ptr<SFWindow>);
  virtual ~SFApp();
  void    OnEvent(SFEvent &);
  int     OnExecute();
  void    OnUpdateWorld();
  void    OnRender();
  //methods
private:
  void    FireProjectile();
  void    PlayerMovement();
  void    PlaceWall();
  bool    HitWall();
  void    CreateExplosion(Point2);
  void    FireAt(Point2,Point2);
  void    AddToScore(int);
  void    DropPowerUp(Point2, POWERTYPE);
  void    CheckWave();

  //variables
private:
  bool                                is_running;
  shared_ptr<SFWindow>                sf_window;
  shared_ptr<SFBoundingBox>           app_box;
  shared_ptr<SFWave>                  wave_get;

  shared_ptr<SFPlayer>                player;
  shared_ptr<SFGUI>                   scoreText;
  //Split enemy and player projectile so collision doesnt have to loop through
  //unnecessary projectiles.(eg player hitting player projectiles)
  list<shared_ptr<SFProjectile> >     e_projectiles;
  list<shared_ptr<SFProjectile> >     p_projectiles;
  list<shared_ptr<SFBasic_Enemy> >    aliens;
  list<shared_ptr<SFPower_Up> >       power_ups;
  list<shared_ptr<SFWall> >           walls;
  list<shared_ptr<SFGUI> >            GUI;

  int      score;
  int      wall_power;
  bool     gun_power;
  int      fire;
  int      power_timer;
  int      fire_timer;
  bool     P_UP;
  bool     P_DOWN;
  bool     P_LEFT;
  bool     P_RIGHT;
  bool     P_SHOOT;
  bool     P_FIRE;
};
#endif
