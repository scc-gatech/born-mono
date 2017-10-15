/*$

=head1 NAME

cent - nth percentile of an array

=head1 SYNOPSIS

C<val=cent(p,x,n)>

=head1 INPUT PARAMETER

=over 4

=item  p - float   

       percentile <0.,99.999>

=item  x - float*  

       data

=item  n - int      

       vector raslength

=back

=head1 RETURN VALUES

=over 4

=item float = 

      value at given percentile

=back

=head1 DESCRIPTION

Percentile routine based on Canales, SEP-10

=head1 LIBRARY

B<sepaux>

=cut

$*/
/*      percentile subroutine
 *      based on Canales, SEP-10
 *      p - percentile <0.,99.999>
 *      x - data
 *      n - vector raslength
 */
/* 1-22-93  Martin   for rs6000 99.9999999999 arithmetic is bogus  */

#include <sepaux.h>
#include<seplib.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float cent (float  p,float *x,int n)
_XFUNCPROTOEND
#else
float cent (p,x,n)
int n;
float *x, p;
#endif
{
        int q;
        register float *i, *j, ak;
        float *low, *hi, buf, *k;
        if (p>99.999) p = 99.999;
        if (p<0.) p = 0.; q = ((double)(p*n))/100.;
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
