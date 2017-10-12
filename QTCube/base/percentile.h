#ifndef PERCENTILE_H
#define PERCENTILE_H 1
namespace SEP{
class percentile{

  public:
   percentile(){};
   float find(float *buf, long long nelem, long long ielem);
  float find_abs(float *buf, long long nelem, long long ielem);

};
}
#endif
