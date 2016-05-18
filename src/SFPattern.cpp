#include "SFPattern.h"

SFPattern::SFPattern(shared_ptr<SFWindow> window) : sf_window(window){}
SFPattern::~SFPattern(){}

list<shared_ptr<SFProjectile>> SFPattern::CreateExplosion(Point2 point, int degree = 0){
  list<shared_ptr<SFProjectile>> explosion;
  auto rotation = Vector2(1,0);
  rotation = rotation.RotateVector(degree);
	for(int i = 0; i < 8; i++){
		auto p = make_shared<SFProjectile>(P_ENEMY_ROUND,sf_window);
		p->SetPosition(point);
		p->SetDirection(rotation);
		explosion.push_back(p);
		//rotating the vector 45 degrees
    rotation = rotation.RotateVector(45);
	}
  return explosion;
}

list<shared_ptr<SFProjectile>> SFPattern::FireAt(Point2 pos1, Point2 pos2){
  list<shared_ptr<SFProjectile>> fire;
  //needed to normalise the vector
	float length = abs(sqrt((pow(pos1.getX()-pos2.getX(),2))
											+(pow(pos1.getY()-pos2.getY(),2))));
	// normalised vector
	auto dir = Vector2((pos2.getX()-pos1.getX())/length,(pos2.getY()-pos1.getY())/length);

  //create projectile
	auto p = make_shared<SFProjectile>(P_ENEMY_ROUND,sf_window);
	p->SetPosition(pos1);
	p->SetDirection(dir);
	fire.push_back(p);
  return fire;
}

list<shared_ptr<SFProjectile>> SFPattern::SprayAt(Point2 pos1, Point2 pos2){
  list<shared_ptr<SFProjectile>> spray;
  spray.splice(spray.end(), FireAt(pos1,pos2));
  //create two more projectiles aiming for either side of pos2
  spray.splice(spray.end(), FireAt(pos1,Point2(pos2.getX()+60,pos2.getY())));
  spray.splice(spray.end(), FireAt(pos1,Point2(pos2.getX()-60,pos2.getY())));
  return spray;
}

list<shared_ptr<SFProjectile>> SFPattern::FireAngle(Point2 pos, int angle){
  list<shared_ptr<SFProjectile>> fire;
  auto rotation = Vector2(1,0);
  rotation = rotation.RotateVector(angle);
  
	auto p = make_shared<SFProjectile>(P_ENEMY_ROUND,sf_window);
	p->SetPosition(pos);
	p->SetDirection(rotation);
	fire.push_back(p);
  return fire;
}
