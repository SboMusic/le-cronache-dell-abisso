#pragma once
namespace alpha {
inline void World::warEnemyEconomy(){
 std::string saved=message;std::vector<int> halls,producers,workers;bool constructing=false;int army=0;
 for(const auto& e:entities)if(e.hp>0&&e.team==1){if(e.kind==Kind::Hall&&e.build<=0)halls.push_back(e.id);if(e.kind==Kind::Worker){if(!busy(e))workers.push_back(e.id);}else if(mobile(e.kind)&&e.kind!=Kind::Tanker&&e.kind!=Kind::CrystalMiner)++army;if(e.build>0)constructing=true;if(!mobile(e.kind)&&e.build<=0)producers.push_back(e.id);}
 if(halls.empty())return;
 const int limit=mission==10?48:mission==9?36:mission==8?28:mission==7?24:mission==6?20:14;
 auto buildNear=[&](Kind kind){if(constructing||workers.empty())return false;for(int id:halls){auto anchor=get(id);if(!anchor)continue;for(int y=-350;y<=490;y+=140)for(int x=-490;x<=490;x+=140){auto p=anchor->pos+sf::Vector2f(float(x),float(y));if(p.x<100||p.x>width()-100||p.y<100||p.y>height()-100||water(p)||!placementIssue(p,kind).empty())continue;for(int worker:workers)if(build(worker,p,kind))return true;}}return false;};
 if(!has(1,Kind::Farm)||(capacity(1)<90&&population(1)+4>capacity(1)))buildNear(Kind::Farm);
 else if(!has(1,Kind::Barracks))buildNear(Kind::Barracks);
 else if(!has(1,Kind::LumberMill))buildNear(Kind::LumberMill);
 else if(!has(1,Kind::Forge))buildNear(Kind::Forge);
 else if(!has(1,Kind::Stable))buildNear(Kind::Stable);
 else if(mission>=6&&!has(1,Kind::Workshop))buildNear(Kind::Workshop);
 else if(mission>=8&&!has(1,Kind::Academy))buildNear(Kind::Academy);
 else if(mission>=9&&!has(1,Kind::Aviary))buildNear(Kind::Aviary);
 int countWorkers=0;for(auto& e:entities)if(e.hp>0&&e.team==1&&e.kind==Kind::Worker)++countWorkers;
 for(int id:halls)if(auto h=get(id)){if(h->production<=0&&countWorkers<(mission==10?10:6)&&has(1,Kind::Farm)){if(train(id,Kind::Worker))++countWorkers;}}
 for(int id:producers){auto b=get(id);if(!b||b->production>0)continue;
  if(b->kind==Kind::Barracks&&b->level<2&&has(1,Kind::LumberMill)){research(id,Research::Barracks2);continue;}
  if(b->kind==Kind::Barracks&&b->level==2&&has(1,Kind::Sanctuary)){research(id,Research::Barracks3);continue;}
  if(army>=limit)continue;Kind kind=Kind::Count;
  if(b->kind==Kind::Barracks)kind=b->level>=3?(army%3==0?Kind::Soldier:Kind::Ranger):b->level>=2?Kind::Archer:Kind::Soldier;
  if(b->kind==Kind::Stable)kind=Kind::Knight;
  if(b->kind==Kind::Workshop)kind=army%3?Kind::Siege:Kind::Demolition;
  if(b->kind==Kind::Sanctuary)kind=townLevel[1]>=3&&army%3?Kind::Guardian:Kind::Healer;
  if(b->kind==Kind::Academy)kind=faction[1]==0?Kind::Mage:Kind::Warlock;
  if(b->kind==Kind::Aviary)kind=Kind::Warbird;
  if(mission==7&&b->kind==Kind::Shipyard){int tankers=0,ships=0;for(auto& e:entities)if(e.hp>0&&e.team==1){if(e.kind==Kind::Tanker)++tankers;if(def(e.kind).domain==Domain::Sea&&mobile(e.kind)&&e.kind!=Kind::Tanker)++ships;}if(tankers==0)kind=Kind::Tanker;else if(ships<4)kind=ships%3==0?Kind::Submarine:ships%3==1?Kind::Destroyer:Kind::Battleship;}
  if(kind!=Kind::Count&&train(id,kind))++army;
 }
 for(auto& e:entities)if(e.hp>0&&e.team==1&&!busy(e)&&e.order==Order::Idle){if(e.kind==Kind::Worker){int source=-1;float nearest=1e9;Kind wanted=e.id%2?Kind::Mine:Kind::Forest;for(auto& resource:entities)if(resource.hp>0&&resource.kind==wanted&&distance(e.pos,resource.pos)<nearest){nearest=distance(e.pos,resource.pos);source=resource.id;}if(source>=0)gather(e,source);}
  if(e.kind==Kind::Tanker){int rig=-1;for(auto& r:entities)if(r.hp>0&&r.team==1&&r.kind==Kind::OilRig&&r.build<=0)rig=r.id;if(rig>=0)gather(e,rig);}
 }
 // Coordinated paid reinforcements: no spawned waves or income grants.
 if(missionStage>=1&&time>=wave){int target=-1;for(auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Hall){target=e.id;break;}int dispatched=0;int group=mission==10?10:mission==9?10:mission==8?8:mission==6?6:4;if(target>=0){for(auto& e:entities)if(dispatched<group&&e.hp>0&&e.team==1&&e.carrier<0&&mobile(e.kind)&&e.kind!=Kind::Worker&&e.kind!=Kind::CrystalMiner&&e.kind!=Kind::Tanker&&e.order==Order::Idle){if(def(e.kind).domain==Domain::Sea){int port=-1;for(auto& p:entities)if(p.hp>0&&p.team==0&&p.kind==Kind::Shipyard){port=p.id;break;}if(port>=0)attack(e,port);}else if(e.kind==Kind::Healer)move(e,get(target)->pos+sf::Vector2f(300,0));else attack(e,target);++dispatched;}}wave=time+(mission==10?75.f:mission>=8?75.f:100.f);}
 if(mission>=8){std::vector<int> casters;for(auto& e:entities)if(e.hp>0&&e.team==1&&(e.kind==Kind::Warlock||e.kind==Kind::Mage)&&e.mana>=75&&e.spellCooldown<=0)casters.push_back(e.id);for(int id:casters){auto mage=get(id);if(!mage)continue;for(const auto& target:entities)if(target.hp>0&&target.team==0&&mobile(target.kind)&&distance(target.pos,mage->pos)<430){cast(id,mage->kind==Kind::Mage?Spell::Firestorm:Spell::Storm,target.pos);break;}}}
 message=saved;
}
}
