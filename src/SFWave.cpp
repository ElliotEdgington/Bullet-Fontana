#include "SFWave.h"

SFWave::SFWave(shared_ptr<SFWindow> window, int w , int h) : sf_window(window), w_width(w), w_height(h){}
SFWave::~SFWave(){}

list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave(int w){
  switch(w){
    case 1:
      return GetWave1();
    case 2:
      return GetWave2();
    case 3:
      return GetWave3();
    case 4:
      return GetWave4();
  }
}

list<shared_ptr<SFBasic_Enemy> > SFWave::AddRowOf(int x, int y, int space, ENEMYTYPE type, int numOf){
  list<shared_ptr<SFBasic_Enemy> > row;
  for(int i = 0; i < numOf; i++){
    auto e = make_shared<SFBasic_Enemy>(type,sf_window);
    auto pos = Point2(x+space*i,y);
    e->SetPosition(pos);
    row.push_back(e);
  }
  return row;
}

list<shared_ptr<SFBasic_Enemy> > SFWave::AddColumnOf(int x, int y, int space, ENEMYTYPE type, int numOf){
  list<shared_ptr<SFBasic_Enemy> > column;
  for(int i = 0; i < numOf; i++){
    auto e = make_shared<SFBasic_Enemy>(type,sf_window);
    auto pos = Point2(x,y+space*i);
    e->SetPosition(pos);
    column.push_back(e);
  }
  return column;
}

list<shared_ptr<SFBasic_Enemy> > SFWave::AddDiagonalOf(int x, int y, int space, ENEMYTYPE type, int numOf, int dir = 1){
  list<shared_ptr<SFBasic_Enemy> > diagonal;
  for(int i = 0; i < numOf; i++){
    auto e = make_shared<SFBasic_Enemy>(type,sf_window);
    auto pos = Point2(x+space*i*dir,y+space*i);
    e->SetPosition(pos);
    diagonal.push_back(e);
  }
  return diagonal;
}


list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave1(){
  list<shared_ptr<SFBasic_Enemy> > wave;
  wave.splice(wave.end(),AddRowOf(50,w_height+40,50,E_STRAIGHT, 5));
  wave.splice(wave.end(),AddRowOf(50,w_height+120,50,E_STRAIGHT, 5));
  wave.splice(wave.end(),AddRowOf((w_width/2)+50,w_height+80,50,E_STRAIGHT, 5));
  return wave;
}
list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave2(){}
list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave3(){}
list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave4(){}
