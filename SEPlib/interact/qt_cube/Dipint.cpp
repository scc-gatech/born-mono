#include<flatten.h>
#include<sregf.h>
#include<sep_params.h>
void resample_down(int *n, int j, int *ns, float *tbig, float *tsmall, float *dbig,
  float *dsmall, std::vector<ploc> pbig, std::vector<ploc>* psmall);
int jfact(int n,int fact);
void resample_up(int *n, int *ns, int j, float *tsmall, float *tbig);


main(int argc, char **argv){


  sep_params *pars=new sep_params(argc,argv);
  sregf *in=new sregf("in"); 
  axis *ax=in->return_axes(4);
  hypercube_float *buf=new hypercube_float(4,ax);
  in->read_next("in",buf);
  int one=1;putch("n4","d",&one);
  int *n=in->return_ns();
  
  int i[30];
  i[ 0]=73;  i[ 1]=4;  i[ 2]=28;
  i[ 3]=115;  i[ 4]=54;  i[ 5]=28;
  i[ 6]=119;  i[ 7]=87;  i[ 8]=28;
  i[ 9]=106;  i[10]=142;  i[11]=28;
  i[12]=111;  i[13]=258;  i[14]=28;
  i[15]=61;  i[16]=4;  i[17]=53;
  i[18]=107;  i[19]=62;  i[20]=53;
  i[21]=107;  i[22]=153;  i[23]=53;
  i[24]=104;  i[25]=163;  i[26]=2;
  i[27]=122;  i[28]=88;  i[29]=2;
  i[1]=20; i[2]=28; 
  float *tau=new float[n[0]*n[2]*n[1]];
   

 for(int j=0; j < n[0]*n[1]*n[2]; j++) tau[j]=0.;
 int nsmall=(n[0]/jfact(n[0],2))*(n[1]/jfact(n[1],2))*(n[2]/jfact(n[2],2));

 float *tsmall=new float [nsmall], *dsmall=new  float [nsmall*2];
 int ns[3];
 std::vector<ploc> pts, psmall;
 int jsmall[3];
 int niter=pars->get_int("niter",20);
 
 float eps_t=pars->get_float("eps_t",0.);
 for(int ii=0; ii < 10; ii++) pts.push_back(ploc(i[ii*3],i[ii*3+1],i[ii*3+2]));
 if(1==2){
  for(int jsamp=8; jsamp >=2; jsamp=jsamp/2){

    resample_down(n,jsamp,ns,tau,tsmall,buf->vals,dsmall,pts,&psmall);
    for(int ie=0; ie < 3; ie++){
      flatten flat(ns,0.0,psmall[0].i2,psmall[0].i3,10);
      //flat.set_fix(psmall);
      flat.calc_tau(tsmall,dsmall);
    }
// seperr("");
    resample_up(n,ns,jsamp,tsmall,tau);
    srite("out",tau,(int)in->get_n123()*2);

  }
  }
  flatten flat(n,pars->get_float("eps_t",0.),i[1],i[2],pars->get_int("niter",8));
  flat.set_fix(pts);
  flat.calc_tau(tau,buf->vals);
  

  float val=tau[i[0]+i[1]*n[0]+i[2]*n[0]*n[1]]+i[0];
  float *vals=new float[n[0]*n[1]*n[2]];
  float mymax=-9;
  float d;
  for(int i2=0; i2 <  n[2]*n[1]; i2++){
    for(int i1=0; i1 < n[0]; i1++){  
      d=(tau[i1+i2*n[0]]-val)*tau[i1+i2*n[0]];
      if(d > mymax) mymax=d;
      vals[i1+i2*n[0]]=d;
    }
  }
  for(int i2=0; i2 <n[1]*n[0]*n[2]; i2++) vals[i2]=mymax-vals[i2];
  
  srite("out",tau,(int)in->get_n123()*2);
  srite("val",vals,(int)in->get_n123()*2);
  
}
void resample_down(int *n, int j, int *ns, float *tbig, float *tsmall, float *dbig,
  float *dsmall, std::vector<ploc> pbig, std::vector<ploc> *psmall){
   int jdo[3]; 
   for(int i=0; i < 3; i++) {
     jdo[i]=jfact(n[i],j);
     ns[i]=n[i]/jdo[i];
   }
  
  fprintf(stderr,"NS 1 %d %d %d -%d \n",ns[0],ns[1],ns[2],ns[0]*ns[1]*ns[2]);
  int i,ibig;
  int *ax1=new int[2*jdo[0]-1];
  int *ax2=new int[2*jdo[1]-1];
  int *ax3=new int[2*jdo[2]-1];
  int nsz=(2*jdo[0]-1)*(2*jdo[1]-1)*(2*jdo[2]-1);
  float *fmap=new float[nsz];
  fprintf(stderr,"ALLOCATED SIZE %d \n",nsz);
  float scale=1.;
  for(int i=0; i < 3; i++) scale=scale*0.5*(jdo[i]*(jdo[i]-1)+jdo[i]*jdo[i]+1);
  for(int i3=-jdo[2]+1,i=0; i3< jdo[2]; i3++){
    for(int i2=-jdo[1]+1; i2< jdo[1]; i2++){
      for(int i1=-jdo[0]+1; i1 < jdo[0]; i1++,i++){
        fmap[i]=(1.0*(jdo[0]-abs(i1))*(jdo[1]-abs(i2))*(jdo[2]-abs(i3)))/scale;
      }
    }
  }
  fprintf(stderr,"THROUGH 1 \n");
   int jj=0,l,k;
   float t;
   float rat01=((float)jdo[1])/((float)jdo[0]);
   float rat02=((float)jdo[2])/((float)jdo[0]);
   for(int i3=0,i=0; i3 < ns[2]; i3++){
     for(int j3=i3*jdo[2]-jdo[2]+1,k=0; j3 < i3*jdo[2]+jdo[2]; j3++,k++) 
        ax3[k]=MIN(MAX(j3,0),n[2]-1)*n[0]*n[1];
     
     for(int i2=0; i2 < ns[1]; i2++){
       for(int j2=i2*jdo[1]-jdo[1]+1,k=0; j2 < i2*jdo[1]+jdo[1]; j2++,k++) {
         ax2[k]=MIN(MAX(j2,0),n[1]-1)*n[0];
         
         }
       for(int i1=0; i1 < ns[0]; i1++,i++){
         for(int j1=i1*jdo[0]-jdo[0]+1,k=0; j1 < i1*jdo[0]+jdo[0]; j1++,k++) 
           ax1[k]=MIN(MAX(j1,0),n[0]-1);
         dsmall[i]=dsmall[i+ns[0]*ns[1]*ns[2]]=tsmall[i]=0.;
         for(int j3=0,jj=0; j3 < 2*jdo[2]-1; j3++){
           for(int j2=0; j2< 2*jdo[1]-1; j2++){
             for(int j1=0; j1< 2*jdo[0]-1; j1++,jj++){
               l=ax3[j3]+ax2[j2]+ax1[j1];
              // fprintf(stderr,"IN LOOP %d %d %d (%d) %d %d %d (%d) %d %d \n",
               // i1,i2,i3,i,j1,j2,j3,l,jj,ax2[j2]);
               tsmall[i]+=tbig[l]*fmap[jj];
               dsmall[i]+=dbig[l]*fmap[jj];
               dsmall[i+ns[0]*ns[1]*ns[2]]+=dbig[l+n[0]*n[1]*n[2]]*fmap[jj];
             }
           }
         }
         tsmall[i]=tsmall[i]/((float)jdo[0]);
        //  dsmall[i]=dbig[i1*jdo[0]+i2*jdo[1]*n[0]+i3*jdo[2]*n[0]*n[1]]*rat01;
          // dsmall[i+ns[0]*ns[1]*ns[2]]=
            //  dbig[i1*jdo[0]+i2*jdo[1]*n[0]+i3*jdo[2]*n[0]*n[1]+n[0]*n[1]*n[2]]*rat02;
            dsmall[i]=dsmall[i]*rat01;
            dsmall[i+ns[0]*ns[1]*ns[2]]*=rat02;
     //  fprintf(stderr,"CH ECK D %d %f %f \n",i,dsmall[i],dsmall[i+ns[0]*ns[1]*ns[2]]);
       }
     }
  }
  delete[]ax2; delete []ax3;  
  delete []fmap;delete [] ax1;
   fprintf(stderr,"WRITE SIZE %d %d %d %d \n",ns[0],ns[1],ns[2]);
  
   psmall->clear();
   for(i=0; i < pbig.size(); i++){
     psmall->push_back(ploc(
     pbig[i].i1/jdo[0],
     pbig[i].i2/jdo[1],
     pbig[i].i3/jdo[2]));
   }
  
  if(ns[0]==48) {srite("xx.H",tsmall,ns[0]*ns[1]*ns[2]*4);
  
 }
 //seperr("");
}

