#pragma once
namespace alpha {
struct CampaignLayout {
 const char* name;sf::Vector2f home,enemy,goal;
 std::array<sf::Vector2f,4> sites;
 std::array<sf::Vector2f,6> woods;
};
inline const CampaignLayout& campaignLayout(int n){
 static const CampaignLayout maps[]={
 {"Valle dei Mulini",{.16f,.62f},{.72f,.74f},{.77f,.34f},{{{.34f,.57f},{.50f,.35f},{.60f,.64f},{.77f,.34f}}},{{{.12f,.20f},{.36f,.18f},{.57f,.14f},{.89f,.58f},{.43f,.83f},{.15f,.88f}}}},
 {"Passo delle Sentinelle",{.13f,.77f},{.84f,.22f},{.84f,.18f},{{{.35f,.65f},{.53f,.48f},{.71f,.34f},{.84f,.18f}}},{{{.14f,.22f},{.35f,.86f},{.56f,.80f},{.49f,.20f},{.86f,.61f},{.75f,.86f}}}},
 {"Bosco dei Custodi",{.16f,.76f},{.81f,.22f},{.57f,.33f},{{{.57f,.33f},{.37f,.58f},{.72f,.48f},{.81f,.22f}}},{{{.14f,.18f},{.36f,.25f},{.75f,.76f},{.89f,.53f},{.45f,.86f},{.36f,.48f}}}},
 {"Gola delle Braci",{.14f,.72f},{.84f,.24f},{.54f,.49f},{{{.54f,.49f},{.75f,.28f},{.34f,.64f},{.65f,.67f}}},{{{.13f,.24f},{.36f,.18f},{.29f,.86f},{.73f,.84f},{.90f,.57f},{.63f,.16f}}}},
 {"Costa delle Citta Libere",{.16f,.34f},{.81f,.25f},{.70f,.80f},{{{.70f,.80f},{.16f,.54f},{.86f,.54f},{.26f,.82f}}},{{{.13f,.12f},{.36f,.18f},{.60f,.15f},{.86f,.13f},{.43f,.42f},{.72f,.82f}}}},
 {"Altipiano delle Testimonianze",{.14f,.73f},{.84f,.22f},{.55f,.47f},{{{.34f,.31f},{.55f,.47f},{.69f,.74f},{.84f,.22f}}},{{{.12f,.24f},{.31f,.16f},{.45f,.84f},{.84f,.57f},{.91f,.82f},{.64f,.15f}}}},
 {"Valle delle Fornaci",{.13f,.76f},{.85f,.22f},{.85f,.22f},{{{.37f,.59f},{.57f,.36f},{.74f,.61f},{.85f,.22f}}},{{{.13f,.23f},{.32f,.17f},{.43f,.84f},{.62f,.83f},{.89f,.78f},{.89f,.45f}}}},
 {"Arcipelago delle Chiuse",{.17f,.20f},{.80f,.18f},{.56f,.72f},{{{.56f,.72f},{.80f,.31f},{.17f,.31f},{.27f,.66f}}},{{{.12f,.08f},{.40f,.10f},{.68f,.08f},{.56f,.73f},{.27f,.67f},{.82f,.65f}}}},
 {"La memoria della Pietra",{.14f,.78f},{.84f,.23f},{.56f,.49f},{{{.35f,.29f},{.56f,.49f},{.72f,.74f},{.84f,.23f}}},{{{.14f,.25f},{.35f,.14f},{.43f,.87f},{.88f,.52f},{.89f,.83f},{.64f,.16f}}}},
 {"Prima dell ultima alba",{.14f,.76f},{.86f,.22f},{.69f,.48f},{{{.36f,.32f},{.51f,.67f},{.73f,.37f},{.78f,.69f}}},{{{.12f,.23f},{.33f,.15f},{.36f,.87f},{.65f,.85f},{.91f,.50f},{.62f,.15f}}}},
 {"Il cuore del mondo",{.13f,.76f},{.84f,.20f},{.84f,.47f},{{{.49f,.72f},{.74f,.68f},{.86f,.83f},{.84f,.47f}}},{{{.12f,.22f},{.33f,.14f},{.29f,.88f},{.66f,.18f},{.68f,.87f},{.93f,.60f}}}}
 };return maps[std::clamp(n,0,missionCount-1)];
}
inline void World::prepareCampaignRegion(){
 const auto& spec=campaignLayout(mission);layoutVersion=1;
 sf::Vector2f oldHome;std::vector<sf::Vector2f> oldEnemy;
 for(const auto& e:entities)if(e.kind==Kind::Hall&&e.hp>0){if(e.team==0&&oldHome==sf::Vector2f())oldHome=e.pos;if(e.team==1)oldEnemy.push_back(e.pos);}
 resizeMap(112+mission*8,70+mission*5);
 auto point=[&](sf::Vector2f v){return sf::Vector2f(v.x*width(),v.y*height());};
 const auto home=point(spec.home),enemy=point(spec.enemy);mapGoal=point(spec.goal);
 std::array<sf::Vector2f,4> sites;for(int i=0;i<4;++i)sites[i]=point(spec.sites[i]);
 struct Anchor{sf::Vector2f from,to;};std::vector<Anchor> enemyAnchors;
 for(int i=0;i<int(oldEnemy.size());++i)enemyAnchors.push_back({oldEnemy[i],i==0?enemy:sites[std::min(i,2)]});
 std::vector<Anchor> objectiveAnchors;
 for(int i=0;i<int(missionGuards.size());++i)if(auto e=get(missionGuards[i])){
  sf::Vector2f target=sites[std::min(i,3)];if(mission==2)target=i<2?sites[0]+(e->pos-sf::Vector2f(1240,420)):enemy+(e->pos-sf::Vector2f(2200,430));
  if(mission==3)target=sites[1];objectiveAnchors.push_back({e->pos,target});
 }
 for(const auto& e:entities)if(e.team<0&&!resource(e.kind))objectiveAnchors.push_back({e.pos,mission==0?mapGoal:mission==10?mapGoal:sites[0]});
 if(mission==3)if(auto e=get(archivist))objectiveAnchors.push_back({e->pos,sites[0]});
 if(mission==7){enemyAnchors.push_back({{3100,1320},sites[1]});}
 for(auto& e:entities){
  if(resource(e.kind))continue;const auto original=e.pos;bool placed=false;
  if(mission==3&&e.id==archivist){e.pos=sites[0];placed=true;}
  for(const auto& anchor:objectiveAnchors)if(distance(original,anchor.from)<1){e.pos=anchor.to;placed=true;break;}
  if(!placed&&e.team<0){e.pos=sites[0];placed=true;}
  if(!placed&&e.team==0){e.pos=home+(original-oldHome)*1.45f;placed=true;}
  if(!placed&&e.team==1){float best=1e9;Anchor closest{original,enemy};
   for(auto a:enemyAnchors)if(distance(original,a.from)<best){best=distance(original,a.from);closest=a;}
   for(auto a:objectiveAnchors)if(distance(original,a.from)<std::min(best,360.f)){best=distance(original,a.from);closest=a;}
   e.pos=closest.to+(original-closest.from)*1.5f;
  }
  if(mission==7&&e.kind==Kind::Shipyard)e.pos=e.team==0?sites[2]:sites[1];
  if(mission==4&&e.kind==Kind::Shipyard)e.pos=sites[2];
  if(e.lifetime==-777)e.pos=mapGoal+sf::Vector2f(0,-180);
  e.pos.x=std::clamp(e.pos.x,140.f,width()-140);e.pos.y=std::clamp(e.pos.y,140.f,height()-140);e.goal=e.pos;e.route.clear();
 }
 entities.erase(std::remove_if(entities.begin(),entities.end(),[](const Entity& e){return resource(e.kind);}),entities.end());
 if(mission==0){add(Kind::Hall,1,enemy);add(Kind::Farm,1,enemy+sf::Vector2f(-220,-180));for(int i=0;i<3;++i)add(Kind::Worker,1,enemy+sf::Vector2f(-80+i*80.f,100));raidsEnabled=false;}
 // A new terrain grid is authored for the entire region; no old terrain or forest rows survive.
 auto ellipse=[](sf::Vector2f p,sf::Vector2f c,float rx,float ry){float x=(p.x-c.x)/rx,y=(p.y-c.y)/ry;return x*x+y*y<1;};
 auto coast=[&](float x){return (mission==7?.31f:.57f)+.025f*std::sin(x*14+mission);};
 for(int y=0;y<rows;++y)for(int x=0;x<cols;++x){sf::Vector2f uv{(x+.5f)/cols,(y+.5f)/rows};bool sea=false;
  if(mission==4||mission==7){sea=uv.y>coast(uv.x);if(mission==4&&(ellipse(uv,{.70f,.80f},.145f,.12f)||ellipse(uv,{.26f,.82f},.11f,.10f)))sea=false;
   if(mission==7&&(ellipse(uv,{.56f,.72f},.13f,.12f)||ellipse(uv,{.27f,.66f},.105f,.10f)||ellipse(uv,{.82f,.65f},.10f,.12f)))sea=false;}
  if(mission==10){float river=.49f+.025f*std::sin(uv.y*11);sea=std::abs(uv.x-river)<.022f&&std::abs(uv.y-.38f)>.055f&&std::abs(uv.y-.72f)>.055f;}
  bool ridge=(mission==1||mission==3||mission==6)&&((std::abs(uv.x-(mission==1?.43f:.47f))<.045f&&uv.y<.38f)||(std::abs(uv.x-.60f)<.055f&&uv.y>.65f));
  terrain[x+y*cols]=sea?Terrain::Water:ridge?Terrain::Rock:Terrain::Grass;
 }
 // Keep terrestrial pieces on their actual mainland or island, never on an accidental water tile.
 for(auto& e:entities)if(def(e.kind).domain==Domain::Land&&e.kind!=Kind::Shipyard&&water(e.pos)){
  sf::Vector2f best=e.pos;float nearest=1e9;for(int y=std::max(1,int(e.pos.y/cell)-14);y<std::min(rows-1,int(e.pos.y/cell)+15);++y)for(int x=std::max(1,int(e.pos.x/cell)-14);x<std::min(cols-1,int(e.pos.x/cell)+15);++x){sf::Vector2f p{(x+.5f)*cell,(y+.5f)*cell};if(water(p))continue;bool free=true;for(const auto& other:entities)if(other.hp>0&&other.id!=e.id&&!mobile(other.kind)&&distance(p,other.pos)<160)free=false;if(free&&distance(p,e.pos)<nearest){nearest=distance(p,e.pos);best=p;}}
  e.pos=e.goal=best;
 }
 std::vector<sf::Vector2f> clearings;
 for(const auto& e:entities)if(e.hp>0){clearings.push_back(e.pos);if(!mobile(e.kind)&&def(e.kind).domain!=Domain::Sea&&e.kind!=Kind::Shipyard){for(int y=std::max(0,int((e.pos.y-160)/cell));y<=std::min(rows-1,int((e.pos.y+160)/cell));++y)for(int x=std::max(0,int((e.pos.x-160)/cell));x<=std::min(cols-1,int((e.pos.x+160)/cell));++x)terrain[x+y*cols]=Terrain::Grass;}}
 // Islands and ports have dedicated shore access; ships and oil rigs start in navigable water.
 for(auto& e:entities)if(def(e.kind).domain==Domain::Sea||e.kind==Kind::OilRig){
  if(mission==7)e.pos=sites[1]+sf::Vector2f(e.kind==Kind::OilRig?220.f:50.f,240.f+(e.id%3)*65.f);
  if(mission==4)e.pos=sites[2]+sf::Vector2f(0,240);
  e.goal=e.pos;for(int y=std::max(0,int(e.pos.y/cell)-2);y<=std::min(rows-1,int(e.pos.y/cell)+2);++y)for(int x=std::max(0,int(e.pos.x/cell)-2);x<=std::min(cols-1,int(e.pos.x/cell)+2);++x)terrain[x+y*cols]=Terrain::Water;
 }
 std::vector<std::pair<sf::Vector2f,sf::Vector2f>> roads;
 auto connect=[&](sf::Vector2f a,sf::Vector2f b){roads.push_back({a,b});};
 if(mission==0){connect(home,sites[0]);connect(sites[0],sites[1]);connect(sites[1],mapGoal);connect(sites[0],sites[2]);connect(sites[2],enemy);}
 else if(mission==1){connect(home,sites[0]);connect(sites[0],sites[1]);connect(sites[1],sites[2]);connect(sites[2],mapGoal);}
 else if(mission==4||mission==7){connect(home,point({spec.home.x,.51f*(mission==4?1.f:.5f)}));connect(enemy,sites[1]);connect(sites[0]+sf::Vector2f(-width()*.12f,0),sites[0]);connect(sites[0],sites[0]+sf::Vector2f(0,height()*.07f));}
 else if(mission==10){connect(home,sites[0]);connect(sites[0],sites[1]);connect(sites[1],sites[2]);connect(sites[1],mapGoal);connect(mapGoal,enemy);connect(home,point({.49f,.38f}));connect(point({.49f,.38f}),enemy);}
 else{connect(home,sites[0]);connect(sites[0],sites[1]);connect(sites[1],sites[2]);connect(sites[2],enemy);connect(sites[0],enemy);}
 auto segment=[](sf::Vector2f p,sf::Vector2f a,sf::Vector2f b){auto d=b-a;float l=d.x*d.x+d.y*d.y,t=l?std::clamp(((p.x-a.x)*d.x+(p.y-a.y)*d.y)/l,0.f,1.f):0;return distance(p,a+d*t);};
 auto nearRoad=[&](sf::Vector2f p,float width){for(auto r:roads)if(segment(p,r.first,r.second)<width)return true;return false;};
 // Finite reserves have explicit settlement, expansion and contested roles.
 std::vector<sf::Vector2f> mines={home+sf::Vector2f(-330,320),home+sf::Vector2f(420,420),point({.36f,.75f}),point({.42f,.28f}),enemy+sf::Vector2f(390,-100),enemy+sf::Vector2f(-340,470),point({.82f,.86f})};
 if(mission==4||mission==7){mines[2]=sites[0]+sf::Vector2f(-200,0);mines[3]=sites[3];}
 for(auto p:mines){if(p.x<120||p.y<120||p.x>width()-120||p.y>height()-120||water(p))continue;bool free=true;for(auto c:clearings)if(distance(p,c)<180)free=false;if(free){add(Kind::Mine,-1,p);clearings.push_back(p);}}
 if(mission==4||mission==7){sf::Vector2f oilA{home.x,coast(spec.home.x)*height()+280},oilB{enemy.x,coast(spec.enemy.x)*height()+300};add(Kind::Oil,-1,oilA);add(Kind::Oil,-1,oilB);
  for(auto& e:entities)if(e.kind==Kind::OilRig){int source=-1;float best=1e9;for(const auto& node:entities)if(node.kind==Kind::Oil&&distance(e.pos,node.pos)<best){best=distance(e.pos,node.pos);source=node.id;}e.pos=get(source)->pos;e.goal=e.pos;e.source=source;}
 }
 for(int y=0;y<rows;++y)for(int x=0;x<cols;++x){sf::Vector2f p{(x+.5f)*cell,(y+.5f)*cell};if(nearRoad(p,80)&&terrain[x+y*cols]!=Terrain::Water)terrain[x+y*cols]=Terrain::Road;}
 struct Grove{sf::Vector2f center;float rx,ry;};std::vector<Grove> groves;
 for(int i=0;i<6;++i)groves.push_back({point(spec.woods[i]),width()*(.065f+(i%2)*.012f),height()*(.075f+(i%3)*.008f)});
 groves.push_back({home+sf::Vector2f(-260,620),360,210});groves.push_back({enemy+sf::Vector2f(50,650),350,210});
 int trees=0;
 for(int y=2;y<rows-2&&trees<3100;++y)for(int x=2;x<cols-2&&trees<3100;++x){sf::Vector2f p{x*40.f+(y%2)*12.f,y*40.f};bool inside=false;
  for(auto g:groves){float dx=(p.x-g.center.x)/g.rx,dy=(p.y-g.center.y)/g.ry;float edge=1+.08f*std::sin(p.x*.017f+p.y*.023f);if(dx*dx+dy*dy<edge)inside=true;}
  if(!inside||terrain[x+y*cols]!=Terrain::Grass||nearRoad(p,145))continue;bool free=true;
  for(const auto& e:entities)if(e.hp>0&&e.kind!=Kind::Forest&&distance(p,e.pos)<(mobile(e.kind)?120.f:resource(e.kind)?160.f:190.f)){free=false;break;}
  if(free){add(Kind::Forest,-1,p);++trees;}
 }
 // Remove tiny disconnected scraps; a forest is a contiguous woodland, not isolated decoration.
 std::vector<int> treeGrid(cols*rows,-1);for(int i=0;i<int(entities.size());++i)if(entities[i].kind==Kind::Forest&&entities[i].hp>0)treeGrid[int(entities[i].pos.x/cell)+int(entities[i].pos.y/cell)*cols]=i;
 std::vector<unsigned char> visited(cols*rows);
 for(int cellIndex=0;cellIndex<cols*rows;++cellIndex)if(treeGrid[cellIndex]>=0&&!visited[cellIndex]){std::vector<int> component;std::queue<int> todo;todo.push(cellIndex);visited[cellIndex]=1;
  while(!todo.empty()){int index=todo.front();todo.pop();component.push_back(treeGrid[index]);int cx=index%cols,cy=index/cols;for(int dy=-1;dy<=1;++dy)for(int dx=-1;dx<=1;++dx){int x=cx+dx,y=cy+dy;if(x<0||y<0||x>=cols||y>=rows)continue;int n=x+y*cols;if(!visited[n]&&treeGrid[n]>=0){visited[n]=1;todo.push(n);}}}
  if(component.size()<12)for(int i:component)entities[i].hp=0;
 }
 navDirty=true;
 if(mission==0){std::vector<int> workers;for(const auto& e:entities)if(e.team==1&&e.kind==Kind::Worker)workers.push_back(e.id);int index=0;for(int id:workers){Kind wanted=index++%2?Kind::Forest:Kind::Mine;int source=-1;float best=1e9;for(const auto& e:entities)if(e.hp>0&&e.kind==wanted&&distance(get(id)->pos,e.pos)<best){best=distance(get(id)->pos,e.pos);source=e.id;}if(source>=0)gather(*get(id),source);}}
 clearVision();refreshVision();
}
}
