#pragma once
#include <limits>
#include <locale>
namespace alpha {
// Explicit, versioned fields: no pointer addresses, struct padding or binary memory dumps.
#define FRONTIERE_ENTITY_FIELDS(e) e.id,e.team,e.target,e.objective,e.source,e.cargo,e.carrier,e.pos.x,e.pos.y,e.goal.x,e.goal.y,e.hp,e.maxHp,e.cooldown,e.build,e.buildTotal,e.production,e.productionTotal,e.work,e.mana,e.maxMana,e.spellCooldown,e.haste,e.rage,e.shield,e.reveal,e.lifetime,e.research,e.level,e.constructionWorker,e.paid.gold,e.paid.wood,e.paid.oil,e.selected
inline bool World::save(std::ostream& out)const{
 out.imbue(std::locale::classic());out<<std::setprecision(std::numeric_limits<float>::max_digits10)<<"FRONTIERE_SAVE "<<(modern()?6:4)<<'\n';
 out<<cols<<' '<<rows<<'\n';
 out<<cheatsUsed<<' '<<cheatGod<<' '<<cheatFast<<' '<<cheatFreeze<<' '<<cheatFree<<' '<<cheatUnlock<<' '<<cheatSupply<<'\n';
 auto put=[&](auto... values){((out<<values<<' '),...);out<<'\n';};
 put(mission,missionStage,deliveredGold,deliveredWood,archivist,secureTime,nextId,accumulator,gold,wood,oil,result,enemyGold,enemyWood,enemyOil,guided,chapter,aiTimer,time,wave,raidsEnabled,fogEnabled,visionTimer);
 put(townLevel[0],townLevel[1],faction[0],faction[1]);for(auto& team:upgrades)for(int n=0;n<(modern()?int(Research::Count):int(Research::Town4));++n)out<<team[n]<<' ';out<<'\n';
 out<<std::quoted(message)<<'\n';for(int i=0;i<cols*rows;++i)out<<int(terrain[i])<<' ';out<<'\n';for(auto& team:explored)for(int i=0;i<cols*rows;++i)out<<int(team[i])<<' ';out<<'\n';for(auto& team:sight)for(int i=0;i<cols*rows;++i)out<<int(team[i])<<' ';out<<'\n';
 put(missionGuards.size());for(int id:missionGuards)out<<id<<' ';out<<'\n';put(entities.size());
 for(const auto& e:entities){put(int(e.kind),int(e.training),int(e.cargoKind),(!modern()&&e.upgradeKind==Kind::Count?int(Kind::CrystalCave):int(e.upgradeKind)),int(e.order));put(FRONTIERE_ENTITY_FIELDS(e));if(modern())put(e.paid.crystals,e.duty,e.rune,e.originTeam,e.form,e.anchor.x,e.anchor.y,e.patrol.x,e.patrol.y,e.abilityTarget.x,e.abilityTarget.y,e.dutyTimer,e.abilityCooldown,e.stun,e.stunImmunity,e.runeTime,e.transTime,e.recovery,e.channel);put(e.route.size());for(auto p:e.route)put(p.x,p.y);}
 put(shots.size());for(auto& s:shots){put(s.pos.x,s.pos.y,s.target,s.team,s.damage,s.splash,s.remaining,int(s.source));if(modern())put(s.rogue);}
 put(effects.size());for(auto& e:effects)put(e.pos.x,e.pos.y,e.radius,e.remaining,e.total,e.type,e.team);
 put(zones.size());for(auto& z:zones)put(z.pos.x,z.pos.y,z.remaining,z.tick,z.team);if(layoutVersion)put("LAYOUT",layoutVersion,mapGoal.x,mapGoal.y);if(modern()){put("V2",crystals,enemyCrystals,v2AiTimer);for(int team=0;team<2;++team){put(barrierTime[team],barrierCooldown[team],barrierCenter[team].x,barrierCenter[team].y);for(int value:runes[team])out<<value<<' ';out<<'\n';}}out<<"END\n";return bool(out);
}
inline bool World::restore(std::istream& in){
 in.imbue(std::locale::classic());World w;std::string magic;int version=0;if(!(in>>magic>>version)||magic!="FRONTIERE_SAVE"||(version<1||version>6))return false;
 int columns=64,rowCount=40;
 if(version>=2&&!(in>>columns>>rowCount))return false;
 if(columns<64||rowCount<40||columns>(version<4?96:maxMapCols)||rowCount>(version<4?60:maxMapRows))return false;
 w.resizeMap(columns,rowCount);
 if(version>=3&&!(in>>w.cheatsUsed>>w.cheatGod>>w.cheatFast>>w.cheatFreeze>>w.cheatFree>>w.cheatUnlock>>w.cheatSupply))return false;
 auto read=[&](auto&... values){return bool((in>>...>>values));};auto finite=[](float v){return std::isfinite(v)&&std::abs(v)<10000000.f;};auto position=[&](sf::Vector2f p){return finite(p.x)&&finite(p.y)&&p.x>=0&&p.x<=w.width()&&p.y>=0&&p.y<=w.height();};
 if(!read(w.mission,w.missionStage,w.deliveredGold,w.deliveredWood,w.archivist,w.secureTime,w.nextId,w.accumulator,w.gold,w.wood,w.oil,w.result,w.enemyGold,w.enemyWood,w.enemyOil,w.guided,w.chapter,w.aiTimer,w.time,w.wave,w.raidsEnabled,w.fogEnabled,w.visionTimer))return false;
 if(w.mission < -1||w.mission>=missionCount||w.missionStage<0||w.missionStage>2||w.chapter<0||w.chapter>4||w.result < -1||w.result>1||w.nextId<1||w.nextId>1000000||w.deliveredGold<0||w.deliveredWood<0)return false;
 for(int bank:{w.gold,w.wood,w.oil,w.enemyGold,w.enemyWood,w.enemyOil})if(bank<0||bank>10000000)return false;
 for(float value:{w.secureTime,w.accumulator,w.aiTimer,w.time,w.wave,w.visionTimer})if(!finite(value))return false;
 if(!read(w.townLevel[0],w.townLevel[1],w.faction[0],w.faction[1]))return false;for(int team=0;team<2;++team){if(w.townLevel[team]<1||w.townLevel[team]>(version>=6?4:3)||w.faction[team]<0||w.faction[team]>1)return false;for(int n=0;n<(version>=6?int(Research::Count):int(Research::Town4));++n)if(!read(w.upgrades[team][n])||w.upgrades[team][n]<0||w.upgrades[team][n]>researchCatalog()[n].maxLevel)return false;}
 if(!(in>>std::quoted(w.message))||w.message.size()>4096)return false;
 for(int i=0;i<w.cols*w.rows;++i){auto& value=w.terrain[i];int n;if(!read(n)||n<0||n>3)return false;value=Terrain(n);}for(auto* grid:{&w.explored,&w.sight})for(auto& team:*grid)for(int i=0;i<w.cols*w.rows;++i){auto& value=team[i];int n;if(!read(n)||n<0||n>1)return false;value=static_cast<unsigned char>(n);}
 int count;if(!read(count)||count<0||count>64)return false;for(int i=0;i<count;++i){int id;if(!read(id)||id<1)return false;w.missionGuards.push_back(id);}
 if(!read(count)||count<1||count>4096)return false;std::unordered_set<int> ids;int maximum=0;
 for(int i=0;i<count;++i){Entity e;int kind,training,cargo,upgrade,order;if(!read(kind,training,cargo,upgrade,order))return false;if(kind<0||kind>=int(Kind::Count)||training<0||training>=int(Kind::Count)||cargo<0||cargo>=int(Kind::Count)||upgrade<0||upgrade>int(Kind::Count)||order<0||order>int(Order::Build))return false;
  if(version<6&&upgrade==int(Kind::CrystalCave))upgrade=int(Kind::Count);
  e.kind=Kind(kind);e.training=Kind(training);e.cargoKind=Kind(cargo);e.upgradeKind=Kind(upgrade);e.order=Order(order);if(!read(FRONTIERE_ENTITY_FIELDS(e)))return false;
  if(e.id<1||!ids.insert(e.id).second||e.team < -1||e.team>1||e.level<1||e.level>(version>=6?4:3)||e.research < (version>=6?-8:-2)||e.research>=int(Research::Count)||e.cargo<0||e.cargo>1000||!position(e.pos)||!position(e.goal))return false;
  for(float value:{e.hp,e.maxHp,e.cooldown,e.build,e.buildTotal,e.production,e.productionTotal,e.work,e.mana,e.maxMana,e.spellCooldown,e.haste,e.rage,e.shield,e.reveal,e.lifetime})if(!finite(value))return false;
  if(e.maxHp<=0||e.hp>e.maxHp||e.build<0||e.production<0||e.paid.gold<0||e.paid.wood<0||e.paid.oil<0||e.selected&&e.team!=0)return false;
  if(version>=6){if(!read(e.paid.crystals,e.duty,e.rune,e.originTeam,e.form,e.anchor.x,e.anchor.y,e.patrol.x,e.patrol.y,e.abilityTarget.x,e.abilityTarget.y,e.dutyTimer,e.abilityCooldown,e.stun,e.stunImmunity,e.runeTime,e.transTime,e.recovery,e.channel))return false;
   if(e.paid.crystals<0||e.duty<0||e.duty>4||e.rune < -1||e.rune>4||e.originTeam < -1||e.originTeam>1||e.form<0||e.form>4||!position(e.anchor)||!position(e.patrol)||!position(e.abilityTarget))return false;
   for(float value:{e.dutyTimer,e.abilityCooldown,e.stun,e.stunImmunity,e.runeTime,e.transTime,e.recovery,e.channel})if(!finite(value)||value < -5)return false;
  }
  int steps;if(!read(steps)||steps<0||steps>w.cols*w.rows+1)return false;for(int j=0;j<steps;++j){sf::Vector2f p;if(!read(p.x,p.y)||!position(p))return false;e.route.push_back(p);}maximum=std::max(maximum,e.id);w.entities.push_back(e);
 }
 if(w.nextId<=maximum)return false;for(int id:w.missionGuards)if(!ids.count(id))return false;if(w.archivist>=0&&!ids.count(w.archivist))return false;
 if(!read(count)||count<0||count>10000)return false;for(int i=0;i<count;++i){Shot s;int source;if(!read(s.pos.x,s.pos.y,s.target,s.team,s.damage,s.splash,s.remaining,source)||!position(s.pos)||s.team<0||s.team>1||source<0||source>=int(Kind::Count)||!finite(s.damage)||!finite(s.splash)||!finite(s.remaining))return false;s.source=Kind(source);if(version>=6&&!read(s.rogue))return false;w.shots.push_back(s);}
 if(!read(count)||count<0||count>10000)return false;for(int i=0;i<count;++i){Effect e;if(!read(e.pos.x,e.pos.y,e.radius,e.remaining,e.total,e.type,e.team)||!position(e.pos)||!finite(e.radius)||!finite(e.remaining)||!finite(e.total)||e.total<=0||e.team < -1||e.team>1)return false;w.effects.push_back(e);}
 if(!read(count)||count<0||count>10000)return false;for(int i=0;i<count;++i){Zone z;if(!read(z.pos.x,z.pos.y,z.remaining,z.tick,z.team)||!position(z.pos)||!finite(z.remaining)||!finite(z.tick)||z.team<0||z.team>1)return false;w.zones.push_back(z);}
 if(version>=5){if(!(in>>magic)||magic!="LAYOUT"||!read(w.layoutVersion,w.mapGoal.x,w.mapGoal.y)||w.layoutVersion!=1||!position(w.mapGoal))return false;}if(version>=6){if(!(in>>magic)||magic!="V2"||!read(w.crystals,w.enemyCrystals,w.v2AiTimer)||w.crystals<0||w.enemyCrystals<0||w.crystals>10000000||w.enemyCrystals>10000000||!finite(w.v2AiTimer))return false;
  for(int team=0;team<2;++team){if(!read(w.barrierTime[team],w.barrierCooldown[team],w.barrierCenter[team].x,w.barrierCenter[team].y)||!position(w.barrierCenter[team])||!finite(w.barrierTime[team])||w.barrierTime[team]<0||w.barrierTime[team]>10||!finite(w.barrierCooldown[team])||w.barrierCooldown[team]<0||w.barrierCooldown[team]>90)return false;for(auto& value:w.runes[team])if(!read(value)||value<0||value>8)return false;}
 }if(!(in>>magic)||magic!="END")return false;in>>std::ws;if(!in.eof())return false;if(version==5)w.seedV2();w.navDirty=true;*this=std::move(w);return true;
}
#undef FRONTIERE_ENTITY_FIELDS
}
