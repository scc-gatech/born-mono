#ifdef SEP_MPI
#define SET_SDOC 1

/*
Date Created: 20050227

Purpose: 

*/	 

#include<sep_par.h>
#include<mpi.h>
extern int sep_thread_num(void);
extern int sep_num_thread(void);
int read_local_datasets(sep3d *sects,sep3d *data, distrib *spread, int *nwind, int *fwind,
    float *buf1, float *buf2,float *buf3)
{
int is,idim,n123,e2,n123_s;
int i,ia,isect,ierr,ipt,impi,nh,iloc[9];
int iout;
int *nsect,*fsect,*nout, *fout,*jwind;
int b[9],e[9],s[9],c1,c2,c3,c4,c5,c6,c7,c8,ib;
int tot7,tot5,tot3,tot1,i1,i2,i3,i4,i5,i6,i7,i8,i0;
                                                                                             
  impi=sep_thread_num();
  nsect=(int*) malloc(sizeof(int)*data->ndims);
  fsect=(int*) malloc(sizeof(int)*data->ndims);
  nout=(int*) malloc(sizeof(int)*data->ndims);
  fout=(int*) malloc(sizeof(int)*data->ndims);
  jwind=(int*) malloc(sizeof(int)*data->ndims);
                                                                                             
  for(idim=0,n123=1; idim < data->ndims;idim++){
    jwind[idim]=1;
    n123=n123*nwind[idim];
  }
  e2=sep3dc_get_esize(data)/4;
  for(i=0; i < n123*e2;i++) buf1[i]=0.;

                                                                                             
  /* out is the local versions coordinates
     sect is the shifted version of the window (nwind,fwind) that this represents
   */
  for(ia=0; ia < spread->nsect; ia++){
    isect=spread->isect[ia];
    ierr=local_window(ia,data,spread,nwind,fwind,nsect,fsect,nout,fout);
    ipt=spread->ilocal_pt[ia];
    if(ierr==0 && impi!=0){
      if(0!= sep3dc_grab_headers(spread->tag_sect[ia],&sects[ipt],&nh,&nout[1],
         &fout[1], jwind)) seperr("trouble grabbing headers \n") ;
      if(0!=sep3dc_read_data(spread->tag_sect[ia],&sects[ipt],(void*)buf2,
        nout[0], fout[0],jwind[0])) seperr("trouble reading data");

      if(0!=patch_boundary(data,spread,nwind,isect,nout,fout,fsect,buf2))
        seperr("trouble with patch boundaries");


      for(idim=0,n123_s=1; idim < data->ndims;idim++) n123_s=n123_s*nout[idim];
      if(0!=create_bounds(ia,spread,data->ndims,data->n,nwind,fwind,b,e,s))
         seperr("trouble creating bounds");
     for(i=0; i < data->ndims; i++) {
       b[i]=b[i]-fwind[i];
       e[i]=e[i]-fwind[i];
     }
      for(i=1; i <9; i++) s[i]=s[i]*e2*data->n[0];
      i=0;
      for(i8=b[8],c8=b[8]*s[8]; i8 <= e[8]; i8++,c8+=s[8]){
      for(i7=b[7],c7=b[7]*s[7]; i7 <= e[7]; i7++,c7+=s[7]){
        tot7=c7+c8;
        for(i6=b[6],c6=b[6]*s[6]; i6 <= e[6]; i6++,c6+=s[6]){
        for(i5=b[5],c5=b[5]*s[5]; i5 <= e[5]; i5++,c5+=s[5]){
          tot5=c5+c6+tot7;
          for(i4=b[4],c4=b[4]*s[4]; i4 <= e[4]; i4++,c4+=s[4]){
          for(i3=b[3],c3=b[3]*s[3]; i3 <= e[3]; i3++,c3+=s[3]){
            tot3=c4+c3+tot5;
            for(i2=b[2],c2=b[2]*s[2]; i2 <= e[2]; i2++,c2+=s[2]){
            for(i1=b[1],c1=b[1]*s[1]; i1 <= e[1]; i1++,c1+=s[1]){
              ib=c1+c2+tot3;
              for(i0=b[0]*e2    ; i0 <= (1+e[0])*e2-1; i0++,i++){
                buf1[ib+i0]=buf1[ib+i0]+buf2[i];
             }
            }}
          }}
        }}
      }}
    }
  }
  
  free(nout); free(fout); free(nsect); free(fsect); free(jwind);
  return(0);
}

