#pragma once
#include "NGin.h"
#include "Version.h"
#include "WorkerAnimation.h"

#include "CommandPanel.h"
#include "AlphaSimulation.h"
#include "Localization.h"
#include <array>

class Gameplay : public sf::Drawable {
public:
 Gameplay(int mission=-1,int faction=-1);
 int playerFaction()const{return world_.faction[0];}
 int missionIndex()const{return world_.mission;}
 int outcome()const{return world_.result;}
 std::string statusMessage()const{return world_.message;}
 bool saveCampaign(const std::wstring&);
 bool loadCampaign(const std::wstring&);
 void regressionCampaignControls();
 void regressionCampaignVictoryFixture(int n=0);
 void handleEvents(const sf::Event&, CommandPanel&);
 void update();
 void regressionProductionControls();
 void regressionGraphics();
 void regressionProgressionControls();
 void regressionRosterControls();
 void regressionForestNavigationControls();
 bool consoleOpen()const{return cheatOpen_;}
 void regressionCheats();
 bool hasOverlay()const{return cheatOpen_||catalogMode_>=0||placing_||casting_||planting_||cursorOrder_||commandBuild_;}
 void draw(sf::RenderTarget&,sf::RenderStates)const;
 sf::FloatRect getGlobalBounds()const{return {455,70,1423,976};}
private:
 alpha::World world_;
 bool cheatOpen_=false;std::string cheatInput_,cheatFeedback_;std::vector<std::string> cheatHistory_;int cheatHistoryIndex_=0;
 bool cheatEvent(const sf::Event&);void drawCheats(sf::RenderTarget&)const;
 int currentMission_=-1;
 sf::View view_;
 bool minimapDragging_=false,panning_=false;
 sf::Vector2i panPixel_;
 sf::FloatRect minimapBounds()const{return {25,92,300,187.5f};}
 void minimapCamera(sf::Vector2f);
 void dragCamera(sf::Vector2i);
 sf::Vector2f edgeDirection(sf::Vector2f,sf::Vector2f)const;
 int pickEntity(sf::Vector2f,bool resourcesOnly=false)const;
 bool selectedWorkersGather(int);
 ng::FontPtr font_=std::make_shared<sf::Font>();
 std::array<sf::Texture,6> art_;
 std::array<bool,6> artReady_{};
 sf::Texture ground_,treeArt_,rockArt_,crystalArt_;
 std::array<sf::Texture,4> terrainArt_;
 void drawTerrain(sf::RenderTarget&)const;

 bool treeReady_=false;
 std::array<sf::Texture,2> rosterArt_;
 std::array<bool,2> rosterReady_{};
 std::array<sf::Texture,2> demonArt_;
 std::array<bool,2> demonReady_{};
 std::array<sf::IntRect,static_cast<int>(alpha::Kind::Count)> demonRects_{};
 void loadDemonArt();
 int demonTile(alpha::Kind)const;
 bool groundReady_=false;
 struct Visual { sf::Vector2f last,display; int direction=0; float phase=0,death=0,stride=0,speed=0; bool initialized=false; };
 bool legacyFixture_=false;
 std::unordered_map<int,Visual> visuals_;
 void drawActivity(sf::RenderTarget&,const alpha::Entity&,const Visual&)const;
 bool dragging_=false,placing_=false,paused_=false;
 sf::Vector2f dragStart_,mouseWorld_;
 float zoom_=1;
 int builder_=-1;
 int catalogMode_=-1,catalogPage_=0,factionChoice_=0;
 struct Command {int type,item;alpha::Kind icon;std::string label,detail;};
 std::vector<Command> commands()const;
 void executeCommand(const Command&);
 void executeCommandImpl(const Command&);
 std::string commandNotice_;
 float noticeTime_=0;
 bool knownEntity(const alpha::Entity& e)const{return e.team==0||(e.team==1?world_.visibleAt(e.pos):world_.exploredAt(e.pos));}
 int researchProducer(alpha::Research)const;
 void drawCommands(sf::RenderTarget&)const;
 void drawIcon(sf::RenderTarget&,alpha::Kind,sf::FloatRect)const;
 int commandPage_=0,cursorOrder_=0;
 bool commandBuild_=false,planting_=false;
 sf::Vector2f uiMouse_;
 alpha::Kind placingKind_=alpha::Kind::Barracks;
 bool casting_=false;
 alpha::Spell pendingSpell_=alpha::Spell::Heal;
 sf::Vector2f worldPoint(int,int)const;
 void constrainCamera();
 void action(int);
 void restart();
 void loadArt();
 void updateVisuals(float);
 bool hitEntity(const alpha::Entity&,sf::Vector2f)const;
 const sf::Texture* textureFor(alpha::Kind,int faction=0)const;
 void drawArt(sf::RenderTarget&,const alpha::Entity&,const Visual&,float)const;
 std::vector<int> catalogItems()const;
 void chooseCatalog(int);
 bool produce(alpha::Kind);
 void drawCatalog(sf::RenderTarget&)const;
 bool catalogEvent(const sf::Event&,sf::Vector2f);
 void text(sf::RenderTarget&,const std::string&,sf::Vector2f,unsigned=24,sf::Color=sf::Color::White)const;
};



