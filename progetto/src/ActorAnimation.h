#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
namespace presentation {
inline float constructionProgress(float remaining,float total){return remaining<=0?1.f:std::clamp(1-remaining/std::max(.001f,total),0.f,1.f);}
struct CombatPose {float progress=0,reach=0,lean=0;int frame=0;bool impact=false;};
inline CombatPose combatPose(float remaining,float total){
 CombatPose p;if(remaining<=0)return p;p.progress=std::clamp(1-remaining/std::max(.001f,total),0.f,1.f);
 // Damage is applied at the start of the simulation cooldown: the visible strike begins there too.
 float recover=std::max(0.f,1-p.progress/.42f);p.reach=8*recover*recover;p.lean=9*recover;p.frame=p.progress<.27f?3:0;p.impact=p.progress<.16f;return p;
}
inline void drawConstruction(sf::RenderTarget& target,sf::Sprite sprite,float progress){
 progress=std::clamp(progress,0.f,1.f);if(progress>=1){target.draw(sprite);return;}
 auto rect=sprite.getTextureRect();auto tint=sprite.getColor();const float cut=rect.height*(1-progress);sf::VertexArray fill(sf::Quads);auto transform=sprite.getTransform();
 // Non-overlapping bands prevent the ghost from accumulating alpha beneath the finished artwork.
 float feather=std::min(10.f,rect.height*progress);
 for(int band=0;band<3;++band){float y0=band==0?0:band==1?cut:cut+feather,y1=band==0?cut:band==1?cut+feather:float(rect.height);if(y1<=y0)continue;
  for(int i=0;i<4;++i){float x=(i==1||i==2)?float(rect.width):0.f,y=i>=2?y1:y0;auto c=tint;if(band==0||(band==1&&i<2))c.a=static_cast<sf::Uint8>(tint.a*.19f);fill.append(sf::Vertex(transform.transformPoint(x,y),c,{rect.left+x,rect.top+y}));}
 }
 sf::RenderStates state;state.texture=sprite.getTexture();target.draw(fill,state);
}
inline void drawActor(sf::RenderTarget& target,const sf::Sprite& sprite,float stride,bool moving,float work,float facing){
 auto rect=sprite.getTextureRect();sf::VertexArray mesh(sf::Quads);auto transform=sprite.getTransform();
 // Articulated lower body and shoulders. A planted unit has no residual marching motion.
 for(int row=0;row<16;++row)for(int col=0;col<8;++col)for(int corner=0;corner<4;++corner){
  float u=(col+((corner==1||corner==2)?1.f:0.f))/8.f,v=(row+(corner>=2?1.f:0.f))/16.f;
  float x=u*rect.width,y=v*rect.height,leg=std::clamp((v-.56f)/.28f,0.f,1.f),side=u*2-1;
  float step=moving?std::sin(stride*3.14159265f):0.f;
  x+=step*leg*side*rect.width*.035f;y-=std::max(0.f,step*side)*leg*rect.height*.035f;
  float shoulder=std::max(0.f,1-std::abs(v-.42f)/.21f)*std::abs(u-.5f)*2;
  x+=facing*work*shoulder*rect.width*.035f;
  // Counter-swing at the shoulders, smooth knee flexion and a steady torso.
  x-=step*side*shoulder*rect.width*.018f;
  float knee=std::max(0.f,1-std::abs(v-.72f)/.13f);y+=std::max(0.f,-step*side)*knee*rect.height*.012f;
  mesh.append(sf::Vertex(transform.transformPoint(x,y),sprite.getColor(),{rect.left+u*rect.width,rect.top+v*rect.height}));
 }
 sf::RenderStates state;state.texture=sprite.getTexture();target.draw(mesh,state);
}
}
