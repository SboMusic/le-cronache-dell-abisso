#pragma once
namespace alpha {
inline bool World::newFactionAllows(Kind k,int team)const{
 if(team<0||team>1)return false;
 if(k==Kind::Hippogriff||k==Kind::SpiritKnight||k==Kind::Paladin)return faction[team]==0;
 if(k==Kind::Dragon||k==Kind::AbyssBeast||k==Kind::MinorDemon)return faction[team]==1;return true;
}
inline bool World::newKindAllows(Kind k)const{
 if(cheatUnlock||mission<0)return true;
 int unlock=2;if(k==Kind::Stele||k==Kind::RuneForge||k==Kind::RuneBreaker||k==Kind::FreedomObelisk||k==Kind::MinorDemon)unlock=5;
 if(k==Kind::BarrierAltar||k==Kind::Binder||k==Kind::Transfiguration)unlock=8;
 if(k==Kind::Hippogriff||k==Kind::SpiritKnight||k==Kind::Angel||k==Kind::Paladin||k==Kind::AbyssBeast)unlock=9;
 if(k==Kind::Dragon||k==Kind::AirArk)unlock=10;return mission>=unlock;
}
inline float World::localBonus(int team,sf::Vector2f p)const{if(!modern())return 0;float bonus=0;for(const auto& b:entities)if(b.hp>0&&b.team==team&&b.kind==Kind::Stele&&b.build<=0&&distance(b.pos,p)<320+25*b.level)bonus=std::max(bonus,.08f+.02f*b.level);return bonus;}
inline float World::buildingRate(int team)const{if(!modern())return 1;int level=1;for(const auto& b:entities)if(b.hp>0&&b.team==team&&b.kind==Kind::Hall&&b.build<=0)level=std::max(level,b.level);return 1+.15f*(level-1);}
inline void World::seedV2(){
 if(!modern()||mission<2)return;std::vector<sf::Vector2f> halls;for(const auto& e:entities)if(e.kind==Kind::Hall)halls.push_back(e.pos);
 for(auto& e:entities)if(e.team>=0&&e.kind==Kind::Hall)e.level=townLevel[e.team];
 halls.push_back({width()*.47f,height()*.52f});
 for(auto center:halls){bool placed=false;for(int r=3;r<15&&!placed;++r)for(int a=0;a<16&&!placed;++a){auto p=center+sf::Vector2f(std::cos(a*.392699f),std::sin(a*.392699f))*float(r*80);if(p.x<100||p.y<100||p.x>width()-100||p.y>height()-100||blocked(int(p.x/cell),int(p.y/cell)))continue;bool free=true;for(const auto& e:entities)if(e.hp>0&&distance(e.pos,p)<radius(e.kind)+125)free=false;if(free){add(Kind::CrystalCave,-1,p);placed=true;}}}
}
inline bool World::levelUp(int id){auto b=get(id);if(!modern()||!b||b->team<0||mobile(b->kind)||resource(b->kind)||b->build>0||b->production>0||b->level>=4)return false;
 int tier=b->level+1;if(b->kind==Kind::Hall){if(tier==2)return research(id,Research::Town2);if(tier==3)return research(id,Research::Town3);return research(id,Research::Town4);}
 if(!missionResearchAllows(tier>=4?Research::Town4:tier==3?Research::Town3:Research::Town2)||townLevel[b->team]<tier){message="Sviluppa prima il Comando e la campagna.";return false;}
 Cost c{80*tier,55*tier,0,tier==4?40:0};if(!pay(c,b->team))return false;b->paid=c;b->production=b->productionTotal=10+4.f*tier;b->research=-3;message="Potenziamento edificio avviato.";return true;
}
inline bool World::setDuty(int id,int duty,sf::Vector2f p){auto e=get(id);if(!modern()||!e||e->team<0||!mobile(e->kind)||busy(*e)||e->carrier>=0||e->form>=3||duty<0||duty>4)return false;
 if(duty==1&&e->kind!=Kind::Worker)return false;if(duty>=2&&(e->kind==Kind::Worker||e->kind==Kind::CrystalMiner||def(e->kind).damage<=0))return false;
 e->duty=duty;e->anchor=e->pos;e->patrol=p;e->dutyTimer=0;e->order=Order::Idle;e->route.clear();e->target=-1;message=duty==1?"Riparatore automatico: zona locale, riserva minima 30 oro e legno.":duty==2?"Presidio assegnato.":duty==3?"Pattuglia fra la posizione attuale e il punto indicato.":duty==4?"Riserva difensiva: risponde alle torri vicine.":"Ruolo automatico disattivato.";return true;
}
inline bool World::specialEconomy(Entity& e,float dt){
 if(e.kind!=Kind::CrystalMiner)return false;
 if(e.order!=Order::Gather&&e.order!=Order::Return)return true;
 auto t=get(e.target);if(!t){e.order=Order::Idle;e.route.clear();return true;}
 if(!e.route.empty()&&distance(e.pos,t->pos)>130)return true;e.route.clear();
 if(e.order==Order::Gather){if(t->kind!=Kind::CrystalCave){e.order=Order::Idle;return true;}e.work+=dt*buildingRate(e.team);if(e.work<3)return true;e.work=0;e.cargo=std::min(int((8+e.level*2)*(1+localBonus(e.team,e.pos))),int(t->hp));e.cargoKind=Kind::CrystalCave;t->hp-=e.cargo;if(t->hp<=0)navDirty=true;returnCargo(e);}
 else{int& bank=e.team==0?crystals:enemyCrystals;bank=std::min(9999999,bank+e.cargo);e.cargo=0;auto source=get(e.source);if(source)gather(e,source->id);else {e.order=Order::Idle;e.route.clear();}}
 return true;
}
inline bool World::tactical(int id,int action,sf::Vector2f p){
 auto e=get(id);if(!modern()||!e||e->team<0||result||e->build>0||e->stun>0||e->carrier>=0)return false;int team=e->team;
 if(action==0){if(e->kind!=Kind::BarrierAltar||barrierCooldown[team]>0||!pay({0,0,0,20},team))return false;barrierTime[team]=10;barrierCooldown[team]=90;barrierCenter[team]=e->pos;message="Barriera attiva per 10 secondi.";return true;}
 if(action>=1&&action<=5){int rune=action-1;if(e->kind!=Kind::RuneForge||e->production>0||runes[team][rune]>=8||(rune==3&&faction[team]!=0)||(rune==4&&faction[team]!=1))return false;int queued=0;for(const auto& b:entities)if(b.hp>0&&b.team==team&&b.production>0&&b.research==-4-rune)++queued;if(runes[team][rune]+queued>=8)return false;if(!pay({45,0,0,12},team))return false;e->paid={45,0,0,12};e->research=-4-rune;e->production=e->productionTotal=8;message="Runa in preparazione.";return true;}
 if(action>=6&&action<=10){int rune=action-6;if(!mobile(e->kind)||e->form||e->runeTime>0||runes[team][rune]<=0||(rune==3&&faction[team]!=0)||(rune==4&&faction[team]!=1))return false;--runes[team][rune];e->rune=rune;e->runeTime=30;message="Runa assegnata: durata 30 secondi.";return true;}
 auto facility=[&](){int result=-1;for(const auto& b:entities)if(b.hp>0&&b.team==team&&b.kind==Kind::Transfiguration&&b.build<=0&&distance(e->pos,b.pos)<360){result=b.id;break;}return result;};
 if(action==11){if(!mobile(e->kind)||def(e->kind).domain!=Domain::Land||def(e->kind).damage<=0||e->kind==Kind::Worker||e->kind==Kind::Siege||e->kind==Kind::Demolition||e->form||e->runeTime>0||e->recovery>0||facility()<0||!pay({0,0,0,15},team))return false;e->originTeam=team;e->form=1;e->transTime=25;message="Trasfigurazione: 25 s di potere, poi instabilita. Cura al santuario!";return true;}
 if(action==12){int site=facility();if(e->form<=0||e->form==4||site<0)return false;int patients=0;for(const auto& t:entities)if(t.hp>0&&t.form==4&&t.target==site)++patients;if(patients>=2)return false;
  if(e->form==3){int escorts=0;for(const auto& t:entities)if(t.hp>0&&t.team==team&&!t.form&&mobile(t.kind)&&def(t.kind).damage>0&&distance(t.pos,e->pos)<160)++escorts;if(escorts<2){message="Servono due combattenti vicini per contenere il corrotto.";return false;}}
  if(!pay({60,40,0},team))return false;e->form=4;e->transTime=18;e->target=site;e->order=Order::Move;e->route.clear();path(*e,get(site)->pos,120);e->duty=0;e->channel=0;message="Contenimento e cura: 18 s presso la struttura, poi 240 s di recupero.";return true;}
 if(action==13||action==14){Kind required=action==13?Kind::Binder:Kind::RuneBreaker;if(e->kind!=required||e->abilityCooldown>0||e->channel>0||distance(e->pos,p)>360||!visibleAt(p,team))return false;if(action==13&&e->mana<60)return false;if(action==13)e->mana-=60;e->abilityTarget=p;e->channel=action==13?1.5f:2.f;e->abilityCooldown=action==13?45.f:30.f;e->order=Order::Idle;e->route.clear();message="Rituale annunciato: movimento, danni e paralisi lo interrompono.";return true;}
 return false;
}
inline void World::updateV2(float dt){
 if(!modern())return;for(int team=0;team<2;++team){barrierTime[team]=std::max(0.f,barrierTime[team]-dt);barrierCooldown[team]=std::max(0.f,barrierCooldown[team]-dt);}
 for(auto& e:entities){if(e.hp<=0||e.team<0||(cheatFreeze&&e.team==1))continue;
  e.abilityCooldown=std::max(0.f,e.abilityCooldown-dt);e.recovery=std::max(0.f,e.recovery-dt);e.stunImmunity=std::max(0.f,e.stunImmunity-dt);e.stun=std::max(0.f,e.stun-dt);e.runeTime=std::max(0.f,e.runeTime-dt);if(!e.runeTime)e.rune=-1;
  for(const auto& b:entities)if(b.hp>0&&b.team==e.team&&b.kind==Kind::FreedomObelisk&&b.build<=0&&distance(b.pos,e.pos)<300){e.stun=0;e.stunImmunity=12;break;}
  if(e.form==1||e.form==2){e.transTime-=dt;if(e.transTime<=0){++e.form;e.transTime=e.form==2?10.f:0.f;if(e.form==3){e.selected=false;e.duty=0;e.order=Order::Idle;e.route.clear();message="Unita corrotta ostile! Due scorte e un santuario possono recuperarla.";}}}
  if(e.form==4){auto b=get(e.target);if(!b||b->kind!=Kind::Transfiguration){e.form=2;e.transTime=10;e.order=Order::Idle;}else if(distance(e.pos,b->pos)<=135){e.route.clear();e.transTime-=dt;if(e.transTime<=0){e.form=0;e.originTeam=-1;e.recovery=240;e.rage=e.haste=0;e.order=Order::Idle;e.target=-1;message="Unita guarita: recupero di 240 secondi.";}}else if(e.route.empty())path(e,b->pos,120);continue;}
  if(e.stun>0){e.channel=0;continue;}
  if(e.channel>0){if(!e.route.empty()||e.carrier>=0){e.channel=0;continue;}e.channel=std::max(0.f,e.channel-dt);if(e.channel==0){if(e.kind==Kind::Binder){for(auto& t:entities)if(t.hp>0&&t.team>=0&&t.team!=e.team&&mobile(t.kind)&&def(t.kind).domain==Domain::Land&&t.carrier<0&&distance(t.pos,e.abilityTarget)<110&&t.stunImmunity<=0){bool safe=false;for(const auto& b:entities)if(b.hp>0&&b.team==t.team&&b.kind==Kind::FreedomObelisk&&b.build<=0&&distance(b.pos,t.pos)<300)safe=true;if(!safe){t.stun=t.lifetime==-777?3.f:10.f;t.stunImmunity=t.stun+12;}}}
   else if(e.kind==Kind::RuneBreaker){int other=1-e.team;if(distance(e.abilityTarget,barrierCenter[other])<320)barrierTime[other]=0;for(auto& t:entities)if(t.team!=e.team&&t.team>=0&&distance(t.pos,e.abilityTarget)<130){t.rune=-1;t.runeTime=0;}}
   effects.push_back({e.abilityTarget,110,.7f,.7f,3,e.team});}continue;}
  if(e.kind==Kind::CrystalMiner&&e.order==Order::Idle&&e.cargo>0){returnCargo(e);continue;}
  if(e.kind==Kind::CrystalMiner&&e.order==Order::Idle){int source=-1;float nearest=1e9;for(const auto& n:entities)if(n.hp>0&&n.kind==Kind::CrystalCave&&distance(e.pos,n.pos)<nearest){nearest=distance(e.pos,n.pos);source=n.id;}if(source>=0)gather(e,source);}
  if(e.form==3){int target=-1;float nearest=400;for(const auto& t:entities)if(t.id!=e.id&&t.hp>0&&t.team>=0&&t.carrier<0&&distance(t.pos,e.pos)<nearest){nearest=distance(t.pos,e.pos);target=t.id;}if(target>=0&&(e.order!=Order::Attack||e.target!=target))attack(e,target);continue;}
  if(e.duty<=0||busy(e)||e.carrier>=0)continue;e.dutyTimer-=dt;if(e.dutyTimer>0)continue;e.dutyTimer=1;
  if(e.duty==1){if(e.order==Order::Repair)continue;int g=e.team==0?gold:enemyGold,w=e.team==0?wood:enemyWood;if(g<30||w<30)continue;int target=-1;float nearest=480;for(const auto& b:entities)if(b.hp>0&&b.team==e.team&&!mobile(b.kind)&&!resource(b.kind)&&b.build<=0&&b.hp<b.maxHp&&distance(b.pos,e.anchor)<480&&distance(e.pos,b.pos)<nearest){nearest=distance(e.pos,b.pos);target=b.id;}if(target>=0)repair(e,target);else if(distance(e.pos,e.anchor)>70){e.order=Order::Move;path(e,e.anchor,40);}continue;}
  int target=-1;float radius=e.duty==4?650.f:320.f;bool alarm=e.duty!=4;
  if(!alarm)for(const auto& tower:entities)if(tower.hp>0&&tower.team==e.team&&(tower.kind==Kind::WatchTower||tower.kind==Kind::GuardTower||tower.kind==Kind::CannonTower)&&distance(tower.pos,e.anchor)<650)for(const auto& foe:entities)if(foe.hp>0&&foe.team>=0&&foe.team!=e.team&&visibleAt(foe.pos,e.team)&&detected(foe,e.team)&&distance(foe.pos,tower.pos)<420)alarm=true;
  if(alarm)for(const auto& foe:entities)if(canHit(e,foe)&&visibleAt(foe.pos,e.team)&&distance(foe.pos,e.anchor)<radius){target=foe.id;break;}
  if(target>=0)attack(e,target);else if(e.order!=Order::Move||e.route.empty()){sf::Vector2f destination=e.anchor;if(e.duty==3){destination=distance(e.pos,e.patrol)<70?e.anchor:e.patrol;}if(distance(e.pos,destination)>60){e.order=Order::Move;e.target=-1;path(e,destination,30);}else if(e.duty!=3){e.order=Order::Idle;e.target=-1;e.route.clear();}}
 }
 // Paid V2 enemy economy, using the same commands and resource banks as the player.
 v2AiTimer-=dt;if(v2AiTimer>0||mission<2||!raidsEnabled||cheatFreeze)return;v2AiTimer=3;auto notice=message;
 int hall=-1,worker=-1;bool site=false;for(const auto& e:entities)if(e.hp>0&&e.team==1){if(e.kind==Kind::Hall&&e.build<=0)hall=e.id;if(e.kind==Kind::Worker&&!busy(e))worker=e.id;if(e.build>0)site=true;}if(hall<0){message=notice;return;}
 if(worker>=0&&!site){for(auto kind:{Kind::Reliquary,Kind::Stele,Kind::RuneForge,Kind::FreedomObelisk,Kind::BarrierAltar,Kind::Transfiguration})if(missionAllows(kind)&&!has(1,kind)&&unavailable(kind,1).empty()){bool done=false;for(int r=3;r<9&&!done;++r)for(int a=0;a<16&&!done;++a){auto p=get(hall)->pos+sf::Vector2f(std::cos(a*.392699f),std::sin(a*.392699f))*float(r*90);if(placementIssue(p,kind).empty()&&!water(p))done=build(worker,p,kind);}break;}}
 std::vector<int> producers;for(const auto& e:entities)if(e.hp>0&&e.team==1&&e.build<=0&&e.production<=0)producers.push_back(e.id);
 for(int id:producers){auto b=get(id);if(!b)continue;if(b->kind==Kind::Reliquary){int n=0;for(const auto& t:entities)if(t.hp>0&&t.team==1&&t.kind==Kind::CrystalMiner)++n;if(n<3)train(id,Kind::CrystalMiner);}
  if(b->kind==Kind::Hall&&mission>=8){if(townLevel[1]<3)research(id,Research::Town3);else if(townLevel[1]<4)research(id,Research::Town4);}
  if(b->kind==Kind::RuneForge)tactical(id,2,{});
  if(b->kind==Kind::BarrierAltar){bool enemy=false;for(const auto& t:entities)if(t.hp>0&&t.team==0&&visibleAt(t.pos,1)&&distance(t.pos,b->pos)<350)enemy=true;if(enemy)tactical(id,0,{});}
  if(mission>=9)for(auto k:{Kind::Paladin,Kind::MinorDemon,Kind::SpiritKnight,Kind::AbyssBeast,Kind::Hippogriff,Kind::Dragon,Kind::Angel,Kind::RuneBreaker,Kind::Binder})if(def(k).producer==b->kind&&newFactionAllows(k,1)&&missionAllows(k)){if(train(id,k))break;}
 }
 // Naval reinforcements and landings use paid transports and actual passengers.
 if(mission==7||mission==10){int port=-1,enemyPort=-1,transport=-1;for(const auto& b:entities)if(b.hp>0){if(b.kind==Kind::Shipyard){if(b.team==1&&b.build<=0)port=b.id;if(b.team==0)enemyPort=b.id;}if(b.team==1&&b.kind==Kind::Transport)transport=b.id;}
  if(port>=0&&transport<0)train(port,Kind::Transport);
  if(transport>=0&&enemyPort>=0){auto ship=get(transport);int aboard=0;for(const auto& p:entities)if(p.hp>0&&p.carrier==transport)++aboard;
   if(aboard<3&&ship->order==Order::Idle){int boarding=0;for(auto& p:entities)if(boarding<3-aboard&&p.hp>0&&p.team==1&&p.kind==Kind::Soldier&&p.carrier<0&&p.form==0&&distance(p.pos,ship->pos)<650){board(p,transport);++boarding;}}
   if(aboard>=3&&ship->order!=Order::Move)move(*ship,get(enemyPort)->pos);
   if(aboard>0&&distance(ship->pos,get(enemyPort)->pos)<350)for(int a=0;a<16;++a){auto p=ship->pos+sf::Vector2f(std::cos(a*.392699f),std::sin(a*.392699f))*170.f;if(!water(p)&&!blocked(int(p.x/cell),int(p.y/cell))&&unload(transport,p))break;}
  }
 }
 for(auto& e:entities)if(e.hp>0&&e.team==1&&e.carrier<0&&mobile(e.kind)&&e.stun<=0){int foe=-1;for(const auto& t:entities)if(t.hp>0&&t.team==0&&visibleAt(t.pos,1)&&distance(t.pos,e.pos)<320){foe=t.id;break;}if(foe>=0){if(e.form==0&&e.recovery==0)tactical(e.id,11,{});if(e.kind==Kind::Binder)tactical(e.id,13,get(foe)->pos);if(e.kind==Kind::RuneBreaker)tactical(e.id,14,get(foe)->pos);if(runes[1][1]>0)tactical(e.id,7,{});}if(e.form==2)tactical(e.id,12,{});}
 message=notice;
}
}
