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
   // Compact timber handle with a wrapped grip and a forged, bevelled tool head.
   tip=grip+sf::Vector2f(facing*std::cos(angle)*22,std::sin(angle)*22);
   line(grip,tip,{65,43,27},3.6f);line(grip+sf::Vector2f(0,-.7f),tip+sf::Vector2f(0,-.7f),{169,120,64},1.8f);
   auto shaft=tip-grip;line(grip,grip+shaft*.24f,{92,64,43},4);
   sf::Transform toolTransform;toolTransform.translate(tip).rotate(facing*angle*180/3.14159265f+90);
   auto facet=[&](std::initializer_list<sf::Vector2f> points,sf::Color color){sf::ConvexShape shape(points.size());int i=0;for(auto point:points)shape.setPoint(i++,toolTransform.transformPoint(point));shape.setFillColor(color);target.draw(shape);};
   if(action==A::Mine){facet({{-12,1},{-9,-2},{0,-4},{8,-2},{12,2},{4,0},{-4,0}}, {67,78,81});facet({{-10,-1},{0,-3},{8,-1},{3,-1},{-4,0}}, {183,194,193});}
   else if(action==A::Build||action==A::Repair){
    facet({{-7,-3},{5,-3},{8,-1},{8,3},{5,5},{-7,4},{-9,2},{-9,-1}}, {55,66,70});
    facet({{-7,-3},{5,-3},{8,-1},{-5,0},{-9,-1}}, {202,211,207});
    facet({{-5,0},{8,-1},{8,3},{5,4},{-5,3}}, {119,137,141});
    facet({{-9,-1},{-5,0},{-5,3},{-8,3}}, {226,229,214});
   }else{facet({{-3,-4},{5,-5},{11,-1},{10,6},{3,5},{-3,2}}, {66,80,83});facet({{5,-4},{10,-1},{9,5},{6,4}}, {211,220,214});facet({{-2,-3},{5,-4},{6,3},{-2,1}}, {132,153,155});}
   sf::CircleShape hand(2.5f,9);hand.setOrigin(2.5f,2.5f);hand.setPosition(grip);hand.setFillColor({174,133,91});target.draw(hand);
   if(action!=A::Fight&&pose.cycle>.60f&&pose.cycle<.82f){for(int j=0;j<4;++j){float age=(pose.cycle-.60f)/.22f;sf::CircleShape chip(action==A::Chop?2.4f:1.8f,action==A::Chop?3:5);chip.setPosition(tip+sf::Vector2f(facing*(age*22+j*3),-age*(28+j*5)+age*age*36));chip.setFillColor(action==A::Chop?sf::Color(195,151,88,190):action==A::Mine?sf::Color(246,211,111,200):sf::Color(215,196,162,150));target.draw(chip);}}
   if(action==A::Build||action==A::Repair){sf::CircleShape dust(8+pose.cycle*12,16);dust.setScale(1,.35f);dust.setPosition(e.pos+sf::Vector2f(facing*18,0));dust.setFillColor({182,155,115,static_cast<sf::Uint8>((1-pose.cycle)*55)});target.draw(dust);}
  }
  if((action==A::Walk||action==A::Carry)&&std::fmod(v.stride,1.f)<.25f){sf::CircleShape step(3,12);step.setScale(1,.4f);step.setPosition(e.pos+sf::Vector2f(-facing*5,12));step.setFillColor({185,161,114,60});target.draw(step);}
 }
 // A small stack of cut timber replaces the oversized symbolic scaffolding.
 if(!alpha::mobile(e.kind)&&!alpha::resource(e.kind)&&e.build>0){
  sf::Vector2f origin=e.pos+sf::Vector2f(-47,37);sf::CircleShape shadow(17,24);shadow.setOrigin(17,17);shadow.setScale(1,.35f);shadow.setPosition(origin+sf::Vector2f(0,6));shadow.setFillColor({28,25,17,65});target.draw(shadow);
  auto board=[&](sf::Vector2f p,float shift){
   sf::ConvexShape side(4);side.setPoint(0,p+sf::Vector2f(-14,0));side.setPoint(1,p+sf::Vector2f(11,-4));side.setPoint(2,p+sf::Vector2f(11,0));side.setPoint(3,p+sf::Vector2f(-14,4));side.setFillColor({100,67,39});target.draw(side);
   sf::ConvexShape top(4);top.setPoint(0,p+sf::Vector2f(-14,0));top.setPoint(1,p+sf::Vector2f(11,-4));top.setPoint(2,p+sf::Vector2f(15,-1));top.setPoint(3,p+sf::Vector2f(-10,3));top.setFillColor({163,119,68});target.draw(top);
   line(p+sf::Vector2f(-10,0),p+sf::Vector2f(10,-2.5f),{200,155,94},.7f);line(p+sf::Vector2f(-6,1+shift),p+sf::Vector2f(5,-1+shift),{115,78,42},.6f);
  };board(origin+sf::Vector2f(-2,4),.2f);board(origin+sf::Vector2f(2,0),-.2f);board(origin+sf::Vector2f(-1,-4),0);
 }

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
