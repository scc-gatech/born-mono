#include "change.h"
#include <qstring.h>
change::change(int n,std::vector <QString> commands, QString fr, QString t)
{
 from=fr; to=t;
 for(int i=0; i < n; i++) coms.push_back(commands[i]);
}

QString change::output_for(){
  QString a=coms[0];
  for(int i=1;i<(int)coms.size();i++) a=a+"-"+coms[i];
  return a+"-"+to;
}
QString change::output_back(){
  QString a=coms[0];
  for(int i=1;i<(int)coms.size();i++) a=a+"-"+coms[i];
  return a+"-"+from;
}
