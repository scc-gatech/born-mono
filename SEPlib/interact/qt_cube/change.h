#include <string>
#include <vector>
#include <qstring.h>
#ifndef CHANGE_H
#define CHANGE_H 1
class change{
  public:
    change(int n,std::vector<QString> command, QString fr, QString t);
    QString output_for();
    QString output_back();
  private:
    std::vector<QString> coms;
    QString from,to;
    
 };
#endif
