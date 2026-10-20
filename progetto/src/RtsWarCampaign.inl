#pragma once
namespace alpha {
inline int World::enemyRemaining(bool buildings)const{int n=0;for(const auto& e:entities)if(e.hp>0&&e.team==1&&(!buildings||!mobile(e.kind)))++n;return n;}
inline void World::startWarMission(int n){
 reset(0);mission=n;guided=false;entities.clear();shots.clear();effects.clear();zones.clear();nextId=1;cols=96;rows=60;terrain.fill(Terrain::Grass);explored={};sight={};navDirty=true;
 gold=n==10?1600:1000;wood=n==10?1100:700;oil=0;enemyGold=n==10?2400:700+(n-5)*180;enemyWood=n==10?1600:450+(n-5)*100;enemyOil=0;wave=n==10?420.f:240.f;raidsEnabled=true;
 const sf::Vector2f bases[]={{420,1260},{420,1660},{420,920},{430,1780},{440,1660},{440,1720}};auto base=bases[n-5];sf::Vector2f enemy=n==6?sf::Vector2f(3180,470):n==7?sf::Vector2f(3020,550):n==9?sf::Vector2f(3300,500):sf::Vector2f(3100,500);
 auto road=missionRoad(n);auto segment=[](sf::Vector2f p,sf::Vector2f a,sf::Vector2f b){auto d=b-a;float l=d.x*d.x+d.y*d.y;float t=l?std::clamp(((p.x-a.x)*d.x+(p.y-a.y)*d.y)/l,0.f,1.f):0;return distance(p,a+d*t);};
 for(int y=0;y<rows;++y)for(int x=0;x<cols;++x){sf::Vector2f p{(x+.5f)*cell,(y+.5f)*cell};bool route=false;for(int j=1;j<int(road.size());++j)if(segment(p,road[j-1],road[j])<150)route=true;
  bool rock=n==5?((x>=24&&x<=28&&y<18)||(x>=47&&x<=51&&y>35)):n==6?((x>=35&&x<=39&&y<21)||(x>=61&&x<=64&&y>34)):n==8?((x>=25&&x<=30&&y<26)||(x>=47&&x<=52&&y>38)):n==9?((x>=40&&x<=43&&y<20)||(x>=65&&x<=68&&y>39)):false;
  terrain[x+y*cols]=route?Terrain::Road:rock?Terrain::Rock:Terrain::Grass;
  if(n==7&&y>=35)terrain[x+y*cols]=(x>=52&&x<=71&&y>=45&&y<=55)?Terrain::Grass:Terrain::Water;
  if(n==10&&x>=45&&x<=49&&y>3&&y<57&&!(y>=18&&y<=24)&&!(y>=39&&y<=46))terrain[x+y*cols]=Terrain::Water;
 }
 auto place=[&](Kind k,int team,sf::Vector2f p){for(int y=std::max(0,int((p.y-90)/cell));y<=std::min(rows-1,int((p.y+90)/cell));++y)for(int x=std::max(0,int((p.x-90)/cell));x<=std::min(cols-1,int((p.x+90)/cell));++x)if(terrain[x+y*cols]==Terrain::Rock)terrain[x+y*cols]=Terrain::Grass;return add(k,team,p);};
 int hall=place(Kind::Hall,0,base);townLevel[0]=n>=9?3:2;upgrades[0][int(Research::Town2)]=1;if(townLevel[0]==3)upgrades[0][int(Research::Town3)]=1;get(hall)->maxHp=get(hall)->hp=townLevel[0]==3?1800.f:1400.f;
 int workers=n==10?7:6;for(int i=0;i<workers;++i)add(Kind::Worker,0,base+sf::Vector2f(130.f,float(i*35-110)));
 for(int i=0;i<(n==10?4:3);++i)place(Kind::Farm,0,base+sf::Vector2f(-220.f+i*150,-230));
 int b=place(Kind::Barracks,0,base+sf::Vector2f(330,0));get(b)->level=n>=6?3:2;place(Kind::LumberMill,0,base+sf::Vector2f(-150,240));place(Kind::Forge,0,base+sf::Vector2f(130,-350));place(Kind::GuardTower,0,base+sf::Vector2f(570,-80));
 if(n>=6){place(Kind::Stable,0,base+sf::Vector2f(-180,-420));place(Kind::Sanctuary,0,base+sf::Vector2f(360,-260));upgrades[0][int(Research::Elite)]=1;upgrades[0][int(Research::Barracks3)]=1;}
 if(n>=7&&n<10)place(Kind::Workshop,0,base+sf::Vector2f(560,-320));
 if(n>=9){place(Kind::Academy,0,base+sf::Vector2f(560,-320));upgrades[0][int(Research::Arcana)]=1;upgrades[0][int(Research::Weapons)]=upgrades[0][int(Research::Armor)]=upgrades[0][int(Research::Archery)]=1;}
 for(int i=0;i<(n==10?5:4);++i)add(Kind::Soldier,0,base+sf::Vector2f(180.f+i*43,140));for(int i=0;i<(n==10?4:2);++i)add(n>=6?Kind::Ranger:Kind::Archer,0,base+sf::Vector2f(190.f+i*45,210));if(n>=6){add(Kind::Knight,0,base+sf::Vector2f(420,180));add(Kind::Healer,0,base+sf::Vector2f(140,200));}
 if(n==10){add(Kind::Knight,0,base+sf::Vector2f(460,180));}
 for(auto p:{base+sf::Vector2f(-170,40),base+sf::Vector2f(120,420),sf::Vector2f(1150,420),sf::Vector2f(1050,2140),sf::Vector2f(2400,330),sf::Vector2f(3500,1240),sf::Vector2f(3460,2230)})if(!water(p))place(Kind::Mine,-1,p);
 int enemyTier=n>=8?3:2;townLevel[1]=enemyTier;upgrades[1][int(Research::Town2)]=1;if(enemyTier==3)upgrades[1][int(Research::Town3)]=1;upgrades[1][int(Research::Elite)]=1;upgrades[1][int(Research::Weapons)]=upgrades[1][int(Research::Armor)]=upgrades[1][int(Research::Archery)]=n>=9?2:1;if(n>=8)upgrades[1][int(Research::Arcana)]=n==10?2:1;
 int eh=place(Kind::Hall,1,enemy);get(eh)->maxHp=get(eh)->hp=enemyTier==3?1800.f:1400.f;
 for(int i=0;i<(n==10?5:3);++i)place(Kind::Farm,1,enemy+sf::Vector2f(-260.f+i*140,-220));
 int eb=place(Kind::Barracks,1,enemy+sf::Vector2f(-230,100));get(eb)->level=3;place(Kind::LumberMill,1,enemy+sf::Vector2f(150,130));place(Kind::Forge,1,enemy+sf::Vector2f(-220,-120));place(Kind::Stable,1,enemy+sf::Vector2f(-90,300));place(Kind::Sanctuary,1,enemy+sf::Vector2f(280,300));
 if(n>=6)place(Kind::Workshop,1,enemy+sf::Vector2f(-360,300));if(n>=8)place(Kind::Academy,1,enemy+sf::Vector2f(110,350));if(n>=9)place(Kind::Aviary,1,enemy+sf::Vector2f(-250,480));
 place(Kind::GuardTower,1,enemy+sf::Vector2f(-500,110));if(n>=6)place(Kind::CannonTower,1,enemy+sf::Vector2f(-400,530));
 place(Kind::Mine,-1,enemy+sf::Vector2f(180,-50));for(int i=0;i<(n==10?10:6);++i)add(Kind::Worker,1,enemy+sf::Vector2f(300.f,float(i*34-100)));
 int guards=6+(n-5)*2;for(int i=0;i<guards;++i){Kind kind=i%3==0?Kind::Knight:i%3==1?Kind::Ranger:Kind::Soldier;if(n>=8&&i%7==0)kind=Kind::Guardian;if(n>=9&&i%9==0)kind=Kind::Warbird;add(kind,1,enemy+sf::Vector2f(-500+(i%5)*55.f,250+(i/5)*60.f));}
 if(n>=6)add(Kind::Siege,1,enemy+sf::Vector2f(-520,430));if(n>=8){add(Kind::Warlock,1,enemy+sf::Vector2f(-380,430));add(Kind::Healer,1,enemy+sf::Vector2f(-320,430));}
 auto node=[&](sf::Vector2f p){missionGuards.push_back(place(Kind::Sanctuary,-1,p));add(Kind::Soldier,1,p+sf::Vector2f(110,90));add(Kind::Archer,1,p+sf::Vector2f(-100,90));};
 if(n==5){node({1500,500});node({2300,1000});node({1600,1840});}
 if(n==6){for(auto p:{sf::Vector2f(1680,900),sf::Vector2f(2300,1550),sf::Vector2f(2700,800)}){missionGuards.push_back(place(Kind::Workshop,1,p));add(Kind::Knight,1,p+sf::Vector2f(-100,100));}}
 if(n==7){place(Kind::Shipyard,0,{900,1320});place(Kind::Oil,-1,{1200,1570});place(Kind::Oil,-1,{3230,1600});missionGuards.push_back(place(Kind::Sanctuary,-1,{2440,1980}));missionGuards.push_back(place(Kind::Shipyard,1,{3100,1320}));place(Kind::Foundry,1,{3400,1150});place(Kind::OilRig,1,{3230,1600});int source=-1;for(const auto& e:entities)if(e.kind==Kind::Oil&&distance(e.pos,{3230,1600})<10)source=e.id;entities.back().source=source;add(Kind::Tanker,1,{3350,1510});enemyOil=80;add(Kind::Soldier,1,{2540,2000});add(Kind::Ranger,1,{2600,2090});}
 if(n==8){node({1500,680});node({2350,1200});node({2700,1850});}
 if(n==9){node({1450,740});node({2100,1720});missionGuards.push_back(place(Kind::Workshop,1,{2900,1300}));missionGuards.push_back(place(Kind::Aviary,1,{2920,1900}));add(Kind::Warbird,1,{2700,1830});}
 if(n==10){
  place(Kind::Oil,-1,{1900,1320});
  for(auto camp:{sf::Vector2f(3020,1500),sf::Vector2f(3360,2020)}){place(Kind::Hall,1,camp);for(int i=0;i<3;++i)place(Kind::Farm,1,camp+sf::Vector2f(-160+i*150.f,170));int barracks=place(Kind::Barracks,1,camp+sf::Vector2f(-210,-80));get(barracks)->level=3;place(Kind::GuardTower,1,camp+sf::Vector2f(-380,-140));for(int i=0;i<5;++i)add(i%2?Kind::Guardian:Kind::Ranger,1,camp+sf::Vector2f(-400+i*50.f,80));}
  place(Kind::Workshop,1,{3260,1600});place(Kind::CannonTower,1,{2700,1220});place(Kind::CannonTower,1,{2800,1950});
  archivist=place(Kind::Sanctuary,-1,{3350,1250});get(archivist)->hp=get(archivist)->maxHp=10000;
  int boss=add(Kind::Warlock,1,{3320,1110});auto leader=get(boss);leader->lifetime=-777;leader->hp=leader->maxHp=650;leader->mana=leader->maxMana=180;
  for(int i=0;i<7;++i)place(Kind::Wall,1,{2640.f+i*120,1030});
 }
 // Keep resource groves reachable, objective approaches clear, and layouts distinct.
 for(int y=2;y<rows-2;++y)for(int x=2;x<cols-2;++x){sf::Vector2f p{x*40.f+float((x*17+y*13+n)%11),y*40.f};if(terrain[x+y*cols]!=Terrain::Grass||(x*13+y*7+n)%5==0)continue;bool grove=(p.x<900&&p.y<base.y-500)||(p.x>enemy.x-400&&p.y<220)||(p.x>1100&&p.x<1600&&p.y>1800)||(p.x>3200&&p.y>1200&&n!=10);if(!grove)continue;bool free=true;for(int j=1;j<int(road.size());++j)if(segment(p,road[j-1],road[j])<160)free=false;for(const auto& e:entities)if(distance(e.pos,p)<radius(e.kind)+(e.kind==Kind::Forest?15.f:150.f))free=false;if(free)add(Kind::Forest,-1,p);}
 for(int team=0;team<2;++team){sf::Vector2f p=team==0?base:enemy;for(int i=0;i<35;++i){auto tree=p+sf::Vector2f(-320+(i%7)*40.f,-430+(i/7)*40.f);if(tree.x<40||tree.y<40||water(tree))continue;bool free=true;for(auto& e:entities)if(distance(tree,e.pos)<radius(e.kind)+(e.kind==Kind::Forest?13.f:100.f))free=false;if(free)add(Kind::Forest,-1,tree);}}
 navDirty=true;refreshVision();message=missionObjective();
}
inline sf::Vector2f World::warTarget()const{
 if(mission==10){if(missionStage==0){for(const auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Hall)return e.pos;}if(!has(1,Kind::Hall))for(const auto& e:entities)if(e.hp>0&&e.team==1)return e.pos;auto c=get(archivist);return c?c->pos:sf::Vector2f(3350,1250);}
 if((mission==5||mission==8)&&missionStage==2){auto p=get(missionGuards[1]);return p?p->pos:sf::Vector2f(2300,1000);}
 for(int id:missionGuards){auto e=get(id);if(e&&((mission==5||mission==8||mission==7||mission==9&&id!=missionGuards.back())?e->level<2:true))return e->pos;}
 for(const auto& e:entities)if(e.hp>0&&e.team==1&&e.kind==Kind::Hall)return e.pos;return {3100,500};
}
inline std::vector<sf::Vector2f> World::questTargets()const{
 if(mission<5)return {missionTarget()};std::vector<sf::Vector2f> out;
 if(mission==10){if(!has(1,Kind::Hall)||enemyRemaining()<=12){for(const auto& e:entities)if(e.hp>0&&e.team==1)out.push_back(e.pos);}else{for(const auto& e:entities)if(e.hp>0&&e.team==1&&e.kind==Kind::Hall)out.push_back(e.pos);if(auto crystal=get(archivist))out.push_back(crystal->pos);}return out;}
 for(int id:missionGuards)if(auto e=get(id)){if((mission==5||mission==8||mission==7)&&e->team<0&&e->level>=2)continue;if(mission==9&&e->team<0&&e->level>=2)continue;out.push_back(e->pos);}if(out.empty())out.push_back(warTarget());return out;
}
inline std::string World::warObjective()const{
 auto count=[&](Kind k){int n=0;for(auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==k&&e.build<=0)++n;return n;};int complete=0,alive=0;for(int id:missionGuards){auto e=get(id);if(e){++alive;if(e->level>=2)++complete;}}
 if(mission==5){if(missionStage==0)return "Santuario, 1 guaritore, Dottrina veterana, Caserma III e 2 veterani. Prepara economia e cura.";if(missionStage==1)return "Recupera i tre archivi: "+std::to_string(complete)+"/3. Presidio: 8 s senza nemici per archivio.";return "Difendi l archivio centrale: "+std::to_string(int(secureTime))+"/30 s sicuri. Proteggi i guaritori.";}
 if(mission==6){if(missionStage==0)return "Costruisci l officina e addestra 2 baliste ("+std::to_string(count(Kind::Siege))+"/2).";if(missionStage==1)return "Distruggi i tre depositi militari: "+std::to_string(3-alive)+"/3. Le baliste hanno lunga gittata.";return "Rifornimenti interrotti. Distruggi il comando nemico a nord-est.";}
 if(mission==7){if(missionStage==0)return "Fonderia, corazzata e trasporto. Raccogli petrolio; le fregate vedono i sommergibili.";if(missionStage==1)return "Sbarca e presidia il faro con 3 combattenti per 20 s. Proteggi la flotta.";return "La rotta e aperta. Distruggi il porto nemico segnalato sulla costa nord-est.";}
 if(mission==8){if(missionStage==0)return "Comando III, accademia, Sapienza arcana e 2 arcanisti. Crea un esercito di supporto.";if(missionStage==1)return "Recupera i tre sigilli: "+std::to_string(complete)+"/3. Presidia ogni cerchio per 10 s.";return "Difendi il cerchio centrale: "+std::to_string(int(secureTime))+"/45 s sicuri. Attenzione alla magia nemica.";}
 if(mission==9){if(missionStage==0)return "Due comandi, voliera e 2 roc da guerra. Sviluppa una seconda base con i lavoratori.";if(missionStage==1)return "Recupera i due presidi: "+std::to_string(complete)+"/2. Presidia ogni punto per 12 s.";return "Distruggi le due officine avanzate segnalate: impedisci la produzione di viverne e assedio.";}
 if(mission==10){int troops=0;for(auto& e:entities)if(e.hp>0&&e.team==0&&mobile(e.kind)&&e.kind!=Kind::Worker&&def(e.kind).damage>0)++troops;int structures=enemyRemaining(true),units=enemyRemaining()-structures;if(missionStage==0)return "Prepara officina, accademia e 20 combattenti ("+std::to_string(troops)+"/20). Amplia fattorie e raccolta.";if(time<wave)return "Primo assalto tra "+std::to_string(int(std::ceil(wave-time)))+" s. Nemici: "+std::to_string(units)+" unita, "+std::to_string(structures)+" strutture.";return "GUERRA FINALE - Elimina tutti: "+std::to_string(units)+" unita e "+std::to_string(structures)+" strutture. Pietra intatta.";}
 return "";
}
inline void World::updateWarMission(float dt){
 auto count=[&](Kind k){int n=0;for(const auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==k&&e.build<=0)++n;return n;};
 auto hold=[&](int id,float seconds,int required){auto node=get(id);if(!node)return false;int allies=0;bool enemy=false;for(const auto& e:entities)if(e.hp>0&&e.carrier<0&&mobile(e.kind)&&distance(e.pos,node->pos)<230){if(e.team==1)enemy=true;if(e.team==0&&e.kind!=Kind::Worker&&def(e.kind).domain==Domain::Land&&def(e.kind).damage>0)++allies;}node->work=allies>=required&&!enemy?node->work+dt:0;if(node->work>=seconds)node->level=2;return node->level>=2;};
 auto defend=[&](int id,float seconds){auto node=get(id);if(!node)return;bool ally=false,enemy=false;for(auto& e:entities)if(e.hp>0&&e.carrier<0&&mobile(e.kind)&&distance(e.pos,node->pos)<270){if(e.team==1)enemy=true;if(e.team==0&&e.kind!=Kind::Worker&&def(e.kind).damage>0)ally=true;}if(ally&&!enemy)secureTime+=dt;if(secureTime>=seconds)result=1;};
 if(mission==5){if(missionStage==0&&count(Kind::Healer)>0&&count(Kind::Ranger)>=2)missionStage=1;else if(missionStage==1){int captured=0;for(int id:missionGuards)if(hold(id,8,1))++captured;if(captured==3){missionStage=2;secureTime=0;}}else if(missionStage==2)defend(missionGuards[1],30);}
 if(mission==6){if(missionStage==0&&has(0,Kind::Workshop)&&count(Kind::Siege)>=2)missionStage=1;else if(missionStage==1){bool alive=false;for(int id:missionGuards)if(get(id))alive=true;if(!alive)missionStage=2;}else if(missionStage==2&&!has(1,Kind::Hall))result=1;}
 if(mission==7){if(missionStage==0&&has(0,Kind::Foundry)&&count(Kind::Battleship)>0&&count(Kind::Transport)>0)missionStage=1;else if(missionStage==1&&hold(missionGuards[0],20,3))missionStage=2;else if(missionStage==2&&!get(missionGuards[1]))result=1;}
 if(mission==8){if(missionStage==0&&townLevel[0]>=3&&has(0,Kind::Academy)&&upgrades[0][int(Research::Arcana)]>=1&&count(faction[0]==0?Kind::Mage:Kind::Warlock)>=2)missionStage=1;else if(missionStage==1){int captured=0;for(int id:missionGuards)if(hold(id,10,1))++captured;if(captured==3){missionStage=2;secureTime=0;}}else if(missionStage==2)defend(missionGuards[1],45);}
 if(mission==9){if(missionStage==0&&count(Kind::Hall)>=2&&has(0,Kind::Aviary)&&count(Kind::Warbird)>=2)missionStage=1;else if(missionStage==1){int captured=0;for(int i=0;i<2;++i)if(hold(missionGuards[i],12,1))++captured;if(captured==2)missionStage=2;}else if(missionStage==2&&!get(missionGuards[2])&&!get(missionGuards[3]))result=1;}
 if(mission==10){int army=0;for(auto& e:entities)if(e.hp>0&&e.team==0&&mobile(e.kind)&&e.kind!=Kind::Worker&&def(e.kind).damage>0)++army;if(missionStage==0&&army>=20&&has(0,Kind::Workshop)&&has(0,Kind::Academy)){missionStage=1;wave=std::max(420.f,time+60);}if(missionStage==1&&!has(1,Kind::Hall))missionStage=2;if(enemyRemaining()==0){result=1;message="VITTORIA FINALE: la Pietra e salva. Il campo nemico e completamente sconfitto.";}}
}
}
