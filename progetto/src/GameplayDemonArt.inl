// Original faction atlases: order is independent from the human 4x4 atlas layout.
int Gameplay::demonTile(alpha::Kind k)const{
 using K=alpha::Kind;static const K units[]={K::Worker,K::Soldier,K::Archer,K::Ranger,K::Knight,K::Guardian,K::Siege,K::Demolition,K::Mage,K::Warlock,K::Healer,K::Scout,K::Warbird,K::Tanker,K::Transport,K::Destroyer,K::Battleship,K::Submarine,K::Spirit};
 static const K buildings[]={K::Hall,K::Barracks,K::Farm,K::LumberMill,K::Forge,K::Stable,K::Sanctuary,K::Academy,K::Workshop,K::Aviary,K::WatchTower,K::GuardTower,K::CannonTower,K::Shipyard,K::Foundry,K::Refinery,K::OilRig,K::Wall};
 if(alpha::mobile(k)){for(int i=0;i<19;++i)if(units[i]==k)return i;}else for(int i=0;i<18;++i)if(buildings[i]==k)return i;return -1;
}
void Gameplay::loadDemonArt(){
 const char* paths[]={"assets/demoni-v1/unita-v1.png","assets/demoni-v1/strutture-v1.png"};
 for(int sheet=0;sheet<2;++sheet){demonReady_[sheet]=demonArt_[sheet].loadFromFile(paths[sheet]);if(!demonReady_[sheet])throw std::runtime_error(std::string("Artwork demoni mancante: ")+paths[sheet]);demonArt_[sheet].setSmooth(true);auto image=demonArt_[sheet].copyToImage();auto size=image.getSize();
  for(int n=0;n<int(alpha::Kind::Count);++n){auto kind=alpha::Kind(n);if((alpha::mobile(kind)?0:1)!=sheet)continue;int tile=demonTile(kind);if(tile<0)continue;int col=tile%5,row=tile/5;int left=col*size.x/5,top=row*size.y/4,right=(col+1)*size.x/5,bottom=(row+1)*size.y/4;
   // Trim transparent cell padding when using the sprite; preserve the source PNG unchanged.
   int l=right,t=bottom,r=left,b=top;for(int y=top;y<bottom;++y)for(int x=left;x<right;++x)if(image.getPixel(x,y).a>80){l=std::min(l,x);t=std::min(t,y);r=std::max(r,x+1);b=std::max(b,y+1);}
   if(r<=l||b<=t)throw std::runtime_error("Cella demoni vuota");demonRects_[n]={l,t,r-l,b-t};
  }
 }
}
