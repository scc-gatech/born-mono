#include <axis.h>
#include<hypercube.h>
#ifndef POSITION_H
#define POSITION_H 1
class position
{
  public:
    position(){};
    position(hypercube *dims, int *start, int *beg, int *end,int movie_dir);
    virtual ~position(){delete_position();}
   void update_position(int iax1, int iax2, int iloc1, int iloc2);
   void update_range(int iax1, int iax2, int beg1, int end1, int beg2, int end2);
   void full_range();
   void get_os(float *os){for(int i=0; i < 8; i++) os[i]=axes[i].o;}
   void get_ds(float *ds){for(int i=0; i < 8; i++) ds[i]=axes[i].d;}
     void get_ns(int *ns){for(int i=0; i < 8; i++) ns[i]=axes[i].n;}
   
   int get_beg(int iax){ return beg[iax];}
   int get_end(int iax){ return end[iax];}
   inline int get_loc(int iax){ return loc[iax];}
   void get_poss(float *pos){
     for(int i=0; i<8;i++)
       pos[i]= axes[i].o+axes[i].d*loc[i];
   }
   float get_pos(int iax){ 
   return axes[iax].o+axes[iax].d*loc[iax];}
   void set_beg(int iax, int i){ beg[iax]=i;update_extents();}
  void set_end(int iax, int i){ end[iax]=i;update_extents();}
   void set_loc(int iax, int i){ loc[iax]=i; update_loc();}
   void set_pos(int iax,float p){
      set_loc(iax,(int)((p-axes[iax].o)/axes[iax].d+.5));
      update_loc();
   }
   void set_locs(const int *iloc){
     for(int i=0; i<8;i++) set_loc(i,iloc[i]);
    }
   
   axis get_axis(int iax){return axes[iax];}
   int get_movie_axis(){return movie_ax;}
   int get_movie_dir(){return dir;}
   void increment(int iax,int inc,hypercube *h);
   void set_movie_dir(int iax,int d){movie_ax=iax;dir=d;}
   void set_axis(int iax, axis ax) { 
     axes[iax]=ax;
      beg[iax]=0;
      end[iax]=ax.n;
      loc[iax]=ax.n/2;
   }
   void update_movie_pos(hypercube *hyper){increment(movie_ax,dir,hyper);}
   void get_locs(int *iloc){for(int i=0; i<8;i++) iloc[i]=loc[i];}
   void get_begs(int *iloc){for(int i=0; i<8;i++) iloc[i]=beg[i];}
   void get_ends(int *iloc){for(int i=0; i<8;i++) iloc[i]=end[i];}
   void get_axes(axis *ax){for(int i=0; i<8;i++) ax[i]=axes[i];}
   void set_begs(int *iloc){for(int i=0; i<8;i++) beg[i]=iloc[i]; update_extents();}
  void set_ends(int *iloc){for(int i=0; i<8;i++) end[i]=iloc[i]; update_extents();}    
   void fix_min(int imin);
   virtual void update_extents(){};
   virtual void update_loc(){};
   position *clone();

    inline void loc_to_index(int *ind,long long *loc){
      *loc=ind[0]+ind[1]*block[1]+ind[2]*block[2]+ind[3]*block[3]+ind[4]*block[4]+
        ind[5]*block[5]+ind[6]*block[6]+ind[7]*block[7];
    }
    inline void index_to_loc(long long loc, int *ind){
      if(loc==-1){
        ind[0]=ind[1]=ind[2]=ind[3]=ind[4]=ind[5]=ind[6]=ind[7]=-1;
      }
      ind[7]=loc/block[7]; loc-=ind[7]*block[7];
      ind[6]=loc/block[6]; loc-=ind[6]*block[6];
      ind[5]=loc/block[5]; loc-=ind[5]*block[5];
      ind[4]=loc/block[4]; loc-=ind[4]*block[4];
      ind[3]=loc/block[3]; loc-=ind[3]*block[3];
      ind[2]=loc/block[2]; loc-=ind[2]*block[2];
      ind[1]=loc/block[1]; ind[0]=loc-ind[1]*block[1];
    }
    axis axes[8];
    int loc[8];
    int beg[8];
    long long block[8];
    int end[8];//Note this really end+1
    int movie_ax;
    int dir;
    
    void delete_position();

};

#endif

