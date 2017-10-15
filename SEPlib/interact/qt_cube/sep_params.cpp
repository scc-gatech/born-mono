  #include<sep_params.h>
  
  
  int sep_params::get_int(const char *arg) const{
    int ret;
    char fmt[] = "d";

    if(0==getch(arg,fmt,&ret)) {
       char errmsg[] = "Couldn't grab %s\n";
       seperr(errmsg,arg);
    }
    return ret;
  }
  int sep_params::get_int(const char *arg, int def) const
   {
    int ret;
    char fmt[] = "d";

    int ierr=getch(arg,fmt,&ret);
     if(ierr==0) ret=def;
    

    return ret;
  }
  
 int *sep_params::get_ints(const char *arg,int nmax) const{
    int *ret=new int[nmax];
    char fmt[] = "d";
    

    if(0==getch(arg,fmt,ret)) {
       char errmsg[] = "Couldn't grab %s\n";
       seperr(errmsg,arg);
    }
    return ret;
  }
  int *sep_params::get_ints(const char *arg, int nmax,int *def) const
   {
    int *ret=new int[nmax];
    char fmt[] = "d";

    int ierr=getch(arg,fmt,&ret);
     for(int i=ierr; i < nmax; i++) ret[i]=def[i];

    return ret;
  }
  
  float sep_params::get_float(const char *arg) const
  {
    float ret;
    char fmt[] = "f";
    if(0==getch(arg,fmt,&ret)) {
      char errmsg[] ="Couldn't grab %s\n";
      seperr(errmsg,arg);
    }
    return ret;
  }
  float sep_params::get_float(const char *arg, float def) const
  {
    float ret;
    char fmt[] = "f";
    if(0==getch(arg,fmt,&ret)) ret=def;
    return ret;
  }
   std::string sep_params::get_string(const char *arg) const
   {
    char x[1024];
    std::string ret;
    char fmt[] = "s";
    if(0==getch(arg,fmt,x)) {
      char errmsg[] = "Couldn't grab %s\n";
      seperr(errmsg,arg);
    }
    ret=x;
    return ret;
  }
     std::string sep_params::get_string(const char *arg, const char *def) const
     {
    char x[1024];
    std::string ret;
    char fmt[] = "s";
    int iret=getch(arg,fmt,x);
    if(iret==0) ret=def;
    else ret=x;
    return ret;
  }
  void sep_params::error(const std::string err) const
  {
    char errmsg[] = "%s";
    seperr(errmsg,err.c_str());
  
  }
  bool sep_params::is_input_file(const std::string tag) const
  {
   FILE *junk;
     junk=auxin(tag.c_str());
     if (junk==NULL) return false;
     return true;
 
 }
   bool sep_params::is_inout_file(const std::string tag) const
  {
   FILE *junk;
     junk=auxinout(tag.c_str());
     if (junk==NULL) return false;
     return true;
 
 }
  


