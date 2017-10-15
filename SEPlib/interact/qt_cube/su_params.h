
#ifndef SU_PARAMS_H
#define SU_PARAMS_H 1
#include<float_1d.h>
extern "C" {
#include "par.h"
}
#include<param_func.h>
#include<stdbool.h>
#include<string>


class su_params:public param_func{
  public:
  
    su_params(){} //Default constructor does nothing
    su_params(int argc, char **argv){
     initargs(argc,argv);
  
    }
    su_params(int argc, char **argv, char *prog)
    {
      initargs(argc,argv);
      
    }
    bool is_input_file(const std::string tag) const;
    bool is_inout_file(const std::string tag) const;
    int get_int(const char *arg) const;
    int get_int(const char *arg, int def) const;
    
    
    float get_float(const char *arg, float def) const;
    float get_float(const char *arg) const;
    std::string get_string(const char *arg) const;
    std::string get_string(const char *arg, const char *def) const;
    
    int *get_ints(const char *arg, int nmax) const;
    int *get_ints(const char *arg, int nmax, int *def) const;
    
    int *get_ints(const std::string arg, int nmax) const {
      return this->get_ints(arg.c_str(),nmax);
     }
    int *get_ints(const std::string arg, int nmax, int *def) const {
       return this->get_ints(arg.c_str(),nmax,def);
     }
    int get_int(const std::string arg) const {return this->get_int(arg.c_str());}
    int get_int(const std::string arg, int def) const {
  
       return this->get_int(arg.c_str(),def);}
    float get_float(const std::string arg, float def) const {return this->get_float(arg.c_str(),def);}
    float get_float(const std::string arg) const {return this->get_float(arg.c_str());}
    void error(const std::string) const;
   // std::string get_string(const std::string arg) const {return this->get_string(arg.c_str());}
  //  std::string get_string(const std::string arg, std::string def) const{
  //    return this->get_string(arg.c_str(),def.c_str());}

     
 };
#endif
      

