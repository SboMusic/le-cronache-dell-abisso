// Presentation of the original roster. Entity types come from RtsCatalog.
void Gameplay::draw(sf::RenderTarget& target,sf::RenderStates states)const{
 ng::Main::applyDefaultViewToWindow();
 auto box=[&](sf::Vector2f p,sf::Vector2f size,sf::Color color,sf::Color border=sf::Color::Transparent){sf::RectangleShape r(size);r.setPosition(p);r.setFillColor(color);r.setOutlineThickness(1);r.setOutlineColor(border);target.draw(r);};
 box({0,0},{1920,1080},{15,25,28});
 box({12,12},{425,1056},{23,38,42},{80,103,96});
 box({451,66},{1431,984},{40,62,47},{152,130,78});
 target.setView(view_);
 drawTerrain(target);
 std::vector<const alpha::Entity*> ordered;
 for(const auto& e:world_.entities){if(e.carrier>=0||!world_.detected(e,0)||!knownEntity(e))continue;bool covered=false;if(e.kind==alpha::Kind::Oil)for(auto& b:world_.entities)if(b.kind==alpha::Kind::OilRig&&b.hp>0&&alpha::distance(e.pos,b.pos)<70)covered=true;if(covered)continue;auto v=visuals_.find(e.id);if(e.hp>0||(v!=visuals_.end()&&v->second.death<.65f))ordered.push_back(&e);}
 std::stable_sort(ordered.begin(),ordered.end(),[](auto a,auto b){return a->pos.y<b->pos.y;});
 for(auto ptr:ordered){auto e=*ptr;bool unit=alpha::mobile(e.kind);float r=alpha::radius(e.kind);
  auto vi=visuals_.find(e.id);Visual v;if(vi!=visuals_.end())v=vi->second;if(unit&&v.initialized)e.pos=v.display;
  bool dying=e.hp<=0;float fade=dying?std::max(0.f,1-v.death/.65f):1;
  sf::Color team=e.team==0?sf::Color(65,207,220):sf::Color(233,99,73);
  float shadowRadius=unit?22.f:(e.kind==alpha::Kind::Forest?18.f:58.f);sf::CircleShape shadow(shadowRadius,32);shadow.setOrigin(shadowRadius,shadowRadius);shadow.setPosition(e.pos+sf::Vector2f(0,unit?20.f:(e.kind==alpha::Kind::Forest?2.f:28.f)));shadow.setScale(1,.38f);shadow.setFillColor({0,0,0,static_cast<sf::Uint8>(85*fade)});target.draw(shadow);
  if(e.team>=0&&!dying){sf::CircleShape allegiance(unit?21.f:53.f,32);allegiance.setOrigin(unit?21.f:53.f,unit?21.f:53.f);allegiance.setPosition(e.pos+sf::Vector2f(0,unit?20.f:28.f));allegiance.setScale(1,.4f);allegiance.setFillColor(sf::Color::Transparent);allegiance.setOutlineThickness(e.selected?4.f:2.f);allegiance.setOutlineColor(e.selected?sf::Color(255,225,130):team);target.draw(allegiance);}
  if(world_.mission==3&&e.id==world_.archivist){auto refugee=e;refugee.team=1;drawArt(target,refugee,v,fade);}else drawArt(target,e,v,fade);
  if(dying)continue;
  drawActivity(target,e,v);
  if(e.team>=0&&!unit){box(e.pos+sf::Vector2f(51,-38),{7,34},team);}

  if(!(world_.mission==10&&e.id==world_.archivist)&&(e.selected||(!alpha::resource(e.kind)&&(e.hp<e.maxHp||!unit)))){float w=unit?45.f:100.f;float y=unit?-60.f:-137.f;
   box(e.pos+sf::Vector2f(-w/2,y),{w,5},{17,25,27});box(e.pos+sf::Vector2f(-w/2,y),{w*std::max(0.f,e.hp)/e.maxHp,5},e.team<0?sf::Color(216,185,100):e.team==1?sf::Color(219,108,88):sf::Color(113,205,145));
  }
  if(e.id==world_.archivist||e.lifetime==-777||e.selected||(!unit&&!alpha::resource(e.kind)&&e.kind!=alpha::Kind::Wall&&(e.team<=0||alpha::distance(mouseWorld_,e.pos)<110)))text(target,world_.mission==10&&e.id==world_.archivist?"PIETRA DEL MONDO":e.lifetime==-777?(world_.faction[1]==1?"Vaerak":"Comandante della Lega"):e.id==world_.archivist?(world_.mission==3?"Rifugio dei profughi":"Seren Dhal"):world_.mission==0&&e.kind==alpha::Kind::Farm&&e.team<0?"Villaggio abbandonato":world_.mission==2&&e.kind==alpha::Kind::Sanctuary&&e.team<0?"Archivio dei Custodi":world_.mission==4&&e.kind==alpha::Kind::Sanctuary&&e.team<0?"Tempio delle Citta Libere":alpha::factionUnitName(e.kind,e.team>=0?world_.faction[e.team]:0),e.pos+sf::Vector2f(-42,unit?34.f:55.f),17,{240,232,207});
  if(world_.mission>=5&&e.team<0&&e.kind==alpha::Kind::Sanctuary&&world_.mission!=10){text(target,e.level>=2?"PRESIDIO RECUPERATO":"Presidia il punto",e.pos+sf::Vector2f(-75,80),16,e.level>=2?sf::Color(130,232,163):sf::Color(241,210,128));if(e.level<2&&e.work>0)text(target,std::to_string(int(e.work))+" s",e.pos+sf::Vector2f(-18,100),17);}
  if(alpha::resource(e.kind)&&!dying&&(e.selected||alpha::distance(mouseWorld_,e.pos)<75)){text(target,(e.kind==alpha::Kind::Mine?"Oro: ":e.kind==alpha::Kind::Forest?"Legno: ":"Petrolio: ")+std::to_string(int(e.hp)),e.pos+sf::Vector2f(-50,48),18,{255,227,154});}
  if(e.maxMana>0&&e.selected){box(e.pos+sf::Vector2f(-23,-52),{46,4},{18,25,37});box(e.pos+sf::Vector2f(-23,-52),{46*e.mana/e.maxMana,4},{107,145,233});}
  if(e.build>0||e.production>0){float total=std::max(.1f,e.build>0?e.buildTotal:e.productionTotal);float remaining=std::max(e.build,e.production);
   box(e.pos+sf::Vector2f(-50,79),{100,6},{17,25,27});box(e.pos+sf::Vector2f(-50,79),{100*(1-remaining/total),6},{229,188,98});
   text(target,std::to_string(int(std::ceil(remaining)))+" s",e.pos+sf::Vector2f(57,65),17,{255,224,151});
  }
 }
 for(const auto& s:world_.shots){if(!world_.visibleAt(s.pos))continue;auto victim=world_.get(s.target);sf::Vector2f d=victim?victim->pos-s.pos:sf::Vector2f(1,0);float length=std::max(1.f,alpha::distance({},d));d/=length;sf::RectangleShape trail({s.splash>0?20.f:15.f,s.splash>0?4.f:2.f});trail.setOrigin(trail.getSize().x,trail.getSize().y/2);trail.setPosition(s.pos);trail.setRotation(std::atan2(d.y,d.x)*180/3.14159265f);trail.setFillColor(s.splash>0?sf::Color(255,175,73,180):sf::Color(225,211,169,220));target.draw(trail);sf::CircleShape bolt(s.splash>0?5.f:2.f);bolt.setPosition(s.pos);bolt.setFillColor({255,223,147});target.draw(bolt);}
 for(const auto& f:world_.effects){if(!world_.visibleAt(f.pos))continue;sf::CircleShape ring(f.radius*(1-f.remaining/f.total));ring.setOrigin(ring.getRadius(),ring.getRadius());ring.setPosition(f.pos);ring.setFillColor(sf::Color::Transparent);ring.setOutlineThickness(3);ring.setOutlineColor(f.type==2?sf::Color(105,224,155,170):sf::Color(229,171,105,160));target.draw(ring);}
 for(const auto& z:world_.zones){if(!world_.visibleAt(z.pos))continue;sf::CircleShape storm(130);storm.setOrigin(130,130);storm.setPosition(z.pos);storm.setFillColor({100,65,160,65});storm.setOutlineThickness(3);storm.setOutlineColor({169,133,222,170});target.draw(storm);}
 if(world_.fogEnabled){sf::VertexArray fog(sf::Quads);for(int y=0;y<world_.rows;++y)for(int x=0;x<world_.cols;++x){sf::Vector2f p{x*40.f,y*40.f},center=p+sf::Vector2f(20,20);if(world_.visibleAt(center))continue;sf::Color shade(0,0,0,world_.exploredAt(center)?0:255);for(auto d:{sf::Vector2f(0,0),sf::Vector2f(40,0),sf::Vector2f(40,40),sf::Vector2f(0,40)})fog.append(sf::Vertex(p+d,shade));}target.draw(fog);}
 if(world_.mission>=0&&(world_.mission>=3||world_.missionStage>=1&&world_.mission==2||world_.missionStage==2)){for(auto p:world_.questTargets()){sf::CircleShape ring(65+std::sin(world_.time*3)*4,32);ring.setOrigin(ring.getRadius(),ring.getRadius());ring.setPosition(p);ring.setFillColor({238,202,98,20});ring.setOutlineThickness(4);ring.setOutlineColor({250,220,124});target.draw(ring);text(target,"OBIETTIVO",p+sf::Vector2f(-55,-100),19,{255,235,150});}}
 if(dragging_){sf::RectangleShape rect(mouseWorld_-dragStart_);rect.setPosition(dragStart_);rect.setFillColor({86,212,203,30});rect.setOutlineThickness(2);rect.setOutlineColor({167,240,208});target.draw(rect);}
 if(planting_){alpha::Entity tree;tree.kind=alpha::Kind::Forest;tree.pos=mouseWorld_;drawArt(target,tree,Visual{},.6f);}
 if(placing_){sf::CircleShape marker(alpha::radius(placingKind_));marker.setOrigin(marker.getRadius(),marker.getRadius());marker.setPosition(mouseWorld_);marker.setFillColor((world_.exploredAt(mouseWorld_)&&world_.placementIssue(mouseWorld_,placingKind_).empty())?sf::Color(80,220,130,70):sf::Color(235,75,60,110));target.draw(marker);alpha::Entity preview;preview.kind=placingKind_;preview.pos=mouseWorld_;preview.team=0;preview.build=1;drawArt(target,preview,Visual{},.7f);}
 if(casting_){sf::CircleShape area(125);area.setOrigin(125,125);area.setPosition(mouseWorld_);area.setFillColor({125,100,215,35});area.setOutlineThickness(2);area.setOutlineColor({186,168,241});target.draw(area);}
 ng::Main::applyDefaultViewToWindow();
 if(world_.guided||world_.mission>=0){box({470,985},{1385,48},{20,38,40,238},{152,130,78});text(target,world_.mission>=0?world_.missionObjective():world_.objective(),{485,999},18,{247,223,164});}
 text(target,loc::title(),{28,20},27,{234,210,157});text(target,"CAMPAGNE DI AVARON / " ABYSS_VERSION_TEXT,{29,59},17,{147,173,163});
 auto mini=minimapBounds();box({mini.left-6,mini.top-10},{mini.width+12,mini.height+16},{10,23,24},{152,169,129});
 box({mini.left,mini.top},{mini.width,mini.height},{95,122,74});
 if(world_.mission<0){box({mini.left,mini.top+1200.f/world_.height()*mini.height},{mini.width,400.f/world_.height()*mini.height},{37,102,144});box({mini.left+35,mini.top+720.f/world_.height()*mini.height},{mini.width-70,8},{175,153,100});}else{sf::VertexArray terrain(sf::Quads);for(int y=0;y<world_.rows;++y)for(int x=0;x<world_.cols;++x){auto kind=world_.terrain[x+y*world_.cols];sf::Color color=kind==alpha::Terrain::Water?sf::Color(37,102,144):kind==alpha::Terrain::Rock?sf::Color(98,96,79):kind==alpha::Terrain::Road?sf::Color(180,148,90):world_.mission==1?sf::Color(151,136,86):world_.mission==2?sf::Color(71,105,72):sf::Color(115,143,83);sf::Vector2f p{mini.left+x*mini.width/world_.cols,mini.top+y*mini.height/world_.rows};for(auto d:{sf::Vector2f(0,0),sf::Vector2f(mini.width/world_.cols,0),sf::Vector2f(mini.width/world_.cols,mini.height/world_.rows),sf::Vector2f(0,mini.height/world_.rows)})terrain.append(sf::Vertex(p+d,color));}target.draw(terrain);}
 sf::VertexArray forestPixels(sf::Quads);for(const auto& e:world_.entities)if(e.hp>0&&e.kind==alpha::Kind::Forest&&world_.exploredAt(e.pos)){sf::Vector2f p{mini.left+e.pos.x/world_.width()*mini.width-2,mini.top+e.pos.y/world_.height()*mini.height-2};for(auto offset:{sf::Vector2f(0,0),sf::Vector2f(4.5f,0),sf::Vector2f(4.5f,5),sf::Vector2f(0,5)})forestPixels.append(sf::Vertex(p+offset,{25,67,38}));}target.draw(forestPixels);
 if(world_.fogEnabled){sf::VertexArray fog(sf::Quads);for(int y=0;y<world_.rows;++y)for(int x=0;x<world_.cols;++x){sf::Vector2f center{x*40.f+20,y*40.f+20};if(world_.visibleAt(center))continue;sf::Vector2f p{mini.left+x*mini.width/world_.cols,mini.top+y*mini.height/world_.rows};sf::Color shade(0,0,0,world_.exploredAt(center)?0:255);for(auto d:{sf::Vector2f(0,0),sf::Vector2f(mini.width/world_.cols,0),sf::Vector2f(mini.width/world_.cols,mini.height/world_.rows),sf::Vector2f(0,mini.height/world_.rows)})fog.append(sf::Vertex(p+d,shade));}target.draw(fog);}
 for(const auto& e:world_.entities)if(e.hp>0&&e.kind!=alpha::Kind::Forest&&e.carrier<0&&world_.detected(e,0)&&knownEntity(e)){
  sf::Vector2f pos{mini.left+e.pos.x/world_.width()*mini.width,mini.top+e.pos.y/world_.height()*mini.height};sf::Color color=e.team==0?sf::Color(75,240,249):(e.team==1?sf::Color(255,88,69):(e.kind==alpha::Kind::Mine?sf::Color(255,214,73):sf::Color(162,216,246)));
  float radius=alpha::mobile(e.kind)?2.8f:4.2f;sf::CircleShape dot(radius,alpha::resource(e.kind)?4:16);dot.setOrigin(radius,radius);dot.setPosition(pos);dot.setFillColor(color);dot.setOutlineThickness(1);dot.setOutlineColor({13,31,31});target.draw(dot);
 }
 if(world_.mission>=0&&(world_.mission>=3||world_.missionStage==2||world_.mission==2&&world_.missionStage>=1)){for(auto p:world_.questTargets()){sf::CircleShape pin(6,4);pin.setOrigin(6,6);pin.setPosition(mini.left+p.x/world_.width()*mini.width,mini.top+p.y/world_.height()*mini.height);pin.setFillColor({255,228,118});target.draw(pin);}}
 auto sz=view_.getSize(),c=view_.getCenter();sf::RectangleShape camera({sz.x/world_.width()*mini.width,sz.y/world_.height()*mini.height});camera.setPosition(mini.left+(c.x-sz.x/2)/world_.width()*mini.width,mini.top+(c.y-sz.y/2)/world_.height()*mini.height);camera.setFillColor({255,255,255,12});camera.setOutlineThickness(5);camera.setOutlineColor({7,20,22});target.draw(camera);camera.setFillColor(sf::Color::Transparent);camera.setOutlineThickness(2);camera.setOutlineColor({255,245,199});target.draw(camera);
 text(target,"Mappa "+std::to_string(int(world_.width()))+" x "+std::to_string(int(world_.height()))+" | rotella: zoom",{26,300},16,{170,191,171});
 text(target,alpha::factionName(world_.faction[0]),{26,345},21,{147,173,163});
 if(world_.mission>=0)text(target,alpha::missionTitle(world_.mission),{27,367},14,{238,214,158});
 box({25,382},{390,94},{32,51,53},{68,92,82});
 text(target,"ORO",{36,393},16,{207,182,116});text(target,std::to_string(world_.gold),{36,425},25,{242,225,179});
 text(target,"LEGNO",{132,393},16,{144,184,141});text(target,std::to_string(world_.wood),{132,425},25);
 text(target,world_.mission>=0&&world_.mission!=4&&world_.mission!=7?"OBIETTIVO":"PETROLIO",{224,393},15,{139,185,190});text(target,world_.mission>=0&&world_.mission!=4&&world_.mission!=7?std::to_string(world_.missionStage+1)+"/3":std::to_string(world_.oil),{224,425},25);
 text(target,"UNITA",{330,393},15,{139,185,190});text(target,std::to_string(world_.population(0,false))+"/"+std::to_string(world_.capacity(0)),{330,425},23);
 int count=0;const alpha::Entity* selected=nullptr;for(const auto& e:world_.entities)if(e.hp>0&&e.selected){++count;selected=&e;}
 text(target,count==1?alpha::factionUnitName(selected->kind,world_.faction[0]):"Selezionati: "+std::to_string(count),{28,493},25,{231,222,196});
 std::string status=count>1?"Destro: ordini al gruppo":"Scegli un'unita sulla mappa";
 if(count==1){status=std::to_string(int(selected->hp))+" / "+std::to_string(int(selected->maxHp))+" salute";
  if(selected->build>0)status="Costruzione: "+std::to_string(int(std::ceil(selected->build)))+" s";
  else if(selected->production>0)status=std::string(selected->research!=-1?"Ricerca: ":"Addestramento: ")+std::to_string(int(std::ceil(selected->production)))+" s";
  else if(world_.busy(*selected))status=selected->route.empty()?"Costruisce: operaio impegnato":"Raggiunge il cantiere";
  else if(selected->kind==alpha::Kind::Worker&&selected->order==alpha::Order::Gather){auto source=world_.get(selected->target);status=source&&source->kind==alpha::Kind::Forest?(selected->route.empty()?"Taglia legna":"Va al bordo della foresta"):"Raccoglie oro";if(source)status+=" | Rimasti: "+std::to_string(int(source->hp));}
  else if(selected->kind==alpha::Kind::Worker&&selected->order==alpha::Order::Return)status="Consegna carico: "+std::to_string(selected->cargo);
  else if(selected->kind==alpha::Kind::Barracks)status="Caserma livello "+std::to_string(selected->level)+" | "+status;
  else if(selected->kind==alpha::Kind::Hall)status="Comando livello "+std::to_string(world_.townLevel[0])+" | "+status;
  else if(selected->maxMana>0)status+=" | Mana "+std::to_string(int(selected->mana));
 }
 text(target,status,{28,534},20,{151,184,168});
 const char* menuLabels[]={"F3 Unita","F4 Edifici","F5 Ricerca","F6 Magie"};for(int i=0;i<4;++i){box({25.f+i*97.5f,946},{94,42},{48,75,70});text(target,menuLabels[i],{30.f+i*97.5f,957},15);}
 text(target,"Centrale: trascina | Minimap: clic/trascina",{28,987},15,{171,193,181});
 box({25,1010},{390,45},{42,62,61},{113,132,106});text(target,"ESC  /  MENU DELLA PARTITA",{44,1019},20,{232,216,178});
 box({455,14},{1423,48},{24,40,43},{79,104,91});text(target,world_.message,{473,26},21,{233,218,180});
 if(paused_||world_.result){box({610,420},{1100,170},{15,28,31,245},{181,157,100});text(target,paused_?"PAUSA":(world_.result>0?"VITTORIA":"SCONFITTA"),{680,449},44,{237,215,166});text(target,paused_?"Premi P per riprendere":"Premi R per una nuova partita",{680,512},25,{192,210,192});}
 drawCommands(target);
 drawCatalog(target);

 drawCheats(target);
}