void resample_up(int *n, int *ns, int j, float *tsmall, float *tbig){
//For now do linear probably should think about sinc
  int jdo[3]; for(int i=0; i < 3; i++) jdo[i]=jfact(n[i],j);
  
  int i,b1,e1,b2,e2,b3,e3;
  float f1,f2,f3;
  for(int i3=0,i=0; i3 < n[2]; i3++){
    f3=(i3*1.)/(jdo[2]*1.);b3=f3;f3-=b3;e3=MIN(ns[2]-1,b3+1);
    b3=b3*ns[0]*ns[1]; e3=e3*ns[1]*ns[0];
    for(int i2=0; i2 < n[1]; i2++){
      f2=(i2*1.)/(jdo[1]*1.);b2=f2; f2-=b2;e2=MIN(ns[1]-1,b2+1);
      b2=b2*ns[0]; e2=e2*ns[0];
      for(int i1=0; i1 < n[0];i1++,i++){
        f1=(i1*1.)/(jdo[0]*1.);
        b1=f1; f1-=b1; e1=MIN(ns[0]-1,b1+1);
        tbig[i]=((1.-f3)*(1.-f2)*(1.-f1)*tsmall[b1+b2+b3]+
                (1.-f3)*(1.-f2)*(   f1)*tsmall [e1+b2+b3]+
                (1.-f3)*(   f2)*(1.-f1)*tsmall [b1+e2+b3]+
                (1.-f3)*(   f2)*(   f1)*tsmall [e1+e2+b3]+
                (   f3)*(1.-f2)*(1.-f1)*tsmall [b1+b2+e3]+
                (   f3)*(1.-f2)*(   f1)*tsmall [e1+b2+e3]+
                (   f3)*(   f2)*(1.-f1)*tsmall [b1+e2+e3]+
                (   f3)*(   f2)*(   f1)*tsmall [e1+e2+e3])*(1.*jdo[0]);
     //   tsmall[is1+is2*ns[0]+is3*ns[0]*ns[1]],tbig[i]
      }
    }
  }
  // srite("xx.H",tbig,n[0]*n[1]*n[2]*4);
}
int jfact(int n,int fact){
  int nt=n/fact;
  int j;
  if(nt<24) j=n/24;
  else j=fact;
  return j;
}

