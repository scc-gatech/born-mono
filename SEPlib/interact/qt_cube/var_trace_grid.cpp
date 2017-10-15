#include "var_trace_grid.h"
#include "stdio.h"
#include <assert.h>
var_trace_grid::var_trace_grid(int noff, float ooff, float doff, int ncmpx, float ocmpx, float dcmpx,
     int ncmpy, float ocmpy, float dcmpy,int n_max){
     n[0]=noff; o[0]=ooff; d[0]=doff;
     n[1]=ncmpx; o[1]=ocmpx; d[1]=dcmpx;
     n[2]=ncmpy; o[2]=ocmpy; d[2]=dcmpy;
     nmax=n_max;
     grid=new int[n[0]*n[1]*n[2]*nmax];
     locs=new float[n[0]*n[1]*n[2]*nmax*3];
     
 }
  int var_trace_grid::get_trace_num(float *loc){
     
      int icell[3];
      
      for(int i=0; i < 3; i++){
        icell[i]=(int)((loc[i]-o[i])/d[i]+.5);
        if(icell[i]<0) icell[i]=0;
        if(icell[i] >= n[i]) icell[i]=n[i]-1;
      }
      int ioff_n=nmax*(icell[0]+n[0]*(icell[1]+n[1]*icell[2]));
      int imin=grid[ioff_n];
      float dmin=(loc[0]-locs[ioff_n*3])*(loc[0]-locs[ioff_n*3])+
        (loc[1]-locs[ioff_n*3+1])*(loc[1]-locs[ioff_n*3+1])+
        (loc[2]-locs[ioff_n*3+2])*(loc[2]-locs[ioff_n*3+2]);
        
      for(int i=1; i < nmax; i++){
        float d=(loc[0]-locs[(ioff_n+i)*3])*(loc[0]-locs[(ioff_n+i)*3])+
                (loc[1]-locs[(ioff_n+i)*3+1])*(loc[1]-locs[(ioff_n+i)*3+1])+
                (loc[2]-locs[(ioff_n+i)*3+2])*(loc[2]-locs[(ioff_n+i)*3+2]);
             
        if(d < dmin){
          imin=grid[ioff_n+i];
          dmin=d;
          
        }  
      }
  //    fprintf(stderr,"LOC %f %f %f  \n",locs[imin*3],locs[imin*3+1],locs[imin*3+2]);
    return imin;
  }
  void var_trace_grid::build_grid_expand(int npts, float *cmpx, float *cmpy, float *offset){
    
   float *dists=new float[n[0]*n[1]*n[2]*nmax];
   int *ncount=new int [n[0]*n[1]*n[2]];
     int n123=n[0]*n[1]*n[2];

     float *loc1=new float[n[0]];
     float *loc2=new float[n[1]];
     float *loc3=new float[n[2]];
     for(int i=0; i < n[0]; i++) loc1[i]=o[0]+d[0]*i;
     for(int i=0; i< n[1];  i++) loc2[i]=o[1]+d[1]*i;
     for(int i=0; i < n[2]; i++) loc3[i]=o[2]+d[2]*i;
     for(int i=0; i< n123; i++) ncount[i]=0;
   
   int jprint=npts/100;
       for(int i=0;i < npts; i++){
         float fcmpx=(cmpx[i]-o[1])/d[1];
         float fcmpy=(cmpy[i]-o[2])/d[2];
         float foff =(offset[i] -o[0] )/d[0];
      //   if( 5223055<i){

     //    if(i%jprint==0) fprintf(stderr,"DONE %d-%f \n",i,(float)i/(float)npts*100.);
         int bcmpy=((int)fcmpy)-1;
         int ecmpy=bcmpy+3; 
         bcmpy=MAX(0,MIN(n[2]-1,bcmpy));
         ecmpy=MAX(0,MIN(n[2]-1,ecmpy));
         
         int bcmpx=((int)fcmpx)-1;
         int ecmpx=bcmpx+3; 
         bcmpx=MAX(0,MIN(n[1]-1,bcmpx));
         ecmpx=MAX(0,MIN(n[1]-1,ecmpx));

         int boff=((int)foff)-1;
         int eoff=boff+3; 
          boff=MAX(0,MIN(n[0]-1,boff));
         eoff=MAX(0,MIN(n[0]-1,eoff));
         for(int i3=bcmpy; i3 <= ecmpy; i3++){

         float d3=(cmpy[i]-loc3[i3]); d3=d3*d3;
         for(int i2=bcmpx; i2 <= ecmpx; i2++){

           float d2=(cmpx[i]-loc2[i2]); d2=d2*d2;
           for(int i1=boff; i1 <= eoff; i1++){

             int iloc=i1+i2*n[0]+i3*n[0]*n[1];
             float dist=(offset[i]-loc1[i1]); dist=dist*dist+d2+d3;

           
    
             if(ncount[iloc]< nmax){

               grid [iloc*nmax+ncount[iloc]]=i;
               dists[iloc*nmax+ncount[iloc]]=dist;
               ncount[iloc]+=1;

             }
             else{

               float dcheck=dist;
               for(int iclose=0; iclose < nmax; iclose++){

                 if(dcheck < dists[iloc*nmax+iclose]){ 

                   grid[iclose+iloc*nmax]=i;

                   float dswap=dists[iclose+iloc*nmax];

                   dists[iclose+iloc*nmax]=dcheck;
                   dcheck=dswap;
         
                 }
              }
        //      }//DELETE
            }
          }
        }
      }
    }
    
    
    
    //FILL NON FILLED
    
    
    int ibig=0;
    jprint=n123/100;
  //  jprint=1;
    for(int ibig3=0;ibig3< n[2]; ibig3++){
     for(int ibig2=0; ibig2<n[1]; ibig2++){
      for(int ibig1=0; ibig1<n[0]; ibig1++,ibig++){
        int isize=2;
//if(ibig> 1425279){
   //    if(ibig%jprint==0) fprintf(stderr,"REDID %d-%f \n",ibig,(float)ibig/(float)n123*100.);

     //  fprintf(stderr,"WHAT HTE %d %d %d %d \n",ibig1,ibig2,ibig3,ncount[ibig]);
        while(ncount[ibig]< nmax){
         int b3=ibig3-isize; b3=MAX(b3,0);
         int e3=ibig3+isize; e3=MIN(e3,0);
         
         int b2=ibig2-isize; b2=MAX(b2,0);
         int e2=ibig2+isize; e2=MIN(e2,0);         
         
         int b1=ibig1-isize; b1=MAX(b1,0);
         int e1=ibig1+isize; e1=MIN(e1,0);  
       
         
         for(int i3=b3; i3 <= e3; i3++){
           for(int i2=b2; i2 <=e2; i2++){
             for(int i1=b1; i1<=e1; i1++){
               if(i1!= ibig1 || i2!=ibig2 || i3!=ibig3){
                 int iloc=i1+i2*n[0]+i3*n[1]*n[0];
                 for(int i=0; i < ncount[iloc]; i++){
                   float d1=(offset[iloc*nmax+i]-loc1[ibig1]); d1=d1*d1;
                   float d2=(cmpx[iloc*nmax+i]-loc2[ibig1]); d2=d2*d2;
                   float d3=(cmpy[iloc*nmax+i]-loc3[ibig1]); d3=d3*d3;
                   float dist=d1+d2+d3;
                  if(ncount[ibig]< nmax){
                    grid [ibig*nmax+ncount[ibig]]=i;
                    dists[ibig*nmax+ncount[ibig]]=dist;
                    ncount[ibig]+=1;
                  }
                  else{
                   float dcheck=dist;
                   for(int iclose=0; iclose < nmax; iclose++){
                     if(dcheck < dists[ibig*nmax+iclose]){ 
                       grid[iclose+ibig*nmax]=i;
                       float dswap=dists[i+ibig*nmax];
                       dists[iclose+ibig*nmax]=dcheck;
                       dcheck=dswap;
                     }
                   }
                 }
              }
            }
          }
        }
       }
       isize+=1;
// }//remove
     }
    }
   }
  } 
  
    int ifill=0;
    int izero=0;
    int imid=0;
    int ip=0;
    int ii=0;
     for(int i3=0; i3 < n[2]; i3++){
         float d3=(cmpy[i3]-loc3[i3]); d3=d3*d3;
         for(int i2=0; i2 < n[1]; i2++){
           float d2=(cmpy[i2]-loc2[i2]); d2=d2*d2;
           for(int i1=0; i1 < n[0]; i1++,ip++){
           if(ncount[ip]==nmax) ifill+=1;
           else if(ncount[ip]==0) izero+=1;
           else imid+=1;
           for(int ipos=0; ipos < nmax; ipos++,ii++){
                locs[ii*3]=offset[grid[ii]];
               locs[ii*3+1]=cmpx[grid[ii]];
               locs[ii*3+2]=cmpy[grid[ii]];
             
             }
            }
           }
          }
      
     
     
     delete [] dists;
     delete [] loc1;
     delete [] loc2;
     delete [] loc3;
     delete [] ncount;
     
  }
  void var_trace_grid::build_grid_total_stream(int npts, float *cmpx, float *cmpy, float *offset){
     float *dists=new float[n[0]*n[1]*n[2]*nmax];
     int n123=n[0]*n[1]*n[2];
     
     
     
     
     float *loc1=new float[n[0]];
     float *loc2=new float[n[1]];
     float *loc3=new float[n[2]];
     for(int i=0; i < n[0]; i++) loc1[i]=o[0]+d[0]*i;
     for(int i=0; i< n[1];  i++) loc2[i]=o[1]+d[1]*i;
     for(int i=0; i < n[2]; i++) loc3[i]=o[2]+d[2]*i;
     
     int ncells=n[0]*n[1]*n[2];
     for(int i=0; i < nmax; i++){
         int icell=0;
         for(int i3=0; i3 < n[2]; i3++){
         float d3=(cmpy[i]-loc3[i3]); d3=d3*d3;
         for(int i2=0; i2 < n[1]; i2++){
           float d2=(cmpy[i]-loc2[i2]); d2=d2*d2;
          
           for(int i1=0; i1 < n[0]; i1++){
            
             float dist=(offset[i]-loc1[i]); dist=dist*dist+d2*d3;
             locs[i+icell*nmax]=i;
          
             dists[i+icell*nmax]=dist;
             if(i3==n[2]-1 && i2==n[1]-1) 
             //  fprintf(stderr,"FILLING (%d %d %d %d) %d %f  %f,%f,%f \n",i1,i2,i3,i,
              //   i+icell*nmax,dist,cmpx[i],cmpy[i],offset[i]);
            icell+=1;

          }
        }
      }
    }
             
       
       for(int i=0;i < npts; i++){
         int icell=0;
         for(int i3=0; i3 < n[2]; i3++){
         float d3=(cmpy[i3]-loc3[i3]); d3=d3*d3;
         for(int i2=0; i2 < n[1]; i2++){
           float d2=(cmpy[i2]-loc2[i2]); d2=d2*d2;
           for(int i1=0; i1 < n[0]; i1++){
            
             float dist=(offset[i1]-loc1[i1]); dist=dist*dist+d2*d3;
             float dcheck=dist;
             //fprintf(stderr,"CHECK it 1 (%d) %d %d %d \n",i,i1,i2,i3);
             for(int iclose=0; iclose < nmax; iclose++){
            // fprintf(stderr,"sss %d %d \n",iclose,icell*nmax+iclose);
              if(n[0]*n[1]*n[2]*nmax < icell*nmax+iclose){
                 fprintf(stderr,"WHAT %d-%d %d-%d %d-%d %d %d \n",i1,n[0],i2,n[1],i3,n[2],icell,iclose);
                 assert(1==0);
               }
               if(dcheck < dists[icell*nmax+iclose]){ 
                 grid[iclose+icell*nmax]=i;
                 float dswap=dists[i+icell*nmax];
                 dists[iclose+icell*nmax]=dcheck;
                 dcheck=dswap;
               }
            }
            icell+=1;
         }
        }
      }
    }
    int i=0;
     for(int i3=0; i3 < n[2]; i3++){
         float d3=(cmpy[i3]-loc3[i3]); d3=d3*d3;
         for(int i2=0; i2 < n[1]; i2++){
           float d2=(cmpy[i2]-loc2[i2]); d2=d2*d2;
           for(int i1=0; i1 < n[0]; i1++){
             for(int ipos=0; ipos < nmax; ipos++,i++){
               locs[i*3]=offset[grid[i]];
               locs[i*3+1]=cmpx[grid[i]];
               locs[i*3+2]=cmpy[grid[i]];
             
             }
            }
           }
          }
             
     
     delete [] dists;
     delete [] loc1;
     delete [] loc2;
     delete [] loc3;
  
  
  }

