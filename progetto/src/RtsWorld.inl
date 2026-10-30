#pragma once
namespace alpha {
inline void World::clearVision(){
 for(auto* grid:{&explored,&sight})for(auto& team:*grid)team.assign(cols*rows,0);
 visionTimer=0;
}
inline void World::resizeMap(int columns,int rowCount,bool preserve){
 if(columns<64||columns>maxMapCols||rowCount<40||rowCount>maxMapRows)throw std::invalid_argument("Dimensioni mappa non valide");
 const int size=columns*rowCount;
 std::vector<Terrain> newTerrain(size,Terrain::Grass);
 std::array<std::vector<unsigned char>,2> newExplored{std::vector<unsigned char>(size),std::vector<unsigned char>(size)},newSight=newExplored;
 std::vector<unsigned char> newLand(size),newSea(size);
 if(preserve)for(int y=0;y<std::min(rows,rowCount);++y)for(int x=0;x<std::min(cols,columns);++x){
  int oldIndex=x+y*cols,newIndex=x+y*columns;newTerrain[newIndex]=terrain[oldIndex];
  for(int team=0;team<2;++team){newExplored[team][newIndex]=explored[team][oldIndex];newSight[team][newIndex]=sight[team][oldIndex];}
 }
 terrain.swap(newTerrain);explored.swap(newExplored);sight.swap(newSight);landNav.swap(newLand);seaNav.swap(newSea);
 cols=columns;rows=rowCount;navDirty=true;visionTimer=0;
}
inline Entity* World::get(int id){for(auto& e:entities)if(e.id==id&&e.hp>0)return &e;return nullptr;}
inline const Entity* World::get(int id)const{for(const auto& e:entities)if(e.id==id&&e.hp>0)return &e;return nullptr;}
inline int World::add(Kind k,int team,sf::Vector2f p){
 Entity e;e.id=nextId++;e.kind=k;e.team=team;e.pos=e.goal=p;e.hp=e.maxHp=def(k).hp;
 if(k==Kind::Mage||k==Kind::Warlock||k==Kind::Healer||k==Kind::Guardian)e.mana=e.maxMana=120;
 if(k==Kind::Angel||k==Kind::Paladin||k==Kind::Binder)e.mana=e.maxMana=120;
 if(k==Kind::Spirit)e.lifetime=45;
 entities.push_back(e);if(!mobile(k))navDirty=true;return e.id;
}
inline void World::reset(int playerFaction){
 cheatsUsed=cheatGod=cheatFast=cheatFreeze=cheatFree=cheatUnlock=cheatSupply=false;
 layoutVersion=0;mapGoal={};crystals=enemyCrystals=0;runes={};v2AiTimer=0;for(int i=0;i<2;++i){barrierTime[i]=barrierCooldown[i]=0;barrierCenter[i]={};}resizeMap(64,40);std::fill(terrain.begin(),terrain.end(),Terrain::Grass);mission=-1;missionStage=0;deliveredGold=deliveredWood=0;archivist=-1;secureTime=0;missionGuards.clear();for(int y=0;y<rows;++y)for(int x=0;x<cols;++x)terrain[x+y*cols]=y*cell>=coast?Terrain::Water:Terrain::Grass;guided=false;chapter=0;aiTimer=0;enemyGold=240;enemyWood=140;enemyOil=0;fogEnabled=true;clearVision();visionTimer=0;entities.clear();shots.clear();effects.clear();zones.clear();nextId=1;accumulator=0;gold=240;wood=140;oil=0;result=0;time=wave=0;townLevel[0]=townLevel[1]=1;upgrades={};faction[0]=playerFaction;faction[1]=1-playerFaction;navDirty=true;raidsEnabled=true;
 message="Sviluppa la citta, prepara le truppe e distruggi il comando nemico. F3-F6: cataloghi.";
 add(Kind::Hall,0,{280,750});add(Kind::Hall,1,{2210,750});
 add(Kind::Mine,-1,{470,500});add(Kind::Forest,-1,{470,1020});add(Kind::Mine,-1,{1980,500});add(Kind::Forest,-1,{1980,1020});
 for(int i=0;i<4;++i)add(Kind::Worker,0,{360.f,680.f+i*40});
 for(int i=0;i<2;++i)add(Kind::Soldier,0,{520.f,700.f+i*50});
 for(int i=0;i<3;++i)add(Kind::Soldier,1,{2050.f,680.f+i*50});
 add(Kind::Oil,-1,{900,1410});add(Kind::Oil,-1,{1700,1410});add(Kind::Shipyard,1,{2140,1120});
 // Irregular connected woodlands, with clear starting areas and a central route.
 struct Grove {float x,y,rx,ry;};
 const Grove groves[]={{330,1030,300,155},{200,140,220,135},{680,175,325,145},{1220,155,330,130},{1800,160,350,135},{2330,175,240,150},{1160,490,270,150},{1060,1050,320,130},{1690,1040,300,145},{2310,1060,240,120}};
 for(int row=0;row<27;++row)for(int col=0;col<55;++col){unsigned hash=unsigned(col*73856093)^unsigned(row*19349663);sf::Vector2f p{80+col*44.f+float(hash%15)-7,75+row*40.f+float((hash>>8)%13)-6};bool forest=false;
  for(const auto& g:groves){float dx=(p.x-g.x)/g.rx,dy=(p.y-g.y)/g.ry;float edge=1.f+.12f*std::sin(p.x*.041f+p.y*.033f);if(dx*dx+dy*dy<edge)forest=true;}
  if(!forest||p.y>=1170||(p.y>620&&p.y<890)||distance(p,{280,750})<240||distance(p,{2210,750})<240)continue;
  bool free=true;for(const auto& e:entities)if(e.hp>0&&distance(e.pos,p)<radius(e.kind)+(e.kind==Kind::Forest?20.f:55.f)){free=false;break;}if(free)add(Kind::Forest,-1,p);
 }
 refreshVision();
}
inline void World::prepareArsenal(){
 fogEnabled=false;refreshVision();
 gold=8000;wood=6000;oil=4000;townLevel[0]=3;raidsEnabled=false;
 for(int r=0;r<int(Research::Count);++r)upgrades[0][r]=researchCatalog()[r].maxLevel;
 int barracks=add(Kind::Barracks,0,{640,820});get(barracks)->level=3;int index=0;
 for(int i=int(Kind::Farm);i<=int(Kind::Wall);++i){auto k=Kind(i);if(k==Kind::OilRig||k==Kind::Shipyard)continue;add(k,0,{180.f+(index%7)*260,180.f+(index/7)*250});++index;}
 add(Kind::Farm,0,{120,940});add(Kind::Farm,0,{1320,980});add(Kind::Farm,0,{1550,980});add(Kind::Shipyard,0,{650,1120});int rig=add(Kind::OilRig,0,{900,1410});get(rig)->source=16;
 for(auto& tree:entities)if(tree.kind==Kind::Forest&&tree.hp>0)for(const auto& b:entities)if(b.hp>0&&!mobile(b.kind)&&!resource(b.kind)&&distance(tree.pos,b.pos)<radius(b.kind)+18){tree.hp=0;navDirty=true;break;}
 for(int i=int(Kind::Archer);i<=int(Kind::Submarine);++i){auto k=Kind(i);if(!unavailable(k).empty())continue;int producer=-1;for(const auto& b:entities)if(b.hp>0&&b.team==0&&b.kind==def(k).producer){producer=b.id;break;}if(producer>=0){sf::Vector2f p;if(spawnPosition(*get(producer),k,p))add(k,0,p);}}
 message="ARSENALE DI PROVA: tecnologie e risorse disponibili. F3-F6 cataloghi, F2 altra fazione.";
}
inline bool World::exploredAt(sf::Vector2f p,int team)const{int x=int(std::floor(p.x/cell)),y=int(std::floor(p.y/cell));if(x<0||y<0||x>=cols||y>=rows)return false;return !fogEnabled||explored[team][x+y*cols]!=0;}
inline bool World::visibleAt(sf::Vector2f p,int team)const{int x=int(std::floor(p.x/cell)),y=int(std::floor(p.y/cell));if(x<0||y<0||x>=cols||y>=rows)return false;return !fogEnabled||sight[team][x+y*cols]!=0;}
inline void World::refreshVision(){
 if(!fogEnabled){for(auto& cells:explored)std::fill(cells.begin(),cells.end(),1);for(auto& cells:sight)std::fill(cells.begin(),cells.end(),1);return;}for(auto& cells:sight)std::fill(cells.begin(),cells.end(),0);
 for(const auto& e:entities)if(e.hp>0&&e.team>=0&&e.form!=3&&e.carrier<0){float r=e.reveal>0?640.f:(e.kind==Kind::Scout?520.f:(mobile(e.kind)?300.f:380.f));if(e.build>0)r=140;
  int left=std::max(0,int((e.pos.x-r)/cell)),right=std::min(cols-1,int((e.pos.x+r)/cell)),top=std::max(0,int((e.pos.y-r)/cell)),bottom=std::min(rows-1,int((e.pos.y+r)/cell));
  for(int y=top;y<=bottom;++y)for(int x=left;x<=right;++x){float dx=(x+.5f)*cell-e.pos.x,dy=(y+.5f)*cell-e.pos.y;if(dx*dx+dy*dy<r*r)sight[e.team][x+y*cols]=explored[e.team][x+y*cols]=1;}
 }
}
inline bool World::has(int team,Kind k)const{if(k==Kind::Count)return true;for(const auto& e:entities)if(e.team==team&&e.kind==k&&e.hp>0&&e.build<=0)return true;return false;}
inline int World::capacity(int team)const{if(team==0&&cheatSupply)return modern()?140:90;int n=10;for(const auto& e:entities)if(e.team==team&&e.hp>0&&e.kind==Kind::Farm&&e.build<=0)n+=6+(modern()?2*(e.level-1):0);return std::min(modern()?140:90,n);}
inline int World::population(int team,bool reserved)const{int n=0;for(const auto& e:entities)if(e.team==team&&e.hp>0){if(mobile(e.kind))n+=def(e.kind).supply;if(reserved&&e.production>0&&e.research==-1)n+=def(e.training).supply;}return n;}
inline bool World::pay(Cost c,int team){if(team==0&&cheatFree)return true;int& g=team==0?gold:enemyGold;int& w=team==0?wood:enemyWood;int& o=team==0?oil:enemyOil;int& crystal=team==0?crystals:enemyCrystals;if(crystal<c.crystals){if(team==0)message="Cristalli insufficienti.";return false;}if(g<c.gold||w<c.wood||o<c.oil){if(team==0)message=std::string(g<c.gold?"Oro insufficiente: ":w<c.wood?"Legno insufficiente: ":"Petrolio insufficiente: ")+std::to_string(c.gold)+" oro, "+std::to_string(c.wood)+" legno, "+std::to_string(c.oil)+" petrolio.";return false;}g-=c.gold;w-=c.wood;o-=c.oil;crystal-=c.crystals;return true;}
inline void World::rebuildNav()const{
 if(!navDirty)return;navDirty=false;
 for(int y=0;y<rows;++y)for(int x=0;x<cols;++x){int n=x+y*cols;landNav[n]=terrain[n]==Terrain::Water||terrain[n]==Terrain::Rock;seaNav[n]=terrain[n]!=Terrain::Water;}
 // Touch only the cells near each obstacle, rather than scanning every tree for every cell.
 for(const auto& e:entities)if(e.hp>0&&!mobile(e.kind)){float r=radius(e.kind)+18;
  int left=std::max(0,int((e.pos.x-r)/cell)),right=std::min(cols-1,int((e.pos.x+r)/cell));int top=std::max(0,int((e.pos.y-r)/cell)),bottom=std::min(rows-1,int((e.pos.y+r)/cell));
  for(int y=top;y<=bottom;++y)for(int x=left;x<=right;++x){float dx=(x+.5f)*cell-e.pos.x,dy=(y+.5f)*cell-e.pos.y;if(dx*dx+dy*dy<r*r)landNav[x+y*cols]=seaNav[x+y*cols]=1;}
 }
}
inline bool World::blocked(int x,int y,Domain domain)const{if(x<0||y<0||x>=cols||y>=rows)return true;if(domain==Domain::Air)return false;rebuildNav();return domain==Domain::Sea?seaNav[x+y*cols]!=0:landNav[x+y*cols]!=0;}
inline bool World::path(Entity& e,sf::Vector2f destination,float stop){destination.x=std::clamp(destination.x,0.f,width()-1.f);destination.y=std::clamp(destination.y,0.f,height()-1.f);
 auto domain=def(e.kind).domain;destination.x=std::clamp(destination.x,20.f,width()-20.f);destination.y=std::clamp(destination.y,20.f,height()-20.f);e.route.clear();e.goal=destination;
 if(domain==Domain::Air){e.route.push_back(destination);return true;}
 int sx=std::clamp(int(e.pos.x/cell),0,cols-1),sy=std::clamp(int(e.pos.y/cell),0,rows-1),start=sx+sy*cols,found=-1;
 std::vector<int> parent(cols*rows,-1),cost(cols*rows,1000000);std::vector<unsigned char> closed(cols*rows);
 auto estimate=[&](int n){float dx=(n%cols+.5f)*cell-destination.x,dy=(n/cols+.5f)*cell-destination.y;return std::max(0.f,(std::sqrt(dx*dx+dy*dy)-stop)/cell);};
 using Entry=std::pair<float,int>;std::priority_queue<Entry,std::vector<Entry>,std::greater<Entry>> open;open.push({estimate(start),start});parent[start]=start;cost[start]=0;
 while(!open.empty()){int n=open.top().second;open.pop();if(closed[n])continue;closed[n]=1;int x=n%cols,y=n/cols;sf::Vector2f p{(x+.5f)*cell,(y+.5f)*cell};
  if((stop>0&&distance(p,destination)<=stop)||(stop==0&&x==int(destination.x/cell)&&y==int(destination.y/cell))){found=n;break;}
  const int dx[]={1,-1,0,0},dy[]={0,0,1,-1};for(int d=0;d<4;++d){int nx=x+dx[d],ny=y+dy[d];if(blocked(nx,ny,domain))continue;int m=nx+ny*cols;if(cost[n]+1<cost[m]){cost[m]=cost[n]+1;parent[m]=n;open.push({float(cost[m])+estimate(m),m});}}
 }
 if(found<0)return false;for(int n=found;n!=start;n=parent[n])e.route.push_back({(n%cols+.5f)*cell,(n/cols+.5f)*cell});std::reverse(e.route.begin(),e.route.end());
 if(stop==0&&!blocked(int(destination.x/cell),int(destination.y/cell),domain))e.route.push_back(destination);return true;
}
inline void World::move(Entity& e,sf::Vector2f p){if(e.stun>0||e.form>=3)return;e.channel=0;if(busy(e)){message="Operaio impegnato nella costruzione.";return;}if(!mobile(e.kind)||e.carrier>=0)return;e.target=e.source=e.objective=-1;e.order=Order::Move;float stop=0;for(const auto& object:entities)if(object.hp>0&&!mobile(object.kind)&&distance(object.pos,p)<radius(object.kind)+35)stop=std::max(stop,radius(object.kind)+65);if(!path(e,p,stop)){e.order=Order::Idle;message="Destinazione irraggiungibile per questo tipo di unita.";}}
inline void World::gather(Entity& e,int id){if(e.stun>0||e.form>=3)return;if(busy(e)){message="Operaio impegnato nella costruzione.";return;}auto s=get(id);if(!s)return;bool worker=(e.kind==Kind::Worker&&(s->kind==Kind::Mine||s->kind==Kind::Forest))||(e.kind==Kind::CrystalMiner&&s->kind==Kind::CrystalCave);bool tanker=e.kind==Kind::Tanker&&s->kind==Kind::OilRig&&s->team==e.team&&s->build<=0;
 if(s->build>0){message="L'albero sta ancora crescendo.";return;}
 if(!worker&&!tanker){message="Gli operai raccolgono oro/legno; le cisterne raccolgono dalle piattaforme.";return;}
 if(e.order==Order::Gather&&e.target==id&&e.route.empty()&&e.work>0){message="Operaio al lavoro sulla risorsa.";return;}
 e.source=id;e.objective=-1;
 if(e.cargo>0){returnCargo(e);return;}
 if(s->kind==Kind::Forest){if(assignTree(e,s->pos))message="Taglio legna: l'operaio raggiunge il bordo accessibile della foresta.";else message="Nessun albero raggiungibile in questa foresta.";return;}
 e.target=id;e.work=0;e.order=Order::Gather;if(!approachResource(e,s->pos,radius(s->kind)+18,radius(s->kind)+80)){e.order=Order::Idle;message="Risorsa irraggiungibile.";}else message=e.kind==Kind::Worker?"Raccolta oro avviata.":"Raccolta petrolio avviata.";
}
inline void World::repair(Entity& e,int id){if(e.stun>0||e.form>=3)return;if(busy(e)){message="Operaio impegnato nella costruzione.";return;}auto b=get(id);if(e.kind!=Kind::Worker||!b||b->team!=e.team||mobile(b->kind)||resource(b->kind))return;if(b->build>0){auto owner=get(b->constructionWorker);if(owner&&busy(*owner)){message="Cantiere gia assegnato a un operaio.";return;}if(!path(e,b->pos,radius(b->kind)+65)){message="Cantiere irraggiungibile.";return;}b->constructionWorker=e.id;e.target=id;e.order=Order::Build;return;}e.target=id;e.work=0;e.order=Order::Repair;path(e,b->pos,110);}
inline void World::board(Entity& e,int id){if(e.stun>0||e.form>=3)return;if(busy(e)){message="Operaio impegnato nella costruzione.";return;}auto ship=get(id);if(!ship||(ship->kind!=Kind::Transport&&ship->kind!=Kind::AirArk)||ship->team!=e.team||!mobile(e.kind)||def(e.kind).domain!=Domain::Land)return;int count=0;for(const auto& p:entities)if(p.hp>0&&p.carrier==id)++count;if(count>=6){message="Trasporto pieno (6 posti).";return;}e.target=id;e.order=Order::Board;if(!path(e,ship->pos,140)){e.order=Order::Idle;message="Avvicina il trasporto alla costa per imbarcare.";}}
inline bool World::unload(int id,sf::Vector2f p){auto s=get(id);if(!s||(s->kind!=Kind::Transport&&s->kind!=Kind::AirArk)||water(p)||distance(p,s->pos)>200){message="Scegli terra libera vicino al trasporto (entro 200).";return false;}int done=0;
 for(auto& e:entities)if(e.carrier==id&&e.hp>0){bool found=false;for(int r=0;r<5&&!found;++r)for(int a=0;a<8&&!found;++a){sf::Vector2f v=p+sf::Vector2f(std::cos(a*.785398f),std::sin(a*.785398f))*float(r*40);if(distance(v,s->pos)>220||blocked(int(v.x/cell),int(v.y/cell)))continue;bool free=true;for(const auto& o:entities)if(o.id!=e.id&&o.hp>0&&o.carrier<0&&mobile(o.kind)&&distance(v,o.pos)<34)free=false;if(free){e.carrier=-1;e.pos=e.goal=v;e.order=Order::Idle;e.route.clear();found=true;++done;}}}
 message=done?"Sbarco completato.":"Nessuno spazio libero sulla costa.";return done>0;
}
inline bool World::detected(const Entity& target,int observer)const{if(target.kind!=Kind::Submarine||target.team==observer)return true;for(const auto& e:entities)if(e.hp>0&&e.carrier<0&&e.team==observer&&(e.kind==Kind::Scout||e.kind==Kind::Destroyer||e.kind==Kind::WatchTower||e.kind==Kind::GuardTower||e.reveal>0)&&distance(e.pos,target.pos)<420)return true;return false;}
inline bool World::canHit(const Entity& a,const Entity& b)const{if(a.duty>=2&&distance(b.pos,a.anchor)>(a.duty==4?650.f:320.f))return false;if(b.hp<=0||b.carrier>=0||b.team<0||a.id==b.id||(a.team==b.team&&a.form!=3&&b.form!=3)||def(a.kind).damage<=0||!detected(b,a.team))return false;
 if(def(b.kind).domain==Domain::Air&&!def(a.kind).antiAir)return false;
 if(a.kind==Kind::Submarine&&def(b.kind).domain!=Domain::Sea)return false;
 if(def(a.kind).domain==Domain::Land&&def(a.kind).range<100&&def(b.kind).domain==Domain::Sea)return false;return true;
}
inline void World::attack(Entity& e,int id){if(e.stun>0||e.form==4)return;e.channel=0;if(busy(e)){message="Operaio impegnato nella costruzione.";return;}auto t=get(id);if(e.kind==Kind::Healer&&t&&t->team>=0&&t->team!=e.team){e.target=e.objective=e.source=-1;e.order=Order::Move;path(e,t->pos,260);message="Il guaritore segue il gruppo e cura gli alleati vicini.";return;}if(!t||!canHit(e,*t)){message="Questa unita non puo colpire quel bersaglio.";return;}e.target=e.objective=id;e.source=-1;e.order=Order::Attack;path(e,t->pos,std::max(40.f,def(e.kind).range+radius(t->kind)-8));}
inline std::string World::unavailable(Kind k,int team)const{const auto& d=def(k);if(!factionAllows(k,team))return "Unita riservata all altra fazione.";if(!missionAllows(k))return "Disponibile nelle missioni successive della campagna.";if(guided&&team==0&&((chapter==0&&k!=Kind::Farm&&k!=Kind::Worker)||(chapter==1&&k!=Kind::Farm&&k!=Kind::Worker&&k!=Kind::Barracks)||(chapter==2&&k!=Kind::Farm&&k!=Kind::Worker&&k!=Kind::Barracks&&k!=Kind::Soldier)))return "Avanza nell obiettivo guidato per sbloccare questa scelta.";if(townLevel[team]<d.tier)return "Richiede comando livello "+std::to_string(d.tier);if(!has(team,d.prerequisite))return std::string("Richiede ")+name(d.prerequisite);
 if((k==Kind::Guardian||k==Kind::Ranger)&&upgrades[team][int(Research::Elite)]==0)return "Richiede Dottrina veterana al santuario";
 if(k==Kind::Mage&&faction[team]!=0)return "Riservato alla Lega del Confine";if(k==Kind::Warlock&&faction[team]!=1)return "Riservato alla Stirpe dell Ombra";return "";
}
inline bool World::train(int id,Kind kind){auto b=get(id);const auto& d=def(kind);if(result||!b||b->team<0||!d.unit||d.producer==Kind::Count||b->kind!=d.producer){message="Seleziona un edificio adatto, completato e libero.";return false;}
 if(b->level<requiredProducerLevel(kind)){message="Potenzia questa caserma al livello "+std::to_string(requiredProducerLevel(kind))+".";return false;}
 if(b->build>0){message="Edificio ancora in costruzione: "+std::to_string(int(std::ceil(b->build)))+" secondi.";return false;}if(b->production>0){message="Addestramento gia in corso: "+std::to_string(int(std::ceil(b->production)))+" secondi.";return false;}
 auto reason=unavailable(kind,b->team);if(!reason.empty()){message=reason;return false;}if(population(b->team)+d.supply>capacity(b->team)){message="Popolazione piena: costruisci una fattoria.";return false;}
 if(!pay(d.cost,b->team))return false;b->training=kind;b->research=-1;b->paid=(b->team==0&&cheatFree)?Cost{}:d.cost;b->production=b->productionTotal=d.seconds;message=std::string(name(kind))+" in addestramento: "+std::to_string(int(d.seconds))+" secondi.";return true;
}
inline bool World::build(int worker,sf::Vector2f p,Kind kind){auto w=get(worker);if(result||!w||w->team<0||mobile(kind)||resource(kind))return false;
 if((kind==Kind::OilRig?w->kind!=Kind::Tanker:w->kind!=Kind::Worker)){message=kind==Kind::OilRig?"Seleziona una cisterna per la piattaforma.":"Seleziona un operaio.";return false;}
 if(busy(*w)){message="Operaio gia impegnato nella costruzione.";return false;}int team=w->team;
 auto reason=unavailable(kind,team);if(!reason.empty()){message=reason;return false;}
 Entity approach=*w;if(!path(approach,p,radius(kind)+65)){message="Cantiere irraggiungibile.";return false;}
 reason=placementIssue(p,kind);if(!reason.empty()){message=reason;return false;}
 int oilSource=-1;
 if(kind==Kind::OilRig){for(const auto& n:entities)if(n.kind==Kind::Oil&&n.hp>0&&distance(n.pos,p)<65){oilSource=n.id;p=n.pos;break;}if(oilSource<0){message="La piattaforma va collocata su un giacimento in mare.";return false;}}
 if(!pay(def(kind).cost,team))return false;

 int id=add(kind,team,p);auto b=get(id);b->build=b->buildTotal=def(kind).seconds;b->source=oilSource;
 // Units do not block placement: move them to the closest free perimeter cell.
 for(auto& e:entities)if(e.hp>0&&mobile(e.kind)&&!busy(e)&&e.carrier<0&&distance(e.pos,p)<radius(kind)+28){bool done=false;for(int r=2;r<8&&!done;++r)for(int a=0;a<16&&!done;++a){auto q=p+sf::Vector2f(std::cos(a*.392699f),std::sin(a*.392699f))*float(r*40);if(!blocked(int(q.x/cell),int(q.y/cell),def(e.kind).domain)){e.pos=e.goal=q;e.route.clear();e.order=Order::Idle;done=true;}}}
 b=get(id);b->constructionWorker=worker;auto builder=get(worker);builder->order=Order::Build;builder->target=id;builder->source=-1;builder->route=approach.route;builder->goal=p;
 message=std::string(name(kind))+" in costruzione: operaio assegnato.";return true;
}
inline std::string World::placementIssue(sf::Vector2f p,Kind kind)const{
 float r=radius(kind);if(mission>=0)for(int y=std::max(0,int((p.y-r)/cell));y<=std::min(rows-1,int((p.y+r)/cell));++y)for(int x=std::max(0,int((p.x-r)/cell));x<=std::min(cols-1,int((p.x+r)/cell));++x)if(terrain[x+y*cols]==Terrain::Rock)return "Terreno roccioso: scegli una radura.";if(p.x<r||p.x>width()-r||p.y<r||p.y>height()-r)return "Fuori dai bordi della mappa.";
 for(const auto& e:entities)if(e.hp>0&&distance(e.pos,p)<radius(e.kind)+r){if(e.kind==Kind::Forest)return "Abbatti prima gli alberi in questo spazio.";if(e.kind==Kind::Mine)return "La miniera occupa ancora questo spazio.";if(e.kind==Kind::CrystalCave)return "La caverna occupa ancora questo spazio.";}
 for(const auto& e:entities)if(e.hp>0&&!mobile(e.kind)&&!resource(e.kind)&&distance(e.pos,p)<radius(e.kind)+r+8)return "Spazio occupato da un'altra struttura.";
 return "";
}
inline bool World::approachResource(Entity& worker,sf::Vector2f center,float inner,float outer){
 struct Slot{sf::Vector2f p;float score;};std::vector<Slot> slots;std::unordered_set<int> cells;
 for(float r=inner+10;r<=outer;r+=16)for(int i=0;i<24;++i){float a=i*6.2831853f/24;auto raw=center+sf::Vector2f(std::cos(a)*r,std::sin(a)*r);int x=int(raw.x/cell),y=int(raw.y/cell);if(blocked(x,y,def(worker.kind).domain))continue;int index=x+y*cols;if(!cells.insert(index).second)continue;sf::Vector2f p{(x+.5f)*cell,(y+.5f)*cell};float reach=distance(p,center);if(reach<inner||reach>outer)continue;
  float score=distance(worker.pos,p);for(const auto& other:entities)if(other.hp>0&&other.id!=worker.id&&mobile(other.kind)&&other.carrier<0){if(distance(other.pos,p)<30)score+=500;if((other.order==Order::Gather||other.order==Order::Return||other.order==Order::Build)&&distance(other.goal,p)<25)score+=250;}
  slots.push_back({p,score});
 }
 std::stable_sort(slots.begin(),slots.end(),[](auto a,auto b){return a.score<b.score;});
 for(auto slot:slots)if(path(worker,slot.p))return true;return false;
}
inline bool World::assignTree(Entity& worker,sf::Vector2f preferred,int exclude){
 struct Candidate {int id;float score;};std::vector<Candidate> trees;std::vector<int> occupied;
 for(const auto& e:entities)if(e.id!=worker.id&&e.hp>0&&e.order==Order::Gather)occupied.push_back(e.target);
 for(const auto& e:entities)if(e.id!=exclude&&e.hp>0&&e.build<=0&&e.kind==Kind::Forest&&distance(preferred,e.pos)<900){float score=distance(preferred,e.pos)*.25f+distance(worker.pos,e.pos)*.75f;for(int target:occupied)if(target==e.id)score+=60;trees.push_back({e.id,score});}
 std::stable_sort(trees.begin(),trees.end(),[](const auto& a,const auto& b){return a.score<b.score;});
 int attempts=0;for(const auto& candidate:trees){if(++attempts>64)break;auto tree=get(candidate.id);if(approachResource(worker,tree->pos,radius(Kind::Forest)+18,radius(Kind::Forest)+56)){worker.source=worker.target=candidate.id;worker.objective=-1;worker.work=0;worker.order=Order::Gather;return true;}}
 worker.route.clear();worker.order=Order::Idle;return false;
}
inline bool World::returnCargo(Entity& e){
 std::vector<int> depots;for(const auto& b:entities)if(b.hp>0&&b.team==e.team&&b.build<=0&&((e.kind==Kind::Tanker&&b.kind==Kind::Shipyard)||(e.kind==Kind::CrystalMiner&&b.kind==Kind::Reliquary)||(e.kind==Kind::Worker&&(b.kind==Kind::Hall||(e.cargoKind==Kind::Forest&&b.kind==Kind::LumberMill)))))depots.push_back(b.id);
 std::sort(depots.begin(),depots.end(),[&](int a,int b){return distance(e.pos,get(a)->pos)<distance(e.pos,get(b)->pos);});
 for(int id:depots)if(approachResource(e,get(id)->pos,e.kind==Kind::Tanker?130.f:radius(get(id)->kind)+18,e.kind==Kind::Tanker?180.f:125.f)){e.target=id;e.work=0;e.order=Order::Return;return true;}
 e.route.clear();e.order=Order::Idle;message="Nessun deposito raggiungibile: il carico resta all'operaio.";return false;
}
inline bool World::plant(int worker,sf::Vector2f p){auto w=get(worker);if(!w||w->kind!=Kind::Worker||w->team!=0||result)return false;
 if(busy(*w)){message="Operaio impegnato nella costruzione.";return false;}
 if(upgrades[w->team][int(Research::Forestry)]==0){message="Ricerca Silvicoltura alla segheria per piantare alberi.";return false;}
 if(p.x<25||p.x>width()-25||p.y<25||p.y>height()-25||water(p)){message="Pianta su terreno asciutto dentro la mappa.";return false;}
 for(const auto& e:entities)if(e.hp>0&&!mobile(e.kind)&&distance(e.pos,p)<radius(e.kind)+25){message="Spazio occupato per l'albero.";return false;}
 if(!pay({10,0,0}))return false;int id=add(Kind::Forest,-1,p);get(id)->build=get(id)->buildTotal=12;message="Albero piantato: cresce in 12 secondi (10 oro).";return true;
}
inline std::string World::researchIssue(int id,Research r,bool checkCost)const{
 auto b=get(id);const auto& d=researchCatalog()[int(r)];if(!missionResearchAllows(r))return "Ricerca sbloccata in una missione successiva.";if(guided&&b&&b->team==0&&chapter<3)return "Completa prima gli obiettivi: fattoria, caserma e tre soldati.";if(guided&&b&&b->team==0&&r==Research::Town3&&chapter<4)return "Esplora prima la strada fino all avamposto.";if(!b||b->team<0||b->kind!=d.building)return std::string("Seleziona ")+name(d.building)+" per "+d.name+".";
 int team=b->team;if(result)return "Partita terminata.";if(b->build>0)return "Edificio ancora in costruzione.";if(b->production>0)return "Edificio occupato: attendi o annulla la produzione.";
 if((r==Research::Town2&&townLevel[team]>=2)||(r==Research::Town3&&townLevel[team]>=3)||(r==Research::Town4&&townLevel[team]>=4))return "Comando gia potenziato a questo livello.";
 bool local=r==Research::Barracks2||r==Research::Barracks3;
 if(local){int required=r==Research::Barracks2?1:2;if(b->level>required)return std::string(d.name)+" gia completata per questa caserma.";if(b->level<required)return "Richiede prima Caserma II.";}
 else if(upgrades[team][int(r)]>=d.maxLevel)return std::string(d.name)+" gia al livello massimo.";
 if(!local)for(const auto& other:entities)if(other.hp>0&&other.team==team&&other.production>0&&other.research==int(r))return "Questa ricerca e gia in corso in un altro edificio.";
 if(townLevel[team]<d.tier)return "Richiede Comando livello "+std::to_string(d.tier)+".";
 if(r==Research::Barracks2&&!has(team,Kind::LumberMill))return "Richiede Segheria: costruiscila con un operaio.";
 if(r==Research::Barracks3&&upgrades[team][int(Research::Elite)]==0)return "Richiede Dottrina veterana al Santuario.";
 if(r==Research::Town2&&!has(team,Kind::Barracks))return "Richiede una Caserma completata.";
 if(r==Research::Town4&&(townLevel[team]<3||!has(team,Kind::Reliquary)))return "Richiede Comando III e Reliquiario.";
 if(r==Research::Town3&&!has(team,Kind::Forge))return "Richiede una Fucina completata.";
 if(checkCost&&!(team==0&&cheatFree)&&( (team==0?crystals:enemyCrystals)<d.cost.crystals||(team==0?gold:enemyGold)<d.cost.gold||(team==0?wood:enemyWood)<d.cost.wood||(team==0?oil:enemyOil)<d.cost.oil))return "Risorse insufficienti: "+std::to_string(d.cost.gold)+" oro, "+std::to_string(d.cost.wood)+" legno, "+std::to_string(d.cost.oil)+" petrolio.";
 return "";
}
inline bool World::research(int id,Research r){auto issue=researchIssue(id,r);if(!issue.empty()){message=issue;return false;}auto b=get(id);const auto& d=researchCatalog()[int(r)];if(!pay(d.cost,b->team))return false;b->paid=(b->team==0&&cheatFree)?Cost{}:d.cost;b->production=b->productionTotal=d.seconds;b->research=int(r);message=std::string("Ricerca avviata: ")+d.name+" ("+std::to_string(int(d.seconds))+" s).";return true;}
inline bool World::cancel(int id){auto b=get(id);if(!b||b->team<0||b->production<=0)return false;auto refund=[](int& bank,int amount){bank=std::min(9999999,bank+amount);};refund(b->team==0?gold:enemyGold,b->paid.gold);refund(b->team==0?wood:enemyWood,b->paid.wood);refund(b->team==0?oil:enemyOil,b->paid.oil);refund(b->team==0?crystals:enemyCrystals,b->paid.crystals);b->production=0;b->research=-1;b->upgradeKind=Kind::Count;b->paid={};message="Produzione/ricerca annullata, risorse restituite.";return true;}
inline bool World::upgradeTower(int id,Kind kind){auto b=get(id);if(!b||b->team<0||b->kind!=Kind::WatchTower||b->build>0||b->production>0||(kind!=Kind::GuardTower&&kind!=Kind::CannonTower)){message="Seleziona una torre di vedetta completata e libera.";return false;}auto reason=unavailable(kind,b->team);if(!reason.empty()){message=reason;return false;}Cost c{def(kind).cost.gold-def(Kind::WatchTower).cost.gold,def(kind).cost.wood-def(Kind::WatchTower).cost.wood,0};if(!pay(c,b->team))return false;b->paid=(b->team==0&&cheatFree)?Cost{}:c;b->upgradeKind=kind;b->production=b->productionTotal=8;b->research=-2;message="Evoluzione torre avviata.";return true;}
inline void World::finishResearch(Entity& e){auto r=Research(e.research);if(r==Research::Barracks2||r==Research::Barracks3){e.level=r==Research::Barracks2?2:3;upgrades[e.team][int(r)]=1;float bonus=modern()?def(e.kind).hp*.25f:150.f;e.maxHp+=bonus;e.hp+=bonus;}else ++upgrades[e.team][int(r)];if(r==Research::Town2||r==Research::Town3||r==Research::Town4){townLevel[e.team]=r==Research::Town2?2:r==Research::Town3?3:4;if(modern())e.level=townLevel[e.team];e.maxHp+=400;e.hp+=400;}
 message=std::string("Completata: ")+researchCatalog()[int(r)].name;e.research=-1;e.paid={};
}
inline float World::attackDamage(const Entity& e)const{int team=e.team;auto& u=upgrades[team];float extra=def(e.kind).domain==Domain::Sea?u[int(Research::ShipGuns)]*7.f:((e.kind==Kind::Archer||e.kind==Kind::Ranger)?u[int(Research::Archery)]*4.f:u[int(Research::Weapons)]*4.f);return (def(e.kind).damage+extra)*(e.rage>0?1.5f:1.f)*(1+localBonus(team,e.pos))*(e.rune==1?1.2f:1.f)*(e.rune==3||e.rune==4?1.15f:1.f)*(e.form==1?1.4f:1.f)*(modern()?1+.08f*(e.level-1):1.f);}
inline void World::damage(Entity& e,float amount){if(e.team>=0&&barrierTime[e.team]>0&&distance(e.pos,barrierCenter[e.team])<320)return;if(e.channel>0)e.channel=0;if(e.team==0&&cheatGod)return;float armor=def(e.kind).armor;if(e.team>=0)armor+=upgrades[e.team][int(def(e.kind).domain==Domain::Sea?Research::ShipArmor:Research::Armor)]*2.f;
 e.hp-=std::max(1.f,amount-armor)*(e.shield>0?.45f:1.f)*(e.rune==2?.8f:1.f);if(e.hp<=0){navDirty=true;for(auto& p:entities)if(p.carrier==e.id)p.hp=0;}}
inline bool World::spawnPosition(const Entity& b,Kind kind,sf::Vector2f& out)const{sf::Vector2f center=b.pos;auto domain=def(kind).domain;if(domain==Domain::Sea&&mission<0)center.y=1320;
 for(int ring=0;ring<(domain==Domain::Sea?10:5);++ring)for(int a=0;a<16;++a){float radius=domain==Domain::Sea?float(40+ring*35):float(105+ring*35);auto p=center+sf::Vector2f(std::cos(a*.392699f),std::sin(a*.392699f))*radius;
  if(blocked(int(p.x/cell),int(p.y/cell),domain))continue;bool free=true;for(const auto& e:entities)if(e.hp>0&&e.carrier<0&&mobile(e.kind)&&def(e.kind).domain==domain&&distance(e.pos,p)<def(kind).radius+25){free=false;break;}if(free){out=p;return true;}}
 return false;
}
inline bool World::supports(Kind k,Spell s,int team)const{
 if(k==Kind::Mage)return s==Spell::Firestorm||s==Spell::Haste||s==Spell::Shield||s==Spell::Reveal;
 if(k==Kind::Warlock)return s==Spell::Raise||s==Spell::Storm||s==Spell::Rage||s==Spell::Reveal;
 if(k==Kind::Healer)return s==Spell::Heal||s==Spell::Haste;
 if(k==Kind::Paladin||k==Kind::Angel)return s==Spell::Heal||s==Spell::Shield||s==Spell::Reveal;
 if(k==Kind::Guardian)return s==Spell::Shield||(faction[team]==0?s==Spell::Heal:s==Spell::Rage);
 if(k==Kind::Scout)return s==Spell::Reveal;return false;
}
inline bool World::cast(int id,Spell spell,sf::Vector2f p){auto e=get(id);if(result||!e||e->stun>0||e->form>=3||e->carrier>=0||!supports(e->kind,spell,e->team)){message="L'unita selezionata non conosce questa magia.";return false;}
 int team=e->team;int price=e->kind==Kind::Scout?0:spellCost(spell);if(e->mana<price||e->spellCooldown>0){message="Mana insufficiente o magia in ricarica.";return false;}
 if(distance(e->pos,p)>460){message="Bersaglio della magia troppo lontano (460).";return false;}
 if((spell==Spell::Firestorm||spell==Spell::Storm||spell==Spell::Raise)&&upgrades[team][int(Research::Arcana)]<1){message="Ricerca Sapienza arcana prima di usare questa magia.";return false;}
 Entity* ally=nullptr;for(auto& t:entities)if(t.hp>0&&t.team==team&&t.carrier<0&&distance(t.pos,p)<90){if(!ally||distance(t.pos,p)<distance(ally->pos,p))ally=&t;}
 if((spell==Spell::Heal||spell==Spell::Haste||spell==Spell::Shield||spell==Spell::Rage)&&!ally){message="Scegli un bersaglio alleato.";return false;}
 if(spell==Spell::Heal&&ally->hp>=ally->maxHp){message="Il bersaglio e gia in piena salute.";return false;}
 if(spell==Spell::Raise){int living=0,corpses=0;for(const auto& t:entities){if(t.hp>0&&t.kind==Kind::Spirit&&t.team==team)++living;if(t.hp<=0&&t.lifetime!=-999&&mobile(t.kind)&&t.kind!=Kind::Spirit&&distance(t.pos,p)<150)++corpses;}if(!corpses||living>=8){message="Servono caduti vicini; massimo otto spiriti.";return false;}}
 e->mana-=price;e->spellCooldown=spell==Spell::Reveal?15.f:4.f;
 if(spell==Spell::Heal)ally->hp=std::min(ally->maxHp,ally->hp+75);
 if(spell==Spell::Haste)ally->haste=20;
 if(spell==Spell::Rage)ally->rage=20;
 if(spell==Spell::Shield)ally->shield=15;
 if(spell==Spell::Reveal)e->reveal=25;
 if(spell==Spell::Firestorm)for(auto& t:entities)if(t.hp>0&&t.team>=0&&t.team!=team&&t.carrier<0&&distance(t.pos,p)<130)damage(t,85+upgrades[team][int(Research::Arcana)]*10.f);
 if(spell==Spell::Storm)zones.push_back({p,7,.5f,team});
 if(spell==Spell::Raise){int living=0;for(auto& t:entities)if(t.hp>0&&t.kind==Kind::Spirit&&t.team==team)++living;std::vector<sf::Vector2f> sites;for(auto& t:entities)if(t.hp<=0&&mobile(t.kind)&&t.kind!=Kind::Spirit&&distance(t.pos,p)<150&&t.lifetime!=-999){sites.push_back(t.pos);t.lifetime=-999;if(int(sites.size())==std::min(2,8-living))break;}for(auto pos:sites){pos.y=std::min(pos.y,1150.f);add(Kind::Spirit,team,pos);}}
 effects.push_back({p,100,.6f,.6f,spell==Spell::Heal?2:3,team});message=std::string("Magia: ")+spellName(spell);return true;
}
inline void World::economy(Entity& e,float dt){if(specialEconomy(e,dt))return;
 if(e.order==Order::Board&&e.route.empty()){auto ship=get(e.target);if(!ship||(ship->kind!=Kind::Transport&&ship->kind!=Kind::AirArk)||distance(e.pos,ship->pos)>155){e.order=Order::Idle;return;}int count=0;for(const auto& p:entities)if(p.hp>0&&p.carrier==ship->id)++count;if(count<6){e.carrier=ship->id;e.selected=false;e.order=Order::Idle;e.pos=ship->pos;}else {e.order=Order::Idle;message="Trasporto pieno.";}return;}
 if(e.order==Order::Gather||e.order==Order::Return){auto t=get(e.target);if(!t){
   if(e.order==Order::Return&&e.cargo>0){returnCargo(e);return;}
   bool tree=false;sf::Vector2f preferred=e.goal;for(const auto& dead:entities)if(dead.id==e.target&&dead.kind==Kind::Forest){tree=true;preferred=dead.pos;break;}
   if(tree&&e.kind==Kind::Worker)assignTree(e,preferred);else {e.order=Order::Idle;e.route.clear();}return;
  }
  float reach=e.order==Order::Return?(e.kind==Kind::Tanker?185.f:130.f):radius(t->kind)+(t->kind==Kind::Forest?56.f:85.f);
  // Work starts as soon as a worker reaches the usable perimeter, even if another worker occupies the last waypoint.
  if(distance(e.pos,t->pos)<=reach)e.route.clear();
 }
 if(!e.route.empty())return;
 if(e.order==Order::Repair){auto b=get(e.target);if(!b||b->hp>=b->maxHp){e.order=Order::Idle;return;}if(distance(e.pos,b->pos)>125){e.order=Order::Idle;return;}e.work+=dt;if(e.work>=1){e.work=0;if(!pay({2,2,0},e.team)){e.order=Order::Idle;return;}b->hp=std::min(b->maxHp,b->hp+30);}return;}
 if(e.order!=Order::Gather&&e.order!=Order::Return)return;auto target=get(e.target);if(!target)return;
 if(e.order==Order::Gather){float reach=radius(target->kind)+(target->kind==Kind::Forest?56.f:85.f);if(distance(e.pos,target->pos)>reach){if(target->kind==Kind::Forest)assignTree(e,target->pos);else if(!path(e,target->pos,radius(target->kind)+65))e.order=Order::Idle;return;}
  e.work+=dt*(e.haste>0?1.4f:1.f)*buildingRate(e.team);if(e.work<2.5f)return;e.work=0;
  Entity* source=target;if(e.kind==Kind::Tanker)source=get(target->source);if(!source){e.order=Order::Idle;return;}
  int amount=10;if(modern()){amount+=2*(townLevel[e.team]-1);if(source->kind==Kind::Forest&&has(e.team,Kind::LumberMill))amount+=2;if(source->kind==Kind::Oil&&has(e.team,Kind::Refinery))amount+=3;amount=int(amount*(1+localBonus(e.team,e.pos)));}e.cargo=std::min(amount,int(source->hp));e.cargoKind=source->kind;source->hp-=e.cargo;if(source->hp<=0)navDirty=true;returnCargo(e);
 }else{
  if(distance(e.pos,target->pos)>(e.kind==Kind::Tanker?185.f:130.f)){returnCargo(e);return;}
  int& g=e.team==0?gold:enemyGold;int& w=e.team==0?wood:enemyWood;int& o=e.team==0?oil:enemyOil;if(e.cargoKind==Kind::Mine)g+=e.cargo+(modern()?0:(townLevel[e.team]-1)*2);else if(e.cargoKind==Kind::Forest)w+=e.cargo+(!modern()&&has(e.team,Kind::LumberMill)?2:0);else o+=e.cargo+(!modern()&&has(e.team,Kind::Refinery)?3:0);
  g=std::min(9999999,g);w=std::min(9999999,w);o=std::min(9999999,o);
  if(e.team==0){if(e.cargoKind==Kind::Mine)deliveredGold+=e.cargo;else if(e.cargoKind==Kind::Forest)deliveredWood+=e.cargo;}
  e.cargo=0;auto source=get(e.source);if(source)gather(e,source->id);else {sf::Vector2f preferred=e.pos;for(const auto& dead:entities)if(dead.id==e.source){preferred=dead.pos;break;}if(e.cargoKind!=Kind::Forest||!assignTree(e,preferred)){e.order=Order::Idle;message="Risorsa esaurita: assegna un nuovo giacimento o ripianta gli alberi.";}}
 }
}
inline void World::combat(Entity& e,float){if(busy(e)||e.stun>0||e.channel>0||e.form==4)return;if(e.duty>=2&&e.order==Order::Attack){auto t=get(e.target);if(!t||distance(t->pos,e.anchor)>(e.duty==4?650.f:320.f)){e.order=Order::Idle;e.target=-1;e.route.clear();}}if(layoutVersion&&mission==0&&e.team==1&&e.kind==Kind::Worker){for(const auto& attacker:entities)if(attacker.hp>0&&attacker.team==0&&attacker.order==Order::Attack&&attacker.target==e.id&&distance(e.pos,attacker.pos)<240){e.target=e.objective=attacker.id;e.order=Order::Attack;break;}}
 const auto& d=def(e.kind);if(d.damage<=0||e.build>0)return;
 if(raidsEnabled&&missionRaidReady()&&(mission!=4||def(e.kind).domain==Domain::Sea)&&(!guided||(chapter>=4&&time>=600))&&e.team==1&&time>=120&&e.kind!=Kind::Worker&&mobile(e.kind)&&e.order==Order::Idle){int target=-1;if(mission==3&&get(archivist))target=archivist;else for(const auto& t:entities)if(t.team==0&&t.hp>0&&t.kind==(def(e.kind).domain==Domain::Sea?Kind::Shipyard:Kind::Hall)){target=t.id;break;}if(target>=0)attack(e,target);}
 if(e.order==Order::Idle||!mobile(e.kind)){int id=-1;float best=mobile(e.kind)?std::max(220.f,d.range):d.range+40;for(const auto& t:entities)if(canHit(e,t)&&distance(e.pos,t.pos)<best){best=distance(e.pos,t.pos);id=t.id;}if(id>=0){e.target=e.objective=id;e.order=Order::Attack;}}
 if(e.order!=Order::Attack)return;auto t=get(e.target);if(!t||!canHit(e,*t)){t=get(e.objective);if(!t||!canHit(e,*t)){e.order=Order::Idle;e.route.clear();return;}e.target=e.objective;e.route.clear();}
 if(mobile(e.kind)&&!mobile(t->kind))for(auto& candidate:entities)if(mobile(candidate.kind)&&canHit(e,candidate)&&distance(candidate.pos,e.pos)<std::max(180.f,d.range)){e.target=candidate.id;t=&candidate;break;}
 float reach=d.range+radius(t->kind);if(distance(e.pos,t->pos)<=reach){e.route.clear();if(e.cooldown>0)return;e.cooldown=d.cooldown/(e.haste>0?1.4f:1.f);float raw=attackDamage(e);
  if(e.kind==Kind::Demolition){sf::Vector2f p=e.pos;for(auto& victim:entities)if(victim.hp>0&&victim.team>=0&&victim.team!=e.team&&distance(victim.pos,p)<155)damage(victim,raw);e.hp=0;effects.push_back({p,155,.7f,.7f,1,e.team});}
  else if(d.range>100)shots.push_back({e.pos,t->id,e.team,raw,d.splash,2.5f,e.kind,e.form==3});
  else {damage(*t,raw);effects.push_back({t->pos,22,.16f,.16f,0,e.team});}
 }else if(mobile(e.kind)&&(e.route.empty()||distance(e.goal,t->pos)>65))path(e,t->pos,std::max(40.f,reach-3));
}
inline void World::steer(Entity& e,float dt){if(e.route.empty()){if(e.order==Order::Move)e.order=Order::Idle;return;}auto dest=e.route.front();auto domain=def(e.kind).domain;
 if(blocked(int(dest.x/cell),int(dest.y/cell),domain)){auto goal=e.goal;auto target=get(e.target);if(e.order==Order::Gather&&target&&target->kind==Kind::Forest)assignTree(e,target->pos);else if(e.order==Order::Return)returnCargo(e);else if(!path(e,goal,e.order==Order::Move?0.f:130.f)&&e.order!=Order::Build)e.order=Order::Idle;return;}
 float length=distance(dest,e.pos),speed=def(e.kind).speed*(e.haste>0?1.4f:1.f)*(e.rune==0?1.2f:1.f)*(e.form==1?1.15f:1.f);auto p=length<=speed*dt?dest:e.pos+(dest-e.pos)*(speed*dt/length);const auto intended=p;
 for(const auto& o:entities)if(o.hp>0&&o.carrier<0&&o.id!=e.id&&mobile(o.kind)&&def(o.kind).domain==domain){float dist=distance(p,o.pos),minDist=domain==Domain::Sea?42.f:25.f;if(dist>.1f&&dist<minDist)p+=(p-o.pos)*((minDist-dist)*dt*3/dist);}
 if((e.kind==Kind::Worker||e.kind==Kind::Tanker)&&distance(p,dest)>length-std::min(length,speed*dt)*.2f)p=intended;
 if(blocked(int(p.x/cell),int(p.y/cell),domain))p=intended;
 if(!blocked(int(p.x/cell),int(p.y/cell),domain)||(blocked(int(e.pos.x/cell),int(e.pos.y/cell),domain)&&distance(p,dest)<length))e.pos=p;if(distance(e.pos,dest)<(e.route.size()>1?18.f:8.f)){
  bool clear=true;if(e.route.size()>1){auto next=e.route[1];for(int sample=1;sample<=12;++sample){auto point=e.pos+(next-e.pos)*(sample/12.f);if(blocked(int(point.x/cell),int(point.y/cell),domain)){clear=false;break;}}}
  if(clear)e.route.erase(e.route.begin());
 }
}
inline void World::processShots(float dt){for(auto& s:shots){if(cheatFreeze&&s.team==1)continue;s.remaining-=dt;auto t=get(s.target);if(!t||t->carrier>=0||(t->team==s.team&&t->form!=3&&!s.rogue)){s.remaining=0;continue;}float dist=distance(s.pos,t->pos);if(dist<=580*dt+radius(t->kind)){
  if(s.splash>0){for(auto& v:entities)if(v.hp>0&&v.carrier<0&&v.team>=0&&(v.team!=s.team||v.form==3||s.rogue)&&distance(v.pos,t->pos)<=s.splash&&!(def(v.kind).domain==Domain::Air&&!def(s.source).antiAir))damage(v,s.damage);}
  else damage(*t,s.damage);effects.push_back({t->pos,s.splash>0?s.splash:20,.35f,.35f,s.splash>0?1:0,s.team});s.remaining=0;
 }else s.pos+=(t->pos-s.pos)*(580*dt/dist);}
 shots.erase(std::remove_if(shots.begin(),shots.end(),[](auto& s){return s.remaining<=0;}),shots.end());
 for(auto& z:zones){if(cheatFreeze&&z.team==1)continue;z.remaining-=dt;z.tick-=dt;if(z.tick<=0){z.tick=.5f;for(auto& e:entities)if(e.hp>0&&e.carrier<0&&e.team>=0&&e.team!=z.team&&distance(e.pos,z.pos)<130)damage(e,17);}}
 zones.erase(std::remove_if(zones.begin(),zones.end(),[](auto& z){return z.remaining<=0;}),zones.end());for(auto& fx:effects)fx.remaining-=dt;effects.erase(std::remove_if(effects.begin(),effects.end(),[](auto& f){return f.remaining<=0;}),effects.end());
}
inline void World::step(float dt){
 if(result)return;time+=dt;updateV2(dt);if(mission<5)wave+=dt;visionTimer-=dt;if(visionTimer<=0){refreshVision();visionTimer=.15f;}std::vector<int> ready;
 for(auto& e:entities){if(e.hp<=0||(cheatFreeze&&e.team==1))continue;if(resource(e.kind)){e.build=std::max(0.f,e.build-dt);continue;}if(e.carrier>=0){auto c=get(e.carrier);if(c)e.pos=c->pos;else e.hp=0;continue;}
  e.cooldown=std::max(0.f,e.cooldown-dt);e.spellCooldown=std::max(0.f,e.spellCooldown-dt);e.haste=std::max(0.f,e.haste-dt);e.rage=std::max(0.f,e.rage-dt);e.shield=std::max(0.f,e.shield-dt);e.reveal=std::max(0.f,e.reveal-dt);
  if(e.stun>0||e.channel>0)continue;if(e.form==4){steer(e,dt);continue;}
  if(e.maxMana>0)e.mana=std::min(e.maxMana,e.mana+dt*(1.5f+upgrades[e.team][int(Research::Arcana)]*.5f));
  if(e.kind==Kind::Spirit){e.lifetime-=dt;if(e.lifetime<=0){e.hp=0;continue;}}
  if(!mobile(e.kind)){
   if(e.build>0){auto worker=get(e.constructionWorker);if(worker&&worker->order==Order::Build&&worker->target==e.id&&distance(worker->pos,e.pos)<=radius(e.kind)+85){worker->route.clear();e.build=std::max(0.f,e.build-dt*(cheatFast&&e.team==0?40.f:1.f)*buildingRate(e.team));if(e.build==0){worker->order=Order::Idle;worker->target=-1;message=std::string(name(e.kind))+" completata. Operaio disponibile.";}}}
   else if(e.production>0){e.production=std::max(0.f,e.production-dt*(cheatFast&&e.team==0?40.f:1.f)*(modern()?1+.1f*(e.level-1):1.f));if(e.production==0){if(e.research==-3){++e.level;e.hp+=def(e.kind).hp*.25f;e.maxHp+=def(e.kind).hp*.25f;e.paid={};e.research=-1;}else if(e.research<=-4&&e.research>=-8){++runes[e.team][-4-e.research];e.research=-1;e.paid={};}else if(e.upgradeKind!=Kind::Count&&e.research==-2){float missing=e.maxHp-e.hp;e.kind=e.upgradeKind;e.maxHp=def(e.kind).hp;e.hp=std::max(1.f,e.maxHp-missing);e.upgradeKind=Kind::Count;e.research=-1;e.paid={};navDirty=true;message="Torre potenziata.";}else if(e.research>=0)finishResearch(e);else ready.push_back(e.id);}}
   if(e.kind==Kind::GuardTower||e.kind==Kind::CannonTower)combat(e,dt);continue;
  }
  if((e.kind==Kind::Healer||e.kind==Kind::Angel)&&e.mana>=25&&e.spellCooldown<=0){int ally=-1;for(auto& t:entities)if(t.hp>0&&t.team==e.team&&t.carrier<0&&mobile(t.kind)&&t.hp<t.maxHp-35&&distance(t.pos,e.pos)<280){ally=t.id;break;}if(ally>=0){auto target=get(ally);target->hp=std::min(target->maxHp,target->hp+65);e.mana-=25;e.spellCooldown=4;effects.push_back({target->pos,45,.5f,.5f,2,e.team});}}
  if(e.order==Order::Build){auto site=get(e.target);if(!site||site->build<=0){e.order=Order::Idle;e.target=-1;e.route.clear();}else{if(distance(e.pos,site->pos)<=radius(site->kind)+85)e.route.clear();else if(e.route.empty())path(e,site->pos,radius(site->kind)+65);steer(e,dt);continue;}}
  combat(e,dt);economy(e,dt);steer(e,dt);
 }
 // No entity insertions occur while iterating entities.
 for(int id:ready){auto b=get(id);if(!b)continue;sf::Vector2f pos;Kind k=b->training;int team=b->team;if(population(team,false)+def(k).supply>capacity(team)){b->production=.05f;if(team==0)message="Produzione in attesa: completa una fattoria per avere posti disponibili.";continue;}if(spawnPosition(*b,k,pos)){b->paid={};int trained=add(k,team,pos);if(modern()){auto unit=get(trained);unit->level=get(id)->level;unit->hp=unit->maxHp=def(k).hp*(1+.08f*(unit->level-1));}message=std::string(name(k))+" pronto.";}else{b->production=.05f;message="Uscita occupata: libera spazio davanti all'edificio.";}}
 processShots(dt);if(mission>=0){updateMission(dt);if(!result&&raidsEnabled&&!cheatFreeze){aiTimer-=dt;if(aiTimer<=0){aiTimer=2;enemyEconomy();}}return;}bool player=false,enemy=false;for(const auto& e:entities)if(e.hp>0&&e.kind==Kind::Hall){if(e.team==0)player=true;else enemy=true;}
 if(!player||!enemy){result=player?1:-1;message=player?"VITTORIA! R: nuova partita.":"SCONFITTA. R: riprova.";return;}
 if(guided)updateProgression();if(raidsEnabled&&!cheatFreeze){aiTimer-=dt;if(aiTimer<=0){aiTimer=2;enemyEconomy();}}

}
}







