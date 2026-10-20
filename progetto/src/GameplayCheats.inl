#pragma once
#include "CampaignFiles.h"
bool Gameplay::cheatEvent(const sf::Event& ev){
 if(!cheatOpen_){if(ev.type==sf::Event::KeyPressed&&ev.key.code==sf::Keyboard::Tab&&!world_.result){cheatOpen_=true;cheatInput_.clear();cheatHistoryIndex_=int(cheatHistory_.size());panning_=minimapDragging_=dragging_=false;return true;}return false;}
 if(ev.type==sf::Event::TextEntered){auto c=ev.text.unicode;if(c>=32&&c<=126&&cheatInput_.size()<96)cheatInput_+=char(c);return true;}
 if(ev.type==sf::Event::KeyPressed){auto key=ev.key.code;
  if(key==sf::Keyboard::Escape||key==sf::Keyboard::Tab){cheatOpen_=false;return true;}
  if(key==sf::Keyboard::BackSpace&&!cheatInput_.empty())cheatInput_.pop_back();
  if(key==sf::Keyboard::Up&&!cheatHistory_.empty()){cheatHistoryIndex_=std::max(0,cheatHistoryIndex_-1);cheatInput_=cheatHistory_[cheatHistoryIndex_];}
  if(key==sf::Keyboard::Down){cheatHistoryIndex_=std::min(int(cheatHistory_.size()),cheatHistoryIndex_+1);cheatInput_=cheatHistoryIndex_<int(cheatHistory_.size())?cheatHistory_[cheatHistoryIndex_]:"";}
  if(key==sf::Keyboard::Enter){if(cheatInput_.empty()){cheatOpen_=false;return true;}std::string command=cheatInput_;for(char& c:command)c=char(std::tolower(static_cast<unsigned char>(c)));
   bool success;if(command=="campagna"||command=="campaign"){success=world_.mission>=0&&campaignFiles::progress(alpha::missionCount);world_.cheatsUsed=true;world_.message=success?"Tutte le missioni sbloccate nel menu campagna.":"Impossibile salvare l avanzamento su disco.";}else success=world_.cheat(command);
   cheatFeedback_=world_.message;commandNotice_=world_.message;noticeTime_=5;if(success){cheatHistory_.push_back(command);if(cheatHistory_.size()>50)cheatHistory_.erase(cheatHistory_.begin());cheatOpen_=false;}cheatHistoryIndex_=int(cheatHistory_.size());cheatInput_.clear();
  }
 }
 return true;
}
void Gameplay::drawCheats(sf::RenderTarget& target)const{
 if(!cheatOpen_)return;
 sf::RectangleShape panel({1120,220});panel.setPosition(400,420);panel.setFillColor({15,28,35,248});panel.setOutlineThickness(2);panel.setOutlineColor({161,143,95});target.draw(panel);
 text(target,loc::english?"Command":"Comando",{440,440},28,{239,213,156});
 sf::RectangleShape input({1040,60});input.setPosition(440,490);input.setFillColor({30,48,55});target.draw(input);text(target,"> "+cheatInput_+"_",{455,500},26,{245,233,186});
 if(!cheatFeedback_.empty())text(target,cheatFeedback_,{440,577},19,{217,220,204});
}
void Gameplay::regressionCheats(){
 int n=0;auto check=[&](bool b,const char* s){if(!b)throw std::runtime_error(s);++n;std::cout<<"PASS "<<s<<std::endl;};CommandPanel panel;sf::Event e{};auto key=[&](sf::Keyboard::Key k){e={};e.type=sf::Event::KeyPressed;e.key.code=k;handleEvents(e,panel);};auto type=[&](const std::string& s){for(char c:s){e={};e.type=sf::Event::TextEntered;e.text.unicode=c;handleEvents(e,panel);}};
 restart();auto before=world_.gold;key(sf::Keyboard::Enter);check(!cheatOpen_,"Enter never exposes hidden console");key(sf::Keyboard::Tab);check(cheatOpen_,"Tab opens hidden console");type("gold 2345");key(sf::Keyboard::R);check(cheatOpen_&&world_.gold==before,"typing never triggers restart hotkey");key(sf::Keyboard::Enter);check(!cheatOpen_&&world_.gold==before+2345,"typed resource command applies");key(sf::Keyboard::Tab);key(sf::Keyboard::Up);check(cheatInput_=="gold 2345","history retrieves last command");key(sf::Keyboard::Escape);check(!cheatOpen_,"Escape closes console without menu");key(sf::Keyboard::Tab);type("bad command");key(sf::Keyboard::Enter);check(cheatOpen_&&world_.gold==before+2345,"invalid command keeps console and state");key(sf::Keyboard::Escape);
 loc::english=true;check(loc::tr("Operaio")=="Worker","unit name translated");check(loc::tr(alpha::missionBriefing(10)).find("Victory:")!=std::string::npos,"entire final briefing translated");check(loc::tr("Oro insufficiente: 80 oro, 25 legno, 0 petrolio.").find("Insufficient gold:")==0,"dynamic resource message translated");for(int i=0;i<int(alpha::Kind::Count);++i){auto k=alpha::Kind(i);for(int faction=0;faction<2;++faction){std::string name=alpha::factionUnitName(k,faction);check(loc::tr(name)!=name,"every faction unit and building name has English text");}std::string role=alpha::def(k).role;if(!role.empty())check(loc::tr(role)!=role,"every catalog role has English text");}for(auto& research:alpha::researchCatalog())check(loc::tr(research.name)!=research.name,"every research has English text");loc::english=false;
 loc::english=GetEnvironmentVariableA("FRONTIERE_ENGLISH",nullptr,0)!=0;std::cout<<"SUCCESS "<<n<<" console and localization checks"<<std::endl;key(sf::Keyboard::Tab);
}
