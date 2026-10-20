#include "../src/AlphaSimulation.h"
#include <iostream>
#include <stdexcept>
using namespace alpha;
int checks=0;
void check(bool pass,const char* label){if(!pass)throw std::runtime_error(label);++checks;std::cout<<"PASS "<<label<<std::endl;}
void advance(World& w,float seconds){for(int i=0;i<int(seconds*20);++i)w.step(.05f);}
int main(){try{
 World w;w.reset();check(w.population(0)==6,"initial population");
 int worker=0,hall=0,mine=0,enemyHall=0;for(auto e:w.entities){if(e.kind==Kind::Worker&&!worker)worker=e.id;if(e.kind==Kind::Hall)(e.team==0?hall:enemyHall)=e.id;if(e.kind==Kind::Mine&&!mine)mine=e.id;}
 auto before=w.gold;check(w.train(hall,Kind::Worker),"train worker accepted");check(w.gold==before-50,"worker cost");check(!w.train(hall,Kind::Worker),"busy production rejected");check(w.population(0)==7,"production reserves population");advance(w,5.2f);check(w.population(0,false)==7,"worker spawns after timer");
 // Gather must travel to source and return cargo before crediting resources.
 w.gather(*w.get(worker),mine);before=w.gold;w.step(.05f);check(w.gold==before,"no immediate resource credit");advance(w,35);check(w.gold>before,"gather delivers gold to hall");check(w.get(mine)->hp<def(Kind::Mine).hp,"resource node depletes");
 auto gold=w.gold,wood=w.wood;
 check(!w.build(worker,w.get(hall)->pos),"overlapping construction rejected");check(w.gold==gold&&w.wood==wood,"invalid construction costs nothing");
 w.get(worker)->pos={640,800};w.get(worker)->route.clear();w.get(worker)->order=Order::Idle;
 check(w.build(worker,{750,800}),"barracks placement accepted");int barracks=w.entities.back().id;check(w.gold==gold-120&&w.wood==wood-80,"construction cost");check(!w.train(barracks,Kind::Soldier),"unfinished barracks cannot train");advance(w,10.2f);w.gold=200;check(w.train(barracks,Kind::Soldier),"completed barracks trains");advance(w,7.2f);check(w.population(0,false)==8,"soldier production completes");
 auto& mover=*w.get(worker);mover.pos={640,800};w.move(mover,{940,800});check(!mover.route.empty(),"route around building found");for(auto point:mover.route)check(!w.blocked(int(point.x/40),int(point.y/40)),"route avoids occupied grid");advance(w,8);check(distance(w.get(worker)->pos,{940,800})<12,"unit reaches destination");
 w.reset();hall=1;w.gold=10000;while(w.population(0)<30)w.add(Kind::Soldier,0,{900.f,1200.f});check(!w.train(hall,Kind::Worker),"population cap enforced");
 w.reset();w.gold=0;check(!w.train(1,Kind::Worker),"insufficient resources rejected");
 w.reset();advance(w,30);check(w.get(2)->hp==1000&&w.get(1)->hp==1000,"opening preparation period");
 // Simulate complete combat against the command using actual orders and damage.
 w.reset();for(auto& e:w.entities)if(e.team==1&&mobile(e.kind))e.hp=0;
 for(int i=0;i<12;++i)w.add(Kind::Soldier,0,{2050.f,500.f+i*40});
 for(auto& e:w.entities)if(e.team==0&&e.kind==Kind::Soldier)w.attack(e,2);
 advance(w,80);check(w.result==1,"army destroys enemy hall and wins");
 before=w.gold;advance(w,10);check(w.gold==before,"completed match stops simulation");
 w.reset();w.get(1)->hp=0;w.step(.05f);check(w.result==-1,"hall destruction causes defeat");
 w.reset();check(w.result==0&&w.gold==240&&w.population(0)==6,"restart restores state");
 // Economy and combat run together for a long session without invalid pointers.
 for(auto& e:w.entities)if(e.kind==Kind::Worker)w.gather(e,mine);
 advance(w,600);check(w.result!=0,"unattended match reaches an outcome");
 // End-to-end playable rules: no free allied units or resources.
 w.reset();
 int builder=7;
 check(w.build(builder,{580,620}),"economic match starts with paid barracks");
 int production=w.entities.back().id;
 check(w.build(8,{430,820},Kind::Farm),"economic match adds paid population capacity");
 for(auto& e:w.entities)if(e.kind==Kind::Worker)w.gather(e,3);
 bool launched=false;
 for(int tick=0;tick<12000&&w.result==0;++tick){
  auto b=w.get(production);
  if(b&&b->build<=0&&b->production<=0&&w.gold>=80&&w.population(0)<30)w.train(production,Kind::Soldier);
  int soldiers=0;for(const auto& e:w.entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Soldier)++soldiers;
  if(soldiers>=8)launched=true;
  if(launched&&tick%40==0)for(auto& e:w.entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Soldier&&e.order==Order::Idle)w.attack(e,2);
  w.step(.05f);
 }
 std::cout<<"MATCH result="<<w.result<<" time="<<w.time<<" gold="<<w.gold<<" population="<<w.population(0)<<" launched="<<launched<<std::endl;
 check(w.result==1,"full economy-production-combat match can be won");
 std::cout<<"SUCCESS "<<checks<<" checks"<<std::endl;return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<std::endl;return 1;}}

