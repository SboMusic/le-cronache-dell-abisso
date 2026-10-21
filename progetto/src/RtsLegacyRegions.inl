#pragma once
namespace alpha {
inline void World::prepareLegacyRegion(){
 const int oldCols=cols,oldRows=rows;
 const int newCols=112+mission*8,newRows=70+mission*5;
 resizeMap(newCols,newRows,true);
 auto road=missionRoad(mission);
 if(mission==0){
  sf::Vector2f village{width()*.68f,height()*.42f};
  for(auto& e:entities)if(e.team<0&&e.kind==Kind::Farm)e.pos=e.goal=village;
  road={{360,1050},{1100,1000},{1900,1280},village+sf::Vector2f(0,120)};
 }
 // Link the original objective route to regional reserves, rather than an empty map border.
 auto segment=[](sf::Vector2f p,sf::Vector2f a,sf::Vector2f b){auto d=b-a;float l=d.x*d.x+d.y*d.y;float t=l?std::clamp(((p.x-a.x)*d.x+(p.y-a.y)*d.y)/l,0.f,1.f):0;return distance(p,a+d*t);};
 auto onRoad=[&](sf::Vector2f p,float margin){for(int i=1;i<int(road.size());++i)if(segment(p,road[i-1],road[i])<margin)return true;return false;};
 road.push_back({width()-560,900});
 for(int y=0;y<rows;++y)for(int x=0;x<cols;++x){sf::Vector2f p{(x+.5f)*cell,(y+.5f)*cell};bool extra=x>=oldCols||y>=oldRows;
  if(extra){bool naval=mission==4||mission==7;bool island=(x%30>8&&x%30<24&&y>oldRows+8&&y%24<15);
   terrain[x+y*cols]=naval&&y>=oldRows-1&&!island?Terrain::Water:Terrain::Grass;
   if(terrain[x+y*cols]==Terrain::Grass&&((x/5+y/7+mission)%17==0))terrain[x+y*cols]=Terrain::Rock;
  }
  if((extra||mission==0)&&onRoad(p,85)&&terrain[x+y*cols]!=Terrain::Water)terrain[x+y*cols]=Terrain::Road;
 }
 std::vector<sf::Vector2f> clearings;
 for(const auto& e:entities)if(e.hp>0&&e.kind!=Kind::Forest)clearings.push_back(e.pos);
 // Preserve IDs referenced by objectives; clear obstructing trees without erasing entities.
 for(auto& e:entities)if(e.kind==Kind::Forest&&e.hp>0){bool clear=(mission==0||e.pos.x>=oldCols*cell||e.pos.y>=oldRows*cell)&&onRoad(e.pos,115);for(const auto& b:entities)if(b.hp>0&&!resource(b.kind)&&distance(e.pos,b.pos)<(mobile(b.kind)?90.f:150.f))clear=true;if(clear)e.hp=0;}
 for(auto p:{sf::Vector2f(600,oldRows*cell+350.f),sf::Vector2f(1000,oldRows*cell+850.f),sf::Vector2f(width()-650,650),sf::Vector2f(width()*.55f,height()-450),sf::Vector2f(width()-600,height()-500)}){
  if(water(p)||blocked(int(p.x/cell),int(p.y/cell)))continue;
  bool free=true;for(const auto& e:entities)if(e.hp>0&&distance(e.pos,p)<190)free=false;
  if(free){add(Kind::Mine,-1,p);clearings.push_back(p);}
 }
 // Nearby working groves remain outside building silhouettes and the main access roads.
 std::vector<sf::Vector2f> bases;for(const auto& e:entities)if(e.hp>0&&e.kind==Kind::Hall)bases.push_back(e.pos);
 for(auto base:bases)for(int y=0;y<6;++y)for(int x=0;x<6;++x){auto p=base+sf::Vector2f(-280+x*44.f,280+y*44.f);
  if(p.x<40||p.y<40||p.x>width()-40||p.y>height()-40||terrain[int(p.x/cell)+int(p.y/cell)*cols]!=Terrain::Grass||onRoad(p,130))continue;
  bool free=true;for(const auto& e:entities)if(e.hp>0&&distance(p,e.pos)<(e.kind==Kind::Forest?34.f:mobile(e.kind)?90.f:150.f))free=false;
  if(free)add(Kind::Forest,-1,p);
 }
 std::vector<sf::Vector2f> saplings;const int limit=300+mission*35;
 for(int y=3;y<rows-3;y+=2)for(int x=3;x<cols-3;x+=2){
  sf::Vector2f p{x*40.f+float((x*13+y*7)%13),y*40.f};
  bool extra=x>=oldCols||y>=oldRows;
  if(!extra||terrain[x+y*cols]!=Terrain::Grass||((x/6)*13+(y/5)*7+mission*3)%7>=4||onRoad(p,130))continue;
  bool free=true;for(auto c:clearings)if(distance(p,c)<230)free=false;
  if(free)saplings.push_back(p);
 }
 auto key=[](sf::Vector2f p){return (unsigned(p.x)*73856093u)^(unsigned(p.y)*19349663u);};
 std::stable_sort(saplings.begin(),saplings.end(),[&](auto a,auto b){return key(a)<key(b);});
 for(int i=0;i<std::min(limit,int(saplings.size()));++i)add(Kind::Forest,-1,saplings[i]);
 navDirty=true;refreshVision();
}
}
