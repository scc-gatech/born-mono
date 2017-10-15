#include<flatten_op.h>
flatten_op::flatten_op(hypercube_float *mod, hypercube_float *dat, float e,
  std::vector<pt3> msk,int imaster,std::vector<int> froze){

  scale=false;
  eps=e;
  n1=mod->get_axis(1).n;
  n2=mod->get_axis(2).n;
  n3=mod->get_axis(3).n;
  n123=n1*n2*n3;
  mask=new std::vector<int>[n3];
  int idepth;
  if(msk.size()>0){
    idepth=msk[imaster].i1;  
    for(int i=0; i < msk.size(); i++){
      mask[msk[i].i3].push_back(idepth+n1*msk[i].i2);
    }
    mys=(double)n123/(double)msk.size()*.2;
    mys=1.;
  }
  tbuf=new float[n1*n2*2];
  this->set_domain(mod);
  this->set_range(dat);
  for(int i=0;i< froze.size();i++) frozen.push_back(froze[i]);
}
void flatten_op::scale_it(float *dat){
  
  for(int i3=0; i3 < n3; i3++){
    for(int i1=0; i1 < mask[i3].size(); i1++){
      dat[i3*n1*n2+mask[i3][i1]]*=mys;
      if(i3>0)   dat[i3*n1*n2+mask[i3][i1]-n1*n2]*=mys;
      if(mask[i3][i1]>n1) dat[i3*n1*n2+mask[i3][i1]-n1]*=mys;
      dat[i3*n1*n2+mask[i3][i1]-1]*=mys;
    }
  }
}
flatten_prec_op::flatten_prec_op(hypercube_float *mod, hypercube_float *dat, float e,
  std::vector<pt3> msk,int imaster){

  this->set_domain(mod);
  this->set_range(dat);
  eps=e;
  n1=mod->get_axis(1).n;
  n2=mod->get_axis(2).n;
  n3=mod->get_axis(3).n;
  n123=n1*n2*n3;
  mask=new std::vector<int>[n3];
    int idepth;
  if(msk.size()>0){
    idepth=msk[imaster].i1;  
  }
  for(int i=0; i < msk.size(); i++){
    mask[msk[i].i3].push_back(idepth+n1*msk[i].i2);
  }
  tbuf=new float[n1*n2*2];
  mid=new float[n1*n2*n3];
  int zero=0;
  fftw_test2_(&n1,&n2,&n3,&zero,mid,(long long*)&planF,(long long*)&planI);
}

void flatten_op::both_op(bool adj, bool add, float *mod, float*dat){
  if(!add){
    if(adj){ 
      for( int i=0; i< n123; i++) mod[i]=0.;
    }
    else{
      for(int i=0; i < n123*3;i++) dat[i]=0.;
   }
 }


  int j,i;
  if(scale && adj) scale_it(dat);
  for(int i3=1; i3 <n3; i3++){
   
      
     if(!adj){
       memcpy(tbuf,&mod[(i3-1)*n1*n2],n1*n2*2*sizeof(float));
  if(scale){
    if(frozen.size()==0){for(int i1=0; i1 < mask[i3-1].size(); i1++) tbuf[mask[i3-1][i1]]=0.;
       for(int i1=0; i1 < mask[i3].size(); i1++) tbuf[n2*n1+mask[i3][i1]]=0.;
       }
       else{
         for(int i1=0; i1 < frozen.size(); i1++){
            for(int i=0; i < n2; i++) tbuf[i*n1+frozen[i1]]=0.;
          }
       
       }
       }
       for(int i2=1; i2 < n2; i2++){
         i=1+i2*n1+i3*n1*n2; j=1+i2*n1+n1*n2;        
         for(int i1=1; i1 < n1; i1++,j++,i++){
         
           dat[i]+=eps*(tbuf[j]-tbuf[j-1]);
         
         dat[i+n123]+=tbuf[j]-tbuf[j-n1];
           dat[i+n123*2]+=tbuf[j]-tbuf[j-n1*n2];
           
           
     //      dat[i]+=eps*(mod[i]-mod[i-1]);
        }
      }
    }
    else{
      for(int i1=0; i1 < n1*n2*2; i1++) tbuf[i1]=0.;
        for(int i2=1; i2 < n2; i2++){
          i=1+i2*n1+i3*n1*n2; j=1+i2*n1+n1*n2;   
          for(int i1=1; i1 < n1; i1++,j++,i++){
          
            tbuf[j]+=dat[i]*eps+dat[i+n123]+dat[i+n123*2];
            tbuf[j-1]-=dat[i]*eps;
          
          tbuf[j-n1]-=dat[i+n123];
            tbuf[j-n1*n2]-=dat[i+n123*2];
            
           // mod[i]+=dat[i]*eps;
           // mod[i-1]-=dat[i]*eps;
          // fprintf(stderr,"CHECK IT %d %d %f \n",i,j,tbuf[j]);
          }
       }
       if(scale){
         if(frozen.size()==0){
       for(int i1=0; i1 < mask[i3-1].size(); i1++) tbuf[mask[i3-1][i1]]=0.;
       
       
       for(int i1=0; i1< mask[i3].size(); i1++) {
     //  fprintf(stderr,"SETTING %d to zero \n",i3*n1*n2+mask[i3][i1]);
         tbuf[n2*n1+mask[i3][i1]]=0.;
         
         }
         }
         else{
           for(int i1=0; i1 < frozen.size(); i1++){
             for(int i=0; i < n2; i++){ 
                tbuf[frozen[i1]+i*n1]=tbuf[frozen[i1]+i*n1+n1*n2]=0.;
             }
           }
         
         }
       }
       for(int i1=0; i1 < n1*n2*2; i1++) {
          
         mod[(i3-1)*n2*n1+i1]+=tbuf[i1];
      //    fprintf(stderr,"CHKC MOD %d %f %f\n",i1,mod[(i3-1)*n2*n1+i1],tbuf[i1]);
       
       }
     }
    //  fprintf(stderr,"MOD OUT 1596726=%f i3=%d\n",mod[1596726 ],i3); 

   }
     if(scale && !adj) scale_it(dat);

}
void flatten_prec_op::combo_op(bool adj, bool add, float *mod, float*dat){
  if(!add){
    if(adj){ 
      for( int i=0; i< n123; i++) mod[i]=0.;
    }
    else{
      for(int i=0; i < n123;i++) dat[i]=0.;
   }
 }
if(!adj){
   
   memcpy(mid,mod,n123*4);
   fftwf_execute(planF);
   for(int i=0; i < n123; i++) mid[i]*=mult[i];
    fftwf_execute(planI);
    both_op(adj,add,mid,dat);
}
else{
    both_op(adj,false,mid,dat);
    fftwf_execute(planI);
   for(int i=0; i < n123; i++) tbuf[i]*=mult[i];
    fftwf_execute(planF);
    for(int i=0; i < n123; i++) mod[i]+=tbuf[i];
}



}