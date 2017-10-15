#include "stdafx.h"
#include "dm.h"

using namespace Yang;

dm::dm(const string& _tag, typeIO _tIO, const string& szInfo, bool bNoDummy): esize(4) {
   FUNC_TRACE_BEG(dm::dm)
   this->tag =_tag; tIO = _tIO;
	m_szInfo = szInfo;
	if((tag.empty()||!tagExist(this->tag)) && !bNoDummy)
	{
	   if (!canOutput()) {//only output dm obj can be dummy
		  seperr("tag %s corresponds to an input data that is dummy !",tag.c_str());
	   }
	   m_bDummy = true;
	   //dbPrt("Dummy dm Object %s\n", m_szInfo.c_str());
	}
	else {
	   m_bDummy = false;
	   
	}

	//if it is used only for input, then the tag can not be dummy.
	if (this->tIO == IN) {
		VERIFY(!m_bDummy);
	}
	if (!m_bDummy) {
	   VERIFY(this->tIO == dm::IN || this->tIO== dm::OUT);
	}
	FUNC_TRACE_END()
}

axis dm::read_axis(int iax) // iax is 1 based
{
   FUNC_TRACE_BEG(dm::read_axis)
	axis a;
	char l[128];
	if(0!=sep_get_data_axis_par(tag.c_str(),&iax,&a.n,&a.o,&a.d,l)) 
	   seperr("trouble getting axis %d from tag %s \n",iax,tag.c_str());
	a.label=l;
	FUNC_TRACE_END(a)
}

void dm::write_axis(int iax, axis a){
   if(!m_bDummy) {
	   char l[128];
	   strcpy(l,a.label.c_str());
	   if(0!=sep_put_data_axis_par(tag.c_str(),&iax,&a.n,&a.o,&a.d,l)) {
       seperr("trouble writing axis %d to tag %s\n",iax,tag.c_str());
     }
   }
}

//Get the number of data axes, this one is different that it count in the axes that are considered
//dummy in sep_get_number_data_axes function
int dm::getNumDataAxes(const char* sztag, bool bRemoveDummyAxis) {
   int n;
   if (0 != sep_get_number_data_axes(sztag,&n)) {
	  seperr("trouble reading number of data axes from tag %s \n",sztag);
   }
   //Test how many dummy axes are there.
   int iax = n+1;

   if(!bRemoveDummyAxis) //Consider the axis have n=1 are not dummy, thus need to find all of them, this is default
   {
	  int nval, dval;
	  char arg[3];
	  bool bCantGrab_n;
	  bool bCanGrab_d;
	  do {
		 //Test if we can grab n#iax
		 sprintf(arg,"n%1d",iax); bCantGrab_n =  (0==auxpar((char*)arg,"d",&nval,sztag));
		 sprintf(arg,"d%1d",iax); bCanGrab_d  =  (0!=auxpar((char*)arg,"d",&dval,sztag));
		 //if can grab both n and d, then this is an axis that should not be treated as dummy
		 if(!bCantGrab_n && bCanGrab_d) { 
		   VERIFY(nval==1); 
		 } else {
		   break;
		 }
		 iax++;
	  } while (1);
   } //else do nothing
   
   return (iax-1);
}


int dm::get_int(const char *arg) const
{

    int ret;
    if(0==auxpar((char*)arg,"d",&ret,tag.c_str()))
      seperr("Couldn't grab %s from tag %s \n",arg, tag.c_str());
    return ret;
  }

int dm::get_int(const char *arg, int def) const
   {
    int ret;

    int ierr=auxpar((char*)arg,"d",&ret,(char*)tag.c_str());
     if(ierr==0) ret=def;
    
    return ret;
  }


  float dm::get_float(const char *arg) const
  {
    float ret;
    if(0==auxpar((char*)arg,"f",&ret,(char*)tag.c_str())) 
	   seperr("Couldn't grab %s from tag %s\n",arg,tag.c_str());
    return ret;
  }
  float dm::get_float(const char *arg, float def) const
  {
    float ret;
    if(0==auxpar((char*)arg,"f",&ret,(char*)tag.c_str())) ret=def;
    return ret;
  }
  
   std::string dm::get_string(const char* arg) const
   {
    char x[1024];
    std::string ret;
    if(0==auxpar(arg,"s",&x,(char*)tag.c_str())) 
      seperr("Couldn't grab %s from tag %s\n",arg, tag.c_str());
    ret=x;
    return ret;
  }

  std::string dm::get_string(const char* arg,const std::string& def) const
     {
    char x[1024];
    std::string ret;
    int iret=auxpar(arg,"s",x,(char*)tag.c_str());
    if(iret==0) ret=def;
    else ret=x;
    return ret;
  };
  
   void dm::put_int(const char *arg, int val) const {
	  if(!m_bDummy) {
		 if (0==auxputch(arg,"d",&val,tag.c_str())) {
			   seperr("Couldn't write %s to tag %s\n",arg, tag.c_str());
		 }
	  }
   }
   
   void	dm::put_float(const char *arg, float val) const {
	  if(!m_bDummy) {
		  if (0==auxputch(arg,"f",&val,tag.c_str())) {
				seperr("Couldn't write %s to tag %s\n",arg, tag.c_str());
		  }
	  }
   }
   void dm::put_string(const char *arg, const std::string& val) const {
	  if(!m_bDummy) {
		  if (0==auxputch(arg,"s",val.c_str(),tag.c_str())) {
				seperr("Couldn't write %s to tag %s\n",arg, tag.c_str());
		  }
	   }
   }


