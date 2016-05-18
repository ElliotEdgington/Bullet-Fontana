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
#include "SFPattern.h"
//including entities
#include "SFPlayer.h"
#include "SFPower_Up.h"
#include "SFBasic_Enemy.h"
#include "SFProjectile.h"
#include "SFWall.h"
#include "SFBoss.h"

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
  //gui functions
  void    AddToScore(int);
  void    DisplayStatus();

  void    AddProjectiles(list<shared_ptr<SFProjectile>>);
  void    DropPowerUp(Point2, POWERTYPE);
  void    CheckWave();

  //On Lose and on Win
  void    WinGame();
  void    LoseGame();


  //variables
private:
  bool                                is_running;
  shared_ptr<SFWindow>                sf_window;
  shared_ptr<SFBoundingBox>           app_box;
  shared_ptr<SFWave>                  wave_get;
  shared_ptr<SFPattern>               pattern_get;

  shared_ptr<SFPlayer>                player;
  shared_ptr<SFBoss>                  boss;
  shared_ptr<SFGUI>                   bossHealth;
  shared_ptr<SFGUI>                   scoreText;
  shared_ptr<SFGUI>                   healthText;
  shared_ptr<SFGUI>                   wallsText;
  //Split enemy and player projectile so collision doesnt have to loop through
  //unnecessary projectiles.(eg player hitting player projectiles)
  list<shared_ptr<SFProjectile> >     e_projectiles;
  list<shared_ptr<SFProjectile> >     p_projectiles;
  list<shared_ptr<SFBasic_Enemy> >    aliens;
  list<shared_ptr<SFPower_Up> >       power_ups;
  list<shared_ptr<SFWall> >           walls;
  list<shared_ptr<SFGUI> >            GUI;

  //required for GUI
  int      score;
  int      wall_power;
  bool     gun_power;
  //timers
  int      power_timer;
  int      fire_timer;

  //wave and bossWave
  int      wave;
  bool     bossWave;

  //bools for movement
  int      fire;
  bool     P_UP;
  bool     P_DOWN;
  bool     P_LEFT;
  bool     P_RIGHT;
  bool     P_SHOOT;
  bool     P_FIRE;
};
#endif
