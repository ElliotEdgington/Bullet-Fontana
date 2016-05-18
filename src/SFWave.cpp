#include "SFWave.h"

SFWave::SFWave(shared_ptr<SFWindow> window, int w , int h) : sf_window(window), w_width(w), w_height(h){}
SFWave::~SFWave(){}

//returns a list of enemies in a particular wave
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

//adds a row of numOf enemies at x,y posiiton with space in bwtween them of type
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


// as above but with a column instead
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


// as above but with diagonal and in direction dir (1 for right -1 for left)
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


//To create the waves I used the methods above to create enemies in level like structures
//These were pretty rushed so theyre quite messy and not entirely fair.

list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave1(){
  list<shared_ptr<SFBasic_Enemy> > wave;
  wave.splice(wave.end(),AddRowOf(50,w_height+40,50,E_STRAIGHT, 5));
  wave.splice(wave.end(),AddRowOf(50,w_height+120,50,E_STRAIGHT, 5));
  wave.splice(wave.end(),AddRowOf(100,w_height+300,50,E_STRAIGHT, 10));
  wave.splice(wave.end(),AddRowOf((w_width/2)+50,w_height+80,50,E_STRAIGHT, 5));

  wave.splice(wave.end(),AddRowOf(50,w_height+120,60,E_STRAIGHT_SLOW, 4));
  wave.splice(wave.end(),AddRowOf(w_width/2,w_height+180,60,E_STRAIGHT_SLOW, 4));

  wave.splice(wave.end(),AddRowOf(50,w_height+50,0,E_LARGE,1));
  wave.splice(wave.end(),AddRowOf(w_width-50,w_height+50,0,E_LARGE,1));
  wave.splice(wave.end(),AddRowOf(w_width/2,w_height+90,0,E_LARGE,1));
  return wave;
}
list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave2(){
  list<shared_ptr<SFBasic_Enemy> > wave;
  wave.splice(wave.end(),AddRowOf(50,w_height+300,50,E_STRAIGHT, 10));
  wave.splice(wave.end(),AddRowOf((w_width/2)+50,w_height+80,50,E_STRAIGHT, 5));
  wave.splice(wave.end(),AddColumnOf((w_width/2) - 40,w_height+400,50,E_STRAIGHT, 10));
  wave.splice(wave.end(),AddColumnOf((w_width/2) + 40,w_height+400,50,E_STRAIGHT, 10));

  wave.splice(wave.end(),AddColumnOf(w_width/2,w_height+180,60,E_STRAIGHT_SLOW, 4));

  wave.splice(wave.end(),AddRowOf(150,w_height+60,80,E_LARGE,2));
  wave.splice(wave.end(),AddRowOf(w_width-150,w_height+30,80,E_LARGE,2));
  return wave;
}


list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave3(){
  list<shared_ptr<SFBasic_Enemy> > wave;
  wave.splice(wave.end(),AddDiagonalOf((w_width/2) + 20,w_height+400,50,E_STRAIGHT, 5, 1));
  wave.splice(wave.end(),AddDiagonalOf((w_width/2) - 20,w_height+400,50,E_STRAIGHT, 10, -1));

  wave.splice(wave.end(),AddColumnOf(80,w_height+400,50,E_WAVE, 10));
  wave.splice(wave.end(),AddColumnOf((w_width) - 80,w_height+400,50,E_WAVE, 10));

  wave.splice(wave.end(),AddRowOf((w_width/4)*2,w_height+60,50,E_LARGE,4));

  wave.splice(wave.end(),AddColumnOf(w_width+60,w_height+60,50,E_DIAGONAL_LEFT,4));
  wave.splice(wave.end(),AddColumnOf(w_width-120,w_height+100,50,E_DIAGONAL_RIGHT,4));

  wave.splice(wave.end(),AddRowOf(150,w_height+80,100,E_LARGE,4));
  wave.splice(wave.end(),AddRowOf(150,w_height+40,100,E_LARGE,2));

  wave.splice(wave.end(),AddRowOf(50,w_height+300,40,E_STRAIGHT_SLOW, 4));
  wave.splice(wave.end(),AddRowOf(w_width/2,w_height+250,40,E_STRAIGHT_SLOW,5));
  wave.splice(wave.end(),AddRowOf(50,w_height+350,40,E_STRAIGHT_SLOW, 6));

  return wave;
}
list<shared_ptr<SFBasic_Enemy> > SFWave::GetWave4(){
  list<shared_ptr<SFBasic_Enemy> > wave;
  wave.splice(wave.end(),AddDiagonalOf((w_width/2) + 20,w_height+200,70,E_STRAIGHT, 5, 1));
  wave.splice(wave.end(),AddDiagonalOf((w_width/2) + 20,w_height+400,50,E_STRAIGHT, 5, 1));
  wave.splice(wave.end(),AddDiagonalOf((w_width/2) + 20,w_height+800,50,E_STRAIGHT, 5, 1));

  wave.splice(wave.end(),AddDiagonalOf((w_width/2) - 20,w_height+200,70,E_WAVE, 10, -1));
  wave.splice(wave.end(),AddDiagonalOf((w_width/2) - 20,w_height+400,50,E_WAVE, 10, -1));
  wave.splice(wave.end(),AddDiagonalOf((w_width/2) - 20,w_height+800,50,E_WAVE, 10, -1));

  wave.splice(wave.end(),AddColumnOf(w_width+60,w_height,40,E_DIAGONAL_LEFT,4));
  wave.splice(wave.end(),AddColumnOf(w_width-120,w_height,40,E_DIAGONAL_RIGHT,4));

  wave.splice(wave.end(),AddColumnOf(w_width/2,w_height+50,200,E_LARGE,4));

  wave.splice(wave.end(),AddRowOf(50,w_height+600,40,E_STRAIGHT_SLOW, 4));
  wave.splice(wave.end(),AddRowOf(w_width/2,w_height+550,40,E_STRAIGHT_SLOW,5));
  wave.splice(wave.end(),AddRowOf(50,w_height+700,40,E_STRAIGHT_SLOW, 6));
  return wave;
}
