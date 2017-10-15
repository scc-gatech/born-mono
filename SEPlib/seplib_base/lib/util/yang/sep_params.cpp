#include "stdafx.h"
#include"sep_params.h"
  
  
sep_params::sep_params(int argc, char **argv)
{
initpar(argc,argv);
//verbPrt("mark1");
doc(" ");
//verbPrt("mark2");
}

sep_params::sep_params(int argc, char **argv, char *prog)
{
 initpar(argc,argv);
 doc(prog);
}

int sep_params::get_int(const string& arg) const{
	int ret;  
	if(0==getch(arg.c_str(),"d",&ret)) seperr("Couldn't grab %s\n",arg.c_str());
	return ret;
}

int sep_params::get_int(const string& arg, int def) const
{
	int ret;
	int ierr=getch(arg.c_str(),"d",&ret);
	if(ierr==0) {
		ret=def;
	}
	return ret;
}
  
bool sep_params::get_bool(const string& arg) const
{
	bool ret;
	char cFetch[128];
	bool fetchFail = false;
	//get the param as int type from par files
	if(0==getch(arg.c_str(),"s",cFetch)) {
		 fetchFail = true;
	}
	if(fetchFail) {
		 seperr("Couldn't grab %s\n",arg.c_str());
	}
	ret = true;
	if (cFetch[0]=='0' || cFetch[0]=='n') {
		ret=false;
	}
	else {
		if (cFetch[0]!='1' && cFetch[0]!='y') {
			 prt("bool param entry %s with a non 0/1 value\n",arg.c_str());
		}
	}
	return ret;
}

bool sep_params::get_bool(const string& arg, bool bDef) const
{
  bool ret;
	char cFetch[128];
	bool fetchFail = false;
	//get the param as int type from par files
	if(0==getch(arg.c_str(),"s",cFetch)) {
	   fetchFail = true;
	}
	if(fetchFail) {
	   ret = bDef;
	   return ret;
	}
	
  ret = true;
  if (cFetch[0]=='0' || cFetch[0]=='n') {
	  ret=false;
	}
	else {
		if (cFetch[0]!='1' && cFetch[0]!='y') {
			prt("bool param entry %s with a non 0/1/y/n value\n",arg.c_str());
		}
	}
  return ret;
}
  
 int *sep_params::get_ints(const string& arg,int nmax, int* p_nRead) const{
    int *ret=new int[nmax];
    
		int nRead;
		nRead = getch(arg.c_str(),"d",ret);
		if (nRead>nmax) {
			prt("The nmax in get_ints might not be big enough");
			VERIFY(0);
		}
		
    if(0==nRead) { 
			seperr("Couldn't grab %s\n in sep_param::get_ints",arg.c_str());
		}
		if(p_nRead != NULL) {
			*p_nRead = nRead;
		}
    return ret;
  }
 
  int *sep_params::get_ints(const string&arg, int nmax,int *def, int* p_nRead) const
  {
    int *ret=new int[nmax];

		int nRead;
		nRead = getch(arg.c_str(),"d",ret);
		if (nRead>nmax) {
			prt("The nmax(size of the returned array) in get_ints is too small!!");
			VERIFY(0);
		}
    for(int i=nRead; i < nmax; i++) {
			ret[i]=def[i];
		}
		if(p_nRead != NULL) {
			*p_nRead = nRead;
		}
    return ret;
  }
  
  float sep_params::get_float(const string&arg) const
  {
    float ret;
    if(0==getch(arg.c_str(),"f",&ret)) seperr("Couldn't grab %s\n",arg.c_str());
    return ret;
  }
  float sep_params::get_float(const string&arg, float def) const
  {
    float ret;
    if(0==getch(arg.c_str(),"f",&ret)) ret=def;
    return ret;
  }
   std::string sep_params::get_string(const string&arg) const
   {
    char x[1024];
    std::string ret;
    if(0==getch(arg.c_str(),"s",x)) seperr("Couldn't grab %s\n",arg.c_str());
    ret=x;
    return ret;
  }
  std::string sep_params::get_string(const string&arg, string def) const
  {
    char x[1024];
    std::string ret;
    int iret=getch(arg.c_str(),"s",x);
    if(iret==0) {
			ret=def;
		} else {
			ret=x;
		}
    return ret;
  }
		 		
  void sep_params::error(const std::string& err) const
  {
    seperr("%s",err.c_str());
  
  }
  bool sep_params::is_input_file(const std::string& tag) const
  {
   FILE *junk;
     junk=auxin(tag.c_str());
     if (junk==NULL) return false;
     return true;
 }
   bool sep_params::is_inout_file(const std::string& tag) const
  {
   FILE *junk;
     junk=auxinout(tag.c_str());
     if (junk==NULL) return false;
     return true;
 }
  
//Function that analyze all parameters appeared in command Line, and tell the user the ones that is not collected
//in the program, so the user can realize a typo very quickly. !!!! something to do


