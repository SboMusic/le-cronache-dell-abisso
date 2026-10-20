#pragma once
#include <array>
#include <string>
namespace alpha {
inline constexpr int missionCount=11;
// The first six values preserve the approved alpha artwork and saved test IDs.
enum class Kind { Worker, Soldier, Hall, Barracks, Mine, Forest,
 Archer, Ranger, Knight, Guardian, Siege, Demolition, Mage, Warlock, Healer,
 Scout, Warbird, Tanker, Transport, Destroyer, Battleship, Submarine, Spirit,
 Farm, LumberMill, Forge, Stable, Sanctuary, Academy, Workshop, Aviary,
 WatchTower, GuardTower, CannonTower, Shipyard, Foundry, Refinery, OilRig, Wall, Oil, Count };
enum class Domain { Land, Sea, Air };
enum class Research { Town2, Town3, Weapons, Armor, Archery, ShipGuns, ShipArmor, Elite, Arcana, Barracks2, Barracks3, Forestry, Count };
enum class Spell { Heal, Firestorm, Haste, Rage, Shield, Reveal, Raise, Storm, Count };
struct Cost { int gold=0,wood=0,oil=0; };
struct Definition {
 const char* name="",*clanName="",*role="";
 bool unit=false,node=false,antiAir=false;
 Domain domain=Domain::Land;
 Cost cost;
 float hp=100,damage=0,armor=0,range=0,speed=0,seconds=10,cooldown=1,splash=0,radius=46;
 Kind producer=Kind::Count,prerequisite=Kind::Count;
 int tier=1,supply=1,atlas=-1;
};
inline const std::array<Definition,static_cast<int>(Kind::Count)>& catalog(){
 static const auto data=[](){std::array<Definition,static_cast<int>(Kind::Count)> d{};
  auto unit=[&](Kind k,const char* n,const char* role,Kind producer,Cost price,float hp,float damage,float range,float speed,float seconds,int tier,Kind prerequisite,int atlas,Domain domain=Domain::Land,bool antiAir=false){
   auto& v=d[int(k)];v.name=v.clanName=n;v.role=role;v.unit=true;v.producer=producer;v.cost=price;v.hp=hp;v.damage=damage;v.range=range;v.speed=speed;v.seconds=seconds;v.tier=tier;v.prerequisite=prerequisite;v.atlas=atlas;v.domain=domain;v.antiAir=antiAir;v.radius=domain==Domain::Sea?25.f:15.f;
  };
  auto building=[&](Kind k,const char* n,const char* role,Cost price,float hp,float seconds,int tier,Kind prerequisite,int atlas){auto& v=d[int(k)];v.name=v.clanName=n;v.role=role;v.cost=price;v.hp=hp;v.seconds=seconds;v.tier=tier;v.prerequisite=prerequisite;v.atlas=atlas;};
  unit(Kind::Worker,"Operaio","Raccoglie oro/legno, costruisce e ripara",Kind::Hall,{50,0,0},80,5,40,100,5,1,Kind::Count,-1);
  unit(Kind::Soldier,"Soldato","Fanteria da mischia",Kind::Barracks,{80,0,0},130,19,42,115,7,1,Kind::Count,-1);
  building(Kind::Hall,"Comando","Deposito, operai e sviluppo della citta",{300,200,0},1000,18,1,Kind::Count,-1);
  building(Kind::Barracks,"Caserma","Soldati; potenzia per arcieri e veterani",{120,80,0},500,10,1,Kind::Count,-1);
  for(auto k:{Kind::Mine,Kind::Forest,Kind::Oil}){auto& v=d[int(k)];v.node=true;v.hp=6000.f;v.radius=38;}
  d[int(Kind::Mine)].name=d[int(Kind::Mine)].clanName="Miniera";d[int(Kind::Forest)].name=d[int(Kind::Forest)].clanName="Bosco";d[int(Kind::Oil)].name=d[int(Kind::Oil)].clanName="Giacimento";
  unit(Kind::Archer,"Arciere","Tiro a distanza e difesa antiaerea",Kind::Barracks,{75,25,0},85,14,290,108,8,1,Kind::LumberMill,0,Domain::Land,true);
  unit(Kind::Ranger,"Esploratore veterano","Tiratore rapido, ampia gittata",Kind::Barracks,{105,35,0},105,20,350,120,10,2,Kind::LumberMill,1,Domain::Land,true);
  unit(Kind::Knight,"Cavaliere","Mischia veloce e corazzata",Kind::Stable,{150,35,0},240,28,48,160,12,2,Kind::Barracks,2);
  unit(Kind::Guardian,"Custode","Cavaliere sacro, cura e protezione",Kind::Sanctuary,{190,60,0},290,32,48,150,14,3,Kind::Stable,3);
  unit(Kind::Siege,"Balista","Assedio a lunga gittata e danni ad area",Kind::Workshop,{180,100,0},160,65,420,65,16,2,Kind::Forge,4);
  unit(Kind::Demolition,"Guastatore","Esplosione ravvicinata, monouso",Kind::Workshop,{100,65,0},85,180,42,125,10,2,Kind::Forge,5);
  unit(Kind::Mage,"Arcanista","Magie offensive e controllo",Kind::Academy,{190,60,0},100,20,260,100,14,3,Kind::Count,6,Domain::Land,true);
  unit(Kind::Warlock,"Evocatore","Spiriti, tempesta e furia",Kind::Academy,{190,60,0},100,20,260,100,14,3,Kind::Count,7,Domain::Land,true);
  unit(Kind::Healer,"Guaritore","Cura automatica e magia di supporto",Kind::Sanctuary,{120,50,0},95,0,0,105,11,2,Kind::Count,8);
  unit(Kind::Scout,"Aeronave da ricognizione","Volo e rilevamento dei sommergibili",Kind::Workshop,{110,60,0},130,0,0,160,12,2,Kind::Count,9,Domain::Air);
  unit(Kind::Warbird,"Roc da guerra","Attacco volante ad area, anche antiaereo",Kind::Aviary,{250,100,0},280,35,240,140,20,3,Kind::Count,10,Domain::Air,true);
  unit(Kind::Tanker,"Nave cisterna","Costruisce piattaforme e trasporta petrolio",Kind::Shipyard,{100,40,0},160,0,0,115,10,1,Kind::Count,11,Domain::Sea);
  unit(Kind::Transport,"Trasporto","Imbarca fino a sei unita terrestri",Kind::Shipyard,{130,70,20},250,0,0,120,12,1,Kind::Count,12,Domain::Sea);
  unit(Kind::Destroyer,"Fregata","Scorta navale, antiaerea e rilevamento",Kind::Shipyard,{170,80,40},280,24,320,115,14,2,Kind::Count,13,Domain::Sea,true);
  unit(Kind::Battleship,"Corazzata","Artiglieria navale e bombardamento costiero",Kind::Shipyard,{260,150,100},480,60,430,75,20,2,Kind::Foundry,14,Domain::Sea);
  unit(Kind::Submarine,"Sommergibile","Siluri navali, occultato senza rilevatori",Kind::Shipyard,{190,80,70},180,45,300,100,16,2,Kind::Workshop,15,Domain::Sea);
  unit(Kind::Spirit,"Spirito evocato","Servitore temporaneo richiamato dai caduti",Kind::Count,{0,0,0},65,13,40,130,0,3,Kind::Count,-1);d[int(Kind::Spirit)].supply=0;
  building(Kind::Farm,"Fattoria","Aggiunge sei posti di popolazione",{70,40,0},350,8,1,Kind::Count,0);
  building(Kind::LumberMill,"Segheria","Deposito legno e sblocco tiratori",{100,80,0},450,10,1,Kind::Count,1);
  building(Kind::Forge,"Fucina","Ricerca armi e armature",{140,90,0},550,12,1,Kind::Barracks,2);
  building(Kind::Stable,"Scuderia","Addestra la cavalleria",{170,100,0},550,13,2,Kind::Barracks,3);
  building(Kind::Sanctuary,"Santuario","Guaritori, custodi e dottrina veterana",{180,110,0},550,14,2,Kind::Forge,4);
  building(Kind::Academy,"Accademia arcana","Incantatori e ricerca magica",{220,140,0},500,16,3,Kind::Sanctuary,5);
  building(Kind::Workshop,"Officina","Assedio, guastatori e ricognizione",{180,120,0},550,14,2,Kind::Forge,6);
  building(Kind::Aviary,"Voliera","Addestra i roc da guerra",{220,160,0},500,16,3,Kind::Stable,7);
  building(Kind::WatchTower,"Torre di vedetta","Rilevamento; evoluzione in torre armata",{80,60,0},300,8,1,Kind::Count,8);
  building(Kind::GuardTower,"Torre di guardia","Difesa a distanza e antiaerea",{140,90,0},450,12,1,Kind::LumberMill,9);
  building(Kind::CannonTower,"Torre di artiglieria","Difesa ad area contro terra e navi",{190,110,0},550,14,2,Kind::Forge,10);
  building(Kind::Shipyard,"Cantiere navale","Si costruisce sulla costa; produce navi",{160,120,0},650,14,1,Kind::LumberMill,11);
  building(Kind::Foundry,"Fonderia navale","Sblocca corazzate e migliora la flotta",{170,120,30},550,14,2,Kind::Shipyard,12);
  building(Kind::Refinery,"Raffineria","Aumenta il petrolio per consegna",{140,100,0},500,12,1,Kind::Shipyard,13);
  building(Kind::OilRig,"Piattaforma petrolifera","Cisterna: costruisci su un giacimento in mare",{120,70,0},500,12,1,Kind::Shipyard,14);d[int(Kind::OilRig)].domain=Domain::Sea;
  building(Kind::Wall,"Palizzata","Blocca i percorsi terrestri",{10,15,0},280,3,1,Kind::Count,15);d[int(Kind::Wall)].radius=20;
  d[int(Kind::GuardTower)].damage=23;d[int(Kind::GuardTower)].range=360;d[int(Kind::GuardTower)].antiAir=true;
  d[int(Kind::CannonTower)].damage=48;d[int(Kind::CannonTower)].range=400;d[int(Kind::CannonTower)].splash=85;d[int(Kind::CannonTower)].cooldown=2;
  for(auto k:{Kind::Siege,Kind::Battleship}){d[int(k)].splash=75;d[int(k)].cooldown=2.3f;d[int(k)].supply=2;}
  d[int(Kind::Warbird)].splash=45;d[int(Kind::Warbird)].supply=2;
  for(auto k:{Kind::Knight,Kind::Guardian})d[int(k)].armor=4;
  d[int(Kind::Battleship)].armor=6;d[int(Kind::Hall)].armor=3;
  d[int(Kind::Worker)].clanName="Servo d ombra";d[int(Kind::Soldier)].clanName="Ghoul cornuto";d[int(Kind::Archer)].clanName="Spina oscura";d[int(Kind::Ranger)].clanName="Predatore antico";
  d[int(Kind::Knight)].clanName="Cavaliere delle steppe";d[int(Kind::Guardian)].clanName="Campione degli spiriti";d[int(Kind::Siege)].clanName="Lanciapietre";d[int(Kind::Scout)].clanName="Dirigibile";d[int(Kind::Warbird)].clanName="Viverna delle nebbie";
  const std::pair<Kind,const char*> darkNames[]={
   {Kind::Worker,"Imp lavoratore"},{Kind::Soldier,"Diavolo guerriero"},{Kind::Archer,"Arciere infernale"},{Kind::Ranger,"Predatore abissale"},{Kind::Knight,"Cavaliere infernale"},{Kind::Guardian,"Custode dell abisso"},{Kind::Siege,"Balista demoniaca"},{Kind::Demolition,"Imp dinamitardo"},{Kind::Mage,"Piromante infernale"},{Kind::Warlock,"Evocatore dell abisso"},{Kind::Healer,"Sacerdote oscuro"},{Kind::Scout,"Occhio alato"},{Kind::Warbird,"Viverna infernale"},{Kind::Tanker,"Cisterna infernale"},{Kind::Transport,"Arca dei dannati"},{Kind::Destroyer,"Fregata demoniaca"},{Kind::Battleship,"Corazzata infernale"},{Kind::Submarine,"Predatore sommerso"},{Kind::Spirit,"Spettro abissale"},
   {Kind::Hall,"Cittadella infernale"},{Kind::Barracks,"Caserma delle fauci"},{Kind::Farm,"Nido demoniaco"},{Kind::LumberMill,"Segheria infernale"},{Kind::Forge,"Forgia delle braci"},{Kind::Stable,"Serraglio infernale"},{Kind::Sanctuary,"Altare dell abisso"},{Kind::Academy,"Torre degli evocatori"},{Kind::Workshop,"Officina dei tormenti"},{Kind::Aviary,"Nido delle viverne"},{Kind::WatchTower,"Torre del teschio"},{Kind::GuardTower,"Torre delle spine"},{Kind::CannonTower,"Torre del fuoco"},{Kind::Shipyard,"Cantiere dei dannati"},{Kind::Foundry,"Fonderia abissale"},{Kind::Refinery,"Raffineria infernale"},{Kind::OilRig,"Trivella infernale"},{Kind::Wall,"Muro delle corna"}};
  for(const auto& item:darkNames)d[int(item.first)].clanName=item.second;
  d[int(Kind::Forest)].name=d[int(Kind::Forest)].clanName="Albero";d[int(Kind::Forest)].hp=80;d[int(Kind::Forest)].radius=16;
  return d;}();return data;
}
inline const Definition& def(Kind k){return catalog().at(static_cast<int>(k));}
inline bool mobile(Kind k){return k!=Kind::Count&&def(k).unit;}
inline bool resource(Kind k){return k!=Kind::Count&&def(k).node;}
inline float radius(Kind k){return def(k).radius;}
inline const char* name(Kind k){return k==Kind::Count?"Nessuno":def(k).name;}
inline const char* factionName(int f){return f==0?"Lega del Confine":"Stirpe dell Ombra";}
inline const char* factionUnitName(Kind k,int f){return f==0?def(k).name:def(k).clanName;}
inline int legacyArt(Kind k){if(int(k)<6)return int(k);if(resource(k))return -1;if(mobile(k))return k==Kind::Spirit?1:-1;return -1;}
struct ResearchDef {const char* name;Kind building;Cost cost;float seconds;int tier,maxLevel;};
inline const std::array<ResearchDef,int(Research::Count)>& researchCatalog(){static const std::array<ResearchDef,int(Research::Count)> v{{
 {"Comando II",Kind::Hall,{180,120,0},18,1,1},{"Comando III",Kind::Hall,{300,200,0},22,2,1},
 {"Armi",Kind::Forge,{120,80,0},12,1,2},{"Armature",Kind::Forge,{120,80,0},12,1,2},
 {"Tiro di precisione",Kind::LumberMill,{110,90,0},12,1,2},{"Cannoni navali",Kind::Foundry,{140,80,60},14,2,2},
 {"Corazze navali",Kind::Foundry,{130,100,40},14,2,2},{"Dottrina veterana",Kind::Sanctuary,{180,120,0},16,2,1},
 {"Sapienza arcana",Kind::Academy,{160,100,0},16,3,2},
 {"Caserma II",Kind::Barracks,{120,80,0},12,1,1},{"Caserma III",Kind::Barracks,{180,120,0},16,2,1},
 {"Silvicoltura",Kind::LumberMill,{140,80,0},18,2,1}}};return v;}
inline const char* spellName(Spell s){static const char* names[]={"Cura","Pioggia di fuoco","Celerita","Furia","Scudo","Rivelazione","Richiamo degli spiriti","Vortice"};return names[int(s)];}
inline int spellCost(Spell s){static const int costs[]={25,60,30,30,35,20,55,75};return costs[int(s)];}
}

