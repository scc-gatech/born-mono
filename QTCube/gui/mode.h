#ifndef MODE_H
#define MODE_H 1
#include<qstring.h>
#include<vector>
namespace SEP{
class mode{
  public:
    mode(QString init_mode);
    void change_mode(QString);
    QString return_mode();

  private:
    std::vector<QString> modes;
    QString cur_mode;
};

}
#endif
