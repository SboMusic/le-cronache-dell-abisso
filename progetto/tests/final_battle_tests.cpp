#include "../src/AlphaSimulation.h"
using namespace alpha;
int main(){try{
 for(int faction=0;faction<2;++faction){Kind caster=faction==0?Kind::Mage:Kind::Warlock;World w;w.startLegacyMission(10,faction);int paidUnits=0,paidBuildings=0,spells=0;bool offensive=false;
 auto count=[&](Kind kind,bool reserved=false){int n=0;for(auto& e:w.entities)if(e.hp>0&&e.team==0){if(e.kind==kind)++n;if(reserved&&e.production>0&&e.research==-1&&e.training==kind)++n;}return n;};
 auto construct=[&](Kind kind){for(auto& e:w.entities)if(e.hp>0&&e.team==0&&e.build>0){auto owner=w.get(e.constructionWorker);if(!owner||!w.busy(*owner)){int site=e.id;for(auto& worker:w.entities)if(worker.hp>0&&worker.team==0&&worker.kind==Kind::Worker&&!w.busy(worker)){w.repair(worker,site);break;}}return false;}if(kind==Kind::GuardTower||kind==Kind::CannonTower){std::vector<sf::Vector2f> sites=kind==Kind::GuardTower?std::vector<sf::Vector2f>{{1100,1380},{1250,1690}}:std::vector<sf::Vector2f>{{1100,1560},{1200,1840},{900,1960}};for(auto p:sites)if(w.placementIssue(p,kind).empty())for(auto& worker:w.entities)if(worker.hp>0&&worker.team==0&&worker.kind==Kind::Worker&&!w.busy(worker)){int id=worker.id;if(w.build(id,p,kind)){++paidBuildings;return true;}break;}}for(int y=1180;y<2260;y+=140)for(int x=120;x<1500;x+=140){sf::Vector2f p{float(x),float(y)};if(!w.placementIssue(p,kind).empty()||w.water(p))continue;std::vector<int> workers;for(auto& e:w.entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Worker&&!w.busy(e))workers.push_back(e.id);for(int id:workers)if(w.build(id,p,kind)){++paidBuildings;return true;}}return false;};
 for(int tick=0;tick<72000&&!w.result;++tick){
  if(tick%40==0){
   std::vector<int> workers,producers;for(auto& e:w.entities)if(e.hp>0&&e.team==0){if(e.kind==Kind::Worker&&!w.busy(e)&&e.order==Order::Idle)workers.push_back(e.id);if(!mobile(e.kind)&&e.build<=0)producers.push_back(e.id);}
   for(int id:workers){auto person=w.get(id);if(!person)continue;int miners=0,loggers=0;for(const auto& ally:w.entities)if(ally.hp>0&&ally.team==0&&ally.kind==Kind::Worker){auto source=w.get(ally.source);if(source){if(source->kind==Kind::Mine)++miners;else if(source->kind==Kind::Forest)++loggers;}}Kind wanted=miners*3<loggers+1?Kind::Mine:Kind::Forest;std::vector<std::pair<float,int>> resources;for(auto& e:w.entities)if(e.hp>0&&e.kind==wanted)resources.push_back({distance(e.pos,person->pos),e.id});std::sort(resources.begin(),resources.end());for(auto source:resources){w.gather(*person,source.second);if(person->order==Order::Gather)break;}}
   if((w.gold>1000&&w.wood<300)||(w.wood>1000&&w.gold<300)){
    Kind wanted=w.gold>w.wood?Kind::Forest:Kind::Mine;
    for(auto& person:w.entities)if(person.hp>0&&person.team==0&&person.kind==Kind::Worker&&!w.busy(person)){
     auto current=w.get(person.source);if(current&&current->kind!=wanted){int source=-1;float best=1e9;for(const auto& e:w.entities)if(e.hp>0&&e.kind==wanted&&distance(person.pos,e.pos)<best){source=e.id;best=distance(person.pos,e.pos);}if(source>=0){w.gather(person,source);break;}}
    }
   }
   if(w.capacity(0)<90&&w.capacity(0)<w.population(0)+5)construct(Kind::Farm);
   else if(count(Kind::Workshop)==0)construct(Kind::Workshop);
   else if(count(Kind::Academy)==0)construct(Kind::Academy);
   else if(count(Kind::GuardTower)<3)construct(Kind::GuardTower);
   else if(count(Kind::CannonTower)<3)construct(Kind::CannonTower);
   else if(count(Kind::Aviary)<2)construct(Kind::Aviary);
   else if(count(Kind::Sanctuary)<2)construct(Kind::Sanctuary);
   else if(count(Kind::Barracks)<2)construct(Kind::Barracks);
   else if(count(Kind::Hall)<2)construct(Kind::Hall);
   for(int id:producers){auto producer=w.get(id);if(!producer||producer->production>0)continue;
    bool researching=false;for(auto r:{Research::Armor,Research::Weapons,Research::Archery,Research::Arcana})if(researchCatalog()[int(r)].building==producer->kind&&w.upgrades[0][int(r)]<2){if(w.research(id,r)){researching=true;break;}}if(researching)continue;
    if(producer->kind==Kind::Barracks&&producer->level<3){w.research(id,producer->level==1?Research::Barracks2:Research::Barracks3);continue;}
    Kind unit=Kind::Count;if(producer->kind==Kind::Hall&&count(Kind::Worker,true)<22)unit=Kind::Worker;if(producer->kind==Kind::Barracks&&count(Kind::Ranger,true)<14)unit=w.wood<35&&count(Kind::Soldier,true)<14?Kind::Soldier:Kind::Ranger;
    if(producer->kind==Kind::Stable&&count(Kind::Knight,true)<7)unit=Kind::Knight;
    if(producer->kind==Kind::Sanctuary)unit=count(Kind::Healer,true)<6?Kind::Healer:count(Kind::Guardian,true)<10?Kind::Guardian:Kind::Count;
    if(producer->kind==Kind::Workshop&&count(Kind::Siege,true)<5)unit=Kind::Siege;
    if(producer->kind==Kind::Academy&&count(caster,true)<4)unit=caster;
    if(producer->kind==Kind::Aviary&&count(Kind::Warbird,true)<12)unit=Kind::Warbird;
    if(unit!=Kind::Count&&w.train(id,unit))++paidUnits;
   }
   int fighters=0;sf::Vector2f average;std::vector<int> army,healers,mages,guardians;for(auto& e:w.entities)if(e.hp>0&&e.team==0&&mobile(e.kind)&&e.kind!=Kind::Worker){if(e.kind==Kind::Healer)healers.push_back(e.id);if(e.kind==caster)mages.push_back(e.id);if(e.kind==Kind::Guardian)guardians.push_back(e.id);if(def(e.kind).damage>0){army.push_back(e.id);average+=e.pos;++fighters;}}
   if(fighters>=18&&w.time>=900)offensive=true;if(fighters<12)offensive=false;if(fighters)average/=float(fighters);
   int target=-1;float best=1e9;for(auto& e:w.entities)if(e.hp>0&&e.team==1){float range=distance(e.pos,offensive?average:sf::Vector2f(700,1700));if(!offensive&&range>850)continue;float rank=range-(mobile(e.kind)?100.f:0);if(rank<best){best=rank;target=e.id;}}
   if(target>0){for(int id:army)if(w.get(id))w.attack(*w.get(id),target);auto destination=w.get(target)->pos+sf::Vector2f(-160,120);for(int id:healers)if(w.get(id))w.move(*w.get(id),destination);}
   for(int id:mages){auto mage=w.get(id);if(!mage)continue;for(auto& enemy:w.entities)if(enemy.hp>0&&enemy.team==1&&distance(enemy.pos,mage->pos)<440){if(w.cast(id,faction==0?Spell::Firestorm:Spell::Storm,enemy.pos))++spells;break;}}
   for(int id:guardians){auto guardian=w.get(id);if(guardian&&guardian->hp<guardian->maxHp*.65f&&guardian->shield<=0)if(w.cast(id,Spell::Shield,guardian->pos))++spells;}
  }
  w.step(.05f);
  if(tick%2400==0){std::cout<<"PROGRESS "<<int(w.time)<<" s: player "<<w.population(0)<<"/"<<w.capacity(0)<<", enemies "<<w.enemyRemaining()<<", gold "<<w.gold<<", wood "<<w.wood<<", phase "<<w.missionStage<<std::endl;}
 }
 if(w.result!=1||w.enemyRemaining()!=0||!w.get(w.archivist)){std::cout<<"ENDING result "<<w.result<<" time "<<w.time<<" units "<<w.population(0)<<" enemies "<<w.enemyRemaining()<<std::endl;throw std::runtime_error("final war full-combat run did not win");}
 if(paidUnits<25||paidBuildings<8||spells==0)throw std::runtime_error("final war did not exercise full economy and combined combat");
 std::cout<<"SUCCESS full final war faction "<<faction<<": "<<paidUnits<<" paid units, "<<paidBuildings<<" paid buildings, "<<spells<<" spells, "<<int(w.time)<<" simulated seconds, no free resources"<<std::endl;}std::cout<<"SUCCESS both human and dark final wars, paid economy only"<<std::endl;return 0;
 }catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<std::endl;return 1;}}
