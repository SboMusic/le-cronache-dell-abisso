#pragma once
namespace alpha {
inline const char* missionTitle(int n){static const char* names[]={"1. Un posto da chiamare casa","2. La strada di Velmora","3. I custodi delle carte","4. Il prezzo dell obbedienza","5. Le citta libere","6. Le parole dei Cinque","7. Le officine del confine","8. La rotta delle profondita","9. La memoria della Pietra","10. Prima dell ultima alba","Finale. Il cuore del mondo"};return n>=0&&n<missionCount?names[n]:"Partita libera";}
inline const char* missionBriefing(int n){static const char* lines[]={
 "Liora Venn - Valle dei Mulini\nLa Lega deve riaprire un insediamento evacuato.\nCostruisci due fattorie, consegna 120 oro e 80 legno,\npoi raggiungi il villaggio lungo il sentiero.\nLavoratori rivali in un campo separato: scontro facoltativo.\nDisponibili: operai, comando e fattorie.\nSconfitta: perdi il comando.",
 "Liora Venn - Passo delle Sentinelle\nLa strada dei convogli attraversa un passo roccioso.\nCompleta una caserma e porta i soldati a tre.\nElimina le tre sentinelle e raggiungi la fine della strada.\nNon ci sono ondate o assalti automatici alla base.\nNuovi elementi: soldati e combattimento di gruppo.\nSconfitta: perdi il comando.",
 "Liora Venn - Bosco dei Custodi\nSeren Dhal attende nell archivio, oltre il bosco.\nCompleta una segheria, potenzia una caserma a II\ne addestra due arcieri. Libera e presidia l archivio\nper 10 secondi, poi scorta Seren al tuo comando.\nIl nemico raccoglie e paga le proprie truppe.\nSconfitta: perdi il comando oppure Seren dopo il recupero.",
 "Liora Venn - Gola delle Braci\nSeren ha trovato ordini contro i profughi dell Abisso.\nLiora sceglie di proteggere il loro rifugio.\nSviluppa Comando II, scuderia, due cavalieri\ne una torre di guardia. Poi presidia il passo per 90 s.\nInfine distruggi la caserma che minaccia il rifugio.\nSconfitta: perdi il comando o il rifugio.\nAssalti annunciati solo dopo la preparazione.",
 "Liora Venn - Costa delle Citta Libere\nLe comunita costiere custodiscono altre testimonianze.\nCostruisci un cantiere vicino alla costa meridionale.\nCrea una cisterna, costruisci una piattaforma sul petrolio\ne accumula 60 petrolio. Crea fregata e trasporto.\nImbarca truppe con clic destro sul trasporto vicino riva.\nSbarca sull isola a sud-est e libera il tempio.\nPresidialo con almeno tre combattenti per 20 secondi.\nSconfitta: perdi il comando.",
 "Liora Venn - Altipiano delle Testimonianze\nSeren confronta gli archivi delle due stirpi.\nCostruisci il santuario e addestra un guaritore.\nRicerca Dottrina veterana, Caserma III e due veterani.\nLibera i tre archivi e presidiali per 8 secondi ciascuno.\nPoi difendi l archivio centrale per 30 secondi.\nLa guerra cresce: i nemici producono pattuglie miste.\nSconfitta: perdi tutti i comandi.",
 "Liora Venn - Valle delle Fornaci\nLe officine riforniscono i reparti dei rastrellamenti.\nCostruisci un officina e addestra due baliste.\nDistruggi i tre depositi segnalati, poi il comando nemico.\nGli avversari impiegano cavalleria e macchine d assedio.\nColpisci fattorie e lavoratori per ridurre i rinforzi.\nSconfitta: perdi tutti i comandi.",
 "Liora Venn - Arcipelago delle Chiuse\nLe comunita costiere chiedono una rotta sicura.\nSviluppa la fonderia navale e crea corazzata e trasporto.\nPetrolio: cisterna > piattaforma > raccolta.\nSbarca sull isola e presidia il faro per 20 secondi\ncon almeno tre combattenti. Poi distruggi il porto nemico.\nLe fregate rilevano i sommergibili avversari.\nSconfitta: perdi tutti i comandi.",
 "Liora Venn - Terrazze della Memoria\nI frammenti spiegano come preservare la Pietra.\nSviluppa Comando III, accademia e Sapienza arcana.\nAddestra due arcanisti, poi recupera i tre sigilli.\nPresidia ogni sigillo per 10 secondi senza nemici.\nInfine difendi il cerchio centrale per 45 secondi.\nGli evocatori avversari usano magia: cura e disperdi i gruppi.\nSconfitta: perdi tutti i comandi.",
 "Liora Venn - Il Fronte dell Alba\nUnisci le guarnigioni prima della guerra decisiva.\nCostruisci un secondo comando e una voliera.\nAddestra due roc da guerra e recupera i due presidi.\nPoi distruggi le due officine militari segnalate.\nLe viverne nemiche richiedono arcieri e difese antiaeree.\nSconfitta: perdi tutti i comandi.",
 "Liora Venn - Il Cuore del Mondo\nLa Pietra e custodita nella fortezza avversaria.\nPrepara economia, fattorie e un esercito di almeno 20\ncombattenti, con officina e accademia.\nTre campi nemici, veterani, assedio, magie e viverne.\nGli assalti partono dopo la preparazione e almeno 7 minuti.\nVittoria: distruggi ogni struttura e ogni unita nemica.\nLa Pietra deve restare intatta. Nessun rinforzo gratuito.\nSconfitta: perdi tutti i comandi."};return n>=0&&n<missionCount?lines[n]:"";}
inline std::vector<sf::Vector2f> missionRoad(int n){if(n==5)return {{420,1260},{1150,1100},{1500,500},{2300,1000},{1600,1840},{3100,500}};if(n==6)return {{420,1660},{1300,1500},{1680,900},{2300,1550},{2700,800},{3180,470}};if(n==7)return {{420,920},{1200,1000},{1750,760},{3020,550}};if(n==8)return {{430,1780},{1300,1620},{1500,680},{2350,1200},{2700,1850},{3220,500}};if(n==9)return {{440,1660},{1450,740},{2100,1720},{2900,1300},{3300,500}};if(n==10)return {{440,1720},{1250,1720},{1900,1720},{2500,1500},{3350,1250},{3100,500}};if(n==3)return {{350,1050},{800,1000},{1100,800},{1420,800},{1700,620},{2200,500}};if(n==4)return {{350,920},{800,920},{1050,760},{1500,760},{2200,550}};return n==0?std::vector<sf::Vector2f>{{360,1050},{680,1000},{1000,780}}:n==1?std::vector<sf::Vector2f>{{350,1280},{800,1200},{1140,940},{1500,720},{1840,500},{2130,300}}:std::vector<sf::Vector2f>{{350,1220},{850,1190},{1000,860},{1240,420}};}
inline sf::Vector2f World::missionTarget()const{if(layoutVersion){
 if(mission>=5)return warTarget();if(mission==1)return mapGoal;
 if(mission==2){if(missionStage>=2){for(const auto& e:entities)if(e.team==0&&e.kind==Kind::Hall&&e.hp>0)return e.pos;}for(const auto& e:entities)if(e.team<0&&e.kind==Kind::Sanctuary&&e.hp>0)return e.pos;}
 if(mission==3){if(missionStage==2&&!missionGuards.empty()){if(auto e=get(missionGuards[0]))return e->pos;}if(auto e=get(archivist))return e->pos+sf::Vector2f(220,0);}
 if(mission==4){if(missionStage==0)for(const auto& e:entities)if(e.kind==Kind::Oil)return e.pos;for(const auto& e:entities)if(e.team<0&&e.kind==Kind::Sanctuary)return e.pos;}
 }if(mission==0)for(const auto& e:entities)if(e.hp>0&&e.team<0&&e.kind==Kind::Farm)return e.pos+sf::Vector2f(0,120);if(mission>=5)return warTarget();if(mission==3)return missionStage==2?sf::Vector2f(1990,510):sf::Vector2f(1350,800);if(mission==4)return missionStage==0?sf::Vector2f(900,1320):sf::Vector2f(2020,1480);return mission==0?sf::Vector2f(1000,900):mission==1?sf::Vector2f(2130,300):missionStage>=2?sf::Vector2f(350,1220):sf::Vector2f(1240,420);}
inline bool World::missionAllows(Kind k)const{
 if(cheatUnlock||mission<0)return true;
 if(mission>=5&&mission!=7&&mission!=10&&(def(k).domain==Domain::Sea||k==Kind::Shipyard||k==Kind::Foundry||k==Kind::Refinery))return false;
 if(mission>=9)return true;
 if(mission>=8&&k!=Kind::Warbird&&k!=Kind::Aviary)return true;
 if(mission>=7&&(k==Kind::Foundry||k==Kind::Battleship||k==Kind::Submarine))return true;
 if(mission>=6&&(k==Kind::Workshop||k==Kind::Siege||k==Kind::Demolition||k==Kind::Scout))return true;
 if(mission>=5&&k==Kind::Ranger)return true;
 if(k==Kind::Worker||k==Kind::Farm)return true;
 if(mission>=1&&(k==Kind::Soldier||k==Kind::Barracks||k==Kind::Wall))return true;
 if(mission>=3&&(k==Kind::Hall||k==Kind::Knight||k==Kind::Stable||k==Kind::CannonTower))return true;
 if(mission>=4&&(k==Kind::Shipyard||k==Kind::Tanker||k==Kind::OilRig||k==Kind::Transport||k==Kind::Destroyer||k==Kind::Refinery||k==Kind::Sanctuary||k==Kind::Healer))return true;
 return mission>=2&&(k==Kind::Archer||k==Kind::LumberMill||k==Kind::Forge||k==Kind::WatchTower||k==Kind::GuardTower);
}
inline bool World::missionResearchAllows(Research r)const{return cheatUnlock||mission<0||(mission>=8&&(r==Research::Town3||r==Research::Arcana))||(mission>=7&&(r==Research::ShipGuns||r==Research::ShipArmor))||(mission>=5&&(r==Research::Elite||r==Research::Barracks3))||(mission>=3&&(r==Research::Town2||r==Research::Forestry))||(mission>=2&&(r==Research::Barracks2||r==Research::Weapons||r==Research::Armor||r==Research::Archery));}
inline void World::startHumanMission(int n){
 if(n<0||n>=missionCount)throw std::runtime_error("Missione non disponibile");if(n>=5){startWarMission(n);return;}if(n>=3){startLaterMission(n);return;}reset(0);mission=n;guided=false;chapter=0;missionStage=0;deliveredGold=deliveredWood=0;archivist=-1;secureTime=0;missionGuards.clear();entities.clear();shots.clear();effects.clear();zones.clear();nextId=1;std::fill(terrain.begin(),terrain.end(),Terrain::Grass);clearVision();navDirty=true;raidsEnabled=n==2;enemyGold=160;enemyWood=100;enemyOil=0;gold=n==0?260:n==1?300:400;wood=n==0?160:n==1?180:240;oil=0;
 const sf::Vector2f base=n==0?sf::Vector2f(360,1050):n==1?sf::Vector2f(350,1280):sf::Vector2f(350,1220);
 add(Kind::Hall,0,base);for(int i=0;i<4;++i)add(Kind::Worker,0,base+sf::Vector2f(130.f,float(i*36-70)));
 if(n>0){add(Kind::Farm,0,base+sf::Vector2f(-130,-140));for(int i=0;i<(n==1?2:3);++i)add(Kind::Soldier,0,base+sf::Vector2f(210.f,float(i*42-20)));}
 if(n==2)add(Kind::Barracks,0,{650,1220});
 add(Kind::Mine,-1,n==0?sf::Vector2f(600,1060):n==1?sf::Vector2f(560,1470):sf::Vector2f(600,1450));
 // Each map has its own road geometry. Terrain is also used by pathfinding and saving.
 auto road=missionRoad(n);
 auto segmentDistance=[](sf::Vector2f p,sf::Vector2f a,sf::Vector2f b){auto d=b-a;float length=d.x*d.x+d.y*d.y;float t=length>0?std::clamp(((p.x-a.x)*d.x+(p.y-a.y)*d.y)/length,0.f,1.f):0;return distance(p,a+d*t);};
 for(int y=0;y<rows;++y)for(int x=0;x<cols;++x){sf::Vector2f p{(x+.5f)*cell,(y+.5f)*cell};bool onRoad=false;for(int i=1;i<int(road.size());++i)if(segmentDistance(p,road[i-1],road[i])<90)onRoad=true;
  bool rock=n==1&&((x>=22&&x<=25&&y<20)||(x>=36&&x<=39&&y>24)||(x>=47&&x<=51&&y>19));
  if(n==2)rock=(x>=43&&x<=46&&y>=22&&y<=31)||(x>=13&&x<=17&&y<=8);
  terrain[x+y*cols]=onRoad?Terrain::Road:rock?Terrain::Rock:Terrain::Grass;
 }
 if(n==0){add(Kind::Mine,-1,{1730,550});add(Kind::Farm,-1,{1000,780});}
 if(n==1){for(auto p:{sf::Vector2f(1090,1010),sf::Vector2f(1510,770),sf::Vector2f(1830,530)})missionGuards.push_back(add(Kind::Soldier,1,p));add(Kind::Mine,-1,{1800,1250});}
 if(n==2){add(Kind::Sanctuary,-1,{1240,420});add(Kind::Hall,1,{2200,430});add(Kind::Farm,1,{2370,640});add(Kind::Barracks,1,{1970,430});add(Kind::Mine,-1,{2210,830});for(int i=0;i<4;++i)add(Kind::Worker,1,{2340.f,330.f+i*37});for(auto p:{sf::Vector2f(1130,510),sf::Vector2f(1370,520),sf::Vector2f(2010,580)})missionGuards.push_back(add(Kind::Soldier,1,p));}
 // Forest patterns and clear corridors differ; starting buildings, mines and objectives remain usable.
 for(int y=2;y<38;++y)for(int x=2;x<62;++x){unsigned h=unsigned(x*73856093)^unsigned(y*19349663)^unsigned(n*83492791);sf::Vector2f p{x*40.f+float(h%13),y*40.f+float((h>>7)%11)};bool grove=false;
  if(n==0)grove=(p.y<550&&p.x<1700)||(p.x>1300&&p.y>950)||(p.x<220&&p.y>450);
  if(n==1)grove=(p.x<460&&p.y<800)||(p.x>1750&&p.y>1120)||(p.x>1000&&p.x<1330&&p.y<360);
  if(n==2)grove=(p.x>740&&p.x<1020)||(p.x>1450&&p.x<1770)||(p.y<260)||(p.y>900&&p.x>1850);
  if(!grove||terrain[int(p.x/40)+int(p.y/40)*cols]!=Terrain::Grass||h%4==0)continue;bool free=true;
  for(int i=1;i<int(road.size());++i)if(segmentDistance(p,road[i-1],road[i])<150)free=false;
  if(n==2&&(distance(p,{1240,420})<290||distance(p,{2200,430})<320))free=false;
  for(const auto& e:entities)if(e.hp>0&&distance(p,e.pos)<radius(e.kind)+(e.kind==Kind::Forest?15.f:135.f)){free=false;break;}
  if(free)add(Kind::Forest,-1,p);
 }
 // Local harvestable groves, separate from the distant forest masses.
 for(int i=0;i<14;++i){sf::Vector2f p=base+sf::Vector2f(-180.f+(i%7)*43,(n==0?-270.f:-330.f)+(i/7)*43);bool free=true;for(const auto& e:entities)if(distance(p,e.pos)<radius(e.kind)+24)free=false;if(free)add(Kind::Forest,-1,p);}
 if(n==2)for(int i=0;i<12;++i)add(Kind::Forest,-1,{2050.f+(i%6)*43,1040.f+(i/6)*43});
 refreshVision();message=missionObjective();
}
inline std::string World::missionObjective()const{if(mission>=5){auto text=warObjective();if(mission<10&&missionStage>=1&&time<wave&&wave-time<90)text+=" | Assalto tra "+std::to_string(int(std::ceil(wave-time)))+" s.";return text;}
 if(mission==0){if(missionStage==0){int count=0;for(auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Farm&&e.build<=0)++count;return "Liora: costruisci due fattorie ("+std::to_string(count)+"/2). Operaio > Costruisci.";}if(missionStage==1)return "Consegna oro "+std::to_string(std::min(120,deliveredGold))+"/120 e legno "+std::to_string(std::min(80,deliveredWood))+"/80. Destro sulle risorse.";return "Raggiungi il villaggio segnalato con un operaio. Puoi evitare il campo dei lavoratori rivali.";}
 if(mission==1){if(missionStage==0){int n=0;for(auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Soldier)++n;return std::string("Caserma: ")+(has(0,Kind::Barracks)?"OK":"da costruire")+" | Soldati "+std::to_string(n)+"/3. Addestra dalla caserma.";}int alive=0;for(int id:missionGuards)if(get(id))++alive;if(missionStage==1)return "Apri la strada: elimina le tre sentinelle ("+std::to_string(3-alive)+"/3). Esplora con le truppe.";return "Porta una pattuglia al segnale alla fine della strada, verso nord-est.";}
 if(mission==2){if(missionStage==0){int n=0;bool upgraded=false;for(auto& e:entities)if(e.hp>0&&e.team==0){if(e.kind==Kind::Archer)++n;if(e.kind==Kind::Barracks&&e.level>=2)upgraded=true;}return std::string("Scorta - Segheria: ")+(has(0,Kind::LumberMill)?"OK":"da costruire")+" | Caserma II: "+(upgraded?"OK":"da ricercare")+" | Arcieri "+std::to_string(n)+"/2.";}if(missionStage==1)return "Libera l archivio segnalato e presidialo per 10 s ("+std::to_string(int(secureTime))+"/10).";return "Scorta Seren Dhal al comando. Segue la strada: proteggila dalle pattuglie.";}
 if(mission==3){if(missionStage==0)return "Prepara: Comando II, scuderia, 2 cavalieri e torre di guardia. Nessun assalto prima dei preparativi.";if(missionStage==1&&time<wave)return "Assalto tra "+std::to_string(int(std::ceil(wave-time)))+" s! Porta l esercito al passo segnalato. Proteggi il rifugio.";if(missionStage==1)return "Difendi il passo segnalato: "+std::to_string(std::min(90,int(secureTime)))+"/90 s sicuri. Proteggi il rifugio!";return "Il rifugio resiste. Distruggi la caserma nemica segnalata a nord-est.";}
 if(mission==4){if(missionStage==0)return "Costa: cantiere > cisterna > piattaforma sul petrolio. Accumula 60 petrolio ("+std::to_string(std::min(60,oil))+"/60).";if(missionStage==1)return "Crea fregata (40 petrolio) e trasporto (20). La fregata scorta lo sbarco sull isola a sud-est.";return "Scorta la nave con la fregata. Presidia il tempio con 3 combattenti: "+std::to_string(std::min(20,int(secureTime)))+"/20 s. Navi escluse.";}
 return objective();
}
inline void World::updateMission(float dt){
 bool hall=false;sf::Vector2f base;for(const auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Hall){hall=true;base=e.pos;break;}if(!hall){result=-1;message="SCONFITTA: il comando e stato distrutto.";return;}
 auto count=[&](Kind k){int n=0;for(const auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==k&&e.build<=0)++n;return n;};
 auto reached=[&](sf::Vector2f p,float reach){for(const auto& e:entities)if(e.hp>0&&e.team==0&&mobile(e.kind)&&e.carrier<0&&distance(e.pos,p)<reach)return true;return false;};int old=missionStage;
 if(mission==0){if(missionStage==0&&count(Kind::Farm)>=2)missionStage=1;else if(missionStage==1&&deliveredGold>=120&&deliveredWood>=80)missionStage=2;else if(missionStage==2&&reached(missionTarget(),130))result=1;}
 if(mission==1){if(missionStage==0&&has(0,Kind::Barracks)&&count(Kind::Soldier)>=3)missionStage=1;else if(missionStage==1){bool alive=false;for(int id:missionGuards)if(get(id))alive=true;if(!alive)missionStage=2;}else if(missionStage==2&&reached(missionTarget(),130))result=1;}
 if(mission==2){if(missionStage==0){bool upgraded=false;for(const auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Barracks&&e.level>=2)upgraded=true;if(has(0,Kind::LumberMill)&&upgraded&&count(Kind::Archer)>=2)missionStage=1;}
  else if(missionStage==1){sf::Vector2f archive{1240,420};if(layoutVersion)for(const auto& e:entities)if(e.team<0&&e.kind==Kind::Sanctuary)archive=e.pos;bool enemy=false;for(const auto& e:entities)if(e.hp>0&&e.team==1&&distance(e.pos,archive)<240)enemy=true;secureTime=!enemy&&reached(archive,150)?secureTime+dt:0;if(secureTime>=10){missionStage=2;archivist=add(Kind::Healer,0,archive+sf::Vector2f(0,160));auto e=get(archivist);e->order=Order::Move;path(*e,base,165);}}
  else{auto person=get(archivist);if(!person){result=-1;message="SCONFITTA: Seren Dhal e caduta durante la scorta.";return;}if(distance(person->pos,base)<190)result=1;else if(person->route.empty()){person->order=Order::Move;path(*person,base,165);}}
 }
 if(mission>=5)updateWarMission(dt);else if(mission>=3)updateLaterMission(dt);
 if(old!=missionStage)message="Obiettivo completato. "+missionObjective();if(result>0)message="MISSIONE COMPLETATA: "+std::string(missionTitle(mission));
}
inline void World::startLegacyMission(int n,int playerFaction){
 startHumanMission(n);prepareLegacyRegion();faction[0]=playerFaction==1?1:0;faction[1]=1-faction[0];
 for(auto& e:entities)if(e.team>=0){if(e.kind==Kind::Mage||e.kind==Kind::Warlock)e.kind=faction[e.team]==0?Kind::Mage:Kind::Warlock;if(e.training==Kind::Mage||e.training==Kind::Warlock)e.training=faction[e.team]==0?Kind::Mage:Kind::Warlock;}
 refreshVision();message=missionObjective();
}

inline void World::startMission(int n,int playerFaction){
 startHumanMission(n);prepareCampaignRegion();faction[0]=playerFaction==1?1:0;faction[1]=1-faction[0];
 for(auto& e:entities)if(e.team>=0){if(e.kind==Kind::Mage||e.kind==Kind::Warlock)e.kind=faction[e.team]==0?Kind::Mage:Kind::Warlock;if(e.training==Kind::Mage||e.training==Kind::Warlock)e.training=faction[e.team]==0?Kind::Mage:Kind::Warlock;}
 refreshVision();message=missionObjective();
}

}