int combo_data(int impi, int nmpi,float *data,float *buf, int n123){
  int to_do,done,i1;
  int block,i,ip;
  MPI_Status status;
                                                                                             
  done=0;
  to_do=n123;
                                                                                             
  i=0;
  while (done<to_do){
    i++;
    block=to_do-done;

    if(impi!=nmpi-1){
      MPI_Recv((buf+done),block, MPI_FLOAT, impi+1,i, MPI_COMM_WORLD,&status);
      if(impi==0) {
        for(i1=0; i1 <  block; i1++) data[i1+done]=buf[i1+done];
      }
      else{
        fprintf(stderr,"CHECK SUM %f %f %f %f -%d \n",
          data[100],buf[100],data[10000],buf[10000],impi);
        for(i1=0; i1 <  block; i1++) data[i1+done]=data[i1+done]+buf[i1+done];
      } 
    }
                                                                                             
    if(impi!=0){
      MPI_Send((data+done),block, MPI_FLOAT, impi-1, i,MPI_COMM_WORLD);
    }
    done+=block;
  }
  return(0);
}

int calc_output_sections(sep3d *big, distrib *spread){
  int idim,isect,icoord[9],ip[9];
  int nmin,nextra,i,nhalf,i2,nbig,j;

  for( idim=0; idim < spread->ndim; idim++){
    if(spread->nblock[idim]==1){
      for(i=0;  i < spread->nsect; i++){
        spread->axis_beg[i][idim]=0;
        spread->axis_end[i][idim]=big->n[spread->dff_axis[idim]]-1;
      }
    }
    else {
      nbig   =big->n[spread->dff_axis[idim]];
      nmin=nbig/spread->nblock[idim];
      nextra=nbig- nmin*spread->nblock[idim];
                                                                                             
/*      if(nmin <= spread->noverlap[idim])*/
/*        seperr("overlap can not be larger than %d for axis %d \n",*/
/*          nmin-1,spread->dff_axis[idim]+1);*/

      for(i=0;  i < spread->nsect; i++){
        isect=spread->isect[i];
        h2c(isect,spread->nblock,spread->ndim,ip);
        spread->axis_beg[i][idim]=ip[idim]*nmin;
        if(ip[idim]>0){
          if(ip[idim] <= nextra) spread->axis_beg[i][idim]+=ip[idim];
          else spread->axis_beg[i][idim]+=nextra;
        }
        spread->axis_end[i][idim]=
          spread->axis_beg[i][idim]-1+nmin;
        if(ip[idim]+1 <=nextra)
          spread->axis_end[i][idim]= spread->axis_end[i][idim]+1;
        spread->axis_beg[i][idim]=MAX(0,
           spread->axis_beg[i][idim]-spread->noverlap[idim]);
        spread->axis_end[i][idim]=MIN(big->n[spread->dff_axis[idim]]-1,
           spread->axis_end[i][idim]+spread->noverlap[idim]);
         if(sep_thread_num()==0){
         }
     }
                                                                                             
    }
  }
  return(0);
}
                                                                                             
