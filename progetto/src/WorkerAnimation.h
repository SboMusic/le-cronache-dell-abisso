#pragma once
#include "AlphaSimulation.h"
namespace presentation {
enum class WorkerAction { Idle, Walk, Carry, Mine, Chop, Build, Repair, Fight };
inline WorkerAction workerAction(const alpha::World& w,const alpha::Entity& e,bool moving){
 if(moving)return e.cargo>0?WorkerAction::Carry:WorkerAction::Walk;
 if(w.busy(e)&&e.route.empty())return WorkerAction::Build;
 if(e.order==alpha::Order::Repair&&e.route.empty())return WorkerAction::Repair;
 if(e.order==alpha::Order::Gather&&e.work>0&&e.route.empty()){auto source=w.get(e.target);if(source)return source->kind==alpha::Kind::Forest?WorkerAction::Chop:WorkerAction::Mine;}
 if(e.order==alpha::Order::Attack&&e.cooldown>0&&e.route.empty())return WorkerAction::Fight;
 return WorkerAction::Idle;
}
struct WorkerPose {int frame=0;float lean=0,bob=0,reach=0,cycle=0,stroke=0;};
inline WorkerPose workerPose(WorkerAction action,float phase,float stride,float cooldown=0,float total=1){
 WorkerPose p;float rate=action==WorkerAction::Chop?1.6f:action==WorkerAction::Mine?1.9f:action==WorkerAction::Build?2.6f:2.2f;
 p.cycle=std::fmod(phase*rate,1.f);
 if(action==WorkerAction::Walk||action==WorkerAction::Carry){p.frame=1+(int(stride)%2);p.bob=std::sin(stride*3.14159265f)*(action==WorkerAction::Carry?1.f:1.8f);p.lean=action==WorkerAction::Carry?3.f:std::sin(stride*3.14159265f)*1.5f;return p;}
 if(action==WorkerAction::Idle){p.bob=std::sin(phase*2)*.4f;return p;}
 if(action==WorkerAction::Fight){p.cycle=std::clamp(1-cooldown/std::max(.01f,total),0.f,1.f);float recover=std::max(0.f,1-p.cycle/.42f);p.stroke=recover;p.frame=0;p.lean=recover*6;p.reach=recover*5;return p;}
 // Deliberate lift, quick impact, then recovery; avoids a continuously oscillating tool.
 float swing=p.cycle<.50f?-std::sin(p.cycle*3.14159265f):p.cycle<.66f?-1+2*(p.cycle-.50f)/.16f:1-(p.cycle-.66f)/.34f;
 float stroke=swing;p.stroke=stroke;p.frame=0; // Activity tools are drawn once, over the unarmed base pose.
 p.lean=stroke*(action==WorkerAction::Chop?7.f:action==WorkerAction::Mine?3.f:action==WorkerAction::Fight?5.f:4.f);
 p.bob=std::max(0.f,stroke)*1.5f;p.reach=action==WorkerAction::Fight?std::max(0.f,stroke)*5:std::max(0.f,stroke)*2;return p;
}
}
