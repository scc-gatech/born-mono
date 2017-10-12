#include "my_colors.h"
/* Defines the colors in the color map*/
using namespace SEP;

 my_colors::my_colors(){
   cvec.push_back("red"); cmap["red"]=Qt::red; cnum["red"]=8; 
   cvec.push_back("cyan"); cmap["cyan"]=Qt::cyan; cnum["cyan"]=24;
   cvec.push_back("blue"); cmap["blue"]=Qt::blue; cnum["blue"]=40;
   cvec.push_back("yellow"); cmap["yellow"]=Qt::yellow; cnum["yellow"]=56;
   cvec.push_back("magenta"); cmap["magenta"]=Qt::magenta; cnum["magenta"]=72;
   cvec.push_back("white"); cmap["white"]=Qt::white; cnum["white"]=88;
   cvec.push_back("green"); cmap["green"]=Qt::green; cnum["green"]=104;
   cvec.push_back("gray"); cmap["gray"]=Qt::gray; cnum["gray"]=120;
   cvec.push_back("black"); cmap["black"]=Qt::black; cnum["black"]=136;
   cvec.push_back("darkred"); cmap["darkred"]=Qt::darkRed; cnum["darkred"]=152;
   cvec.push_back("darkcyan"); cmap["darkcyan"]=Qt::darkCyan; cnum["darkcyan"]=168;
   cvec.push_back("darkblue"); cmap["darkblue"]=Qt::darkBlue; cnum["darkblue"]=184;
   cvec.push_back("darkyellow"); cmap["darkyellow"]=Qt::darkYellow; cnum["darkyellow"]=200;
   cvec.push_back("darkmagenta"); cmap["darkmagenta"]=Qt::darkMagenta; cnum["darkmagenta"]=216;
   cvec.push_back("darkgray"); cmap["darkgray"]=Qt::darkGray; cnum["darkgray"]=232;
   cvec.push_back("darkgreen"); cmap["darkgreen"]=Qt::darkGreen; cnum["darkgreen"]=248;
   for(int i=0; i < (int)cvec.size(); i++){
     loc[cvec[i]]=i;
     cvec_big.push_back(cvec[i]);
     cmap_big[cvec[i]]=cmap[cvec[i]];
   }
   for(int i=2; i < 4; i++){
   
     QString ex=QString::number(i);
     cvec_big.push_back("red"+ex); cmap_big["red"+ex]=Qt::red; 
     cvec_big.push_back("cyan"+ex); cmap_big["cyan"+ex]=Qt::cyan; 
     cvec_big.push_back("blue"+ex); cmap_big["blue"+ex]=Qt::blue; 
     cvec_big.push_back("yellow"+ex); cmap_big["yellow"+ex]=Qt::yellow; 
     cvec_big.push_back("magenta"+ex); cmap_big["magenta"+ex]=Qt::magenta; 
     cvec_big.push_back("white"+ex); cmap_big["white"+ex]=Qt::white;
     cvec_big.push_back("green"+ex); cmap_big["green"+ex]=Qt::green; 
     cvec_big.push_back("gray"+ex); cmap_big["gray"+ex]=Qt::gray; 
     cvec_big.push_back("black"+ex); cmap_big["black"+ex]=Qt::black; 
     cvec_big.push_back("darkred"+ex); cmap_big["darkred"+ex]=Qt::darkRed;
   cvec_big.push_back("darkcyan"+ex); cmap_big["darkcyan"+ex]=Qt::darkCyan; 
   cvec_big.push_back("darkblue"+ex); cmap_big["darkblue"+ex]=Qt::darkBlue; 
   cvec_big.push_back("darkyellow"+ex); cmap_big["darkyellow"+ex]=Qt::darkYellow; 
   cvec_big.push_back("darkmagenta"+ex); cmap_big["darkmagenta"+ex]=Qt::darkMagenta; 
   cvec_big.push_back("darkgray"+ex);cmap_big["darkgray"+ex]=Qt::darkGray;
   cvec_big.push_back("darkgreen"+ex); cmap_big["darkgreen"+ex]=Qt::darkGreen;
   }
   
   
 }
 