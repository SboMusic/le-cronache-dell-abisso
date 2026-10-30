void Gameplay::drawActivity(sf::RenderTarget& target,const alpha::Entity& e,const Visual& v)const{
 if(e.hp<=0)return;
 auto line=[&](sf::Vector2f a,sf::Vector2f b,sf::Color color,float width){auto d=b-a;sf::RectangleShape r({alpha::distance(a,b),width});r.setPosition(a);r.setRotation(std::atan2(d.y,d.x)*180/3.14159265f);r.setFillColor(color);target.draw(r);};
 if(e.kind==alpha::Kind::Worker||e.kind==alpha::Kind::CrystalMiner){
  using A=presentation::WorkerAction;auto action=presentation::workerAction(world_,e,v.speed>5);auto pose=presentation::workerPose(action,v.phase+e.id*.07f,v.stride,e.cooldown,alpha::def(e.kind).cooldown/(e.haste>0?1.4f:1.f));
  float facing=(v.direction==1||v.direction==2)?-1.f:1.f;
  const bool dark=e.team>=0&&world_.faction[e.team]==1;
  if(e.cargo>0){
   // Cargo shares the worker's body motion and sits against the near hip, not beside the feet.
   float sway=v.speed>5?std::sin(v.stride*3.14159265f)*.8f:0;
   sf::Vector2f load=e.pos+sf::Vector2f(-facing*12+sway,-17+pose.bob);
   auto oval=[&](sf::Vector2f p,sf::Vector2f scale,float radius,sf::Color color){sf::CircleShape shape(radius,24);shape.setOrigin(radius,radius);shape.setScale(scale);shape.setPosition(p);shape.setFillColor(color);target.draw(shape);};
   auto polygon=[&](std::initializer_list<sf::Vector2f> points,sf::Color color){sf::ConvexShape shape(points.size());int i=0;for(auto p:points)shape.setPoint(i++,load+p);shape.setFillColor(color);target.draw(shape);};
   if(e.cargoKind==alpha::Kind::Forest){
    oval(load+sf::Vector2f(0,5),{1.5f,.45f},9,{33,25,16,100});
    for(int j=0;j<3;++j){sf::Vector2f p=load+sf::Vector2f(j==2?-1.f:0.f,j==2?-5.f:j*6.f-1);
     line(p+sf::Vector2f(-12,-2),p+sf::Vector2f(10,0),{65,43,27},7);
     line(p+sf::Vector2f(-11,-3),p+sf::Vector2f(10,-1),{119,79,42},4);
     line(p+sf::Vector2f(-10,-4),p+sf::Vector2f(7,-2.5f),{165,115,64},1.2f);
     line(p+sf::Vector2f(-8,0),p+sf::Vector2f(5,1),{82,53,29},.7f);
     oval(p+sf::Vector2f(10,0),{.55f,1},3.7f,{74,48,28});oval(p+sf::Vector2f(10,0),{.52f,1},2.8f,{202,159,96});
     sf::CircleShape ring(1.65f,16);ring.setOrigin(1.65f,1.65f);ring.setScale(.52f,1);ring.setPosition(p+sf::Vector2f(10,0));ring.setFillColor(sf::Color::Transparent);ring.setOutlineThickness(.55f);ring.setOutlineColor({143,98,53});target.draw(ring);
    }
    for(float x:{-6.f,3.f}){line(load+sf::Vector2f(x,-9),load+sf::Vector2f(x-1,8),{75,61,39},2.5f);line(load+sf::Vector2f(x+.6f,-9),load+sf::Vector2f(x-.4f,8),{192,167,112},1.1f);}
   }else if(e.cargoKind==alpha::Kind::CrystalCave){
    for(int j=0;j<3;++j){sf::Vector2f p{float(j*5-5),float((j%2)*4)};polygon({p+sf::Vector2f(0,-8),p+sf::Vector2f(4,-2),p+sf::Vector2f(2,5),p+sf::Vector2f(-3,4),p+sf::Vector2f(-4,-2)},dark?sf::Color(121,52,167):sf::Color(48,166,203));line(load+p+sf::Vector2f(0,-6),load+p+sf::Vector2f(0,3),dark?sf::Color(245,128,214):sf::Color(171,240,255),1);}
   }else{
    // Stitched leather pouch: asymmetric silhouette, folds, cinched neck and a few visible nuggets.
    oval(load+sf::Vector2f(1,7),{1,.35f},9,{38,28,17,85});
    polygon({{-5,-8},{3,-9},{7,-3},{9,4},{6,10},{0,12},{-7,9},{-10,3},{-8,-3}}, {71,47,28});
    polygon({{-4,-7},{2,-8},{6,-2},{7,4},{4,9},{-2,10},{-7,6},{-8,1}}, {133,94,50});
    polygon({{-4,-5},{-1,-6},{1,-1},{0,7},{-4,6},{-6,2}}, {171,126,67});
    line(load+sf::Vector2f(3,-3),load+sf::Vector2f(4,6),{91,59,32},1);
    for(int j=0;j<4;++j)line(load+sf::Vector2f(-5+j*2.f,8),load+sf::Vector2f(-4+j*2.f,8.5f),{205,169,107},.6f);
    oval(load+sf::Vector2f(-1,-7),{1,.4f},4.5f,{48,36,22});
    for(int j=0;j<3;++j){sf::Vector2f n=load+sf::Vector2f(-3+j*2.7f,-8-(j%2)*2.f);oval(n,{1,.75f},2.1f,{143,98,24});oval(n+sf::Vector2f(-.5f,-.7f),{1,.65f},1.5f,{242,199,74});}
    line(load+sf::Vector2f(-5,-5),load+sf::Vector2f(4,-5),{197,167,108},1.4f);line(load+sf::Vector2f(2,-5),load+sf::Vector2f(5,0),{190,154,94},1);
   }
  }
  bool tool=action==A::Mine||action==A::Chop||action==A::Build||action==A::Repair||action==A::Fight;
  if(tool){float stroke=pose.stroke,angle=-1.1f+stroke*1.15f;sf::Vector2f grip=e.pos+sf::Vector2f(facing*13,-22+pose.bob),tip=grip+sf::Vector2f(facing*std::cos(angle)*28,std::sin(angle)*28);
   // Compact timber handle with a wrapped grip and a forged, bevelled tool head.
   tip=grip+sf::Vector2f(facing*std::cos(angle)*22,std::sin(angle)*22);
   line(grip,tip,dark?sf::Color(29,23,36):sf::Color(65,43,27),3.6f);line(grip+sf::Vector2f(0,-.7f),tip+sf::Vector2f(0,-.7f),dark?sf::Color(100,71,104):sf::Color(169,120,64),1.8f);
   auto shaft=tip-grip;line(grip,grip+shaft*.24f,{92,64,43},4);
   sf::Transform toolTransform;toolTransform.translate(tip).rotate(facing*angle*180/3.14159265f+90);
   auto facet=[&](std::initializer_list<sf::Vector2f> points,sf::Color color){sf::ConvexShape shape(points.size());int i=0;for(auto point:points)shape.setPoint(i++,toolTransform.transformPoint(point));shape.setFillColor(color);target.draw(shape);};
   if(dark&&action==A::Mine){
    // Hooked obsidian pick, red binding and a cold violet cutting edge.
    facet({{-15,6},{-11,-3},{-4,-6},{3,-5},{10,-1},{13,7},{7,2},{1,-1},{-5,-2},{-10,1}}, {27,22,38});
    facet({{-15,6},{-11,-3},{-4,-6},{3,-5},{10,-1},{8,0},{1,-3},{-5,-3},{-10,0}}, {122,103,154});
    facet({{-3,-4},{2,-4},{3,1},{-2,2}}, {131,39,40});
    facet({{-1,-4},{1,-4},{1,0},{0,1}}, {245,113,62});
   }else if(dark&&(action==A::Build||action==A::Repair)){
    // Horned forge hammer: heavier silhouette than the human steel mallet.
    facet({{-10,-5},{-6,-6},{6,-6},{10,-3},{9,4},{5,7},{-7,5},{-11,2}}, {29,25,37});
    facet({{-10,-5},{-6,-6},{6,-6},{10,-3},{-5,-2}}, {110,89,121});
    facet({{-5,-2},{10,-3},{9,4},{5,7},{-5,4}}, {63,47,66});
    facet({{-10,-5},{-13,-9},{-11,-1},{-9,1}}, {157,142,161});
    facet({{7,-5},{12,-8},{10,-1}}, {157,142,161});
    facet({{-3,-2},{1,-4},{5,-1},{4,3},{0,5},{-3,2}}, {132,28,37});
    facet({{0,-2},{2,-1},{2,2},{0,3},{-1,1}}, {255,125,61});
   }else if(action==A::Mine){facet({{-12,1},{-9,-2},{0,-4},{8,-2},{12,2},{4,0},{-4,0}}, {67,78,81});facet({{-10,-1},{0,-3},{8,-1},{3,-1},{-4,0}}, {183,194,193});}
   else if(action==A::Build||action==A::Repair){
    facet({{-7,-3},{5,-3},{8,-1},{8,3},{5,5},{-7,4},{-9,2},{-9,-1}}, {55,66,70});
    facet({{-7,-3},{5,-3},{8,-1},{-5,0},{-9,-1}}, {202,211,207});
    facet({{-5,0},{8,-1},{8,3},{5,4},{-5,3}}, {119,137,141});
    facet({{-9,-1},{-5,0},{-5,3},{-8,3}}, {226,229,214});
   }else{facet({{-3,-4},{5,-5},{11,-1},{10,6},{3,5},{-3,2}}, {66,80,83});facet({{5,-4},{10,-1},{9,5},{6,4}}, {211,220,214});facet({{-2,-3},{5,-4},{6,3},{-2,1}}, {132,153,155});}
   sf::CircleShape hand(2.5f,9);hand.setOrigin(2.5f,2.5f);hand.setPosition(grip);hand.setFillColor(dark?sf::Color(133,57,47):sf::Color(174,133,91));target.draw(hand);
   if(action!=A::Fight&&pose.cycle>.60f&&pose.cycle<.82f){for(int j=0;j<4;++j){float age=(pose.cycle-.60f)/.22f;sf::CircleShape chip(action==A::Chop?2.4f:1.8f,action==A::Chop?3:5);chip.setPosition(tip+sf::Vector2f(facing*(age*22+j*3),-age*(28+j*5)+age*age*36));chip.setFillColor(action==A::Chop?sf::Color(195,151,88,190):action==A::Mine?sf::Color(246,211,111,200):sf::Color(215,196,162,150));target.draw(chip);}}
   if(action==A::Build||action==A::Repair){sf::CircleShape dust(8+pose.cycle*12,16);dust.setScale(1,.35f);dust.setPosition(e.pos+sf::Vector2f(facing*18,0));dust.setFillColor({182,155,115,static_cast<sf::Uint8>((1-pose.cycle)*55)});target.draw(dust);}
  }
  if((action==A::Walk||action==A::Carry)&&std::fmod(v.stride,1.f)<.25f){sf::CircleShape step(3,12);step.setScale(1,.4f);step.setPosition(e.pos+sf::Vector2f(-facing*5,12));step.setFillColor({185,161,114,60});target.draw(step);}
 }
 if(world_.modern()&&!alpha::mobile(e.kind)&&!alpha::resource(e.kind)&&e.build<=0&&e.level>=2){
  bool dark=e.team>=0&&world_.faction[e.team]==1;float spread=52+e.level*4.f;
  for(int side:{-1,1}){auto p=e.pos+sf::Vector2f(side*spread,25);line(p,p+sf::Vector2f(0,-30-e.level*9.f),dark?sf::Color(76,57,82):sf::Color(147,158,155),5);sf::CircleShape cap(e.level>=4?6.f:4.f,e.level>=4?4:3);cap.setOrigin(cap.getRadius(),cap.getRadius());cap.setPosition(p+sf::Vector2f(0,-30-e.level*9.f));cap.setFillColor(dark?sf::Color(218,66,113):sf::Color(223,194,102));target.draw(cap);}
  for(int n=0;n<e.level-1;++n){sf::CircleShape seal(3.5f,4);seal.setOrigin(3.5f,3.5f);seal.setPosition(e.pos+sf::Vector2f(-9+n*9.f,40));seal.setFillColor(dark?sf::Color(224,82,113):sf::Color(240,211,120));target.draw(seal);}
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
