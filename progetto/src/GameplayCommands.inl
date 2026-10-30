// Context-sensitive command cards. No automatic producer substitution here.
std::vector<Gameplay::Command> Gameplay::commands()const{
 std::vector<Command> out;const alpha::Entity* selected=nullptr;
 for(const auto& e:world_.entities)if(e.hp>0&&e.team==0&&e.selected){selected=&e;break;}
 if(!selected)return out;const auto& e=*selected;using K=alpha::Kind;
 auto push=[&](int type,int item,K icon,const std::string& label,const std::string& detail){out.push_back({type,item,icon,label,detail});};
 auto price=[](alpha::Cost c){return std::to_string(c.gold)+" oro, "+std::to_string(c.wood)+" legno, "+std::to_string(c.oil)+" petrolio, "+std::to_string(c.crystals)+" cristalli";};
 if(e.build>0){push(0,0,e.kind,"In costruzione",std::to_string(int(std::ceil(e.build)))+" secondi rimanenti");return out;}
 if(e.production>0){push(7,0,e.kind,"Annulla", "Annulla produzione/ricerca e restituisci le risorse");return out;}
 if(commandBuild_&&e.kind==K::Worker){for(int i=0;i<int(K::Count);++i){auto k=K(i);if(!alpha::mobile(k)&&!alpha::resource(k)&&k!=K::OilRig&&world_.missionAllows(k))push(2,i,k,alpha::factionUnitName(k,world_.faction[0]),price(alpha::def(k).cost)+" | "+world_.unavailable(k));}push(10,0,K::Worker,"Indietro","Torna ai comandi dell'operaio");return out;}
 if(world_.busy(e)){push(0,0,K::Forge,"Costruisce", "Operaio impegnato fino al completamento del cantiere");return out;}
 if(world_.modern()&&e.form==4){push(0,0,K::Transfiguration,"In trattamento",std::to_string(int(std::ceil(e.transTime)))+" s | Azioni sospese");return out;}
 if(alpha::mobile(e.kind)){
  push(4,1,e.kind,"Muovi","Scegli una destinazione sulla mappa");push(5,0,e.kind,"Ferma","Interrompi gli ordini delle unita selezionate");
  if(alpha::def(e.kind).damage>0)push(4,3,K::Soldier,"Attacca","Scegli un bersaglio nemico");
  if(e.kind==K::Worker){push(4,2,K::Forest,"Raccogli","Scegli un albero o una miniera");push(4,4,K::Forge,"Ripara","Scegli una struttura alleata danneggiata");push(9,0,K::Farm,"Costruisci","Apri gli edifici costruibili dall'operaio");push(8,0,K::Forest,"Pianta albero","10 oro | Richiede Silvicoltura | Crescita: 12 s");}
  if(world_.modern()){
   if(e.kind==K::CrystalMiner)push(4,2,K::CrystalCave,"Estrai cristalli","Scegli una caverna; deposito al Reliquiario");
   if(e.kind==K::Worker)push(21,1,K::Forge,"Riparatore auto","Zona 480; riparazioni pagate; riserva 30 oro/legno");
   if(alpha::def(e.kind).damage>0&&e.kind!=K::Worker){push(21,2,K::GuardTower,"Presidia","Difende la posizione e ritorna al posto");push(23,20,K::Scout,"Pattuglia","Clic sul secondo punto della pattuglia");push(21,4,K::WatchTower,"Difendi campo","Risponde agli allarmi delle torri della zona");}
   if(e.duty)push(21,0,e.kind,"Ruolo manuale","Disattiva il ruolo automatico");
   const char* labels[]={"Runa rapidita","Runa forza","Runa vigore","Runa luce","Runa abissale"};for(int i=0;i<5;++i)if(world_.runes[0][i]>0)push(22,6+i,K::RuneForge,labels[i],"Consuma una runa per unita valida; durata 30 s");
   if(e.kind==K::Binder)push(23,21,K::Binder,"Paralizza","60 mana | 1.5 s rituale | Durata 10 s | Ricarica 45 s");
   if(e.kind==K::RuneBreaker)push(23,22,K::RuneBreaker,"Spezza sigilli","2 s rituale interrompibile | Ricarica 30 s");
   if(alpha::def(e.kind).domain==alpha::Domain::Land&&alpha::def(e.kind).damage>0&&e.kind!=K::Worker){push(22,11,K::Transfiguration,"Trasfigura","Vicino al santuario | 15 cristalli | 25 s + 10 s rischio");if(e.form)push(22,12,K::Transfiguration,"Purifica","Vicino al santuario | 60 oro, 40 legno | 18 s cura");}
  }
  if(e.kind==K::Tanker){push(4,2,K::OilRig,"Petrolio","Scegli una piattaforma completata");push(2,int(K::OilRig),K::OilRig,"Piattaforma",price(alpha::def(K::OilRig).cost)+" | Su giacimento petrolifero");}
  if(e.kind==K::Transport||e.kind==K::AirArk)push(11,0,K::Transport,"Sbarca","Sbarca i passeggeri sulla costa vicina");
  for(int i=0;i<int(alpha::Spell::Count);++i)if(world_.supports(e.kind,alpha::Spell(i)))push(6,i,e.kind,alpha::spellName(alpha::Spell(i)),std::to_string(alpha::spellCost(alpha::Spell(i)))+" mana | Scegli un bersaglio");
 }else{
  for(int i=0;i<int(K::Count);++i){auto k=K(i);if(alpha::mobile(k)&&alpha::def(k).producer==e.kind&&world_.missionAllows(k)&&world_.factionAllows(k)){std::string info=price(alpha::def(k).cost);auto reason=world_.unavailable(k);if(e.level<world_.requiredProducerLevel(k))reason="Richiede questa caserma livello "+std::to_string(world_.requiredProducerLevel(k));if(!reason.empty())info+=" | "+reason;push(1,i,k,alpha::factionUnitName(k,world_.faction[0]),info);}}
  for(int i=0;i<int(alpha::Research::Count);++i){auto r=alpha::Research(i);auto& d=alpha::researchCatalog()[i];if(d.building!=e.kind||!world_.missionResearchAllows(r))continue;if((r==alpha::Research::Town2&&world_.townLevel[0]>=2)||(r==alpha::Research::Town3&&world_.townLevel[0]>=3)||(r==alpha::Research::Town4&&world_.townLevel[0]>=4))continue;
   if(r==alpha::Research::Barracks2&&e.level!=1)continue;if(r==alpha::Research::Barracks3&&e.level!=2)continue;
   bool local=r==alpha::Research::Barracks2||r==alpha::Research::Barracks3;if(!local&&world_.upgrades[0][i]>=d.maxLevel)continue;
   push(3,i,e.kind,d.name,price(d.cost)+" | "+std::to_string(int(d.seconds))+" s | "+world_.researchIssue(e.id,r));
  }
  if(e.kind==K::WatchTower){push(12,int(K::GuardTower),K::GuardTower,"Torre frecce","60 oro, 30 legno | 8 secondi");push(12,int(K::CannonTower),K::CannonTower,"Torre cannone","110 oro, 50 legno | 8 secondi");}
  if(world_.modern()){
   if(e.level<4)push(20,0,e.kind,"Livello "+std::to_string(e.level+1),price(e.kind==K::Hall?alpha::researchCatalog()[int(e.level==1?alpha::Research::Town2:e.level==2?alpha::Research::Town3:alpha::Research::Town4)].cost:alpha::Cost{80*(e.level+1),55*(e.level+1),0,e.level==3?40:0})+" | Richiede Comando e tecnologia adeguati");
   if(e.kind==K::BarrierAltar)push(22,0,e.kind,"Barriera 10 s","20 cristalli | Ricarica condivisa 90 s | Raggio 320");
   if(e.kind==K::RuneForge){const char* labels[]={"Crea rapidita","Crea forza","Crea vigore","Crea luce","Crea abissale"};for(int i=0;i<5;++i)if(i<3||(i==3&&world_.faction[0]==0)||(i==4&&world_.faction[0]==1))push(22,1+i,e.kind,labels[i],"45 oro, 12 cristalli | 8 s | Max 8 per tipo");}
   if(e.kind==K::Transfiguration)push(23,23,e.kind,"Recupera corrotto","Clic sul corrotto: due scorte vicine, struttura entro 360");
  }
  std::string info=alpha::def(e.kind).role;
  if(e.kind==K::LumberMill)info="Deposito legno: +2 per carico. Sblocca Caserma II. Ricerche: tiro e silvicoltura (se disponibili).";
  if(e.kind==K::Forge)info="Compra Armi e Armature con le icone: costruire la fucina da sola non applica le ricerche.";
  push(0,0,e.kind,e.kind==K::Farm?"+6 posti":"Info edificio",info);
 }
 return out;
}
void Gameplay::executeCommand(const Command& c){executeCommandImpl(c);commandNotice_=world_.message;noticeTime_=5;}
void Gameplay::executeCommandImpl(const Command& c){
 if(paused_||world_.result){world_.message="Riprendi la partita per impartire ordini.";return;}
 int id=-1;for(auto& e:world_.entities)if(e.hp>0&&e.team==0&&e.selected){id=e.id;break;}if(id<0)return;
 auto e=world_.get(id);commandPage_=0;
 if(c.type==20){world_.levelUp(id);return;}
 if(c.type==21){for(auto& unit:world_.entities)if(unit.selected&&unit.team==0)world_.setDuty(unit.id,c.item,unit.pos);return;}
 if(c.type==22){std::vector<int> group;for(const auto& unit:world_.entities)if(unit.selected&&unit.team==0)group.push_back(unit.id);for(int unit:group)if(!world_.tactical(unit,c.item,{}))world_.message="Azione non disponibile: controlla risorse, distanza, stato e ricarica.";return;}
 if(c.type==23){cursorOrder_=c.item;world_.message=c.detail;return;}
 if(c.type==0){world_.message=c.detail;return;}
 if(c.type==1){world_.train(id,alpha::Kind(c.item));return;}
 if(c.type==2){auto k=alpha::Kind(c.item);auto reason=world_.unavailable(k);if(!reason.empty()){world_.message=reason;return;}builder_=id;placingKind_=k;placing_=true;planting_=casting_=false;commandBuild_=false;world_.message="Scegli la posizione di "+std::string(alpha::name(k))+"; destro annulla.";return;}
 if(c.type==3){world_.research(id,alpha::Research(c.item));return;}
 if(c.type==4){cursorOrder_=c.item;world_.message=c.detail;return;}
 if(c.type==5){for(auto& unit:world_.entities)if(unit.selected&&unit.team==0&&!world_.busy(unit)&&alpha::mobile(unit.kind)){unit.duty=0;unit.channel=0;unit.order=alpha::Order::Idle;unit.target=unit.objective=unit.source=-1;unit.route.clear();}return;}
 if(c.type==6){pendingSpell_=alpha::Spell(c.item);casting_=true;world_.message=c.detail;return;}
 if(c.type==7){world_.cancel(id);return;}
 if(c.type==8){if(world_.upgrades[0][int(alpha::Research::Forestry)]==0){world_.message="Ricerca Silvicoltura alla segheria.";return;}builder_=id;planting_=true;placing_=casting_=false;world_.message="Scegli dove piantare (10 oro). Destro annulla.";return;}
 if(c.type==9){commandBuild_=true;return;}if(c.type==10){commandBuild_=false;return;}
 if(c.type==11){cursorOrder_=5;world_.message="Clic sulla costa vicino al trasporto per sbarcare. Destro annulla.";return;}if(c.type==12)world_.upgradeTower(id,alpha::Kind(c.item));
}
void Gameplay::drawIcon(sf::RenderTarget& target,alpha::Kind kind,sf::FloatRect rect)const{
 auto tex=textureFor(kind,world_.faction[0]);if(!tex)return;sf::Sprite icon(*tex);bool demon=world_.faction[0]==1&&demonTile(kind)>=0;int tile=alpha::def(kind).atlas;
 if(int(kind)>=int(alpha::Kind::CrystalCave))icon.setTextureRect(v2Rect(kind,world_.faction[0]));
 else if(demon)icon.setTextureRect(demonRects_[int(kind)]);
 else if(alpha::legacyArt(kind)>=0&&alpha::mobile(kind))icon.setTextureRect({0,0,int(tex->getSize().x/4),int(tex->getSize().y/4)});
 else if(tile>=0){int col=tile%4,row=tile/4;int x=col*tex->getSize().x/4,y=row*tex->getSize().y/4;icon.setTextureRect({x,y,int((col+1)*tex->getSize().x/4)-x,int((row+1)*tex->getSize().y/4)-y});}
 auto b=icon.getLocalBounds();float scale=std::min(rect.width/b.width,rect.height/b.height);icon.setScale(scale,scale);icon.setPosition(rect.left+(rect.width-b.width*scale)/2,rect.top+(rect.height-b.height*scale)/2);target.draw(icon);
}
void Gameplay::drawCommands(sf::RenderTarget& target)const{
 auto list=commands();int pages=std::max(1,(int(list.size())+8)/9),page=std::min(commandPage_,pages-1);std::string detail;
 auto box=[&](sf::FloatRect rect,sf::Color color,sf::Color line){sf::RectangleShape r({rect.width,rect.height});r.setPosition(rect.left,rect.top);r.setFillColor(color);r.setOutlineThickness(1);r.setOutlineColor(line);target.draw(r);};
 text(target,commandBuild_?"EDIFICI DELL'OPERAIO":"COMANDI DELLA SELEZIONE",{28,579},18,{222,204,159});
 for(int n=0;n<9;++n){int idx=page*9+n;if(idx>=int(list.size()))break;auto& c=list[idx];sf::FloatRect rect(25+(n%3)*132.f,610+(n/3)*94.f,120,86);bool hover=rect.contains(uiMouse_);bool locked=false;
  const alpha::Entity* selected=nullptr;for(auto& e:world_.entities)if(e.hp>0&&e.selected&&e.team==0){selected=&e;break;}
  if(c.type==1&&selected)locked=!world_.unavailable(alpha::Kind(c.item)).empty()||selected->level<world_.requiredProducerLevel(alpha::Kind(c.item));
  if(c.type==3&&selected)locked=!world_.researchIssue(selected->id,alpha::Research(c.item)).empty();
  if(c.type==2)locked=!world_.unavailable(alpha::Kind(c.item)).empty();if(c.type==8)locked=world_.upgrades[0][int(alpha::Research::Forestry)]==0;
  box(rect,locked?sf::Color(35,41,43):sf::Color(37,62,61),hover?sf::Color(234,205,130):sf::Color(91,119,104));
  drawIcon(target,c.icon,{rect.left+30,rect.top+3,60,56});
  text(target,loc::tr(c.label).substr(0,17),{rect.left+4,rect.top+63},15,locked?sf::Color(142,151,143):sf::Color(239,222,180));
  const char* badge=c.type==4?(c.item==1?">":c.item==2?"L":c.item==3?"!":"R"):c.type==5?"X":c.type==3?"+":c.type==8?"+":c.type==7?"X":"";
  text(target,badge,{rect.left+5,rect.top+3},23,{229,190,104});if(locked)text(target,"!",{rect.left+101,rect.top+3},20,{216,142,108});
  if(hover)detail=c.label+": "+c.detail;
 }
 if(list.empty())text(target,"Seleziona una struttura o un'unita\nper vedere le sue azioni e i costi.",{28,625},19,{161,187,174});
 if(pages>1){box({25,900,390,36},{40,65,62},{92,117,101});text(target,"< Indietro       "+std::to_string(page+1)+"/"+std::to_string(pages)+"       Avanti >",{38,907},19,{229,217,183});}
 if(noticeTime_>0)detail=commandNotice_;
 if(!detail.empty()){box({455,14,1423,48},{28,47,46},{150,135,90});text(target,loc::tr(detail).substr(0,123),{473,26},20,{245,224,171});}
}