int get_distrib_info( distrib *spread, int output,char *prefix)
{
  int i,ndim;
  int nblock[9];
  int coord[9],idim,ierr;
  int nmin[9],nextra[9],nsect_tot;
  char temp_ch[1024],my_tag[1024],string[1024];
                                                                                             
                                                                                             
  sprintf(temp_ch,"%snsect",prefix);

  if(0==getch(temp_ch,"d",&spread->nsect))
    return(sepwarn(NOT_MET,"nsect not provided\n"));
                                                                                             
                                                                                             
  spread->tag_sect=(char**) malloc(sizeof(char*)*spread->nsect);
  for(i=0; i < spread->nsect; i++) spread->tag_sect[i]=(char*) malloc(1024);
  spread->isect=(int*) malloc(sizeof(int)*spread->nsect);
  spread->sect_thread=(int*) malloc(sizeof(int)*spread->nsect);
                                                                                             

  sprintf(temp_ch,"%sdff_axis",prefix);
  ndim=getch(temp_ch,"d",nblock);
  if(ndim==0) return(sepwarn(NOT_MET,"dff_axis not provided\n"));
                                                                                             
  spread->dff_axis=(int*)malloc(sizeof(int)*ndim);
  ndim=getch(temp_ch,"d",spread->dff_axis);
  for(i=0; i <ndim; i++) spread->dff_axis[i]-=1;
                                                                                             
                                                                                             
                                                                                             
                                                                                             
  sprintf(temp_ch,"%snblock",prefix);
  i=getch(temp_ch,"d",nblock);

  if(i==0) return(sepwarn(NOT_MET,"nblock not provided\n"));
  if(i!=ndim) return(sepwarn(NOT_MET,"nblock and dff_axis not the same size\n"));
                                                                                             
  spread->nblock=(int*)malloc(sizeof(int)*i);
  spread->noverlap=(int*)malloc(sizeof(int)*i);
  spread->ndim=getch(temp_ch,"d",spread->nblock);
  for(i=0,nsect_tot=1; i <ndim; i++) nsect_tot=nsect_tot*spread->nblock[i];
  sprintf(temp_ch,"%snoverlap",prefix);
  ierr=getch(temp_ch,"d",spread->noverlap);
  if(ierr==0){
    for(i=0; i < spread->ndim; i++) spread->noverlap[i]=0;
  } 
  else if(ierr!=spread->ndim) seperr("noverlap must be the same ndims (%d) of nblock (%d) \n",spread->ndim,ierr);
                                                                                             
                                                                                             
                                                                                             
  spread->axis_beg=(int**) malloc(sizeof(int*)*spread->nsect);
  spread->axis_end=(int**) malloc(sizeof(int*)*spread->nsect);
  for(i=0; i < spread->nsect; i++) {
    sprintf(temp_ch,"%sisect_sect%d",prefix,i);
    if(1!=getch(temp_ch,"d",&spread->isect[i]))
      return(sepwarn(NOT_MET,"%s not specified \n",temp_ch));
                                                                                             
    if(nsect_tot <= spread->isect[i] || spread->isect[i] < 0)
      return(sepwarn(NOT_MET,"illegal section %d \n",spread->isect[i]));
                                                                                             
    sprintf(temp_ch,"%ssect_thread%d",prefix,i);
    if(1!=getch(temp_ch,"d",&spread->sect_thread[i]))
      return(sepwarn(NOT_MET,"%s not specified \n",temp_ch));
                                                                                             
    sprintf(temp_ch,"%stag_sect%d",prefix,i);
    if(1!=getch(temp_ch,"s",spread->tag_sect[i]))
      return(sepwarn(NOT_MET,"%s not specified \n",temp_ch));
                                                                                             
    spread->axis_beg[i]=(int*) malloc(ndim*sizeof(int));
    spread->axis_end[i]=(int*) malloc(ndim*sizeof(int));
                                                                                             
  }
  for(i=0,spread->nown=0; i < spread->nsect; i++) {
                                                                                             
   if(sep_thread_num()==0 || sep_thread_num()==spread->sect_thread[i])
     spread->nown+=1;
  }
  spread->iown=(int*) malloc(sizeof(int)*spread->nown);
  spread->ilocal_pt=(int*) malloc(sizeof(int)*spread->nsect);
  for(i=0,spread->nown=0; i < spread->nsect; i++) {
    spread->ilocal_pt[i]=-1;
    if(sep_thread_num()==0 || sep_thread_num()==spread->sect_thread[i]) {
      if(sep_thread_num()!= 0 && output==1){
        sprintf(temp_ch,"%sdatapath_sect%d",prefix,i);
        if(0==getch(temp_ch,"s",my_tag))
          seperr("%s not specified",temp_ch);
        sprintf(string,"datapath=%s",my_tag);
        getch_add_string(string);
      }
      spread->iown[spread->nown]=i;
      spread->ilocal_pt[i]=spread->nown;
      spread->nown+=1;
    }
  }
                                                                                             
  return(0);
}
int create_bounds(int isect, distrib *spread, int ndims,int *ngrid,int *nwind, int *fwind, int *b, int *e, int *s)
{
  int i,nlast,iax;
  if(ndims > 9) seperr("Can handle at maximum 9 dimensions");
                                                                                             
  b[0]=fwind[0]; e[0]=fwind[0]-1+nwind[0]; s[0]=1;
  nlast=1;
  for(i=1 ; i < 9; i++){
    s[i]=nlast*s[i-1];
    if(ndims > i){
      nlast=ngrid[i];
      b[i]=fwind[i];
      e[i]=fwind[i]+nwind[i]-1;
    }
    else{
      nlast=1;
      b[i]=0;
      e[i]=0;
    }
  }
  for(i=0; i < spread->ndim;i++){
    iax=spread->dff_axis[i];
    if(spread->axis_beg[isect][i] > fwind[iax]) b[iax]=spread->axis_beg[isect][i];
    if(spread->axis_end[isect][i] < fwind[iax]+nwind[iax]-1) {
        e[iax]=spread->axis_end[isect][i];
    }
  }
  return(0);
                                                                                             
}
int patch_boundary(sep3d *data,distrib *spread,int *nwind,  int isect, int *nout, int *fout, int *fsect, float *buf2)
{
  int idim,ibeg0,ibeg1,iend1,iend0,i2;
  int nuse[3],i,iloc,i1,i3;
  int nblock[3];
  float f;
                                                                                             
  for(idim=0; idim < spread->ndim; idim++){
  create_3d(data, spread,nout,idim,isect,nuse,nblock,&ibeg0,&ibeg1,&iend0,&iend1);
                                                                                             
    /*do the begining*/
    for(i2=ibeg0+1; i2 < ibeg1; i2++){
      iloc=i2-fsect[spread->dff_axis[idim]];
      f=(float)(ibeg1-i2)/((float)(ibeg1-ibeg0));
      /*if we actually have this element*/
      if(iloc >-1 && iloc < nout[spread->dff_axis[idim]]){
        for(i3=0; i3 < nuse[2]; i3++){
          i=i3*nblock[2]+iloc*nblock[1];
          for(i1=0; i1 < nuse[0]; i1++) buf2[i+i1]=buf2[i+i1]*f;
        }
      }
    }
    /*do the end*/
    for(i2=iend1+1; i2 < iend0; i2++){
      iloc=i2-fsect[spread->dff_axis[idim]];
      f=((float)(i2-iend1))/((float)(iend0-iend1));
      /*if we actually have this element*/
      if(iloc >-1 && iloc < nout[spread->dff_axis[idim]]){
        for(i3=0; i3 < nuse[2]; i3++){
          i=i3*nblock[2]+iloc*nblock[1];
          for(i1=0; i1 < nuse[0]; i1++) buf2[i+i1]=buf2[i+i1]*f;
        }
      }
    }
  }
    return(0);
}
void create_3d(sep3d *data, distrib *spread,int *nsect,int idim,  int isect,
int *nuse, int *nblock, int *ibeg0,int *ibeg1, int *iend0, int *iend1)
{
  int iax,i;
                                                                                             
  iax=spread->dff_axis[idim];
  nuse[0]=sep3dc_get_esize(data)/4;
  for(i=0; i<iax; i++) nuse[0]=nuse[0]*nsect[i];
  nuse[1]=nsect[iax];
  for(i=iax+1,nuse[2]=1; i<data->ndims; i++) {
    nuse[2]=nuse[2]*nsect[i];
  }
                                                                                             
  nblock[0]=1;
  nblock[1]=nuse[0]*nblock[0];
  nblock[2]=nuse[1]*nblock[1];
                                                                                             
  *ibeg0=spread->axis_beg[isect][idim]-1;
  *iend0=spread->axis_end[isect][idim]+1;
  *ibeg1=*ibeg0+2*spread->noverlap[idim]+1;
  *iend1=*iend0-2*spread->noverlap[idim]-1;
                                                                                             
                                                                                             
  /*do nothing on the ends*/
  if(*ibeg0 <0) *ibeg1=0;
  if(*iend0 >=data->n[spread->dff_axis[idim]]) *iend1=*iend0;
                                                                                             
  return;
}
                                                                                              
