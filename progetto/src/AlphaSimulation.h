#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <unordered_set>
#include "RtsCatalog.h"

namespace alpha {
enum class Order { Idle, Move, Gather, Return, Attack, Repair, Board, Build };
enum class Terrain : unsigned char { Grass, Road, Rock, Water };
struct Entity {
 int id=0,team=0,target=-1,objective=-1,source=-1,cargo=0,carrier=-1;
 Kind kind=Kind::Worker,training=Kind::Worker,cargoKind=Kind::Mine;
 Kind upgradeKind=Kind::Count;
 sf::Vector2f pos,goal;
 float hp=80,maxHp=80,cooldown=0,build=0,buildTotal=0,production=0,productionTotal=0,work=0;
 float mana=0,maxMana=0,spellCooldown=0,haste=0,rage=0,shield=0,reveal=0,lifetime=0;
 int research=-1,level=1,constructionWorker=-1;
 Cost paid;
 Order order=Order::Idle;
 std::vector<sf::Vector2f> route;
 bool selected=false;
};
struct Shot {sf::Vector2f pos;int target,team;float damage,splash,remaining;Kind source;};
struct Effect {sf::Vector2f pos;float radius,remaining,total;int type,team;};
struct Zone {sf::Vector2f pos;float remaining,tick;int team;};
inline float distance(sf::Vector2f a,sf::Vector2f b){auto d=a-b;return std::sqrt(d.x*d.x+d.y*d.y);}

class World {
 int nextId=1;
 float accumulator=0;
 mutable bool navDirty=true;
 mutable std::vector<unsigned char> landNav=std::vector<unsigned char>(2560),seaNav=std::vector<unsigned char>(2560);
 void rebuildNav()const;
 void steer(Entity&,float);
 void economy(Entity&,float);
 void combat(Entity&,float);
 void processShots(float);
 void damage(Entity&,float);
 float attackDamage(const Entity&)const;
 bool spawnPosition(const Entity&,Kind,sf::Vector2f&)const;
 void finishResearch(Entity&);
 bool pay(Cost c,int team=0);
 void updateProgression();
 void enemyEconomy();
 void laterEnemyEconomy();

 bool assignTree(Entity&,sf::Vector2f,int exclude=-1);
 bool returnCargo(Entity&);
 float visionTimer=0;
 std::array<std::vector<unsigned char>,2> explored{std::vector<unsigned char>(2560),std::vector<unsigned char>(2560)},sight{std::vector<unsigned char>(2560),std::vector<unsigned char>(2560)};
public:
 static constexpr int maxMapCols=256,maxMapRows=256,maxCells=maxMapCols*maxMapRows,cell=40,coast=1200;
 int cols=64,rows=40;
 int layoutVersion=0;sf::Vector2f mapGoal;
 void resizeMap(int columns,int rowCount,bool preserve=false);
 void clearVision();
 float width()const{return float(cols*cell);}
 float height()const{return float(rows*cell);}
 void expandMap();
 void prepareCampaignRegion();
 void prepareLegacyRegion();
 void startLegacyMission(int n,int playerFaction=0);
 bool approachResource(Entity&,sf::Vector2f,float,float);
 std::vector<Entity> entities;
 std::vector<Shot> shots;
 std::vector<Effect> effects;
 std::vector<Zone> zones;
 int gold=240,wood=140,oil=0,result=0;
 int enemyGold=240,enemyWood=140,enemyOil=0;
 bool guided=false;int chapter=0;float aiTimer=0;
 void startGuided();
 std::string objective()const;
 bool busy(const Entity& e)const{return e.order==Order::Build&&get(e.target)&&get(e.target)->build>0;}
 int townLevel[2]={1,1},faction[2]={0,1};
 std::array<std::array<int,int(Research::Count)>,2> upgrades{};
 float time=0,wave=0;
 bool cheatsUsed=false,cheatGod=false,cheatFast=false,cheatFreeze=false,cheatFree=false,cheatUnlock=false,cheatSupply=false;
 bool cheat(const std::string&);
 bool raidsEnabled=true;
 bool fogEnabled=true;
 int mission=-1,missionStage=0,deliveredGold=0,deliveredWood=0,archivist=-1;
 float secureTime=0;
 std::vector<int> missionGuards;
 std::vector<Terrain> terrain=std::vector<Terrain>(2560,Terrain::Grass);
 bool factionAllows(Kind k,int team=0)const{return (k!=Kind::Mage||faction[team]==0)&&(k!=Kind::Warlock||faction[team]==1);}
 void startMission(int,int playerFaction=0);
 void startHumanMission(int);
 void startLaterMission(int);
 void startWarMission(int);
 void updateWarMission(float);
 void warEnemyEconomy();
 std::string warObjective()const;
 sf::Vector2f warTarget()const;
 std::vector<sf::Vector2f> questTargets()const;
 int enemyRemaining(bool buildings=false)const;
 void updateLaterMission(float);
 bool missionRaidReady()const;
 void updateMission(float);
 bool missionAllows(Kind)const;
 bool missionResearchAllows(Research)const;
 std::string missionObjective()const;
 sf::Vector2f missionTarget()const;
 bool save(std::ostream&)const;
 bool restore(std::istream&);
 void refreshVision();
 bool exploredAt(sf::Vector2f,int team=0)const;
 bool visibleAt(sf::Vector2f,int team=0)const;
 std::string message;
 int add(Kind,int,sf::Vector2f);
 Entity* get(int);
 const Entity* get(int)const;
 void reset(int playerFaction=0);
 void prepareArsenal();
 bool has(int,Kind)const;
 int capacity(int team)const;
 int population(int team,bool reserved=true)const;
 bool water(sf::Vector2f p)const{int x=int(std::floor(p.x/cell)),y=int(std::floor(p.y/cell));return x>=0&&x<cols&&y>=0&&y<rows&&terrain[x+y*cols]==Terrain::Water;}
 bool blocked(int,int,Domain domain=Domain::Land)const;
 bool path(Entity&,sf::Vector2f,float stop=0);
 void move(Entity&,sf::Vector2f);
 void gather(Entity&,int);
 void repair(Entity&,int);
 void board(Entity&,int);
 bool unload(int,sf::Vector2f);
 void attack(Entity&,int);
 bool detected(const Entity&,int)const;
 bool canHit(const Entity&,const Entity&)const;
 std::string unavailable(Kind,int team=0)const;
 bool train(int,Kind);
 bool build(int,sf::Vector2f,Kind kind=Kind::Barracks);
 bool plant(int,sf::Vector2f);
 std::string placementIssue(sf::Vector2f,Kind)const;
 int requiredProducerLevel(Kind k)const{return k==Kind::Archer?2:(k==Kind::Ranger?3:1);}
 bool research(int,Research);
 std::string researchIssue(int,Research,bool checkCost=true)const;
 bool upgradeTower(int,Kind);
 bool cancel(int);
 bool cast(int,Spell,sf::Vector2f);
 bool supports(Kind,Spell,int team=0)const;
 void step(float);
 void update(float dt){accumulator+=std::min(dt,.25f);while(accumulator>=.05f){step(.05f);accumulator-=.05f;}}
};
}
#include "RtsWorld.inl"
#include "RtsProgression.inl"
#include "RtsCampaign.inl"
#include "RtsCampaignLater.inl"
#include "RtsWarCampaign.inl"
#include "RtsWarEconomy.inl"
#include "RtsLegacyRegions.inl"
#include "RtsRegions.inl"
#include "RtsCheats.inl"
#include "RtsSave.inl"


