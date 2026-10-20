#include "Gameplay.h"
#include <sstream>
#include <iostream>

Gameplay::Gameplay(int mission,int faction):currentMission_(mission){factionChoice_=faction>=0?faction:(GetEnvironmentVariableA("FRONTIERE_DARK",nullptr,0)?1:0);loadArt();restart();}
void Gameplay::regressionGraphics(){
 int checks=0;auto require=[&](bool ok,const char* label){if(!ok)throw std::runtime_error(label);++checks;std::cout<<"PASS "<<label<<std::endl;};
 for(int i=0;i<6;++i){require(artReady_[i],"original sprite loads");auto image=art_[i].copyToImage();require(image.getPixel(0,0).a==0,"sprite background is transparent");}
 require(groundReady_,"original terrain loads");for(const auto& texture:terrainArt_)require(texture.getSize().x>0&&texture.isRepeated(),"new reusable terrain texture loads and repeats");require(treeReady_,"individual tree sprite loads");require(treeArt_.copyToImage().getPixel(0,0).a==0,"tree sprite background is transparent");
 for(int i=0;i<2;++i){require(rosterReady_[i],"extended roster atlas loads");require(rosterArt_[i].copyToImage().getPixel(0,0).a==0,"extended roster atlas is transparent");}
 for(int i=0;i<2;++i){auto size=art_[i].getSize();for(int row=0;row<4;++row)for(int col=0;col<4;++col){int right=(col+1)*size.x/4,bottom=(row+1)*size.y/4;require(right<=int(size.x)&&bottom<=int(size.y),"animation cell inside texture");}}
 restart();require(hitEntity(*world_.get(7),world_.get(7)->pos+sf::Vector2f(0,-25)),"worker torso is selectable");
 require(hitEntity(*world_.get(1),world_.get(1)->pos+sf::Vector2f(0,-65)),"command building silhouette is selectable");
 require(world_.build(7,{580,620}),"barracks visual fixture");for(int i=0;i<320;++i)world_.step(.05f);
 world_.get(world_.entities.back().id)->selected=true;
 world_.gather(*world_.get(8),3);world_.move(*world_.get(11),{850,720});updateVisuals(.25f);
 auto savedView=view_;float savedTime=world_.time;view_.setCenter(900,1380);view_.setSize(400,400);sf::RenderTexture frame;require(frame.create(320,320),"animated terrain render target initializes");frame.setView(sf::View(sf::FloatRect(700,1180,400,400)));world_.time=0;frame.clear();drawTerrain(frame);frame.display();auto first=frame.getTexture().copyToImage();world_.time=1;frame.clear();drawTerrain(frame);frame.display();auto second=frame.getTexture().copyToImage();require(!std::equal(first.getPixelsPtr(),first.getPixelsPtr()+320*320*4,second.getPixelsPtr()),"water changes rendered pixels over time");world_.time=savedTime;view_=savedView;
 std::cout<<"SUCCESS "<<checks<<" graphics checks"<<std::endl;
}
void Gameplay::loadArt(){
 char windowsDir[MAX_PATH];GetWindowsDirectoryA(windowsDir,MAX_PATH);
 if(!font_->loadFromFile(std::string(windowsDir)+"/Fonts/segoeui.ttf"))font_->loadFromFile(std::string(windowsDir)+"/Fonts/arial.ttf");
 const char* files[]={"worker.png","soldier.png","hall.png","barracks.png","mine.png","forest.png"};
 for(int i=0;i<6;++i){artReady_[i]=art_[i].loadFromFile(std::string("assets/frontiere-v1/")+files[i]);if(artReady_[i])art_[i].setSmooth(true);}
 treeReady_=treeArt_.loadFromFile("assets/frontiere-v1/tree-v1.png");if(treeReady_)treeArt_.setSmooth(true);
 groundReady_=ground_.loadFromFile("assets/frontiere-v1/ground.png");if(groundReady_){ground_.setRepeated(true);ground_.setSmooth(true);}
 rosterReady_[0]=rosterArt_[0].loadFromFile("assets/frontiere-v1/roster-units-v1.png");
 rosterReady_[1]=rosterArt_[1].loadFromFile("assets/frontiere-v1/roster-buildings-v1.png");
 for(int i=0;i<2;++i)if(rosterReady_[i])rosterArt_[i].setSmooth(true);
 if(!rockArt_.loadFromFile("assets/mappe-v1/rocce-v1.png"))throw std::runtime_error("Artwork rocce mancante");rockArt_.setSmooth(true);
 sf::Image terrain;if(!terrain.loadFromFile("assets/mappe-v2/terreni-v2.png"))throw std::runtime_error("Texture terreno mancanti");auto extent=terrain.getSize();for(int i=0;i<4;++i){int w=extent.x/2,h=extent.y/2;if(!terrainArt_[i].loadFromImage(terrain,{(i%2)*w,(i/2)*h,w,h}))throw std::runtime_error("Texture terreno non valida");terrainArt_[i].setRepeated(true);terrainArt_[i].setSmooth(true);}
 if(!crystalArt_.loadFromFile("assets/campagna-v1/pietra-del-mondo.png"))throw std::runtime_error("Artwork Pietra mancante");crystalArt_.setSmooth(true);
 loadDemonArt();
}
void Gameplay::updateVisuals(float dt){
 for(const auto& e:world_.entities){auto& v=visuals_[e.id];if(!v.initialized){v.last=e.pos;v.initialized=true;}
  auto delta=e.pos-v.last;v.last=e.pos;v.phase+=dt;
  if(e.hp<=0){v.death+=dt;continue;}
  if(e.order==alpha::Order::Attack||e.order==alpha::Order::Gather){auto t=world_.get(e.target);if(t)delta=t->pos-e.pos;}
  if(std::abs(delta.x)+std::abs(delta.y)>.1f)v.direction=delta.y>=0?(delta.x>=0?0:1):(delta.x<0?2:3);
 }
}
bool Gameplay::hitEntity(const alpha::Entity& e,sf::Vector2f p)const{
 if(e.kind==alpha::Kind::Forest){float height=100.f*(.9f+(e.id%5)*.04f)*(e.build>0?.35f+.65f*(1-e.build/e.buildTotal):1);float y=(e.pos.y-p.y)/height;return y>=-.05f&&y<=1.f&&std::abs(p.x-e.pos.x)<height*(.045f+.43f*(1-std::max(0.f,y)));}
 if(textureFor(e.kind))return alpha::distance(e.pos+sf::Vector2f(0,alpha::mobile(e.kind)?-12.f:-30.f),p)<(alpha::mobile(e.kind)?32.f:75.f);
 return alpha::distance(e.pos,p)<alpha::radius(e.kind)+8;
}
const sf::Texture* Gameplay::textureFor(alpha::Kind kind,int faction)const{
 if(faction==1&&demonTile(kind)>=0&&demonReady_[alpha::mobile(kind)?0:1])return &demonArt_[alpha::mobile(kind)?0:1];
 if(kind==alpha::Kind::Forest&&treeReady_)return &treeArt_;
 int old=alpha::legacyArt(kind);if(old>=0&&old<6&&artReady_[old])return &art_[old];
 int sheet=alpha::mobile(kind)?0:1;if(alpha::def(kind).atlas>=0&&rosterReady_[sheet])return &rosterArt_[sheet];return nullptr;
}
void Gameplay::drawArt(sf::RenderTarget& target,const alpha::Entity& e,const Visual& v,float fade)const{
 bool humanArchivist=world_.mission==2&&e.id==world_.archivist;const auto* tex=textureFor(e.kind,humanArchivist?0:e.team>=0?world_.faction[e.team]:0);bool unit=alpha::mobile(e.kind);
 if(world_.mission==10&&e.id==world_.archivist){float pulse=.5f+.5f*std::sin(world_.time*1.8f);sf::CircleShape aura(95+pulse*8,64);aura.setOrigin(aura.getRadius(),aura.getRadius());aura.setScale(1,.5f);aura.setPosition(e.pos);aura.setFillColor({71,226,206,static_cast<sf::Uint8>(22+pulse*22)});aura.setOutlineThickness(2);aura.setOutlineColor({171,239,216,static_cast<sf::Uint8>(110+pulse*50)});target.draw(aura);sf::Sprite stone(crystalArt_);auto size=crystalArt_.getSize();float scale=250.f/size.y;stone.setOrigin(size.x*.5f,size.y*.84f);stone.setScale(scale,scale);stone.setPosition(e.pos);target.draw(stone);for(int i=0;i<7;++i){float t=std::fmod(world_.time*.3f+i*.14f,1.f);sf::CircleShape spark(2.f,5);spark.setPosition(e.pos+sf::Vector2f(std::sin(i*2.f+world_.time)*50,-40-t*130));spark.setFillColor({210,246,202,static_cast<sf::Uint8>((1-t)*180)});target.draw(spark);}return;}

 if(e.kind==alpha::Kind::Forest){
  float growth=e.build>0?.35f+.65f*(1-e.build/e.buildTotal):1;float variation=.9f+(e.id%5)*.04f;
  if(treeReady_){for(const auto& worker:world_.entities)if(worker.hp>0&&worker.team==0&&worker.selected&&alpha::mobile(worker.kind)&&worker.pos.y<e.pos.y&&e.pos.y-worker.pos.y<85&&std::abs(e.pos.x-worker.pos.x)<38){fade*=.48f;break;}sf::Sprite tree(treeArt_);auto size=treeArt_.getSize();float scale=100.f*growth*variation/size.y;tree.setOrigin(size.x*.5f,size.y*.98f);tree.setScale(scale,scale);tree.setPosition(e.pos);tree.setColor(sf::Color(235+(e.id%3)*10,245,230,static_cast<sf::Uint8>(255*fade)));target.draw(tree);return;}
  sf::RectangleShape trunk({7*growth,22*growth});trunk.setPosition(e.pos+sf::Vector2f(-3*growth,-7*growth));trunk.setFillColor({105,72,43,static_cast<sf::Uint8>(255*fade)});target.draw(trunk);
  for(int layer=0;layer<3;++layer){float width=(24-layer*5)*growth*variation;sf::ConvexShape crown(3);crown.setPoint(0,{-width,0});crown.setPoint(1,{0,-37*growth});crown.setPoint(2,{width,0});crown.setPosition(e.pos+sf::Vector2f(0,(-4-layer*14)*growth));crown.setFillColor(sf::Color(39+layer*8,91+layer*13+(e.id%3)*7,62+layer*6,static_cast<sf::Uint8>(255*fade)));crown.setOutlineThickness(1);crown.setOutlineColor({35,65,43,static_cast<sf::Uint8>(200*fade)});target.draw(crown);}return;
 }
 if(!humanArchivist&&e.team>=0&&world_.faction[e.team]==1&&demonTile(e.kind)>=0&&demonReady_[unit?0:1]){
  sf::Sprite sprite(*tex);auto rect=demonRects_[int(e.kind)];sprite.setTextureRect(rect);float extent=unit?(alpha::def(e.kind).domain==alpha::Domain::Sea?118.f:98.f):e.kind==alpha::Kind::Wall?72.f:195.f;float scale=extent/std::max(rect.width,rect.height);sprite.setOrigin(rect.width*.5f,rect.height*.83f);sprite.setScale(scale,scale);
  if(unit&&(v.direction==1||v.direction==2))sprite.setScale(-scale,scale);sprite.setPosition(e.pos);sf::Uint8 opacity=static_cast<sf::Uint8>((e.build>0?165:255)*fade);sprite.setColor({255,255,255,opacity});
  bool working=world_.busy(e)||(e.order==alpha::Order::Gather&&e.route.empty()&&e.work>0);bool attack=e.order==alpha::Order::Attack&&e.route.empty()&&e.cooldown>0;
  if(unit&&!e.route.empty())sprite.move(0,std::sin(v.phase*9)*2);if(unit&&(working||attack)){float swing=std::sin(v.phase*10);sprite.setRotation(swing*7);sprite.move(swing*2,0);}
  if(unit&&alpha::def(e.kind).domain==alpha::Domain::Air)sprite.move(0,std::sin(v.phase*5)*4);
  if(e.hp<=0){sprite.setRotation(v.death*75);sprite.setScale(sprite.getScale().x,sprite.getScale().y*(1-v.death*.7f));}target.draw(sprite);return;
 }
 if(e.kind==alpha::Kind::Oil){for(int i=0;i<4;++i){sf::CircleShape slick(24.f+i*4,28);slick.setOrigin(slick.getRadius(),slick.getRadius());slick.setScale(1.4f,.5f);slick.setPosition(e.pos+sf::Vector2f(std::sin(i*2.f)*13,std::cos(i*2.f)*6));slick.setFillColor({21,47,49,static_cast<sf::Uint8>(90*fade)});target.draw(slick);}sf::CircleShape ripple(43+std::sin(v.phase*2)*3,32);ripple.setOrigin(ripple.getRadius(),ripple.getRadius());ripple.setScale(1,.45f);ripple.setPosition(e.pos);ripple.setFillColor(sf::Color::Transparent);ripple.setOutlineThickness(1.5f);ripple.setOutlineColor({112,173,167,static_cast<sf::Uint8>(130*fade)});target.draw(ripple);float bob=std::sin(v.phase*3+e.id)*2;sf::RectangleShape buoy({15,20});buoy.setOrigin(7,16);buoy.setPosition(e.pos+sf::Vector2f(0,bob));buoy.setFillColor({135,87,49,static_cast<sf::Uint8>(255*fade)});buoy.setOutlineColor({204,172,105,static_cast<sf::Uint8>(255*fade)});buoy.setOutlineThickness(2);target.draw(buoy);sf::RectangleShape pole({2,24});pole.setPosition(e.pos+sf::Vector2f(0,-36+bob));pole.setFillColor({176,164,125});target.draw(pole);sf::ConvexShape flag(3);flag.setPoint(0,{0,0});flag.setPoint(1,{15,5});flag.setPoint(2,{0,10});flag.setPosition(e.pos+sf::Vector2f(2,-36+bob));flag.setFillColor({191,112,65});target.draw(flag);return;}
 if(!tex){sf::CircleShape fallback(alpha::radius(e.kind),unit?5:6);fallback.setOrigin(alpha::radius(e.kind),alpha::radius(e.kind));fallback.setPosition(e.pos);fallback.setFillColor(e.team<0?sf::Color(194,166,79):(e.team==0?sf::Color(65,168,178):sf::Color(211,98,72)));target.draw(fallback);return;}
 sf::Sprite sprite(*tex);float width=unit?105.f:(e.kind==alpha::Kind::Forest?210.f:(e.kind==alpha::Kind::Mine?170.f:e.kind==alpha::Kind::Wall?72.f:195.f));
 bool animated=unit&&alpha::legacyArt(e.kind)>=0;int tile=alpha::def(e.kind).atlas;
 if(animated||tile>=0){int col=0,row=0;
  if(animated){if(!e.route.empty())col=1+(int(v.phase*7)%2);else if((e.order==alpha::Order::Attack&&e.cooldown>.3f)||(e.kind==alpha::Kind::Worker&&e.order==alpha::Order::Gather&&e.work>0)||world_.busy(e))col=int(v.phase*5)%2?3:0;row=v.direction;}
  else{col=tile%4;row=tile/4;}
  int left=col*tex->getSize().x/4,top=row*tex->getSize().y/4,cw=(col+1)*tex->getSize().x/4-left,ch=(row+1)*tex->getSize().y/4-top;
  sprite.setTextureRect({left,top,cw,ch});sprite.setOrigin(cw*.5f,ch*(unit?.65f:.68f));sprite.setScale(width/cw,width/ch);
  if(!animated&&unit&&(v.direction==1||v.direction==2))sprite.setScale(-sprite.getScale().x,sprite.getScale().y);
 }else{sprite.setOrigin(tex->getSize().x*.5f,tex->getSize().y*.68f);float scale=width/std::max(tex->getSize().x,tex->getSize().y);sprite.setScale(scale,scale);}
 auto color=sf::Color(255,255,255,static_cast<sf::Uint8>((e.build>0?165:255)*fade));if(e.kind==alpha::Kind::Spirit)color=sf::Color(100,222,240,static_cast<sf::Uint8>(180*fade));if(e.kind==alpha::Kind::Submarine)color.a=static_cast<sf::Uint8>(170*fade);if(!humanArchivist&&e.team>=0&&world_.faction[e.team]==1)color=sf::Color(148,112,191,color.a);sprite.setColor(color);
 sprite.setPosition(e.pos);
 bool chopping=e.kind==alpha::Kind::Worker&&e.order==alpha::Order::Gather&&e.route.empty()&&e.work>0;
 chopping=chopping||(world_.busy(e)&&e.route.empty());
 bool striking=e.order==alpha::Order::Attack&&e.route.empty()&&e.cooldown>0;
 if(chopping||striking){float swing=std::sin(v.phase*(chopping?10.f:14.f));sprite.setRotation(swing*(chopping?9.f:6.f));sprite.move(swing*3,0);}
 if(!animated&&unit&&!e.route.empty())sprite.move(0,std::sin(v.phase*9)*2);
 if(e.hp<=0){sprite.setRotation(v.death*75);sprite.setScale(sprite.getScale().x,sprite.getScale().y*(1-v.death*.7f));}target.draw(sprite);
}
void Gameplay::regressionProductionControls(){
 CommandPanel panel;int checks=0;auto require=[&](bool ok,const char* label){if(!ok)throw std::runtime_error(label);++checks;std::cout<<"PASS "<<label<<std::endl;};
 auto click=[&](){auto size=ng::Main::getWindowPtr()->getSize();sf::Event e{};e.type=sf::Event::MouseButtonPressed;e.mouseButton.button=sf::Mouse::Left;e.mouseButton.x=int(70.f/1920*size.x);e.mouseButton.y=int(650.f/1080*size.y);handleEvents(e,panel);e.type=sf::Event::MouseButtonReleased;handleEvents(e,panel);};
 restart();require(commands().empty(),"no unit means no unrelated commands");action(1);require(world_.message.find("Costruisci")!=std::string::npos,"shortcut explains missing barracks");
 require(world_.build(7,{580,620}),"test barracks built");int b=world_.entities.back().id;for(auto& e:world_.entities)e.selected=e.id==b;require(commands().size()==1&&commands()[0].type==0,"unfinished building shows construction status");for(int i=0;i<320;++i)world_.step(.05f);
 int before=world_.gold;click();require(world_.gold==before-80&&world_.get(b)->production>0,"context icon starts soldier production");require(commands().size()==1&&commands()[0].type==7,"busy building shows cancel command");
 sf::Event key{};key.type=sf::Event::KeyPressed;key.key.code=sf::Keyboard::T;handleEvents(key,panel);require(world_.gold==before-80,"busy shortcut never charges twice");
 for(int i=0;i<145;++i)world_.step(.05f);require(world_.population(0,false)==7,"soldier appears after contextual production");world_.gold=100;handleEvents(key,panel);require(world_.gold==20&&world_.get(b)->production>0,"T shortcut still produces soldier");for(int i=0;i<145;++i)world_.step(.05f);click();require(world_.message.find("Oro insufficiente")!=std::string::npos,"context icon explains missing gold");
 std::cout<<"SUCCESS "<<checks<<" interface checks"<<std::endl;restart();
}
void Gameplay::restart(){
 minimapDragging_=panning_=false;world_.reset(factionChoice_);if(currentMission_>=0)world_.startMission(currentMission_,factionChoice_);else if(GetEnvironmentVariableA("FRONTIERE_ARSENAL",nullptr,0))world_.prepareArsenal();else if(!GetEnvironmentVariableA("FRONTIERE_CONTROL_TEST",nullptr,0)&&!GetEnvironmentVariableA("FRONTIERE_ROSTER_TEST",nullptr,0)&&!GetEnvironmentVariableA("FRONTIERE_GRAPHICS_TEST",nullptr,0)&&!GetEnvironmentVariableA("FRONTIERE_FOREST_TEST",nullptr,0)&&!GetEnvironmentVariableA("FRONTIERE_PACKAGE_TEST",nullptr,0)&&!GetEnvironmentVariableA("FRONTIERE_PROGRESS_TEST",nullptr,0))world_.startGuided();visuals_.clear();updateVisuals(0);noticeTime_=0;commandBuild_=planting_=false;cursorOrder_=commandPage_=0;dragging_=placing_=paused_=casting_=false;catalogMode_=-1;catalogPage_=0;zoom_=1;builder_=-1;placingKind_=alpha::Kind::Barracks;


 view_.setViewport({455.f/1920,70.f/1080,1423.f/1920,976.f/1080});
 view_.setSize(1423,976);view_.setCenter(711.5f,currentMission_>=0?1050.f:800.f);if(currentMission_>=5)for(const auto& e:world_.entities)if(e.hp>0&&e.team==0&&e.kind==alpha::Kind::Hall){view_.setCenter(e.pos+sf::Vector2f(320,-30));break;}constrainCamera();
}
sf::Vector2f Gameplay::worldPoint(int x,int y)const{return ng::Main::getWindowPtr()->mapPixelToCoords({x,y},view_);}
void Gameplay::constrainCamera(){
 auto c=view_.getCenter(),s=view_.getSize();
 c.x=std::clamp(c.x,s.x/2,world_.width()-s.x/2);
 c.y=std::clamp(c.y,s.y/2,world_.height()-s.y/2);view_.setCenter(c);
}
void Gameplay::action(int n){
 if(n==3){paused_=!paused_;return;}
 if(paused_){world_.message="Partita in pausa: premi P per riprendere.";return;}
 if(world_.result)return;
 if(n==1){
  if(!world_.missionAllows(alpha::Kind::Soldier)){world_.message="I soldati si sbloccano nella seconda missione.";return;}
  int candidate=-1;
  for(const auto& e:world_.entities)if(e.hp>0&&e.team==0&&e.kind==alpha::Kind::Barracks){
   if(candidate<0)candidate=e.id;
   if(e.selected){candidate=e.id;break;}
   if(e.build<=0&&e.production<=0)candidate=e.id;
  }
  if(candidate<0){world_.message="Costruisci prima una caserma: seleziona operaio, premi B e scegli il terreno.";return;}
  for(auto& e:world_.entities)e.selected=e.id==candidate;
  world_.train(candidate,alpha::Kind::Soldier);return;
 }
 for(auto& e:world_.entities)if(e.selected&&e.team==0&&e.hp>0){
  if(n==0&&e.kind==alpha::Kind::Hall){world_.train(e.id,alpha::Kind::Worker);return;}
  if(n==1&&e.kind==alpha::Kind::Barracks){world_.train(e.id,alpha::Kind::Soldier);return;}
  if(n==2&&e.kind==alpha::Kind::Worker){auto issue=world_.unavailable(alpha::Kind::Barracks);if(!issue.empty()){world_.message=issue;return;}placingKind_=alpha::Kind::Barracks;placing_=true;builder_=e.id;world_.message="Clic sul terreno libero: caserma. Destro: annulla.";return;}
 }
 world_.message="Seleziona comando (operaio), caserma (soldato) o operaio (costruisci).";
}
void Gameplay::handleEvents(const sf::Event& ev,CommandPanel&){
 if(cheatEvent(ev))return;
 if(ev.type==sf::Event::LostFocus){minimapDragging_=panning_=dragging_=false;return;}
 if(ev.type==sf::Event::KeyPressed){
  if(ev.key.code==sf::Keyboard::F2&&world_.mission<0){factionChoice_=1-factionChoice_;restart();return;}
  if(ev.key.code>=sf::Keyboard::F3&&ev.key.code<=sf::Keyboard::F6){catalogMode_=int(ev.key.code)-int(sf::Keyboard::F3);catalogPage_=0;placing_=casting_=false;return;}
  if(ev.key.code==sf::Keyboard::Escape&&hasOverlay()){catalogMode_=-1;casting_=placing_=planting_=false;cursorOrder_=0;commandBuild_=false;return;}
  if(catalogMode_>=0){catalogEvent(ev,{});return;}
  if(ev.key.code==sf::Keyboard::C){for(auto& e:world_.entities)if(e.selected&&world_.cancel(e.id))break;return;}
  if(ev.key.code==sf::Keyboard::U){for(auto& e:world_.entities)if(e.selected&&e.kind==alpha::Kind::Transport){cursorOrder_=5;world_.message="Clic sulla costa vicino al trasporto per sbarcare. Destro annulla.";break;}return;}
  if(ev.key.code==sf::Keyboard::R){restart();return;}
  if(ev.key.code==sf::Keyboard::P){action(3);return;}
  if(ev.key.code==sf::Keyboard::O)action(0);
  if(ev.key.code==sf::Keyboard::T)action(1);
  if(ev.key.code==sf::Keyboard::B)action(2);
  if(ev.key.code==sf::Keyboard::Space) {view_.setCenter(712,800);constrainCamera();}
  if(ev.key.code==sf::Keyboard::H)for(auto& e:world_.entities)if(e.team==0)e.selected=e.hp>0&&e.carrier<0&&alpha::mobile(e.kind)&&e.id!=world_.archivist&&e.kind!=alpha::Kind::Worker&&(alpha::def(e.kind).damage>0||e.kind==alpha::Kind::Healer);
 }
 auto pixel=sf::Mouse::getPosition(*ng::Main::getWindowPtr());
 if(ev.type==sf::Event::MouseButtonPressed||ev.type==sf::Event::MouseButtonReleased)pixel={ev.mouseButton.x,ev.mouseButton.y};
 else if(ev.type==sf::Event::MouseMoved)pixel={ev.mouseMove.x,ev.mouseMove.y};
 else if(ev.type==sf::Event::MouseWheelScrolled)pixel={ev.mouseWheelScroll.x,ev.mouseWheelScroll.y};
 auto ui=ng::Main::getWindowPtr()->mapPixelToCoords(pixel,sf::View(sf::FloatRect(0,0,1920,1080)));
 mouseWorld_=worldPoint(pixel.x,pixel.y);uiMouse_=ui;
 if(catalogMode_>=0){catalogEvent(ev,ui);return;}
 bool inside=getGlobalBounds().contains(ui);
 if(ev.type==sf::Event::MouseButtonReleased&&ev.mouseButton.button==sf::Mouse::Middle){panning_=false;return;}
 if(ev.type==sf::Event::MouseButtonReleased&&ev.mouseButton.button==sf::Mouse::Left&&minimapDragging_){minimapCamera(ui);minimapDragging_=false;return;}
 if(ev.type==sf::Event::MouseMoved&&minimapDragging_){minimapCamera(ui);return;}
 if(ev.type==sf::Event::MouseMoved&&panning_){dragCamera(pixel);return;}
 if(ev.type==sf::Event::MouseButtonPressed&&ev.mouseButton.button==sf::Mouse::Middle&&inside){panning_=true;panPixel_=pixel;dragging_=false;return;}
 if(ev.type==sf::Event::MouseWheelScrolled&&inside){float factor=ev.mouseWheelScroll.delta>0?.9f:1.1f;float z=std::clamp(zoom_*factor,.65f,1.4f);view_.setSize(1423*z,976*z);zoom_=z;constrainCamera();}
 if(ev.type==sf::Event::MouseButtonPressed){
  if(ev.mouseButton.button==sf::Mouse::Left){
   if(sf::FloatRect(25,946,390,42).contains(ui)){catalogMode_=std::clamp(int((ui.x-25)/97.5f),0,3);catalogPage_=0;return;}
   if(minimapBounds().contains(ui)){minimapDragging_=true;dragging_=false;minimapCamera(ui);return;}
   if(sf::FloatRect(25,610,390,282).contains(ui)){auto list=commands();int col=int((ui.x-25)/132),row=int((ui.y-610)/94),idx=commandPage_*9+row*3+col;if(col<3&&idx<int(list.size())){auto command=list[idx];executeCommand(command);}return;}
   if(sf::FloatRect(25,900,390,36).contains(ui)){int pages=std::max(1,(int(commands().size())+8)/9);commandPage_=(commandPage_+(ui.x<220?pages-1:1))%pages;return;}
   if(!inside||paused_||world_.result)return;
   if(planting_&&world_.fogEnabled&&!world_.exploredAt(mouseWorld_)){world_.message="Esplora prima questa zona.";return;}
   if(planting_){if(world_.plant(builder_,mouseWorld_))planting_=false;return;}
   if(cursorOrder_){int mode=cursorOrder_;cursorOrder_=0;int id=pickEntity(mouseWorld_,mode==2);for(auto& e:world_.entities)if(e.selected&&e.team==0&&alpha::mobile(e.kind)){if(mode==1)world_.move(e,mouseWorld_);else if(mode==2)world_.gather(e,id);else if(mode==3)world_.attack(e,id);else if(mode==4)world_.repair(e,id);else if(mode==5&&e.kind==alpha::Kind::Transport)world_.unload(e.id,mouseWorld_);}return;}
   if(casting_){for(const auto& e:world_.entities)if(e.selected&&e.team==0&&world_.supports(e.kind,pendingSpell_)){world_.cast(e.id,pendingSpell_,mouseWorld_);break;}casting_=false;return;}
   if(placing_&&world_.fogEnabled&&!world_.exploredAt(mouseWorld_)){world_.message="Esplora prima questa zona per costruire.";return;}
   if(placing_){if(world_.build(builder_,mouseWorld_,placingKind_)){
    placing_=false;int id=world_.entities.back().id;
    for(auto& e:world_.entities)e.selected=e.id==id;
   }return;}
   commandBuild_=false;commandPage_=0;dragging_=true;dragStart_=mouseWorld_;
  }else if(ev.mouseButton.button==sf::Mouse::Right){
   if(planting_||cursorOrder_){planting_=false;cursorOrder_=0;return;}if(casting_){casting_=false;return;}
   if(placing_){placing_=false;world_.message="Costruzione annullata.";return;}
   if(!inside||paused_||world_.result)return;
   int target=pickEntity(mouseWorld_);
   auto oilTarget=world_.get(target);if(oilTarget&&oilTarget->kind==alpha::Kind::Oil){for(const auto& e:world_.entities)if(e.selected&&e.team==0&&e.kind==alpha::Kind::Tanker){world_.build(e.id,oilTarget->pos,alpha::Kind::OilRig);return;}}
   int i=0;for(auto& e:world_.entities)if(e.selected&&e.team==0&&alpha::mobile(e.kind)&&e.hp>0){
    auto t=world_.get(target);
    if(t&&((alpha::resource(t->kind)&&e.kind==alpha::Kind::Worker)||(t->kind==alpha::Kind::OilRig&&e.kind==alpha::Kind::Tanker)))world_.gather(e,t->id);
    else if(t&&t->team==0&&t->kind==alpha::Kind::Transport&&alpha::def(e.kind).domain==alpha::Domain::Land)world_.board(e,t->id);
    else if(t&&t->team==0&&!alpha::mobile(t->kind)&&!alpha::resource(t->kind)&&e.kind==alpha::Kind::Worker)world_.repair(e,t->id);
    else if(t&&t->team==1)world_.attack(e,t->id);
    else if(e.kind==alpha::Kind::Transport&&!world_.water(mouseWorld_))world_.unload(e.id,mouseWorld_);
    else {auto destination=world_.mission==0&&t&&t->team<0&&t->kind==alpha::Kind::Farm?world_.missionTarget():mouseWorld_;world_.move(e,destination+sf::Vector2f((i%4)*35.f,(i/4)*35.f));++i;}
   }
  }
 }
 if(ev.type==sf::Event::MouseButtonReleased&&ev.mouseButton.button==sf::Mouse::Left&&dragging_){
  dragging_=false;sf::FloatRect box(std::min(dragStart_.x,mouseWorld_.x),std::min(dragStart_.y,mouseWorld_.y),std::abs(dragStart_.x-mouseWorld_.x),std::abs(dragStart_.y-mouseWorld_.y));
  bool click=alpha::distance(dragStart_,mouseWorld_)<10;
  if(click&&!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&selectedWorkersGather(pickEntity(mouseWorld_)))return;
  if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))for(auto& e:world_.entities)e.selected=false;
  if(click){for(auto& e:world_.entities)if(e.hp>0&&e.carrier<0&&e.team==0&&e.id!=world_.archivist&&hitEntity(e,mouseWorld_)){e.selected=true;break;}}
  else for(auto& e:world_.entities)if(e.hp>0&&e.carrier<0&&e.team==0&&e.id!=world_.archivist&&alpha::mobile(e.kind)&&box.contains(e.pos))e.selected=true;
 }
}
void Gameplay::update(){
 if(cheatOpen_)return;
 float dt=std::min(.1f,ng::Timer::getDeltaTime());noticeTime_=std::max(0.f,noticeTime_-dt);
 if(ng::Main::windowHasFocus()&&catalogMode_<0){
  auto pixel=sf::Mouse::getPosition(*ng::Main::getWindowPtr());auto size=ng::Main::getWindowPtr()->getSize();
  uiMouse_=ng::Main::getWindowPtr()->mapPixelToCoords(pixel,sf::View(sf::FloatRect(0,0,1920,1080)));
  if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))minimapDragging_=false;
  if(!sf::Mouse::isButtonPressed(sf::Mouse::Middle))panning_=false;
  float speed=1300*zoom_*dt;sf::Vector2f movement;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)||sf::Keyboard::isKeyPressed(sf::Keyboard::Left))movement.x-=1;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)||sf::Keyboard::isKeyPressed(sf::Keyboard::Right))movement.x+=1;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)||sf::Keyboard::isKeyPressed(sf::Keyboard::Up))movement.y-=1;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)||sf::Keyboard::isKeyPressed(sf::Keyboard::Down))movement.y+=1;

  float length=alpha::distance(movement,{});if(length>1)movement/=length;
  view_.move(movement*speed);constrainCamera();mouseWorld_=worldPoint(pixel.x,pixel.y);
  if(!paused_){world_.update(dt);updateVisuals(dt);}
 }
}
void Gameplay::text(sf::RenderTarget& t,const std::string& s,sf::Vector2f p,unsigned size,sf::Color c)const{std::string displayed=loc::tr(s);if(world_.faction[0]==1){auto replace=[&](const std::string& a,const std::string& b){size_t n=0;while((n=displayed.find(a,n))!=std::string::npos){displayed.replace(n,a.size(),b);n+=b.size();}};replace("Liora:","Vaerak:");replace("arcanisti","evocatori");replace("arcanists","summoners");replace("roc da guerra","viverne infernali");replace("war rocs","infernal wyverns");}sf::Text v(displayed,*font_,size);v.setPosition(p);v.setFillColor(c);t.draw(v);}
#include "GameplayNavigation.inl"
#include "GameplayCommands.inl"
#include "GameplayRenderer.inl"
#include "GameplayCatalog.inl"
#include "GameplayCheats.inl"











#include "GameplayDemonArt.inl"

#include "GameplayCampaign.inl"

#include "GameplayTerrain.inl"
#include "GameplayActivity.inl"