int create_global_file(distrib *spread,sep3d *sects, sep3d *combo)
{
int impi;
int isect,ipt;
int nsum,i,j;
int axis[9];
                                                                                              
                                                                                              
  impi=sep_thread_num();
  for(isect=0; isect <  spread->nsect; isect++){
    if(impi!=0 && spread->sect_thread[isect]==sep_thread_num()){
      ipt=spread->ilocal_pt[isect];
      if(0!= init_sep3d_tag(spread->tag_sect[isect],&sects[ipt],"SCRATCH"))
         return(sepwarn(NOT_MET,"trouble initializing file %s \n",
           spread->tag_sect[isect]));
      if(0!= sep3dc_pass_headers(&sects[ipt], spread->sect_thread[isect],0))
        seperr("trouble passing headers");
    }
    else if(impi==0){
      ipt=spread->ilocal_pt[isect];
      if(0!= sep3dc_pass_headers(&sects[ipt], spread->sect_thread[isect],0))
        seperr("trouble passing headers");
    }
  }
  if(impi==0){
    if(0!=init_sep3d_struct(sects[0],combo,"SCRATCH"))
      seperr("trouble initializing out");
    if(spread->dff_axis[0]+1 > combo->ndims) {
         for(i=0; i < combo->ndims;  i++) axis[i]=i+1;
         for(i=combo->ndims; i < spread->dff_axis[0]+1;  i++) 
           axis[i]=sects[ipt].ndims;
         if(0!=sep3dc_reshape(combo,spread->dff_axis[0]+1,axis)){
           seperr("trouble reshaping data \n");
         }
    }
    for(i=0; i < spread->ndim; i++) {
      for(isect=0,nsum=0; isect <  spread->nsect; isect++){
        if(sects[isect].ndims < spread->dff_axis[i]+1) nsum+=1;
        else nsum+=sects[isect].n[spread->dff_axis[i]];
        if(spread->isect[isect]==0)
          combo->o[spread->dff_axis[i]]=sects[isect].o[spread->dff_axis[i]];
      }
      combo->n[spread->dff_axis[i]]=nsum-spread->noverlap[i]*2*(spread->nblock[i]-1);
      for(j=0; j < spread->ndim; j++) {
        if(j!=i)
         combo->n[spread->dff_axis[i]]=combo->n[spread->dff_axis[i]]/spread->nblock[j];
      }
                                                                                              
    }
    sep3d_set_sep3d(combo);
  }
  if(0!=sep3dc_broadcast_headers(combo,0))
    seperr("trouble brodcasting data header \n");
  return(0);
}

