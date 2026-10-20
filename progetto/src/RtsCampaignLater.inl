#pragma once
namespace alpha {
inline void World::startLaterMission(int n){
 reset(0);mission=n;guided=false;missionStage=0;entities.clear();shots.clear();effects.clear();zones.clear();nextId=1;
 terrain.fill(Terrain::Grass);explored={};sight={};navDirty=true;raidsEnabled=true;gold=700;wood=500;oil=0;enemyGold=400;enemyWood=260;enemyOil=0;
 sf::Vector2f base=n==3?sf::Vector2f(350,1050):sf::Vector2f(350,920);
 add(Kind::Hall,0,base);for(int i=0;i<5;++i)add(Kind::Worker,0,base+sf::Vector2f(140.f,float(i*38-85)));
 add(Kind::Farm,0,base+sf::Vector2f(-160,-190));add(Kind::Farm,0,base+sf::Vector2f(-160,0));
 int barracks=add(Kind::Barracks,0,base+sf::Vector2f(330,20));add(Kind::LumberMill,0,base+sf::Vector2f(50,-280));
 add(Kind::Forge,0,base+sf::Vector2f(340,-220));get(barracks)->level=2;
 for(int i=0;i<3;++i)add(Kind::Soldier,0,base+sf::Vector2f(180.f+i*45,140));
 for(int i=0;i<2;++i)add(Kind::Archer,0,base+sf::Vector2f(180.f+i*45,200));
 add(Kind::Mine,-1,base+sf::Vector2f(-160,230));add(Kind::Mine,-1,{1110,370});
 if(n==3){archivist=add(Kind::Farm,0,{1100,800});missionGuards.push_back(add(Kind::Barracks,1,{1990,510}));}
 else {townLevel[0]=2;upgrades[0][int(Research::Town2)]=1;get(1)->hp=get(1)->maxHp=1400;add(Kind::GuardTower,0,{850,1050});add(Kind::Sanctuary,-1,{2020,1480});add(Kind::Barracks,1,{1980,550});add(Kind::Shipyard,1,{2440,1120});add(Kind::Oil,-1,{900,1320});add(Kind::Oil,-1,{2110,1310});}
 add(Kind::Hall,1,{2220,400});add(Kind::Farm,1,{2410,580});add(Kind::Farm,1,{2410,770});add(Kind::LumberMill,1,{1940,310});
 add(Kind::Mine,-1,{2200,800});for(int i=0;i<4;++i)add(Kind::Worker,1,{2340.f,280.f+i*38});
 if(n==3){add(Kind::Soldier,1,{1730,710});add(Kind::Soldier,1,{1780,740});}
 else {add(Kind::Soldier,1,{2090,1490});add(Kind::Archer,1,{2180,1500});}
 auto road=missionRoad(n);auto segment=[](sf::Vector2f p,sf::Vector2f a,sf::Vector2f b){auto d=b-a;float l=d.x*d.x+d.y*d.y;float t=l?std::clamp(((p.x-a.x)*d.x+(p.y-a.y)*d.y)/l,0.f,1.f):0;return distance(p,a+d*t);};
 for(int y=0;y<rows;++y)for(int x=0;x<cols;++x){sf::Vector2f p{(x+.5f)*cell,(y+.5f)*cell};bool route=false;for(int i=1;i<int(road.size());++i)if(segment(p,road[i-1],road[i])<90)route=true;
  bool rock=n==3?((x>=27&&x<=32&&y<15)||(x>=27&&x<=32&&y>24)) : ((x>=30&&x<=33&&y<12)||(x>=37&&x<=40&&y>=23&&y<28));
  terrain[x+y*cols]=route?Terrain::Road:rock?Terrain::Rock:Terrain::Grass;
  if(n==4&&y>=30)terrain[x+y*cols]=(x>=46&&x<=61&&y>=35)?Terrain::Grass:Terrain::Water;
 }
 for(int y=2;y<38;++y)for(int x=2;x<62;++x){unsigned h=unsigned(x*73856093)^unsigned(y*19349663)^unsigned(n*83492791);sf::Vector2f p{x*40.f+float(h%11),y*40.f+float((h>>7)%11)};
  bool grove=n==3?((p.x<800&&p.y<540)||(p.x>1450&&p.x<1790&&p.y>940)||(p.x>1740&&p.y<220)) : ((p.x<900&&p.y<480)||(p.x>1000&&p.x<1400&&p.y>940)||(p.x>1700&&p.y<200));
  // Close groves support both economies; no trees on water, buildings, or the road.
  grove=grove||(p.x<240&&p.y>430&&p.y<850)||(p.x>1840&&p.y>880&&p.y<1080);
  if(!grove||h%4==0||terrain[int(p.x/cell)+int(p.y/cell)*cols]!=Terrain::Grass)continue;bool free=true;
  for(int i=1;i<int(road.size());++i)if(segment(p,road[i-1],road[i])<140)free=false;
  for(const auto& e:entities)if(distance(e.pos,p)<radius(e.kind)+(e.kind==Kind::Forest?15.f:120.f))free=false;
  if(free)add(Kind::Forest,-1,p);
 }
 expandMap();refreshVision();message=missionObjective();
}
inline void World::expandMap(){
 // Preserve the tested mission core; add explorable terrain and finite reserves around it.
 auto old=terrain;cols=96;rows=60;terrain.fill(Terrain::Grass);
 for(int y=0;y<rows;++y)for(int x=0;x<cols;++x){auto& tile=terrain[x+y*cols];if(x<64&&y<40)tile=old[x+y*64];else if(mission==4&&y>=30)tile=(x>=72&&x<=88&&y>=46&&y<=55)?Terrain::Grass:Terrain::Water;else if((x>68&&x<73&&(y<15||(y>23&&y<30)))||(x>77&&x<83&&y>42))tile=Terrain::Rock;}
 navDirty=true;
 for(auto p:{sf::Vector2f(2900,620),sf::Vector2f(3450,1040),sf::Vector2f(1280,2020)})if(!water(p)&&!blocked(int(p.x/cell),int(p.y/cell)))add(Kind::Mine,-1,p);
 if(mission==4)add(Kind::Mine,-1,{3200,2040});
 for(int y=2;y<rows-2;++y)for(int x=2;x<cols-2;++x){if(x<64&&y<40)continue;sf::Vector2f p{x*40.f+float((x*17+y*13)%11),y*40.f};if(terrain[x+y*cols]!=Terrain::Grass||(x*13+y*7)%5==0||!((x>65&&y<15)||(x>82&&y<30)||(y>43&&x<57)))continue;bool free=true;for(const auto& e:entities)if(distance(e.pos,p)<radius(e.kind)+(e.kind==Kind::Forest?15.f:140.f))free=false;if(free)add(Kind::Forest,-1,p);}
 navDirty=true;
}
inline void World::laterEnemyEconomy(){
 std::string saved=message;int hall=-1,worker=-1,barracks=-1,port=-1,tanker=-1,rig=-1;int workers=0,troops=0;bool constructing=false;
 for(const auto& e:entities)if(e.hp>0&&e.team==1){if(e.kind==Kind::Hall&&e.build<=0)hall=e.id;if(e.kind==Kind::Worker){++workers;if(!busy(e))worker=e.id;}else if(mobile(e.kind)&&e.kind!=Kind::Tanker)++troops;if(e.kind==Kind::Barracks&&e.build<=0)barracks=e.id;if(e.kind==Kind::Shipyard&&e.build<=0)port=e.id;if(e.kind==Kind::Tanker)tanker=e.id;if(e.kind==Kind::OilRig)rig=e.id;if(e.build>0)constructing=true;}
 if(hall<0)return;
 auto construct=[&](Kind k){if(worker<0||constructing)return false;auto center=get(hall)->pos;for(int y=120;y<=1000;y+=140)for(int x=1770;x<=2440;x+=140){sf::Vector2f p{float(x),float(y)};if(distance(center,p)>700||!placementIssue(p,k).empty())continue;if(build(worker,p,k))return true;}return false;};
 if(!has(1,Kind::Farm))construct(Kind::Farm);
 else if(barracks<0)construct(Kind::Barracks);
 else if(population(1)+2>capacity(1))construct(Kind::Farm);
 else if(!has(1,Kind::LumberMill))construct(Kind::LumberMill);
 if(has(1,Kind::Farm)&&workers<4&&get(hall)->production<=0)train(hall,Kind::Worker);
 else if(townLevel[1]<2&&get(hall)->production<=0)research(hall,Research::Town2);
 if(barracks>=0&&get(barracks)->production<=0){if(get(barracks)->level<2&&has(1,Kind::LumberMill))research(barracks,Research::Barracks2);else if(troops<8)train(barracks,(get(barracks)->level>=2&&troops%3==0)?Kind::Archer:Kind::Soldier);}
 for(auto& e:entities)if(e.hp>0&&e.team==1&&e.kind==Kind::Worker&&!busy(e)&&e.order==Order::Idle){int source=-1;float nearest=1e9;Kind wanted=e.id%2?Kind::Mine:Kind::Forest;for(const auto& node:entities)if(node.hp>0&&node.kind==wanted&&distance(node.pos,e.pos)<nearest){nearest=distance(node.pos,e.pos);source=node.id;}if(source>=0)gather(e,source);}
 if(mission==4&&port>=0){if(tanker<0&&get(port)->production<=0)train(port,Kind::Tanker);
  else if(tanker>=0){if(rig<0&&!busy(*get(tanker)))build(tanker,{2110,1310},Kind::OilRig);else if(rig>=0&&get(rig)->build<=0&&get(tanker)->order==Order::Idle)gather(*get(tanker),rig);}
  int ships=0;for(const auto& e:entities)if(e.hp>0&&e.team==1&&e.kind==Kind::Destroyer)++ships;if(tanker>=0&&ships<1&&get(port)->production<=0)train(port,Kind::Destroyer);
 }
 message=saved;
}
inline bool World::missionRaidReady()const{
 if(mission<0)return true;
 if(mission==2)return missionStage>=2&&time>=180;
 if(mission==3)return missionStage>=1&&time>=wave;
 // The first coastal scenario teaches transport: enemy fleets defend their waters.
 return false;
}
inline void World::updateLaterMission(float dt){
 auto count=[&](Kind k){int n=0;for(const auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==k&&e.build<=0&&e.carrier<0)++n;return n;};
 if(mission==3){
  if(!get(archivist)){result=-1;message="SCONFITTA: il rifugio dei profughi e stato distrutto.";return;}
  if(missionStage==0&&townLevel[0]>=2&&has(0,Kind::Stable)&&count(Kind::Knight)>=2&&has(0,Kind::GuardTower)){missionStage=1;secureTime=0;wave=time+20;}
  else if(missionStage==1){bool ally=false,enemy=false;for(const auto& e:entities)if(e.hp>0&&e.carrier<0&&mobile(e.kind)&&e.kind!=Kind::Worker&&distance(e.pos,{1350,800})<260){if(e.team==0)ally=true;if(e.team==1)enemy=true;}if(ally&&!enemy)secureTime+=dt;if(secureTime>=90)missionStage=2;}
  else if(missionStage==2){bool barracks=false;for(int id:missionGuards)if(get(id))barracks=true;if(!barracks)result=1;}
 }
 if(mission==4){
  if(missionStage==0&&has(0,Kind::Shipyard)&&has(0,Kind::OilRig)&&oil>=60)missionStage=1;
  else if(missionStage==1&&count(Kind::Transport)>0&&count(Kind::Destroyer)>0)missionStage=2;
  else if(missionStage==2){int troops=0;bool enemy=false;for(const auto& e:entities)if(e.hp>0&&e.carrier<0&&distance(e.pos,{2020,1480})<220){if(e.team==1)enemy=true;if(e.team==0&&mobile(e.kind)&&e.kind!=Kind::Worker&&def(e.kind).domain==Domain::Land&&def(e.kind).damage>0)++troops;}secureTime=troops>=3&&!enemy?secureTime+dt:0;if(secureTime>=20)result=1;}
 }
}
}
