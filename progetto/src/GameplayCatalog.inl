int Gameplay::researchProducer(alpha::Research r)const{
 int candidate=-1;auto kind=alpha::researchCatalog()[int(r)].building;
 for(const auto& e:world_.entities)if(e.hp>0&&e.team==0&&e.kind==kind){if(e.selected)return e.id;if(candidate<0||world_.researchIssue(e.id,r,false).empty())candidate=e.id;}
 return candidate;
}
bool Gameplay::produce(alpha::Kind kind){
 auto producer=alpha::def(kind).producer;int candidate=-1;
 for(const auto& e:world_.entities)if(e.hp>0&&e.team==0&&e.kind==producer){if(candidate<0)candidate=e.id;if(e.selected){candidate=e.id;break;}if(e.build<=0&&e.production<=0)candidate=e.id;}
 if(candidate<0){world_.message=std::string("Serve ")+alpha::name(producer)+" per addestrare "+alpha::name(kind)+".";return false;}
 for(auto& e:world_.entities)e.selected=e.id==candidate;
 return world_.train(candidate,kind);
}
void Gameplay::regressionRosterControls(){
 CommandPanel panel;int checks=0;
 auto require=[&](bool ok,const char* name){if(!ok)throw std::runtime_error(name);++checks;std::cout<<"PASS "<<name<<std::endl;};
 auto key=[&](sf::Keyboard::Key k){sf::Event e{};e.type=sf::Event::KeyPressed;e.key.code=k;handleEvents(e,panel);};
 auto click=[&](sf::Vector2f ui){auto size=ng::Main::getWindowPtr()->getSize();sf::Event e{};e.type=sf::Event::MouseButtonPressed;e.mouseButton.button=sf::Mouse::Left;e.mouseButton.x=int(ui.x/1920*size.x);e.mouseButton.y=int(ui.y/1080*size.y);handleEvents(e,panel);e.type=sf::Event::MouseButtonReleased;handleEvents(e,panel);};
 auto worldClick=[&](sf::Vector2f p){auto pixel=ng::Main::getWindowPtr()->mapCoordsToPixel(p,view_);sf::Event e{};e.type=sf::Event::MouseButtonPressed;e.mouseButton.button=sf::Mouse::Left;e.mouseButton.x=pixel.x;e.mouseButton.y=pixel.y;handleEvents(e,panel);e.type=sf::Event::MouseButtonReleased;handleEvents(e,panel);};
 restart();world_.prepareArsenal();key(sf::Keyboard::F3);require(catalogMode_==0&&catalogItems().size()==17,"F3 hides opposite faction caster");
 key(sf::Keyboard::Right);require(catalogPage_==1,"unit catalog pagination");int ships=0;for(auto& e:world_.entities)if(e.kind==alpha::Kind::Battleship&&e.team==0)++ships;
 int before=world_.gold;{auto items=catalogItems();int index=int(std::find(items.begin(),items.end(),int(alpha::Kind::Battleship))-items.begin())%12;click({600.f+(index%3)*440,255.f+(index/3)*170});}require(catalogMode_==-1&&world_.gold==before-260,"catalog mouse starts battleship production");
 for(int i=0;i<410;++i)world_.step(.05f);int after=0;for(auto& e:world_.entities)if(e.kind==alpha::Kind::Battleship&&e.team==0)++after;require(after==ships+1,"catalog-produced battleship appears");
 key(sf::Keyboard::F4);require(catalogMode_==1&&catalogItems().size()==18,"F4 opens 18 building types");click({1480,255});require(placing_&&placingKind_==alpha::Kind::Farm&&catalogMode_==-1,"catalog arms farm placement");
 world_.get(builder_)->pos={360,680};before=world_.gold;worldClick({500,800});require(!placing_&&world_.gold==before-70&&world_.entities.back().kind==alpha::Kind::Farm,"map click places and pays selected building");
 world_.upgrades[0][int(alpha::Research::Weapons)]=0;key(sf::Keyboard::F5);require(catalogMode_==2&&catalogItems().size()==14,"F5 opens research and tower upgrades");click({1480,255});require(catalogMode_==-1,"research catalog starts weapon research");for(int i=0;i<245;++i)world_.step(.05f);require(world_.upgrades[0][int(alpha::Research::Weapons)]==1,"weapon research completes through interface");
 int healer=-1;for(auto& e:world_.entities){e.selected=false;if(e.kind==alpha::Kind::Healer&&e.team==0)healer=e.id;}
 require(healer>=0,"arsenal supplies spell caster");world_.get(healer)->pos={600,750};world_.get(healer)->selected=true;world_.get(11)->hp-=60;float hp=world_.get(11)->hp;
 key(sf::Keyboard::F6);require(catalogMode_==3&&catalogItems().size()==5,"F6 hides shadow-only spells from humans");click({600,255});require(casting_&&pendingSpell_==alpha::Spell::Heal,"spell catalog arms targeting");worldClick(world_.get(11)->pos);require(!casting_&&world_.get(11)->hp>hp,"map target applies healing spell");
 key(sf::Keyboard::F3);key(sf::Keyboard::Escape);require(catalogMode_==-1,"Escape closes catalog");key(sf::Keyboard::F2);require(world_.faction[0]==1,"F2 starts alternate faction");
 restart();world_.prepareArsenal();for(auto& e:world_.entities)e.selected=e.id==1;
 auto list=commands();int countTrain=0;for(auto c:list)if(c.type==1){++countTrain;require(c.item==int(alpha::Kind::Worker),"castle production card is worker");}require(countTrain==1,"castle has exactly one unit production type");
 int base=-1;for(auto& e:world_.entities){e.selected=false;if(e.team==0&&e.kind==alpha::Kind::Barracks)base=e.id;}world_.get(base)->selected=true;world_.get(base)->level=1;
 list=commands();int upgrade=-1;for(int i=0;i<int(list.size());++i)if(list[i].type==3&&list[i].item==int(alpha::Research::Barracks2))upgrade=i;require(upgrade>=0,"selected base barracks offers its upgrade");
 click({25+(upgrade%3)*132.f+50,610+(upgrade/3)*94.f+35});require(world_.get(base)->research==int(alpha::Research::Barracks2),"upgrade icon uses selected barracks");for(int i=0;i<245;++i)world_.step(.05f);require(world_.get(base)->level==2,"contextual barracks upgrade completes");before=world_.gold;click({205,650});require(world_.gold==before-75&&world_.get(base)->training==alpha::Kind::Archer,"archer icon trains from upgraded barracks");
 for(auto& e:world_.entities)e.selected=e.id==7;click({340,740});require(commandBuild_&&commands().size()==18,"worker build button opens its building icons");click({340,650});require(placing_&&placingKind_==alpha::Kind::Farm,"worker farm icon arms placement");before=world_.gold;worldClick({500,800});require(!placing_&&world_.gold==before-70,"worker farm icon places and pays building");

 for(int tech=0;tech<int(alpha::Research::Town4);++tech){
  restart();world_.prepareArsenal();auto research=alpha::Research(tech);const auto& d=alpha::researchCatalog()[tech];int producer=-1;
  for(auto& e:world_.entities){e.selected=false;if(e.hp>0&&e.team==0&&e.kind==d.building)producer=e.id;}
  world_.get(producer)->selected=true;world_.upgrades[0][tech]=0;
  if(research==alpha::Research::Town2)world_.townLevel[0]=1;if(research==alpha::Research::Town3)world_.townLevel[0]=2;
  if(research==alpha::Research::Barracks2)world_.get(producer)->level=1;if(research==alpha::Research::Barracks3)world_.get(producer)->level=2;
  for(int level=0;level<d.maxLevel;++level){auto cards=commands();int index=-1;for(int n=0;n<int(cards.size());++n)if(cards[n].type==3&&cards[n].item==tech)index=n;
   require(index>=0,"each unfinished research has a contextual icon");commandPage_=index/9;int slot=index%9;click({25+(slot%3)*132.f+50,610+(slot/3)*94.f+35});
   require(world_.get(producer)->research==tech&&noticeTime_>0,"every research icon starts selected producer and shows feedback");
   for(int tick=0;tick<int((d.seconds+.5f)*20);++tick)world_.step(.05f);
   require(world_.upgrades[0][tech]==level+1,"every research icon completes correct upgrade level");
  }
 }
 for(auto kind:{alpha::Kind::GuardTower,alpha::Kind::CannonTower}){restart();world_.prepareArsenal();int tower=-1;for(auto& e:world_.entities){e.selected=false;if(e.team==0&&e.kind==alpha::Kind::WatchTower)tower=e.id;}world_.get(tower)->selected=true;auto cards=commands();for(int n=0;n<int(cards.size());++n)if(cards[n].type==12&&cards[n].item==int(kind)){click({25+(n%3)*132.f+50,610+(n/3)*94.f+35});break;}for(int tick=0;tick<170;++tick)world_.step(.05f);require(world_.get(tower)->kind==kind,"both tower evolution icons transform selected tower");}
 restart();world_.gold=world_.wood=1000;int lockedBase=world_.add(alpha::Kind::Barracks,0,{640,750});for(auto& e:world_.entities)e.selected=e.id==lockedBase;
 auto lockedCards=commands();for(auto c:lockedCards)if(c.type==3&&c.item==int(alpha::Research::Barracks2))executeCommand(c);
 require(world_.get(lockedBase)->production==0&&commandNotice_.find("Segheria")!=std::string::npos&&noticeTime_>0,"missing lumber mill explains blocked barracks II click persistently");
 int mill=world_.add(alpha::Kind::LumberMill,0,{800,750});world_.get(mill)->build=2;require(!world_.researchIssue(lockedBase,alpha::Research::Barracks2).empty(),"unfinished prerequisite cannot unlock upgrade");world_.get(mill)->build=0;world_.gold=0;
 require(world_.researchIssue(lockedBase,alpha::Research::Barracks2).find("Risorse")!=std::string::npos,"insufficient upgrade resources explained explicitly");
 std::cout<<"SUCCESS "<<checks<<" roster interface checks"<<std::endl;
 factionChoice_=0;restart();world_.prepareArsenal();for(auto& e:world_.entities)e.selected=e.kind==alpha::Kind::Barracks&&e.team==0;if(GetEnvironmentVariableA("FRONTIERE_WORKER_PREVIEW",nullptr,0)){for(auto& e:world_.entities)e.selected=e.id==7;world_.get(7)->pos={470,970};world_.gather(*world_.get(7),4);for(int i=0;i<20;++i)world_.step(.05f);}updateVisuals(.8f);catalogMode_=-1;catalogPage_=0;
}
std::vector<int> Gameplay::catalogItems()const{
 std::vector<int> items;
 if(catalogMode_<=1)for(int i=0;i<int(alpha::Kind::Count);++i){auto k=alpha::Kind(i);if(!world_.missionAllows(k)||!world_.factionAllows(k))continue;if(catalogMode_==0?alpha::mobile(k)&&alpha::def(k).producer!=alpha::Kind::Count:!alpha::mobile(k)&&!alpha::resource(k))items.push_back(i);}
 else if(catalogMode_==2){for(int i=0;i<int(alpha::Research::Count)+2;++i)if(i<int(alpha::Research::Count)?world_.missionResearchAllows(alpha::Research(i)):world_.missionAllows(i==int(alpha::Research::Count)?alpha::Kind::GuardTower:alpha::Kind::CannonTower))items.push_back(i);}
 else for(int i=0;i<int(alpha::Spell::Count);++i){auto spell=alpha::Spell(i);bool allowed=false;for(int k=0;k<int(alpha::Kind::Count);++k)if(world_.factionAllows(alpha::Kind(k))&&world_.missionAllows(alpha::Kind(k))&&world_.supports(alpha::Kind(k),spell))allowed=true;if(allowed)items.push_back(i);}
 return items;
}
void Gameplay::chooseCatalog(int index){
 auto items=catalogItems();if(index<0||index>=int(items.size()))return;int item=items[index];
 if(catalogMode_==0){if(produce(alpha::Kind(item)))catalogMode_=-1;return;}
 if(catalogMode_==1){auto kind=alpha::Kind(item);auto reason=world_.unavailable(kind);if(!reason.empty()){world_.message=reason;return;}auto builderKind=kind==alpha::Kind::OilRig?alpha::Kind::Tanker:alpha::Kind::Worker;int id=-1;
  for(const auto& e:world_.entities)if(e.hp>0&&e.team==0&&e.carrier<0&&e.kind==builderKind){if(id<0)id=e.id;if(e.selected){id=e.id;break;}}
  if(id<0){world_.message=std::string("Serve un ")+alpha::name(builderKind)+" per costruire.";return;}
  for(auto& e:world_.entities)e.selected=e.id==id;
  builder_=id;placingKind_=kind;placing_=true;casting_=false;catalogMode_=-1;world_.message=std::string("Scegli la posizione di ")+alpha::name(kind)+" in uno spazio libero sulla mappa.";return;
 }
 if(catalogMode_==2){bool tower=item>=int(alpha::Research::Count);auto producer=tower?alpha::Kind::WatchTower:alpha::researchCatalog()[item].building;int id=-1;
  if(!tower)id=researchProducer(alpha::Research(item));else for(const auto& e:world_.entities)if(e.hp>0&&e.team==0&&e.kind==producer){if(id<0)id=e.id;if(e.selected){id=e.id;break;}if(e.build<=0&&e.production<=0)id=e.id;}
  if(id<0){world_.message=std::string("Serve ")+alpha::name(producer)+" per questa ricerca.";return;}
  for(auto& e:world_.entities)e.selected=e.id==id;
  bool ok=tower?world_.upgradeTower(id,item==int(alpha::Research::Count)?alpha::Kind::GuardTower:alpha::Kind::CannonTower):world_.research(id,alpha::Research(item));if(ok)catalogMode_=-1;return;
 }
 auto spell=alpha::Spell(item);bool known=false;for(const auto& e:world_.entities)if(e.selected&&e.hp>0&&e.team==0&&world_.supports(e.kind,spell)){known=true;break;}
 if(!known){world_.message="Seleziona prima un incantatore che conosce questa magia.";return;}
 pendingSpell_=spell;casting_=true;placing_=false;catalogMode_=-1;world_.message=std::string("Magia ")+alpha::spellName(spell)+": clic sul bersaglio; destro annulla.";
}
bool Gameplay::catalogEvent(const sf::Event& ev,sf::Vector2f ui){
 if(ev.type==sf::Event::KeyPressed){auto list=catalogItems();int pages=std::max(1,(int(list.size())+11)/12);
  if(ev.key.code==sf::Keyboard::Left)catalogPage_=(catalogPage_+pages-1)%pages;
  if(ev.key.code==sf::Keyboard::Right)catalogPage_=(catalogPage_+1)%pages;
  if(ev.key.code>=sf::Keyboard::Num1&&ev.key.code<=sf::Keyboard::Num9)chooseCatalog(catalogPage_*12+int(ev.key.code)-int(sf::Keyboard::Num1));
  return true;
 }
 if(ev.type==sf::Event::MouseButtonPressed){if(ev.mouseButton.button==sf::Mouse::Right){catalogMode_=-1;return true;}if(ev.mouseButton.button!=sf::Mouse::Left)return true;
  if(sf::FloatRect(1775,100,50,46).contains(ui)){catalogMode_=-1;return true;}
  for(int i=0;i<4;++i)if(sf::FloatRect(500.f+i*310,155,295,36).contains(ui)){catalogMode_=i;catalogPage_=0;return true;}
  auto list=catalogItems();int pages=std::max(1,(int(list.size())+11)/12);
  if(sf::FloatRect(1530,970,130,40).contains(ui)){catalogPage_=(catalogPage_+pages-1)%pages;return true;}
  if(sf::FloatRect(1690,970,130,40).contains(ui)){catalogPage_=(catalogPage_+1)%pages;return true;}
  for(int n=0;n<12;++n)if(sf::FloatRect(500.f+(n%3)*440,210.f+(n/3)*170,420,150).contains(ui)){chooseCatalog(catalogPage_*12+n);return true;}
 }return true;
}
void Gameplay::drawCatalog(sf::RenderTarget& target)const{
 if(catalogMode_<0)return;
 auto box=[&](sf::Vector2f p,sf::Vector2f size,sf::Color c,sf::Color line=sf::Color(92,119,107)){sf::RectangleShape r(size);r.setPosition(p);r.setFillColor(c);r.setOutlineColor(line);r.setOutlineThickness(1);target.draw(r);};
 box({475,82},{1370,950},{16,29,33,250},{175,150,99});
 text(target,"ARSENALE DELLA FRONTIERA",{500,100},30,{238,218,168});text(target,"X",{1790,104},27);
 const char* tabs[]={"F3  UNITA","F4  EDIFICI","F5  RICERCHE","F6  MAGIE"};for(int i=0;i<4;++i){box({500.f+i*310,155},{295,36},i==catalogMode_?sf::Color(74,103,85):sf::Color(32,50,54));text(target,tabs[i],{510.f+i*310,161},20);}
 auto items=catalogItems();int pages=std::max(1,(int(items.size())+11)/12);
 for(int n=0;n<12;++n){int idx=catalogPage_*12+n;if(idx>=int(items.size()))break;int item=items[idx];sf::Vector2f p{500.f+(n%3)*440,210.f+(n/3)*170};
  std::string label,role,cost,requirement;alpha::Kind artKind=alpha::Kind::Count;
  if(catalogMode_<=1){artKind=alpha::Kind(item);const auto& d=alpha::def(artKind);label=alpha::factionUnitName(artKind,world_.faction[0]);role=d.role;cost=std::to_string(d.cost.gold)+" oro / "+std::to_string(d.cost.wood)+" legno / "+std::to_string(d.cost.oil)+" petrolio / "+std::to_string(d.cost.crystals)+" cristalli";requirement=world_.unavailable(artKind);if(requirement.empty())requirement=catalogMode_==0?std::string("Da ")+alpha::name(d.producer)+" | "+std::to_string(int(d.seconds))+" s":"Livello "+std::to_string(d.tier)+" | "+std::to_string(int(d.seconds))+" s";
  }else if(catalogMode_==2){if(item<int(alpha::Research::Count)){auto& d=alpha::researchCatalog()[item];label=d.name;role=std::string("Edificio: ")+alpha::name(d.building);cost=std::to_string(d.cost.gold)+" oro / "+std::to_string(d.cost.wood)+" legno / "+std::to_string(d.cost.oil)+" petrolio / "+std::to_string(d.cost.crystals)+" cristalli";requirement="Livello ricerca "+std::to_string(world_.upgrades[0][item])+" / "+std::to_string(d.maxLevel);artKind=d.building;if(item==int(alpha::Research::Barracks2)||item==int(alpha::Research::Barracks3))requirement="Potenzia la singola caserma selezionata";auto issue=world_.researchIssue(researchProducer(alpha::Research(item)),alpha::Research(item));if(!issue.empty())requirement=issue;}else{bool guard=item==int(alpha::Research::Count);artKind=guard?alpha::Kind::GuardTower:alpha::Kind::CannonTower;label=std::string("Evolvi in ")+alpha::name(artKind);role="Seleziona o costruisci una torre di vedetta";cost=guard?"60 oro / 30 legno":"110 oro / 50 legno";requirement="Evoluzione: 8 secondi";}}
  else{auto s=alpha::Spell(item);label=alpha::spellName(s);cost=std::to_string(alpha::spellCost(s))+" mana | Bersaglio entro 460";role="Seleziona incantatore, scegli magia e bersaglio";requirement=(s==alpha::Spell::Raise||s==alpha::Spell::Firestorm||s==alpha::Spell::Storm)?"Richiede Sapienza arcana":"Supporto e controllo del campo";}
  box(p,{420,150},{35,54,57});
  if(artKind!=alpha::Kind::Count)drawIcon(target,artKind,{p.x+7,p.y+6,65,65});
  text(target,loc::tr(label).substr(0,33),p+sf::Vector2f(78,9),22,{239,221,177});text(target,cost,p+sf::Vector2f(12,77),15,{216,198,143});
  text(target,loc::tr(role).substr(0,48),p+sf::Vector2f(12,106),16,{175,201,185});text(target,loc::tr(requirement).substr(0,49),p+sf::Vector2f(12,130),15,{163,205,181});
 }
 text(target,loc::tr(world_.message).substr(0,112),{500,913},20,{245,213,149});text(target,"Catalogo in pausa | 1-9: scelta rapida | Esc/destro: chiudi",{500,973},19,{176,199,188});
 text(target,"Pagina "+std::to_string(catalogPage_+1)+"/"+std::to_string(pages),{1330,977},20);box({1530,970},{130,40},{48,73,71});text(target,"< Indietro",{1540,976},20);box({1690,970},{130,40},{48,73,71});text(target,"Avanti >",{1700,976},20);
}




