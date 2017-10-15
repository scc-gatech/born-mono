/*$

=head1 NAME

pqueue - heap priority queue

=head1 SYNOPSIS

pqueue_init(n)  - initialize heap

pqueue_insert(v) - insert value

val=pqueue_extract() -extract value

pqueue_close() -close (free memory)

=head1 PARAMETERS

=over 4

=item n - int  

      number of buckets

=item v - float* 

      value to insert

=back

=head1 RETURN VALUE

=over 4

=item val - float* 

      extracted value

=back


=head1 DESCRIPTION

Simple heap priority queue

=head1 COMMENTS

Reference: Sedgewick "Algorithms in C"

=head1 SEE ALSO

B<fastmarch>

=head1 LIBRARY

B<sepaux>

=cut
>*/

/* 
 * Copyright (c) 1997 Stanford Exploration Project
 * All Rights Reserved
 *
 * Author: Sergey Fomel
 */

#include <stdlib.h>
#include <sepaux.h>

static float **x, **xn, **x1;


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void pqueue_init (int n)
_XFUNCPROTOEND
#else
void pqueue_init (n)
int n;
#endif
{
  x = (float **) malloc ((n+1)*sizeof (float *)); 
  xn = x;
  x1 = x+1;
}
  
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void pqueue_close (void)
_XFUNCPROTOEND
#else
void pqueue_close ()
#endif
{
  free (x);
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void pqueue_insert (float* v)
_XFUNCPROTOEND
#else
void pqueue_insert (v)
float *v;
#endif
{
  float **xi, **xp;
  unsigned int p;

  xi = ++xn;
  *xi = v;
  p = (unsigned int) (xn-x);
  for (p >>= 1; p > 0; p >>= 1) {
    xp = x + p;
    if (*v > **xp) break;
    *xi = *xp; xi = xp;
  }
  *xi = v; 
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float* pqueue_extract (void)
_XFUNCPROTOEND
#else
float* pqueue_extract ()
#endif
{
  unsigned int c;
  int n;
  float *v, *t;
  float **xi, **xc;

  v = *x1;
  *(xi = x1) = t = *(xn--);
  n = (int) (xn-x);
  for (c = 2; c <= n; c <<= 1) {
    xc = x + c;
    if (c < n && **xc > **(xc+1)) {
      c++; xc++;
    }
    if (*t <= **xc) break;
    *xi = *xc; xi = xc;
  }
  *xi = t;
  return v;
}














