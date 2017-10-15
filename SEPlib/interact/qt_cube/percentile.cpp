#include<math.h>
#include <stdio.h>
#include "percentile.h"
float percentile::find_abs(float *x, long long n, long long q)
{
        register float *i, *j, ak;
        float *low, *hi, buf, *k;
        for (low=x, hi=x+n-1, k=x+q; low<hi;)
                {
                ak = fabs(*k);
                i = low; j = hi;
                do
                        {
                        while (fabs(*i) < ak) i++;
                        while (fabs(*j) > ak) j--;
                        if (i<=j)
                                {
                                buf = *i;
                                *i++ = fabs(*j);
                                *j-- = fabs(buf);
                                }
                        } while (i<=j);
                if (j<k) low = i; if (k<i) hi = j;
                }
        return (*k);
        
    }

float percentile::find(float *x, long long n, long long q)
{

        register float *i, *j, ak;
        float *low, *hi, buf, *k;
   
        for (low=x, hi=x+n-1, k=x+q; low<hi;)
                {
                ak = *k;
                i = low; j = hi;
                do
                        {
                        while (*i < ak) i++;
                        while (*j > ak) j--;
                        if (i<=j)
                                {
                                buf = *i;
                                *i++ = *j;
                                *j-- = buf;
                                }
                        } while (i<=j);
                if (j<k) low = i; if (k<i) hi = j;
                }
        return (*k);
    }


