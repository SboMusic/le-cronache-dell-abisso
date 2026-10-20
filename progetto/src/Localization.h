#pragma once
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
namespace loc {
inline bool english=false;
inline std::string decode(std::string s){size_t n=0;while((n=s.find("\\n",n))!=std::string::npos){s.replace(n,2,"\n");++n;}return s;}
inline const std::vector<std::pair<std::string,std::string>>& dictionary(){
 static const auto data=[](){std::vector<std::pair<std::string,std::string>> v;std::ifstream in("assets/localization/en.tsv");std::string line;while(std::getline(in,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();auto p=line.find('\t');if(p!=std::string::npos)v.emplace_back(decode(line.substr(0,p)),decode(line.substr(p+1)));}std::stable_sort(v.begin(),v.end(),[](auto& a,auto& b){return a.first.size()>b.first.size();});return v;}();return data;
}
inline std::string tr(const std::string& s){if(!english)return s;static std::unordered_map<std::string,std::string> cache;auto hit=cache.find(s);if(hit!=cache.end())return hit->second;if(cache.size()>4096)cache.clear();const auto& d=dictionary();for(auto& item:d)if(s==item.first){cache[s]=item.second;return item.second;}std::string out;for(size_t i=0;i<s.size();){bool found=false;for(auto& item:d){if(item.first.empty()||s.compare(i,item.first.size(),item.first)!=0)continue;
 auto letter=[](char c){return (c>='a'&&c<='z')||(c>='A'&&c<='Z');};size_t end=i+item.first.size();if(i&&letter(s[i-1])&&letter(item.first.front()))continue;if(end<s.size()&&letter(s[end])&&letter(item.first.back()))continue;out+=item.second;i=end;found=true;break;}if(!found)out+=s[i++];}cache[s]=out;return out;}
inline std::string title(){return english?"Chronicles of the Abyss":"Le Cronache dell'Abisso";}
}
