#ifndef VAR_TRACE_GRID 
#define VAR_TRACE_GRID 1

#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif
#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif

class var_trace_grid{
  public:
  var_trace_grid(){ grid=0;};
  var_trace_grid(int noff, float ooff, float doff, int ncmpx, float ocmpx, float dcmpx,
     int noff, float ooff, float doff,int n_max);
  
  void build_grid_total_stream(int npts, float *cmpx, float *cmpy, float *offset);
int get_trace_num(float *loc);
  void build_grid_expand(int npts, float *cmpx,float  *cmpy, float *offset);

  ~var_trace_grid(){ 
    if(grid==0) delete [] grid;
    grid=0;
  }

  int nmax;
  int n[3];
  float o[3];
  float d[3];
  int *grid;
  float *locs;

  
};
#endif