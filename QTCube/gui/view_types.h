#ifndef VIEW_TYPES
#define VIEW_TYPES
#include <QString>
#include <map>
namespace SEP{
class view_types{
  public:
  view_types();
  std::map<QString,bool> *return_views(){return &types;}




  std::map<QString,bool>  types;


};
}
#endif

