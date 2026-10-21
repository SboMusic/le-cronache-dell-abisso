#include "../src/AlphaSimulation.h"
#include <chrono>
using namespace alpha;
int checks=0;
void check(bool ok,const char* label){if(!ok)throw std::runtime_error(label);++checks;std::cout<<"PASS "<<label<<'\n';}
std::string snapshot(const World& w){std::ostringstream out;check(w.save(out),"world serializes");return out.str();}
int main(){try{
 World w;w.startMission(0);w.raidsEnabled=false;
 w.terrain[65]=Terrain::Road;w.refreshVision();bool known=w.exploredAt({300,300});
 w.resizeMap(192,120,true);
 check(w.width()==7680&&w.height()==4800&&w.terrain.size()==23040,"four times legacy mission area uses matching storage");
 check(w.terrain[193]==Terrain::Road&&w.exploredAt({300,300})==known,"resize preserves terrain and exploration by coordinates");
 check(w.terrain.back()==Terrain::Grass&&!w.exploredAt({7600,4700}),"new territory starts unexplored");
 int worker=-1;for(auto& e:w.entities)if(e.team==0&&e.kind==Kind::Worker){worker=e.id;break;}
 w.get(worker)->pos={6500,3600};w.move(*w.get(worker),{7000,3800});
 check(!w.get(worker)->route.empty()&&w.get(worker)->goal.x==7000,"land route works outside legacy bounds");
 for(int i=0;i<150;++i)w.step(.05f);
 check(w.get(worker)->pos.x>6900,"unit follows large map route");
 w.refreshVision();check(w.exploredAt(w.get(worker)->pos),"large map exploration follows unit");
 for(int x=0;x<w.cols;++x)w.terrain[x+(w.rows-1)*w.cols]=Terrain::Water;
 // Resizing with preservation also invalidates navigation after terrain changes.
 w.resizeMap(192,120,true);Entity ship;ship.kind=Kind::Destroyer;ship.pos={6500,4780};
 check(w.path(ship,{7400,4780})&&!ship.route.empty(),"sea route crosses extended map coast");
 Entity air;air.kind=Kind::Scout;air.pos={6500,3600};check(w.path(air,{7400,4500})&&air.route.size()==1,"air routing remains independent of terrain");
 w.upgrades[0][int(Research::Forestry)]=1;w.gold=1000;
 check(w.plant(worker,{7200,4000}),"forestry works beyond original hardcoded x boundary");
 check(!w.plant(worker,{7200,4900}),"forestry rejects coordinates below map");
 auto saved=snapshot(w);World copy;std::istringstream input(saved);
 check(saved.rfind("FRONTIERE_SAVE 4\n",0)==0&&copy.restore(input)&&snapshot(copy)==saved,"v4 preserves large map units routes and exploration exactly");
 auto before=snapshot(copy);std::istringstream oversized("FRONTIERE_SAVE 4\n2147483647 120\n");
 check(!copy.restore(oversized)&&snapshot(copy)==before,"oversized map is rejected without changing active game");
 std::istringstream truncated(saved.substr(0,saved.size()/2));check(!copy.restore(truncated)&&snapshot(copy)==before,"truncated large save does not replace active game");
 bool rejected=false;try{copy.resizeMap(0,120);}catch(const std::invalid_argument&){rejected=true;}
 check(rejected&&snapshot(copy)==before,"invalid resize leaves world unchanged");
 World small;small.startMission(0);auto modern=snapshot(small);
 auto legacy=modern;legacy.replace(0,legacy.find('\n'),"FRONTIERE_SAVE 3");World old;std::istringstream v3(legacy);
 check(old.restore(v3)&&snapshot(old)==modern,"real version 3 field layout imports to version 4");
 auto line1=modern.find('\n'),line2=modern.find('\n',line1+1),line3=modern.find('\n',line2+1);
 auto version2=std::string("FRONTIERE_SAVE 2\n")+modern.substr(line1+1,line2-line1)+modern.substr(line3+1);std::istringstream v2(version2);
 check(old.restore(v2)&&snapshot(old)==modern,"version 2 imports without cheat fields");
 auto version1=std::string("FRONTIERE_SAVE 1\n")+modern.substr(line3+1);std::istringstream v1(version1);
 check(old.restore(v1)&&snapshot(old)==modern,"version 1 imports with original dimensions");
 auto fake= saved;fake.replace(0,fake.find('\n'),"FRONTIERE_SAVE 3");std::istringstream badLegacy(fake);
 check(!copy.restore(badLegacy),"legacy format cannot claim v4 dimensions");
 copy.reset();check(copy.cols==64&&copy.rows==40&&copy.terrain.size()==2560,"reset shrinks all map storage safely");copy.startMission(10);check(copy.cols==96&&copy.rows==60,"large test does not alter original mission layouts");
 std::cout<<"SUCCESS "<<checks<<" dynamic map and legacy save checks\n";return 0;
 }catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
