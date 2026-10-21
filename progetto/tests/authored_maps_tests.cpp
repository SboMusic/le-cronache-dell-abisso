#include "../src/AlphaSimulation.h"
#include "../src/WorkerAnimation.h"
#include <set>
using namespace alpha;
int checks=0;void check(bool v,const char* label){if(!v)throw std::runtime_error(label);++checks;std::cout<<"PASS "<<label<<'\n';}
int find(const World& w,Kind k,int team){for(const auto& e:w.entities)if(e.hp>0&&e.kind==k&&e.team==team)return e.id;return -1;}
std::string save(const World& w){std::ostringstream out;check(w.save(out),"authored world saves");return out.str();}
void advance(World& w,float seconds){for(int i=0;i<int(seconds*20)&&!w.result;++i)w.step(.05f);}
int main(){try{
 std::set<std::string> layouts;
 for(int f=0;f<2;++f)for(int n=0;n<missionCount;++n){World w;w.startMission(n,f);w.raidsEnabled=false;check(w.layoutVersion==1&&w.cols==112+8*n,"new campaign selects authored layout");auto home=w.get(find(w,Kind::Hall,0));check(home&&distance(home->pos,{440,1720})>100,"initial settlement is redistributed");
  for(const auto& e:w.entities)if(e.hp>0&&def(e.kind).domain==Domain::Land&&e.kind!=Kind::Shipyard&&!resource(e.kind))check(!w.water(e.pos),"terrestrial unit or structure starts on land");
  int trees=0,water=0;for(auto t:w.terrain)if(t==Terrain::Water)++water;for(const auto& e:w.entities)if(e.hp>0&&e.kind==Kind::Forest){++trees;int neighbors=0;for(const auto& other:w.entities)if(other.hp>0&&other.kind==Kind::Forest&&other.id!=e.id&&distance(e.pos,other.pos)<85)++neighbors;if(neighbors==0)throw std::runtime_error("isolated authored tree");}
  check(trees>100&&w.entities.size()<4096,"coherent forests fit validated entity budget");check(n>=4||water==0,"first four maps remain terrestrial");
  Entity walker;walker.kind=Kind::Worker;walker.pos=home->pos+sf::Vector2f(150,100);auto goals=w.questTargets();if(n!=4&&n!=7)for(auto goal:goals)check(w.path(walker,goal,150),"land objective has navigable approach");
  auto data=save(w);World copy;std::istringstream in(data);check(copy.restore(in)&&copy.layoutVersion==1&&save(copy)==data,"authored objectives and layout survive v5 save exactly");
  std::ostringstream signature;for(auto t:w.terrain)signature<<int(t);signature<<home->pos.x<<home->pos.y;layouts.insert(signature.str());
 }
 check(layouts.size()==missionCount,"all eleven maps have distinct complete layouts");
 {World w;w.startMission(0);w.raidsEnabled=false;int workers=0,soldiers=0;for(const auto& e:w.entities)if(e.team==1){if(e.kind==Kind::Worker)++workers;else if(mobile(e.kind))++soldiers;}check(workers==3&&soldiers==0,"first mission has rival workers only");auto home=w.get(find(w,Kind::Hall,0));auto rival=w.get(find(w,Kind::Worker,1));check(distance(home->pos,rival->pos)>1600,"optional rival camp is separate from starting base");int hall=home->id;check(w.train(hall,Kind::Worker),"new layout keeps worker training free of farm objective");advance(w,6);
  auto a=w.get(find(w,Kind::Worker,0));auto b=w.get(find(w,Kind::Worker,1));a->pos=b->pos+sf::Vector2f(-45,0);float hp=b->hp;w.attack(*a,b->id);advance(w,2);check(!w.get(b->id)||w.get(b->id)->hp<hp,"workers can fight without soldiers");
 }
 {World w;w.startMission(0);w.raidsEnabled=false;w.gold=w.wood=1000;int home=find(w,Kind::Hall,0);auto origin=w.get(home)->pos;std::vector<int> workers;for(auto& e:w.entities)if(e.team==0&&e.kind==Kind::Worker)workers.push_back(e.id);int tree=-1;float best=1e9;for(auto& e:w.entities)if(e.kind==Kind::Forest&&distance(origin,e.pos)<best){tree=e.id;best=distance(origin,e.pos);}for(int id:workers)w.gather(*w.get(id),tree);int initial=w.wood;advance(w,120);check(w.wood>initial+240,"four workers cut and deliver from same dense forest without deadlock");std::set<std::pair<int,int>> goals;for(int id:workers){auto e=w.get(id);goals.insert({int(e->goal.x),int(e->goal.y)});}check(goals.size()>1,"workers reserve distinct service approaches");
 }
 {World w;w.startMission(0);auto worker=*w.get(find(w,Kind::Worker,0));using A=presentation::WorkerAction;worker.order=Order::Idle;check(presentation::workerAction(w,worker,false)==A::Idle,"stationary worker does not walk in place");worker.cargo=10;check(presentation::workerAction(w,worker,true)==A::Carry,"carrying has its own walking pose");auto mine=presentation::workerPose(A::Mine,.2f,0),chop=presentation::workerPose(A::Chop,.2f,0),build=presentation::workerPose(A::Build,.2f,0);check(mine.cycle!=chop.cycle&&chop.cycle!=build.cycle,"mining chopping and building have distinct rhythms");}

 for(int faction=0;faction<2;++faction){World w;w.startMission(0,faction);w.raidsEnabled=false;auto home=w.get(find(w,Kind::Hall,0))->pos;std::vector<int> crew;for(const auto& e:w.entities)if(e.team==0&&e.kind==Kind::Worker)crew.push_back(e.id);
  check(w.build(crew[0],home+sf::Vector2f(250,300),Kind::Farm)&&w.build(crew[1],home+sf::Vector2f(500,260),Kind::Farm),"new first mission accepts two paid farms in planned expansion area");advance(w,40);check(w.missionStage==1,"authored first mission completes real construction objective");
  int mine=-1,tree=-1;float dm=1e9,dt=1e9;for(const auto& e:w.entities)if(e.hp>0){if(e.kind==Kind::Mine&&distance(home,e.pos)<dm){dm=distance(home,e.pos);mine=e.id;}if(e.kind==Kind::Forest&&distance(home,e.pos)<dt){dt=distance(home,e.pos);tree=e.id;}}
  for(int i=0;i<int(crew.size());++i)w.gather(*w.get(crew[i]),i%2?tree:mine);advance(w,160);check(w.missionStage==2,"real deliveries advance authored mission economy for either faction");w.move(*w.get(crew[0]),w.missionTarget());advance(w,100);check(w.result==1,"new first mission wins through exploration without fighting rival camp");
 }
 for(int n:{4,7}){World w;w.startMission(n);w.raidsEnabled=false;sf::Vector2f island{campaignLayout(n).sites[0].x*w.width(),campaignLayout(n).sites[0].y*w.height()},landing,offshore,origin;float best=1e9,near=1e9;auto home=w.get(find(w,Kind::Hall,0))->pos;
  for(int y=1;y<w.rows-1;++y)for(int x=1;x<w.cols-1;++x){sf::Vector2f p{(x+.5f)*w.cell,(y+.5f)*w.cell};if(!w.blocked(x,y,Domain::Sea)&&distance(p,home)<near){near=distance(p,home);origin=p;}
   if(w.blocked(x,y)||distance(p,island)>900)continue;for(auto delta:{sf::Vector2f(40,0),sf::Vector2f(-40,0),sf::Vector2f(0,40),sf::Vector2f(0,-40)}){auto sea=p+delta;if(w.blocked(int(sea.x/40),int(sea.y/40),Domain::Sea))continue;Entity walker;walker.kind=Kind::Soldier;walker.pos=p;if(distance(p,island)<best&&w.path(walker,island,140)){best=distance(p,island);landing=p;offshore=sea;}}
  }
  check(best<1e9&&near<1e9,"authored island has a usable landing connected to objective");Entity ferry;ferry.kind=Kind::Transport;ferry.pos=origin;check(w.path(ferry,offshore),"authored sea connects player coast to island landing");int ship=w.add(Kind::Transport,0,offshore);for(int i=0;i<3;++i){int id=w.add(Kind::Soldier,0,offshore);w.get(id)->carrier=ship;}check(w.unload(ship,landing),"actual transport unloads on authored island shore");
 }
 // Objective-state fixtures validate new landmark bindings, not paid full-campaign playthroughs.
 for(int n=1;n<missionCount;++n){World w;w.startMission(n);w.raidsEnabled=false;auto home=w.get(find(w,Kind::Hall,0))->pos;
  if(n==1){w.missionStage=2;w.get(find(w,Kind::Worker,0))->pos=w.missionTarget();advance(w,.1f);}
  else if(n==2){w.missionStage=2;w.archivist=w.add(Kind::Healer,0,home+sf::Vector2f(120,0));advance(w,.1f);}
  else if(n==3){w.missionStage=1;w.add(Kind::Knight,0,w.missionTarget());advance(w,91);check(w.missionStage==2,"refuge defense uses new pass location");for(int id:w.missionGuards)if(w.get(id))w.get(id)->hp=0;advance(w,.1f);}
  else if(n==4||n==7){w.missionStage=n==4?2:1;auto goal=n==4?w.missionTarget():w.get(w.missionGuards[0])->pos;for(auto& e:w.entities)if(e.team==1&&distance(e.pos,goal)<350)e.hp=0;for(int i=0;i<3;++i)w.add(Kind::Soldier,0,goal+sf::Vector2f(-120,i*40.f));advance(w,21);if(n==7){check(w.missionStage==2,"new lighthouse occupation advances naval mission");w.get(w.missionGuards[1])->hp=0;advance(w,.1f);}}
  else if(n==5||n==8){w.missionStage=1;for(int id:w.missionGuards){auto p=w.get(id)->pos;for(auto& e:w.entities)if(e.team==1&&distance(e.pos,p)<350)e.hp=0;w.add(Kind::Soldier,0,p+sf::Vector2f(-120,0));}advance(w,n==5?40:60);}
  else if(n==6){w.missionStage=1;for(int id:w.missionGuards)w.get(id)->hp=0;advance(w,.1f);for(auto& e:w.entities)if(e.team==1&&e.kind==Kind::Hall)e.hp=0;advance(w,.1f);}
  else if(n==9){w.missionStage=1;for(int i=0;i<2;++i){auto p=w.get(w.missionGuards[i])->pos;for(auto& e:w.entities)if(e.team==1&&distance(e.pos,p)<350)e.hp=0;w.add(Kind::Soldier,0,p+sf::Vector2f(-120,0));}advance(w,13);for(int i=2;i<4;++i)w.get(w.missionGuards[i])->hp=0;advance(w,.1f);}
  else{for(auto& e:w.entities)if(e.team==1)e.hp=0;advance(w,.1f);check(w.get(w.archivist)!=nullptr,"rebuilt finale preserves Worldstone");}
  check(w.result==1,"authored mission can finish at its actual landmarks");
 }
 std::cout<<"SUCCESS "<<checks<<" authored maps, worker activity and navigation checks\n";return 0;
 }catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
