// Native framebuffer tests: exercise the same renderer used in the match.
{
 auto preserved=world_;int oldFaction=world_.faction[0];sf::RenderTexture canvas;require(canvas.create(280,240),"animation framebuffer initializes");
 auto snapshot=[&](alpha::Entity actor,Visual visual,bool activity=true){canvas.clear(sf::Color::Transparent);drawArt(canvas,actor,visual,1);if(activity)drawActivity(canvas,actor,visual);canvas.display();return canvas.getTexture().copyToImage();};
 auto different=[](const sf::Image& a,const sf::Image& b){return !std::equal(a.getPixelsPtr(),a.getPixelsPtr()+a.getSize().x*a.getSize().y*4,b.getPixelsPtr());};
 auto solid=[](const sf::Image& image){int n=0;for(unsigned y=0;y<image.getSize().y;++y)for(unsigned x=0;x<image.getSize().x;++x)if(image.getPixel(x,y).a>180)++n;return n;};
 // Real building atlases, both faction pipelines, multiple reveal stages.
 for(int faction=0;faction<2;++faction){world_.faction[0]=faction;for(int k=0;k<int(alpha::Kind::Count);++k){auto kind=alpha::Kind(k);if(alpha::mobile(kind)||alpha::resource(kind)||!textureFor(kind,faction))continue;
  alpha::Entity building;building.kind=kind;building.pos={140,155};building.buildTotal=20;Visual visual;visual.initialized=true;
  std::array<int,4> counts;for(int stage=0;stage<4;++stage){float p=stage==3?1.f:.1f+stage*.4f;building.build=20*(1-p);visual.buildProgress=p;counts[stage]=solid(snapshot(building,visual,false));}
  if(!(counts[1]>=counts[0]&&counts[2]>counts[0]&&counts[3]>=counts[2]))std::cout<<"BUILD COUNTS "<<faction<<" "<<k<<" "<<counts[0]<<" "<<counts[1]<<" "<<counts[2]<<" "<<counts[3]<<std::endl;
  require(counts[1]>=counts[0]&&counts[2]>counts[0]&&counts[3]>=counts[2],"building color fills monotonically through real faction art");
 }}
 // Paused and resumed actual foundations retain their real progress; loading needs no save migration.
 world_=preserved;int foundation=world_.add(alpha::Kind::Farm,0,{1200,800});auto b=world_.get(foundation);b->build=b->buildTotal=20;require(b&&b->build>0,"foundation remains unfinished for animation test");
 visuals_.clear();updateVisuals(0);float stopped=visuals_[foundation].buildProgress;updateVisuals(.1f);require(std::abs(visuals_[foundation].buildProgress-stopped)<.0001f,"stationary construction never fills itself");
 b->build=std::max(.1f,b->build-2);updateVisuals(.05f);require(visuals_[foundation].buildProgress>stopped,"building fill follows real construction progress");
 b->build=0;updateVisuals(.05f);require(visuals_[foundation].buildProgress==1,"finished building becomes fully solid");world_=preserved;
 int tree=world_.add(alpha::Kind::Forest,-1,{200,140}),mine=world_.add(alpha::Kind::Mine,-1,{200,140}),job=world_.add(alpha::Kind::Farm,0,{200,140}),enemy=world_.add(alpha::Kind::Soldier,1,{200,180});world_.get(job)->build=20;
 auto actor=[&](int role,float time){alpha::Entity e;e.id=120;e.team=0;e.hp=e.maxHp=80;e.pos={140,180};e.kind=role>=5?alpha::Kind::Soldier:alpha::Kind::Worker;
  if(role==0||role==5)e.order=alpha::Order::Move;
  if(role==1||role==2){e.order=alpha::Order::Gather;e.target=role==1?tree:mine;e.work=1;}
  if(role==3){e.order=alpha::Order::Build;e.target=job;}
  if(role==4||role==6){e.order=alpha::Order::Attack;e.target=enemy;e.cooldown=alpha::def(e.kind).cooldown*(1-std::fmod(time,.95f));}return e;};
 for(int faction=0;faction<2;++faction){world_.faction[0]=faction;for(int role=0;role<7;++role){Visual a;a.initialized=true;a.phase=.07f;a.stride=.15f;a.speed=role==0||role==5?70.f:0.f;Visual b=a;b.phase=.33f;b.stride=1.3f;
  require(different(snapshot(actor(role,.07f),a),snapshot(actor(role,.33f),b)),"worker and soldier actions change native pixels for both factions");}
  auto idle=actor(5,0);idle.order=alpha::Order::Idle;Visual a;a.initialized=true;Visual b=a;b.stride=1.3f;require(!different(snapshot(idle,a),snapshot(idle,b)),"stopped soldier does not march in place");
 }
 require(presentation::combatPose(.95f,1).impact&&!presentation::combatPose(.3f,1).impact,"attack trail is limited to real impact and fades during recovery");
 if(std::getenv("FRONTIERE_ANIMATION_CAPTURE")){
  sf::RenderTexture demo;require(demo.create(1200,850),"animation showcase framebuffer initializes");
  const char* roles[]={"Cammino","Taglio legna","Estrazione oro","Costruzione","Attacco operaio","Passo soldato","Attacco soldato"};
  for(int frame=0;frame<24;++frame){float t=frame*.08f,progress=frame/23.f;demo.clear({23,38,42});text(demo,"Anteprima animazioni | " ABYSS_VERSION_TEXT,{24,15},25);
   for(int faction=0;faction<2;++faction){world_.faction[0]=faction;float row=faction?360.f:90.f;text(demo,faction?"STIRPE OSCURA":"UMANI",{24,row-20},19);
    for(int col=0;col<4;++col){alpha::Entity building;building.kind=col%2?alpha::Kind::Barracks:alpha::Kind::Hall;building.pos={200+col*290.f,row+120};building.buildTotal=20;float p=col<2?progress:std::clamp(progress+.3f,0.f,1.f);building.build=20*(1-p);Visual v;v.initialized=true;v.buildProgress=p;drawArt(demo,building,v,1);text(demo,std::to_string(int(p*100))+"%",building.pos+sf::Vector2f(-20,70),19);}
   }
   for(int faction=0;faction<2;++faction){world_.faction[0]=faction;for(int role=0;role<7;++role){auto e=actor(role,t);e.pos={80+role*170.f,690+faction*105.f};Visual v;v.initialized=true;v.phase=t;v.stride=t*3;v.speed=role==0||role==5?70.f:0.f;drawArt(demo,e,v,1);drawActivity(demo,e,v);if(!faction)text(demo,roles[role],{20+role*170.f,710},15);}}
   demo.display();require(demo.getTexture().copyToImage().saveToFile("animation-frame-"+std::to_string(frame)+".png"),"animation showcase frame saved");
  }
 }
 world_=preserved;world_.faction[0]=oldFaction;visuals_.clear();updateVisuals(0);
}
