#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>
#include <cmath>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFAsset.h"
#include "SFGUI.h"
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

  //variables
private:
  bool                    is_running;

  shared_ptr<SFWindow>       sf_window;

  shared_ptr<SFPlayer>        player;
  shared_ptr<SFBoundingBox>  app_box;
  list<shared_ptr<SFProjectile> > projectiles;
  list<shared_ptr<SFBasic_Enemy> > aliens;
  list<shared_ptr<SFPower_Up> > coins;
  list<shared_ptr<SFWall> > walls;
  list<shared_ptr<SFGUI> > GUI;

  int fire;
  int fire_delay;
  bool P_UP;
  bool P_DOWN;
  bool P_LEFT;
  bool P_RIGHT;
  bool P_SHOOT;
  bool P_FIRE;
};
#endif
