#include "plane_contain.h"
#include <math.h>
#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif
#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif
plane_contain::plane_contain(int *n, int i1, int i2, std::map<QString,picks*> *all){
  iax1=i1;
  iax2=i2;
  alloc_plane(n,iax1,iax2,all);
  memcpy(nall,n,sizeof(int)*8);
}
std::vector<long long> plane_contain::find_picks(QString col, int *iloc, 
  int ising,orient_cube *pos){
  
  int iuse,ipos[6],j=0;
  for(int i=0; i < 8; i++){
    if(i==ising) iuse=i;
    if(i!=iax1 && i!=iax2){
      ipos[j]=iloc[i];
      j++;
    }
  }
 
  std::vector<long long> list;
  for(int i=0; i < nall[ising]; i++){
    ipos[iuse]=i;
    std::vector<long long> local;

    pick_plane *tmp=tot[ipos[5]][ipos[4]][ipos[3]][ipos[2]][ipos[1]][ipos[0]];

    local=tmp->maps[col]->all_picks(pos,iax1,iax2);
   
    list.reserve(list.size()+local.size());
  
    list.insert(list.end(),local.begin(),local.end());
 
  }

  return list;
}
  
void plane_contain::form_neighbors(int *iloc,int ndist,std::vector<pick_plane*> *neighs, 
 std::vector<int>*dists){
  int d1,d2,d3,d4,d5,d6,dist;
   int i1,i2,i3,i4,i5,i6;
   i1=iloc[0];
   i2=iloc[1];
   i3=iloc[2];
   i4=iloc[3];
   i5=iloc[4];
   i6=iloc[5];
           for(int j6=MAX(i6-ndist,0); j6<=MIN(nsz[5]-1,i6+ndist); j6++){ d6=(i6-j6)*(i6-j6);
           for(int j5=MAX(i5-ndist,0); j5<=MIN(nsz[4]-1,i5+ndist); j5++){d5=(i5-j5)*(i5-j5);
           for(int j4=MAX(i4-ndist,0); j4<=MIN(nsz[3]-1,i4+ndist); j4++){d4=(i4-j4)*(i4-j4);
           for(int j3=MAX(i3-ndist,0); j3<=MIN(nsz[2]-1,i3+ndist); j3++){d3=(i3-j3)*(i3-j3);
           for(int j2=MAX(i2-ndist,0); j2<=MIN(nsz[1]-1,i2+ndist); j2++){d2=(i2-j2)*(i2-j2);
           for(int j1=MAX(i1-ndist,0); j1<=MIN(nsz[0]-1,i1+ndist); j1++){d1=(i1-j1)*(i1-j1);
             dist=(int)sqrt((double)(d1+d2+d3+d4+d5+d6));   
             
             
             if(dist>0) {
                neighs->push_back(tot[j6][j5][j4][j3][j2][j1]);
                dists->push_back(dist);
             }
          }}}}}}
         
}
void plane_contain::add_pt(QString col, int *ipos,long long hash){
  int iloc[6];
  compress(ipos,iax1,iax2,iloc);
  // fprintf(stderr,"Adding pt %d %lld \n",iloc[0],hash);
 // fprintf(stderr,"adding pt to %d %d %d %d %d %d\n",iloc[0],iloc[1],iloc[2],iloc[3],iloc[4],iloc[5]);
  tot[iloc[5]][iloc[4]][iloc[3]][iloc[2]][iloc[1]][iloc[0]]->add_pt(col,hash);
}
void plane_contain::del_pt(QString col, int *ipos,long long hash){
  int iloc[6];
  compress(ipos,iax1,iax2,iloc);
  tot[iloc[5]][iloc[4]][iloc[3]][iloc[2]][iloc[1]][iloc[0]]->del_pt(col,hash);
}
void plane_contain::compress(int *iloc, int iax1, int iax2, int *ipos){
int j=0;
  for(int i=0; i < 8; i++){
    if(i!=iax1 && i!=iax2){
      ipos[j]=iloc[i];
      j++;
    }
  }

}


void plane_contain::alloc_plane(int *n, int i1, int i2, std::map<QString,picks*> *all){
 
 compress(n,i1,i2,nsz);

  tot=new pick_plane******[nsz[5]];
  for(int i5=0; i5 < nsz[5]; i5++){
    tot[i5]=new pick_plane*****[nsz[4]];
  }
  for(int i5=0; i5 < nsz[5]; i5++){
    for(int i4=0; i4 < nsz[4]; i4++){
      tot[i5][i4]=new pick_plane****[nsz[3]];
    }
  }
  for(int i5=0; i5 < nsz[5]; i5++){
    for(int i4=0; i4 < nsz[4]; i4++){
      for(int i3=0; i3 < nsz[3]; i3++){
        tot[i5][i4][i3]=new pick_plane***[nsz[2]];
      }
    }
  }
  for(int i5=0; i5 < nsz[5]; i5++){
    for(int i4=0; i4 < nsz[4]; i4++){
      for(int i3=0; i3 < nsz[3]; i3++){
        for(int i2=0; i2 < nsz[2]; i2++){
          tot[i5][i4][i3][i2]=new pick_plane**[nsz[1]];
         }
       }
     }
   }
  for(int i5=0; i5 < nsz[5]; i5++){
    for(int i4=0; i4 < nsz[4]; i4++){
      for(int i3=0; i3 <nsz[3]; i3++){
        for(int i2=0; i2 < nsz[2]; i2++){
          for(int i1=0; i1 < nsz[1]; i1++){
            tot[i5][i4][i3][i2][i1]=new pick_plane*[nsz[0]];
          }
        }
      }
    }
  }
  for(int i6=0; i6 <nsz[5]; i6++){
  for(int i5=0; i5 < nsz[4]; i5++){
    for(int i4=0; i4 < nsz[3]; i4++){
      for(int i3=0; i3 <nsz[2]; i3++){
        for(int i2=0; i2 < nsz[1]; i2++){
          for(int i1=0; i1 < nsz[0]; i1++){
            tot[i6][i5][i4][i3][i2][i1]=new pick_plane(all,iax1,iax2);
          }
        }
      }
    }
  }
  }
  
  }
