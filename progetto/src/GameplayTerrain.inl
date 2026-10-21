void Gameplay::drawTerrain(sf::RenderTarget& target)const{
 sf::Sprite ground(ground_);ground.setTextureRect({0,0,int(world_.width()),int(world_.height())});
 if(world_.mission==1)ground.setColor({222,185,130});if(world_.mission==2)ground.setColor({172,205,174});if(world_.mission==3)ground.setColor({219,184,156});if(world_.mission==5)ground.setColor({204,218,198});if(world_.mission==6)ground.setColor({218,185,155});if(world_.mission==7)ground.setColor({223,229,201});if(world_.mission==8)ground.setColor({184,200,211});if(world_.mission==9)ground.setColor({235,208,172});if(world_.mission==10)ground.setColor({162,179,181});target.draw(ground);
 auto quad=[](sf::VertexArray& out,sf::Vector2f p,sf::Vector2f size,sf::Vector2f uv,sf::Color color){for(auto d:{sf::Vector2f(0,0),sf::Vector2f(size.x,0),size,sf::Vector2f(0,size.y)})out.append(sf::Vertex(p+d,color,uv+d*1.35f));};
 auto rect=[&](sf::Vector2f p,sf::Vector2f size,sf::Color color){sf::RectangleShape r(size);r.setPosition(p);r.setFillColor(color);target.draw(r);};
 auto center=view_.getCenter(),size=view_.getSize();int left=std::max(0,int((center.x-size.x/2)/40)-1),right=std::min(world_.cols-1,int((center.x+size.x/2)/40)+1),top=std::max(0,int((center.y-size.y/2)/40)-1),bottom=std::min(world_.rows-1,int((center.y+size.y/2)/40)+1);
 sf::VertexArray water(sf::Quads),flow(sf::Quads);for(int y=top;y<=bottom;++y)for(int x=left;x<=right;++x)if(world_.terrain[x+y*world_.cols]==alpha::Terrain::Water){sf::Vector2f p{x*40.f,y*40.f};quad(water,p,{40,40},p*1.35f+sf::Vector2f(world_.time*3,world_.time*1.5f),{220,245,245});quad(flow,p,{40,40},p*1.35f+sf::Vector2f(-world_.time*5,world_.time*2),{200,235,242,65});}
 sf::RenderStates state;state.texture=&terrainArt_[1];target.draw(water,state);state.texture=&terrainArt_[3];target.draw(flow,state);
 // Shallow edges, beach and a moving foam line follow the navigation shoreline.
 for(int y=top;y<=bottom;++y)for(int x=left;x<=right;++x)if(world_.terrain[x+y*world_.cols]==alpha::Terrain::Water){sf::Vector2f p{x*40.f,y*40.f};for(int side=0;side<4;++side){int dx=side==0?-1:side==1?1:0,dy=side==2?-1:side==3?1:0,nx=x+dx,ny=y+dy;if(nx<0||ny<0||nx>=world_.cols||ny>=world_.rows||world_.terrain[nx+ny*world_.cols]==alpha::Terrain::Water)continue;bool vertical=dx!=0;sf::Vector2f edge=p+sf::Vector2f(dx==1?34.f:0,dy==1?34.f:0);rect(edge,vertical?sf::Vector2f(6,40):sf::Vector2f(40,6),{207,188,139,180});float tide=2+std::sin(world_.time*1.7f+x*.7f+y)*1.5f;edge+=sf::Vector2f(dx==-1?6+tide:dx==1?-tide:0,dy==-1?6+tide:dy==1?-tide:0);rect(edge,vertical?sf::Vector2f(2,40):sf::Vector2f(40,2),{214,241,231,130});}}
 if(world_.layoutVersion==1){
  // Render the saved terrain grid, so roads follow the authored layout and remain on land.
  sf::VertexArray paths(sf::Quads),edges(sf::Quads);
  for(int y=top;y<=bottom;++y)for(int x=left;x<=right;++x)if(world_.terrain[x+y*world_.cols]==alpha::Terrain::Road){
   sf::Vector2f p{x*40.f,y*40.f};quad(paths,p,{40,40},p*1.35f,{255,255,255,245});
   for(int side=0;side<4;++side){int dx=side==0?-1:side==1?1:0,dy=side==2?-1:side==3?1:0,nx=x+dx,ny=y+dy;
    if(nx<0||ny<0||nx>=world_.cols||ny>=world_.rows||world_.terrain[nx+ny*world_.cols]!=alpha::Terrain::Grass)continue;
    sf::Vector2f edge=p+sf::Vector2f(dx==1?40.f:dx==-1?-8.f:0,dy==1?40.f:dy==-1?-8.f:0);
    quad(edges,edge,dx?sf::Vector2f(8,40):sf::Vector2f(40,8),edge*1.35f,{255,255,255,65});
   }
  }
  state.texture=&terrainArt_[world_.mission==1?2:0];target.draw(edges,state);target.draw(paths,state);
 }else{
 auto road=world_.mission<0?std::vector<sf::Vector2f>{{230,750},{2310,750}}:alpha::missionRoad(world_.mission);
 for(int i=1;i<int(road.size());++i){auto delta=road[i]-road[i-1];float length=alpha::distance(road[i],road[i-1]);float angle=std::atan2(delta.y,delta.x)*180/3.14159265f;for(int layer=0;layer<3;++layer){float width=116-layer*16.f;sf::RectangleShape path({length,width});path.setOrigin(0,width/2);path.setPosition(road[i-1]);path.setRotation(angle);path.setTexture(&terrainArt_[world_.mission==1?2:0]);path.setTextureRect({0,0,int(length*1.4f),int(width*1.4f)});path.setFillColor({255,255,255,static_cast<sf::Uint8>(layer==0?30:layer==1?80:245)});target.draw(path);}}
 for(auto p:road){sf::CircleShape joint(42,28);joint.setOrigin(42,42);joint.setPosition(p);joint.setTexture(&terrainArt_[world_.mission==1?2:0]);joint.setTextureRect({int(p.x),int(p.y),118,118});target.draw(joint);}
 }
 for(int y=(top/2)*2;y<=bottom;y+=2)for(int x=0;x<=right;++x)if(world_.terrain[x+y*world_.cols]==alpha::Terrain::Rock){int end=x;while(end+1<world_.cols&&world_.terrain[end+1+y*world_.cols]==alpha::Terrain::Rock)++end;for(int a=x;a<=end;a+=4){int width=std::min(4,end-a+1);auto extent=rockArt_.getSize();int v=(a*17+y*31)%4;sf::IntRect tile{int((v%2)*extent.x/2),int((v/2)*extent.y/2),int(extent.x/2),int(extent.y/2)};sf::Sprite rock(rockArt_);rock.setTextureRect(tile);rock.setOrigin(tile.width*.5f,tile.height*.82f);float scale=(width*40.f+22)/tile.width;rock.setScale(scale,scale);rock.setPosition((a+width*.5f)*40,y*40.f+70);target.draw(rock);}x=end;}
}
