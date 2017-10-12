#ifndef HISTORY_H
#define HISTORY_H
#include<QString>
#include <vector>
#include <QObject>
namespace SEP{
class history: public QObject{
  Q_OBJECT
  signals:
	    void actionDetected(std::vector< QString> text);
  public:
  history();
  public:
  void addEvent(std::vector<QString> coms);
  void write_history(QString text);
  void load_history(QString text);

  private:
    std::vector<QString> events;





};
}
#endif
