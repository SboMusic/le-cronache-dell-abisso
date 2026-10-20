#pragma once
namespace alpha {
inline void World::startGuided(){
 guided=true;chapter=0;raidsEnabled=true;
 get(13)->pos={1250,750};get(14)->pos={1650,750};get(15)->pos={2080,750};
 for(int i=0;i<4;++i)add(Kind::Worker,1,{2350.f,680.f+i*40});
 refreshVision();message=objective();
}
inline std::string World::objective()const{
 switch(chapter){case 0:return "1/5 - Costruisci una fattoria: operaio > Costruisci > Fattoria. +6 posti.";
 case 1:return "2/5 - Costruisci la caserma. L operaio resta al cantiere fino al completamento.";
 case 2:return "3/5 - Porta i soldati a 3. Per l oro: operaio > clic destro sulla miniera.";
 case 3:return "4/5 - Esplora la strada verso est fino all avamposto. Sbloccate Segheria, Fucina e Comando II.";
 default:return "5/5 - Distruggi il comando nemico. Tecnologie avanzate sbloccate; prepara economia ed esercito.";}
}
inline void World::updateProgression(){int previous=chapter;int soldiers=0;for(const auto& e:entities)if(e.hp>0&&e.team==0&&e.kind==Kind::Soldier)++soldiers;
 if(chapter==0&&has(0,Kind::Farm))chapter=1;
 else if(chapter==1&&has(0,Kind::Barracks))chapter=2;
 else if(chapter==2&&soldiers>=3)chapter=3;
 else if(chapter==3&&exploredAt({1250,750}))chapter=4;
 if(previous!=chapter)message="Obiettivo completato! "+objective();
}
inline void World::enemyEconomy(){
 if(mission>=5){warEnemyEconomy();return;}if(mission>=3){laterEnemyEconomy();return;}if(mission>=0&&mission!=2)return;if(guided&&chapter<4)return;std::string saved=message;
 int worker=-1,barracks=-1;bool constructing=false;for(const auto& e:entities)if(e.hp>0&&e.team==1){if(e.kind==Kind::Worker&&!busy(e))worker=e.id;if(e.kind==Kind::Barracks&&e.build<=0)barracks=e.id;if(e.build>0)constructing=true;}
 if(worker>=0&&!constructing){
  if(!has(1,Kind::Farm))build(worker,{2350,850},Kind::Farm);
  else if(barracks<0)build(worker,{1980,800},Kind::Barracks);
  else if(population(1)+1>capacity(1)){
   bool placed=false;for(int y=650;y<=850&&!placed;y+=160)for(int x=1770;x<=2440&&!placed;x+=160)if(placementIssue({float(x),float(y)},Kind::Farm).empty())placed=build(worker,{float(x),float(y)},Kind::Farm);
  }
 }
 for(auto& e:entities)if(e.hp>0&&e.team==1&&e.kind==Kind::Worker&&!busy(e)&&e.order==Order::Idle){int source=-1;float nearest=1e9;Kind wanted=(e.id%2)?Kind::Mine:Kind::Forest;for(const auto& n:entities)if(n.hp>0&&n.kind==wanted&&distance(n.pos,e.pos)<nearest){nearest=distance(n.pos,e.pos);source=n.id;}if(source>=0)gather(e,source);}
 int workers=0,hall=-1;for(const auto& e:entities)if(e.hp>0&&e.team==1){if(e.kind==Kind::Worker)++workers;if(e.kind==Kind::Hall)hall=e.id;}
 if(workers<4&&hall>=0&&get(hall)->production<=0&&has(1,Kind::Farm))train(hall,Kind::Worker);
 int troops=0;for(const auto& e:entities)if(e.hp>0&&e.team==1&&mobile(e.kind)&&e.kind!=Kind::Worker)++troops;
 if(barracks>=0&&get(barracks)->production<=0&&(mission<0||troops<8))train(barracks,Kind::Soldier);
 message=saved;
}
}
