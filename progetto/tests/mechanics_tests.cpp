#include "../src/AlphaSimulation.h"
#include <iostream>
#include <stdexcept>
using namespace alpha;
int checks=0;
void check(bool value,const char* label){if(!value)throw std::runtime_error(label);++checks;std::cout<<"PASS "<<label<<std::endl;}
void advance(World& w,float s){for(int i=0;i<int(s*20);++i)w.step(.05f);}
int main(){try{
 {World w;w.reset();w.raidsEnabled=false;w.gold=w.wood=5000;w.townLevel[0]=3;int b=w.add(Kind::Barracks,0,{800,750}),b2=w.add(Kind::Barracks,0,{1100,750});w.add(Kind::LumberMill,0,{800,450});w.add(Kind::Stable,0,{1100,450});
 check(!w.train(1,Kind::Soldier),"castle only trains workers");check(!w.train(b,Kind::Worker),"barracks cannot train workers");check(!w.train(b,Kind::Archer),"base barracks cannot train archers");
 check(w.research(b,Research::Barracks2),"local barracks II starts");advance(w,12.2f);check(w.get(b)->level==2&&w.get(b2)->level==1,"upgrade affects selected barracks only");check(w.train(b,Kind::Archer),"upgraded barracks trains archer");check(!w.train(b2,Kind::Archer),"second base barracks stays locked");check(w.cancel(b),"cancel refunds production");w.upgrades[0][int(Research::Elite)]=1;check(!w.train(b,Kind::Ranger),"veteran requires barracks III");check(w.research(b,Research::Barracks3),"barracks III starts");advance(w,16.2f);check(w.train(b,Kind::Ranger),"barracks III trains veteran");check(!w.train(b2,Kind::Knight),"cavalry belongs to stable");
 }
 {World w;w.reset();w.raidsEnabled=false;w.gold=w.wood=5000;while(w.population(0)<w.capacity(0))w.add(Kind::Worker,0,{700,800});check(!w.train(1,Kind::Worker),"full population rejects production");w.get(7)->pos={1000,800};check(w.build(7,{1100,800},Kind::Farm),"worker places remote farm");int f=w.entities.back().id;check(w.capacity(0)==10,"unfinished farm grants no capacity");advance(w,8.2f);check(w.capacity(0)==16&&w.train(1,Kind::Worker),"completed farm enables further population");check(!w.build(7,w.get(f)->pos,Kind::Farm),"structures cannot overlap");check(!w.build(7,{1500,1350},Kind::Farm),"worker cannot build unreachable water foundation");int soldier=w.add(Kind::Soldier,0,{1200,750});w.get(7)->pos={1100,750};check(w.build(7,{1200,750},Kind::Farm),"units do not block construction");check(distance(w.get(soldier)->pos,{1200,750})>60,"unit moved outside foundation");
 }
 {World w;w.reset();w.raidsEnabled=false;int trees=0;for(auto& e:w.entities)if(e.kind==Kind::Forest&&e.hp>0)++trees;check(trees>70,"individual trees distributed across groves");w.gold=w.wood=1000;for(auto& tree:w.entities)if(tree.kind==Kind::Forest&&distance(tree.pos,{600,1000})<210)tree.hp=0;int mill=w.add(Kind::LumberMill,0,{700,850});w.get(7)->pos={600,1000};int t=w.add(Kind::Forest,-1,{620,1040});w.get(t)->hp=10;int oldWood=w.wood;w.gather(*w.get(7),t);advance(w,20);check(!w.get(t),"harvested tree disappears");check(w.wood>oldWood,"felled timber delivered");check(!w.plant(7,{1100,650}),"planting locked before research");w.townLevel[0]=2;check(w.research(mill,Research::Forestry),"forestry research available at lumber mill");advance(w,18.2f);int gold=w.gold;check(w.plant(7,{1100,650}),"researched worker plants tree");int seed=w.entities.back().id;check(w.gold==gold-10&&w.get(seed)->build>0,"planting costs gold and needs growth");w.get(7)->order=Order::Idle;w.gather(*w.get(7),seed);check(w.get(7)->order==Order::Idle,"sapling cannot be harvested early");advance(w,12.2f);check(w.get(seed)->build==0&&w.get(seed)->hp==80,"sapling becomes finite mature tree");check(!w.plant(7,{1100,650}),"trees cannot be planted on occupied spot");
 }
 std::cout<<"SUCCESS "<<checks<<" economy and command mechanics checks"<<std::endl;return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<std::endl;return 1;}}