int local_window(int isect, sep3d *data, distrib *spread, int *nwind,
  int *fwind, int *nsect, int *fsect, int  *nout, int *fout)
{
  int i,ibeg,iax,iend;
                                                                                             
  if(spread->ilocal_pt[isect]==-1) return(-1);
                                                                                             
  for(i=0; i < data->ndims; i++){
    nsect[i]=nwind[i];
    fsect[i]=fwind[i];
    nout[i]=nwind[i];
    fout[i]=fwind[i];
  }
  for(i=0; i < spread->ndim;i++){
    iax=spread->dff_axis[i];
    ibeg=fwind[iax];
    iend=ibeg+nwind[iax]-1;
    if(iend < spread->axis_beg[isect][i]) return(1);
    if(ibeg > spread->axis_end[isect][i]) return(1);
    if(spread->axis_beg[isect][i] > ibeg) ibeg=spread->axis_beg[isect][i];
    if(spread->axis_end[isect][i] < iend) iend=spread->axis_end[isect][i];
    fsect[iax]=ibeg;
    nsect[iax]=iend-ibeg+1;
    fout[iax]=ibeg- spread->axis_beg[isect][i];
    nout[iax]=nsect[iax];
  }
  return(0);
}
                                                                                             
int calc_input_sections(sep3d *big, distrib *spread,sep3d *structs){
  int idim,isect,icoord[9],ip[9];
  int nmin,nextra,i,nhalf,i2,nbig,j;
  int **nlens,skip,ntot;
  MPI_Status status;
                                                                                         
  nlens=(int**)malloc(sizeof(int*)*spread->ndim);
  for(i=0; i < spread->ndim; i++)
    nlens[i] =(int*)malloc(sizeof(int)* spread->nsect);
                                                                                         
  /*store the lengths of all the axes*/
  for( idim=0; idim < spread->ndim; idim++){
    if(sep_thread_num()==0){
      for(i=0;  i < spread->nsect; i++){
        isect=spread->isect[i];
        nlens[idim][isect]=structs[i].n[ spread->dff_axis[idim]];
      } 
    }
    if(sep_thread_num() != 0)
      MPI_Recv(nlens[idim],spread->nsect, MPI_INT, sep_thread_num()-1,4834, 
        MPI_COMM_WORLD,&status);
    if(sep_thread_num()  != sep_num_thread()-1)
      MPI_Send(nlens[idim],spread->nsect, MPI_INT, sep_thread_num()+1, 4834,
        MPI_COMM_WORLD);
  }
  for( idim=0; idim < spread->ndim; idim++){
    if(spread->nblock[idim]==1){
      for(i=0;  i < spread->nsect; i++){
        spread->axis_beg[i][idim]=0;
        spread->axis_end[i][idim]=big->n[spread->dff_axis[idim]]-1;
      }
    }
    else{
      /*2-D case a plane of patches
         what we are looking for in the collumn and row the patch exists on
         to determine the beg and end
        We will use the first collumn as a guide*/
      for(i=0,skip=1; i < idim;i++) skip=skip*spread->nblock[i];
      for(i=0;  i < spread->nsect; i++){
        isect=spread->isect[i];
        h2c(isect,spread->nblock,spread->ndim,ip);
        /*loop through the collumn,row,plane to find the begining*/
        for(j=0,spread->axis_beg[i][idim]=spread->noverlap[idim]; j<ip[idim]; j+=skip){
          /*account for the overlap*/
        
          spread->axis_beg[i][idim]+=nlens[idim][j]-spread->noverlap[idim]*2;
        }
            spread->axis_end[i][idim]=spread->axis_beg[i][idim]+nlens[idim][isect]-1;
      }
    }
  }
  for(i=0; i < spread->ndim; i++) free(nlens[i]);
  free(nlens) ;
  return(0);
}

#else

void as(){}

#endif
