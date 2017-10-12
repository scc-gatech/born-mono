#include "history.h"
#include <stdio.h>

using namespace SEP;
history::history(){


}

void history::write_history(QString text){
  FILE *fd;
  fd=fopen(text.toAscii().constData(),"w");
  QString com;
  for(int i=0;  i<(int)events.size();i++) fprintf(fd,"%s\n",events[i].toAscii().constData());
  fclose(fd);
}

void history::addEvent(std::vector<QString> coms){

  QString x=coms[0];
  for(int i=1; i < coms.size(); i++) x+="::"+coms[i];
  events.push_back(x);

}

void history::load_history(QString text){
    FILE *fd;
  char line[2048];
  QString ln;
  QString col;
  std::vector<QString> com2;
  com2.push_back("50");
  com2.push_back(" ");
 if ((fd=fopen(text.toAscii().constData(),"r")) == NULL) fprintf(stderr,"PROBLEM OPENING \n");
 std::vector<QString> lines;
  while (!feof(fd)){
    fgets(line, 1000, fd);
    if (0!=strncmp (line,"#",1)) {
      ln=line;
      ln=ln.trimmed();
//      ln.remove(ln.length()-1,ln.length());
      lines.push_back(ln);
    }
  }
  if(lines.size()>1){
  if(lines[lines.size()-1].contains(lines[lines.size()-2])) lines.erase(lines.begin()+lines.size()-1);
  }
  for(int i=0; i< (int)lines.size(); i++){
      ln=lines[i];
      int nelem=ln.count("::");
      std::vector<QString> command;
      for(int i=0; i < nelem+1; i++) command.push_back(ln.section("::",i,i));
      emit actionDetected(command);
  }
  fclose (fd);
}
