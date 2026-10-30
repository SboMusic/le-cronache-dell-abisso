#pragma once
#include <cctype>
namespace alpha {
// This offline console deliberately shares the normal world, saves and victory flow.
inline bool World::cheat(const std::string& input){
 std::istringstream in(input);std::string cmd,arg,extra;in>>cmd>>arg>>extra;
 for(char& c:cmd)c=char(std::tolower(static_cast<unsigned char>(c)));
 for(char& c:arg)c=char(std::tolower(static_cast<unsigned char>(c)));
 auto fail=[&](const char* s){message=s;return false;};
 if(cmd.empty()||!extra.empty())return fail("Comando non valido.");
 auto toggle=[&](bool& value){if(arg.empty())value=!value;else if(arg=="on"||arg=="1")value=true;else if(arg=="off"||arg=="0")value=false;else return fail("Usa on oppure off.");cheatsUsed=true;message=std::string("Trucco: ")+cmd+(value?" ON":" OFF");return true;};
 if(cmd=="gold")cmd="oro";if(cmd=="wood")cmd="legno";if(cmd=="oil"||cmd=="petrolio")cmd="olio";if(cmd=="resources"||cmd=="ricco")cmd="risorse";
 if(cmd=="crystals")cmd="cristalli";
 if(cmd=="cristalli"||cmd=="oro"||cmd=="legno"||cmd=="olio"||cmd=="risorse"){
  int amount=10000;if(!arg.empty()){if(!std::all_of(arg.begin(),arg.end(),[](char c){return c>='0'&&c<='9';})||arg.size()>7)return fail("Quantita valida: da 1 a 1000000.");amount=std::stoi(arg);if(amount<1||amount>1000000)return fail("Quantita valida: da 1 a 1000000.");}
  if(cmd=="oro"||cmd=="risorse")gold=std::min(9999999,gold+amount);if(cmd=="legno"||cmd=="risorse")wood=std::min(9999999,wood+amount);if(cmd=="olio"||cmd=="risorse")oil=std::min(9999999,oil+amount);if(modern()&&(cmd=="cristalli"||cmd=="risorse"))crystals=std::min(9999999,crystals+amount);cheatsUsed=true;message="Risorse aggiunte: "+std::to_string(amount);return true;
 }
 if(cmd=="invincibile"||cmd=="god"||cmd=="dio")return toggle(cheatGod);
 if(cmd=="veloce"||cmd=="fast")return toggle(cheatFast);
 if(cmd=="blocca"||cmd=="freeze")return toggle(cheatFreeze);
 if(cmd=="gratis"||cmd=="free")return toggle(cheatFree);
 if(cmd=="popolazione"||cmd=="supply")return toggle(cheatSupply);
 if(cmd=="sblocca"||cmd=="unlock")return toggle(cheatUnlock);
 if(cmd=="mappa"||cmd=="map"){bool reveal=!fogEnabled;if(!toggle(reveal))return false;fogEnabled=!reveal;refreshVision();return true;}
 if(cmd=="potenzia"||cmd=="upgrade"||cmd=="ricerche"||cmd=="research"){
  bool all=cmd=="ricerche"||cmd=="research"||arg=="tutto"||arg=="all";if(!arg.empty()&&arg!="tutto"&&arg!="all")return fail("Usa potenzia oppure potenzia tutto.");bool found=false;
  for(auto& e:entities)if(e.hp>0&&e.team==0&&e.build<=0&&(all||e.selected)){
   if(e.kind==Kind::Hall){float missing=e.maxHp-e.hp;townLevel[0]=modern()?4:3;e.level=modern()?4:3;e.maxHp=def(e.kind).hp+800;e.hp=std::max(1.f,e.maxHp-missing);found=true;}if(e.kind==Kind::Barracks){float missing=e.maxHp-e.hp;e.level=modern()?4:3;e.maxHp=def(e.kind).hp+300;e.hp=std::max(1.f,e.maxHp-missing);found=true;}
   if(e.kind==Kind::WatchTower){e.kind=Kind::GuardTower;e.maxHp=def(e.kind).hp;e.hp=e.maxHp;found=true;navDirty=true;}
  }
  if(all){for(auto& e:entities)if(e.team==0&&e.research>=0){e.production=0;e.research=-1;e.paid={};}townLevel[0]=modern()?4:3;for(int i=0;i<int(Research::Count);++i)upgrades[0][i]=researchCatalog()[i].maxLevel;found=true;}
  if(!found)return fail("Seleziona comando, caserma o torre completata.");cheatsUsed=true;message="Potenziamenti applicati.";return true;
 }
 if(!arg.empty())return fail("Questo comando non richiede parametri.");
 if(cmd=="cura"||cmd=="heal"||cmd=="mana"){
  for(auto& e:entities)if(e.hp>0&&e.team==0){if(cmd!="mana")e.hp=e.maxHp;e.mana=e.maxMana;}message="Salute e mana ripristinati.";
 }else if(cmd=="completa"||cmd=="finish"){
  for(auto& e:entities)if(e.hp>0&&e.team==0){if(e.build>0)e.build=.001f;if(e.production>0)e.production=.001f;}message="Cantieri e code pronti al prossimo aggiornamento.";
 }else if(cmd=="annienta"||cmd=="destroy"){
  for(auto& e:entities)if(e.team==1)e.hp=0;navDirty=true;message="Esercito e strutture nemiche eliminati.";
 }else if(cmd=="vittoria"||cmd=="win"){
  for(auto& e:entities){if(e.team==1)e.hp=0;if(e.team<0&&e.kind==Kind::Sanctuary&&e.id!=archivist){e.level=2;e.work=100;}}
  missionStage=2;chapter=4;deliveredGold=std::max(120,deliveredGold);deliveredWood=std::max(80,deliveredWood);secureTime=1000;result=1;navDirty=true;message="MISSIONE COMPLETATA: tutti gli obiettivi conclusi.";
 }else if(cmd=="sconfitta"||cmd=="lose"){result=-1;message="SCONFITTA";
 }else if(cmd=="normale"||cmd=="normal"){
  cheatGod=cheatFast=cheatFreeze=cheatFree=cheatUnlock=cheatSupply=false;fogEnabled=true;refreshVision();message="Trucchi continui disattivati. Risorse e progressi restano.";
 }else return fail("Comando sconosciuto.");
 cheatsUsed=true;return true;
}
}
