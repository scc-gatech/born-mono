#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <obb.h>
#include <stack>
#include <vector>
#include <algorithm>

/* use unnamed namespace to keep C++ names internal */
namespace obb_internal {

    template<class ftype> struct Point { ftype x; ftype y; };

    double almost_zero = 0.0e-10;
    
    template<class ftype> Point<ftype> nextToTop(std::stack< Point<ftype> > &S) 
    {
       Point<ftype> p = S.top();
       S.pop();
       Point<ftype> res = S.top();
       S.push(p);
       return res;
    }
    
    template<class ftype> void swap(Point<ftype> &p1, Point<ftype> &p2)
    {
       Point<ftype> temp = p1;
       p1 = p2;
       p2 = temp;
    }

    template<class ftype> ftype distSq(Point<ftype> p1, Point<ftype> p2)
    {
       return((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
    }

    template<class ftype> int orientation(Point<ftype> p, Point<ftype> q, Point<ftype> r)
    {
       ftype val = (q.y - p.y) * (r.x - q.x) -
                   (q.x - p.x) * (r.y - q.y);

       if(val >= -almost_zero && val <= almost_zero) return 0; /* colinear */
       return (val > 0) ? 1: 2;  /* clock or counterclock wise */
    }

    template<class ftype> class ptCompare {
    public:
       bool operator() (const Point<ftype>& p1, const Point<ftype>& p2) const
       {

          int o = orientation(_p0, p1, p2);
          if(o == 0) return (distSq(p1, p2) > distSq(_p0, p1));
          return (o == 2);
       }
       ptCompare(Point<ftype> p0) {
          _p0.x = p0.x;
          _p0.y = p0.y;
       }
     private:
       Point<ftype> _p0;
    };

    template<class ftype> std::stack< class Point<ftype> > * convexHull(std::vector< Point<ftype> > ptVec)
    {
       ftype ymin = ptVec[0].y; 
       int minloc = 0;

       Point<ftype> p0;

       for(int i = 1; i < ptVec.size(); i++) 
       {
          ftype y = ptVec[i].y;
          if((y < ymin) || (ymin == y && ptVec[i].x < ptVec[minloc].x)) {
             ymin = y; minloc = i;
          }
       }

       swap(ptVec[0], ptVec[minloc]);

       p0 = ptVec[0];
       std::sort(++(ptVec.begin()), ptVec.end(), ptCompare<ftype>(ptVec[0]));

       int m = 1;
       for(int i = 1; i < ptVec.size(); i++)
       {
          while(i < (ptVec.size()-1) && orientation(p0, ptVec[i], ptVec[i+1]) == 0) i++;
          ptVec[m] = ptVec[i];
          m++;
       }

       std::stack< Point<ftype> > *S = new std::stack< Point<ftype> >;

       if (m<3) return S; /* no convex hull possible? */

       S->push(p0);
       S->push(ptVec[1]);
       S->push(ptVec[2]);

       for (int i=3; i < m; i++)
       {
          while(!(S->empty()) && orientation(nextToTop(*S), S->top(), ptVec[i]) != 2) S->pop();
          S->push(ptVec[i]);
       }
       return S;
    }

    template<class ftype> struct Edge { Point<ftype> p1; Point<ftype> p2; };

/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#define CV_CALIPERS_MAXHEIGHT      0
#define CV_CALIPERS_MINAREARECT    1
#define CV_CALIPERS_MAXDIST        2

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    icvRotatingCalipers
//    Purpose:
//      Rotating calipers algorithm with some applications
//
//    Context:
//    Parameters:
//      points      - convex hull vertices ( any orientation )
//      n           - number of vertices
//      mode        - concrete application of algorithm 
//                    can be  CV_CALIPERS_MAXDIST   or   
//                            CV_CALIPERS_MINAREARECT  
//      left, bottom, right, top - indexes of extremal points
//      out         - output info.
//                    In case CV_CALIPERS_MAXDIST it points to float value - 
//                    maximal height of polygon.
//                    In case CV_CALIPERS_MINAREARECT
//                    ((Point<ftype>*)out)[0] - corner 
//                    ((Point<ftype>*)out)[1] - vector1
//                    ((Point<ftype>*)out)[2] - vector2
//                      
//                      ^
//                      |
//              vector2 |
//                      |
//                      |____________\
//                    corner         /
//                               vector1
//
//    Returns:
//    Notes:
//F*/

/* we will use usual cartesian coordinates */
template<class ftype> void
icvRotatingCalipers( Point<ftype>* points, int n, int mode, ftype out[6] )
{
    double minarea = 1.0e38f;
    ftype max_dist = 0;
    char buffer[64];
    int i, k;
    Point<ftype>* vect = new Point<ftype>[n];
    ftype* inv_vect_length = new ftype[n];
    int left = 0, bottom = 0, right = 0, top = 0;
    int seq[4] = { -1, -1, -1, -1 };

    /* rotating calipers sides will always have coordinates    
       (a,b) (-b,a) (-a,-b) (b, -a)     
     */
    /* this is a first base vector (a,b) initialized by (1,0) */
    ftype orientation = 0;
    ftype base_a;
    ftype base_b = 0;

    ftype left_x, right_x, top_y, bottom_y;
    Point<ftype> pt0 = points[0];
    
    left_x = right_x = pt0.x;
    top_y = bottom_y = pt0.y;
    
    for( i = 0; i < n; i++ )
    {
        double dx, dy;
        
        if( pt0.x < left_x )
            left_x = pt0.x, left = i;

        if( pt0.x > right_x )
            right_x = pt0.x, right = i;

        if( pt0.y > top_y )
            top_y = pt0.y, top = i;

        if( pt0.y < bottom_y )
            bottom_y = pt0.y, bottom = i;

        Point<ftype> pt = points[(i+1)%n];
        
        dx = pt.x - pt0.x;
        dy = pt.y - pt0.y;

        vect[i].x = (ftype)dx;
        vect[i].y = (ftype)dy;
        inv_vect_length[i] = (ftype)(1./::sqrt(dx*dx + dy*dy));

        pt0 = pt;
    }

    /* find convex hull orientation */
    {
        double ax = vect[n-1].x;
        double ay = vect[n-1].y;
        
        for( i = 0; i < n; i++ )
        {
            double bx = vect[i].x;
            double by = vect[i].y;

            double convexity = ax * by - ay * bx;

            if( convexity != 0 )
            {
                orientation = (ftype) ((convexity > 0) ? 1.0 : -1.0);
                break;
            }
            ax = bx;
            ay = by;
        }
    }
    base_a = orientation;

/*****************************************************************************************/
/*                         init calipers position                                        */
    seq[0] = bottom;
    seq[1] = right;
    seq[2] = top;
    seq[3] = left;
/*****************************************************************************************/
/*                         Main loop - evaluate angles and rotate calipers               */

    /* all of edges will be checked while rotating calipers by 90 degrees */
    for( k = 0; k < n; k++ )
    {
        /* compute cosine of angle between calipers side and polygon edge */
        /* dp - dot product */
        ftype dp0 = base_a * vect[seq[0]].x + base_b * vect[seq[0]].y;
        ftype dp1 = -base_b * vect[seq[1]].x + base_a * vect[seq[1]].y;
        ftype dp2 = -base_a * vect[seq[2]].x - base_b * vect[seq[2]].y;
        ftype dp3 = base_b * vect[seq[3]].x - base_a * vect[seq[3]].y;

        ftype cosalpha = dp0 * inv_vect_length[seq[0]];
        ftype maxcos = cosalpha;

        /* number of calipers edges, that has minimal angle with edge */
        int main_element = 0;

        /* choose minimal angle */
        cosalpha = dp1 * inv_vect_length[seq[1]];
        maxcos = (cosalpha > maxcos) ? (main_element = 1, cosalpha) : maxcos;
        cosalpha = dp2 * inv_vect_length[seq[2]];
        maxcos = (cosalpha > maxcos) ? (main_element = 2, cosalpha) : maxcos;
        cosalpha = dp3 * inv_vect_length[seq[3]];
        maxcos = (cosalpha > maxcos) ? (main_element = 3, cosalpha) : maxcos;

        /*rotate calipers*/
        {
            //get next base
            int pindex = seq[main_element];
            ftype lead_x = vect[pindex].x*inv_vect_length[pindex];
            ftype lead_y = vect[pindex].y*inv_vect_length[pindex];
            switch( main_element )
            {
            case 0:
                base_a = lead_x;
                base_b = lead_y;
                break;
            case 1:
                base_a = lead_y; 
                base_b = -lead_x;
                break;
            case 2:
                base_a = -lead_x;
                base_b = -lead_y;
                break;
            case 3:
                base_a = -lead_y;
                base_b = lead_x;
                break;
            default: break;
            }
        }                        
        /* change base point of main edge */
        seq[main_element] += 1;
        seq[main_element] = (seq[main_element] == n) ? 0 : seq[main_element];

        
        switch (mode)
        {
        case CV_CALIPERS_MAXHEIGHT:
            {
                /* now main element lies on edge alligned to calipers side */

                /* find opposite element i.e. transform  */
                /* 0->2, 1->3, 2->0, 3->1                */
                int opposite_el = main_element ^ 2;

                ftype dx = points[seq[opposite_el]].x - points[seq[main_element]].x;
                ftype dy = points[seq[opposite_el]].y - points[seq[main_element]].y;
                float dist;

                if( main_element & 1 )
                    dist = (ftype)::fabs(dx * base_a + dy * base_b);
                else
                    dist = (ftype)::fabs(dx * (-base_b) + dy * base_a);

                if( dist > max_dist )
                    max_dist = dist;

                break;
            }
        case CV_CALIPERS_MINAREARECT:
            /* find area of rectangle */
            {
                ftype height;
                ftype area;

                /* find vector left-right */
                ftype dx = points[seq[1]].x - points[seq[3]].x;
                ftype dy = points[seq[1]].y - points[seq[3]].y;

                /* dotproduct */
                ftype width = dx * base_a + dy * base_b;

                /* find vector left-right */
                dx = points[seq[2]].x - points[seq[0]].x;
                dy = points[seq[2]].y - points[seq[0]].y;

                /* dotproduct */
                height = -dx * base_b + dy * base_a;

                area = width * height;
                if( area <= minarea )
                {
                    ftype *buf = (ftype *) buffer;

                    minarea = area;
                    /* leftmost point */
                    ((int *) (buf+0))[0] = seq[3];
                    buf[1] = base_a;
                    buf[2] = width;
                    buf[3] = base_b;
                    buf[4] = height;
                    /* bottom point */
                    ((int *) (buf+5))[0] = seq[0];
                    buf[6] = area;
                }
                break;
            }
        }                       /*switch */
    }                           /* for */

    switch (mode)
    {
    case CV_CALIPERS_MINAREARECT:
        {
            ftype *buf = (ftype *) buffer;

/*
  fprintf(stderr,"cosang %g\n",buf[1]);
  fprintf(stderr,"sinang %g\n",buf[3]);
  fprintf(stderr,"width  %g\n",buf[2]);
  fprintf(stderr,"height %g\n",buf[4]);
  fprintf(stderr,"area   %g\n",buf[6]);
*/

            ftype A1 = buf[1];
            ftype B1 = buf[3];

            ftype A2 = -buf[3];
            ftype B2 = buf[1];

            ftype C1 = A1 * points[((int *) (buf+0))[0]].x + points[((int *) (buf+0))[0]].y * B1;
            ftype C2 = A2 * points[((int *) (buf+5))[0]].x + points[((int *) (buf+5))[0]].y * B2;

            ftype idet = ((ftype) 1.0) / (A1 * B2 - A2 * B1);

            ftype px = (C1 * B2 - C2 * B1) * idet;
            ftype py = (A1 * C2 - A2 * C1) * idet;

            out[0] = px;
            out[1] = py;

            out[2] = A1 * buf[2];
            out[3] = B1 * buf[2];

            out[4] = A2 * buf[4];
            out[5] = B2 * buf[4];
        }
        break;
    case CV_CALIPERS_MAXHEIGHT:
        {
            out[0] = max_dist;
        }
        break;
    }

    delete inv_vect_length;
    delete vect;
}

    template<class ftype> std::vector< Point<ftype> > *CalcOmbb(int n, Point<ftype> *points)
    {
       std::vector< Point<ftype> > *P = new std::vector< Point<ftype> >(n);
       for(int i = 0; i<n; i++) {
          (*P)[i].x = points[i].x;
          (*P)[i].y = points[i].y;
       }
       std::stack< Point<ftype> > *S = convexHull(*P);
       Point<ftype> *hullpts = new Point<ftype>[S->size()];
       int nhull=0;
       while(!(S->empty())) {
             hullpts[nhull] = S->top(); S->pop(); ++nhull; }
       delete S; delete P;
/*
       fprintf(stderr,"Convex Hull: nhull %d\n",nhull);
       for(int i=0; i<nhull; i++) {
          fprintf(stderr,"%d %g %g\n",i,hullpts[i].x, hullpts[i].y);
       }
       fprintf(stderr,"\n\n");
*/

       ftype outbuf[6];
       icvRotatingCalipers(hullpts, nhull, CV_CALIPERS_MINAREARECT, outbuf);
/*
       for(int i=0; i<6; i+=2) {
          fprintf(stderr,"%g %g\n",outbuf[i], outbuf[i+1]);
       }
*/

       std::vector< Point<ftype> >* res = new std::vector< Point<ftype> >(4);
       (*res)[0].x = outbuf[0];
       (*res)[0].y = outbuf[1];
       (*res)[1].x = outbuf[0]+outbuf[2];
       (*res)[1].y = outbuf[1]+outbuf[3];
       (*res)[3].x = outbuf[0]+outbuf[4];
       (*res)[3].y = outbuf[1]+outbuf[5];
       (*res)[2].x = outbuf[0]+outbuf[2]+outbuf[4];
       (*res)[2].y = outbuf[1]+outbuf[3]+outbuf[5];

       return (res);
    }
}
   
/* takes X, Y coordinates and returns minimal oriented bounding box */
extern "C"
void oriented_bounding_box_f(int npts, const float *x, const float *y,
                           float xbb[4], float ybb[4]) {
     obb_internal::Point< float > *ptList = new obb_internal::Point< float >[npts];
     for(int i=0; i<npts; i++) 
     {
        ptList[i].x = x[i];
        ptList[i].y = y[i];
     }
     std::vector< obb_internal::Point< float > > *S = obb_internal::CalcOmbb<float>(npts, ptList);
     xbb[0] = (*S)[0].x;
     ybb[0] = (*S)[0].y;
     xbb[1] = (*S)[1].x;
     ybb[1] = (*S)[1].y;
     xbb[2] = (*S)[2].x;
     ybb[2] = (*S)[2].y;
     xbb[3] = (*S)[3].x;
     ybb[3] = (*S)[3].y;

     delete S;
}

extern "C"
void oriented_bounding_box_d(int npts, const double *x, const double *y,
                           double xbb[4], double ybb[4]){
     obb_internal::Point< double > *ptList = new obb_internal::Point< double >[npts];
     for(int i=0; i<npts; i++) 
     {
        ptList[i].x = x[i];
        ptList[i].y = y[i];
     }
     std::vector< obb_internal::Point< double > > *S = obb_internal::CalcOmbb<double>(npts, ptList);
     xbb[0] = (*S)[0].x;
     ybb[0] = (*S)[0].y;
     xbb[1] = (*S)[1].x;
     ybb[1] = (*S)[1].y;
     xbb[2] = (*S)[2].x;
     ybb[2] = (*S)[2].y;
     xbb[3] = (*S)[3].x;
     ybb[3] = (*S)[3].y;

     delete S;
}

