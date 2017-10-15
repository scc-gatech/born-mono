  #include<su_params.h>
  
  
  int su_params::get_int(const char *arg) const{
    int ret;
     char arg2[256];strcpy(arg2,arg);
    if(!getparint(arg2,&ret)) err("Couldn't grab %s\n",arg2);
    return ret;
  }
  int su_params::get_int(const char *arg, int def) const
   {
    int ret;
 char arg2[256];strcpy(arg2,arg);
    if(!getparint(arg2,&ret)) ret=def;
    return ret;
  }
  
 int *su_params::get_ints(const char *arg,int nmax) const{
    int *ret=new int[nmax];
     char arg2[256];strcpy(arg2,arg);
     int nt=countparval(arg2);
     int *rt=new int[nt];
    if(!getparint(arg2,rt)) err("Couldn't grab %s\n",arg2);
    for(int i=0; i< nt && i < nmax; i++) ret[i]=rt[i];
    delete rt;
    return ret;
  }
  int *su_params::get_ints(const char *arg, int nmax,int *def) const
   {
    int *ret=new int[nmax];
     char arg2[256];strcpy(arg2,arg);
       int nt=countparval(arg2);
        int *rt=new int[nt];
       getparint(arg2,rt) ;
           for(int i=0; i< nt && i < nmax; i++) ret[i]=rt[i];
    delete rt;
  
     for(int i=nt; i < nmax; i++) ret[i]=def[i];

    return ret;
  }
  
  float su_params::get_float(const char *arg) const
  {
   char arg2[256];strcpy(arg2,arg);
    float ret;
    if(!getparfloat(arg2,&ret)) err("couldn't get %s",arg2);
    return ret;
  }
  float su_params::get_float(const char *arg, float def) const
  {
   char arg2[256];strcpy(arg2,arg);
    float ret;
      if(!getparfloat(arg2,&ret)) ret=def;

    return ret;
  }
   std::string su_params::get_string(const char *arg) const
   {
    cwp_String x;
     char arg2[256];strcpy(arg2,arg);
    std::string ret;
    if(!getparstring(arg2,&x)) err("Couldn't grab %s\n",arg);
    ret=x;
    return ret;
  }
     std::string su_params::get_string(const char *arg, const char *def) const
     {
    cwp_String x;
     char arg2[256];strcpy(arg2,arg);
    std::string ret;
      if(!getparstring(arg2,&x)) ret=def;
      else ret=x;

    return ret;
  }
  void su_params::error(const std::string err) const
  {
    char temp_ch[1024];
    sprintf(temp_ch,"%s",err.c_str());
    //err(temp_ch);
    fprintf(stderr,"ERROR:%s\n",temp_ch);
  
  }
  bool su_params::is_input_file(const std::string tag) const
  {
   return true;
 
 }
   bool su_params::is_inout_file(const std::string tag) const
  {
   return true;
 }
  


