#include<sep_params.h> 
#include<sregf.h>
#include<hypercube_float.h>
#include<float_1d.h>
#include<float_2d.h>
#include<my_score.h>
#include<viterbi.h>
 
 float_2d *extract_dat(hypercube_float *buf, int n, int f, float *line, int ns);
 void build_line(int np,int *picks, int f, int n, float *ar);
 
 main(int argc, char **argv){
   
   sep_params pars=sep_params(argc,argv);
   sregf sembf=sregf("in");
   hypercube_float semb=hypercube_float(&sembf);
   sreed("in",semb.vals,(int)semb.get_n123()*4);
   int nt=sembf.get_axis(1).n;
   int nv=sembf.get_axis(2).n;
   int half=20;
   
   
   sregf picksf=sregf("picks");
   int npicks=picksf.get_axis(2).n;
   float nail=10.;
   int *picks=new int[2*npicks];
   float *pickf=new float[2*npicks];
   sreed("picks",pickf,8*npicks);
   for(int i=0; i < 2*npicks; i++) picks[i]=.5+pickf[i];
   
 
      int n=picks[2*npicks-2]-picks[0]+1;
  float_1d *line=new float_1d(n);
   build_line(npicks,picks,picks[0],picks[2*npicks-2]-picks[0],line->vals);
   
   int f=picks[0];
   float_2d *sim=extract_dat(&semb,n,f,line->vals,half);
   sim->normalize(1.);
   //Add nails
   for(int i=0; i < npicks;i++)  sim->vals[half+(picks[i*2]-f)*(2*half+1)]=nail;
   
   
   semb_score *myscore = new semb_score(sembf.get_axis(1),sembf.get_axis(2),1.5);
   viterbi *myd=new viterbi(myscore,sim,7,picks[0]);
   myd->calc_score();
   srite("sim",sim->vals,4*(half*2+1)*n);
   std::vector<path> mypath=myd->return_path();
   
   float o1=sembf.get_axis(1).o,d1=semb.get_axis(1).d;
   float o2=sembf.get_axis(2).o,d2=semb.get_axis(2).d;
   
   float  *buf=new float(2*n);
   int eight=8;
   putch("esize","d",&eight);
   putch("n1","d",&n);
   int one=1;putch("n2","d",&one);
   float *trs=new float[2*mypath.size()];
   float *pick=new float[2*mypath.size()];
   for(int i=0; i < mypath.size();i++){
   
     trs[i*2]=(mypath[i].iy+f)*d1+o1;
     trs[i*2+1]=(mypath[i].ix+line->vals[i]-half)*d2+o2;
   // fprintf(stderr,"MY PATH %d %d %d  %f %f\n",i,mypath[i].ix,mypath[i].iy,
    //    trs[i*2],trs[i*2+1]);
     pick[i*2]=trs[i*2];
     pick[i*2+1]=(mypath[i].ix-half)*d2;
     fprintf(stderr,"CHECK IT OUT %d %f %d \n",i,pick[i*2+1],mypath[i].ix);
   }
   srite("path",pick,8*mypath.size());
   srite("out",trs,8*mypath.size());
 }



void build_line(int np,int *picks, int f, int n, float *ar){


  int i2,i;



  int ifirst=9999,ilast=-1,i1;
  fprintf(stderr,"CHECK IT %d %d \n",f,n);
  for(i=0; i< n; i++) ar[i]=-1;

  for( i=0; i < np; i++){
    i1=picks[i*2]-f; i2=picks[2*i+1];
    fprintf(stderr,"FILLING %d \n",i1);
    ar[i1]=i2;  
    if(i1 < ifirst) ifirst=i1;
    if(i1 > ilast) ilast=i1;
    
  }
  if(ilast!=-1) { 
     for(i=0; i <  ifirst; i++) ar[i]=ar[ifirst];
     for(i=ilast ; i<n; i++) ar[i]=ar[ilast];
     i=0;
     float f;
     int beg=ifirst;
     while(i < n-1){
       i=beg+1;
       while(ar[i] < 0.) i++;
       for(int j=beg+1; j < i; j++){
         f=(float)(j-beg)/((float)(i-beg));
         ar[j]=(1.-f)*ar[beg]+f*ar[i];
       }
       beg=i; 
       if(beg==ilast)  i=n;
    }
  }
}


float_2d *extract_dat(hypercube_float *buf, int n, int floc, float *line, int ns){
  int n1,n2,j1,j2;
 long long first=0;

    axis axes[2];
    axes[0]=axis(ns*2+1);
    axes[1]=axis(n);
    float_2d *dat=new float_2d(axes[0],axes[1]);
    int i,nv=buf->get_axis(2).n;
    int  nt=buf->get_axis(1).n;
    float f;
    int ishift;
 // fprintf(stderr,"ALLOC %d %d \n",ns*2+1,n);
   for(int i2=floc; i2 < n+floc; i2++){
      f=line[i2-floc];
      ishift=f; f=f-ishift;
    //   fprintf(stderr,"THE LINE %d %d %f \n",i2,ishift,f);
      for(int i1=0; i1 < ns*2+1; i1++){
        i=ishift-ns+i1;
        if(i < 0) dat->vals[i1+(i2-floc)*(2*ns+1)]=0.;
        else if(i> nv-2) dat->vals[i1+(i2-floc)*(ns*2+1)]=0.;
        else {dat->vals[i1+(i2-floc)*(ns*2+1)]= (1.-f)*buf->vals[i2+nt*i]+
           f*buf->vals[i2+nt*(i+1)];
     // fprintf(stderr,"GRABBED %d %d %d %d\n",i,i2,i2+nt*(i+1),i1+(i2-floc)*(ns*2+1));
        }
      }
    }
  
//  delete [] buf;
  return dat;
}
