void Gameplay::drawActivity(sf::RenderTarget& target,const alpha::Entity& e,const Visual& v)const{
 if(e.hp<=0)return;
 auto line=[&](sf::Vector2f a,sf::Vector2f b,sf::Color color,float width){auto d=b-a;sf::RectangleShape r({alpha::distance(a,b),width});r.setPosition(a);r.setRotation(std::atan2(d.y,d.x)*180/3.14159265f);r.setFillColor(color);target.draw(r);};
 if(e.kind==alpha::Kind::Worker){
  using A=presentation::WorkerAction;auto action=presentation::workerAction(world_,e,v.speed>5);auto pose=presentation::workerPose(action,v.phase+e.id*.07f,v.stride,e.cooldown,alpha::def(e.kind).cooldown/(e.haste>0?1.4f:1.f));
  float facing=(v.direction==1||v.direction==2)?-1.f:1.f;
  if(e.cargo>0){sf::Vector2f load=e.pos+sf::Vector2f(-facing*13,-13+pose.bob);
   if(e.cargoKind==alpha::Kind::Forest){for(int j=0;j<3;++j){line(load+sf::Vector2f(-12,j*4.f),load+sf::Vector2f(13,j*4.f),{130,83,43},5);sf::CircleShape cut(2.5f,10);cut.setPosition(load+sf::Vector2f(11,j*4.f-2));cut.setFillColor({202,164,99});target.draw(cut);}line(load+sf::Vector2f(-2,-1),load+sf::Vector2f(-2,12),{185,166,118},2);}
   else{sf::CircleShape sack(9,9);sack.setScale(1,.8f);sack.setPosition(load);sack.setFillColor({151,115,61});sack.setOutlineColor({86,63,35});sack.setOutlineThickness(1);target.draw(sack);for(int j=0;j<3;++j){sf::CircleShape nugget(3,5);nugget.setPosition(load+sf::Vector2f(2+j*4.f,1+(j%2)*3.f));nugget.setFillColor({239,192,66});target.draw(nugget);}}
  }
  bool tool=action==A::Mine||action==A::Chop||action==A::Build||action==A::Repair||action==A::Fight;
  if(tool){float stroke=pose.stroke,angle=-1.1f+stroke*1.15f;sf::Vector2f grip=e.pos+sf::Vector2f(facing*13,-22+pose.bob),tip=grip+sf::Vector2f(facing*std::cos(angle)*28,std::sin(angle)*28);
   line(grip,tip,{137,91,48},4);sf::RectangleShape head(action==A::Mine?sf::Vector2f(27,5):(action==A::Build||action==A::Repair)?sf::Vector2f(17,9):sf::Vector2f(15,13));head.setOrigin(head.getSize()/2.f);head.setPosition(tip);head.setRotation(facing*angle*180/3.14159265f+90);head.setFillColor({203,211,211});head.setOutlineThickness(1);head.setOutlineColor({91,109,112});target.draw(head);
   if(action!=A::Fight&&pose.cycle>.60f&&pose.cycle<.82f){for(int j=0;j<4;++j){float age=(pose.cycle-.60f)/.22f;sf::CircleShape chip(action==A::Chop?2.4f:1.8f,action==A::Chop?3:5);chip.setPosition(tip+sf::Vector2f(facing*(age*22+j*3),-age*(28+j*5)+age*age*36));chip.setFillColor(action==A::Chop?sf::Color(195,151,88,190):action==A::Mine?sf::Color(246,211,111,200):sf::Color(215,196,162,150));target.draw(chip);}}
   if(action==A::Build||action==A::Repair){sf::CircleShape dust(8+pose.cycle*12,16);dust.setScale(1,.35f);dust.setPosition(e.pos+sf::Vector2f(facing*18,0));dust.setFillColor({182,155,115,static_cast<sf::Uint8>((1-pose.cycle)*55)});target.draw(dust);}
  }
  if((action==A::Walk||action==A::Carry)&&std::fmod(v.stride,1.f)<.25f){sf::CircleShape step(3,12);step.setScale(1,.4f);step.setPosition(e.pos+sf::Vector2f(-facing*5,12));step.setFillColor({185,161,114,60});target.draw(step);}
 }
 if(!alpha::mobile(e.kind)&&!alpha::resource(e.kind)&&e.build>0){float progress=1-e.build/std::max(1.f,e.buildTotal);for(int side=-1;side<=1;side+=2){sf::Vector2f p=e.pos+sf::Vector2f(side*57.f,35);line(p+sf::Vector2f(0,-100*progress),p+sf::Vector2f(0,12),{143,96,51},5);line(p+sf::Vector2f(-15,-44*progress),p+sf::Vector2f(15,-44*progress),{174,124,64},4);}for(int j=0;j<3;++j)line(e.pos+sf::Vector2f(-64,32+j*8.f),e.pos+sf::Vector2f(-20,26+j*8.f),{151,104,58},5);}
 if(e.order==alpha::Order::Attack&&e.route.empty()&&e.cooldown>0&&alpha::def(e.kind).damage>0){
  auto enemy=world_.get(e.target);if(!enemy)return;auto d=enemy->pos-e.pos;float angle=std::atan2(d.y,d.x);
  auto strike=presentation::combatPose(e.cooldown,alpha::def(e.kind).cooldown/(e.haste>0?1.4f:1.f));
  if(alpha::def(e.kind).range<100&&strike.impact){
   sf::VertexArray arc(sf::TriangleStrip);float sweep=strike.progress/.16f;
   for(int i=0;i<10;++i){float t=i/9.f,a=angle-.9f+t*1.6f+sweep*.35f;auto alpha=static_cast<sf::Uint8>((1-sweep)*180*t);
    for(float radius:{28.f,39.f})arc.append(sf::Vertex(e.pos+sf::Vector2f(std::cos(a)*radius,std::sin(a)*radius*.7f-16),{255,231,181,alpha}));
   }target.draw(arc);
  }else if(alpha::def(e.kind).range>=100&&strike.progress<.12f){
   sf::CircleShape flash(3+5*(1-strike.progress/.12f),7);flash.setPosition(e.pos+sf::Vector2f(std::cos(angle)*28,std::sin(angle)*25-22));flash.setFillColor({255,207,113,static_cast<sf::Uint8>(200*(1-strike.progress/.12f))});target.draw(flash);
  }
 }
}
