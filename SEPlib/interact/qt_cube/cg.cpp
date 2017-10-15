
// See cg.h for documentation of this class

#include <assert.h>
#include <float.h>
// #include <iostream.h>
#include <iostream>
using std::cout;
#include <cmath>
#include <math.h>
#include <stdio.h>

#include "cg.h"

static const int debug = 0;

void CgVector::printme(const char * label) const {
  printf("%s: magnitude=%g\n",label,sqrt(this->dot(*this)));
  fflush(stdout);
}

CgVector *CgQuad::solve(long nter) const {
  // minimize quadratic function 0.5 x'Hx + b'x , and return vector x.
  // b is the gradient for x=0, and H is the hessian
  // algorithm:
  //    x = p = u = 0;
  //    beta = 0;
  //    g = b;
  //    a = A g;
  //    q = H a;
  //    do {
  //       p = -a + beta p
  //       u = Hp = -q + beta u
  //       alpha = -p'g/p'u
  //       x = x + alpha p
  //       composite functions  if(done) return x
  //       g = H x + b = g + alpha u
  //       a = A g
  //       q = H a
  //       beta = p'H A g / p'H p = p'q/p'u
  //    }

  double beta, alpha, pu, pg;
  long iter;
  if(debug) printf("Starting conj grad solution with %d iterations\n",nter);
  double bb = b_->dot(*b_);
  if(bb<10.*FLT_MIN) {
    cout << "Right hand side of conjugate gradient equation is negligible. "
            "Not solving.\n";
    cout.flush();
    b_->scale(0.);
    return b_;
  }
#ifndef NDEBUG
  // see if vector routines are behaving as expected
  {
    CgVector *t = b_->clone();
    CgVector *tt = b_->clone_zero();
    t->scale_add(-3., *b_,*tt);
    double check = t->dot(*b_)/b_->dot(*b_);
    assert (-check > 1.999 && -check < 2.001 );
    delete t;
    delete tt;
  }
#endif
  CgVector *t = b_->clone();  // scratch
  CgVector *x = b_->clone_zero();
  CgVector *p = b_->clone_zero();
  CgVector *u = b_->clone_zero();
  beta = 0.;
  CgVector *g = b_->clone();
  CgVector *a = g->clone();
  cqi_->inverse_hessian(*a);
  CgVector *q = a->clone();
  cqi_->hessian(*q);
  for(iter=0; iter<nter; iter++) {
//    cout << "beta = " <<beta << "; "; cout.flush();
    p->scale(beta);
    p->scale_add(-1., *a, *t);
    u->scale(beta);
    u->scale_add(-1., *q, *t);
    pg = p->dot(*g);
    pu = p->dot(*u);
/*    cout << "pg = " << pg << "; pu = " << pu << "; "; cout.flush();*/
    if(fabs(pg) < 20.*FLT_MIN || fabs(pu) < 20.*FLT_MIN) goto done;
    alpha = -pg/pu;
/*    cout << "alpha = " <<alpha << "\n"; cout.flush();*/
/*p->printme("p");*/
    x->scale_add(alpha, *p, *t);
/*x->printme("x");*/
    if(iter == nter-1) goto done;
    g->scale_add(alpha, *u, *t);
    a->copy(*g);
    cqi_->inverse_hessian(*a);
    q->copy(*a);
    cqi_->hessian(*q);
    beta = p->dot(*q)/pu;
    beta = ((beta >=0.) ? 1. : -1.)* ((fabs(beta) < 5.) ? beta : 5.);
  }
done:
//  cout <<"\n"; cout.flush();
  delete p; delete u; delete g; delete a; delete q; delete t; return x;
}

// unique constructor
CgTrans::CgTrans(const CgTransInterface *cti, const CgVector* data0, 
                 const CgVector& model0, int add_reference_model,
                 int damp_perturb): cti_(cti), data0_(data0),
                 damp_perturb_(damp_perturb),
                 add_reference_model_(add_reference_model) {
  model0_ = model0.clone();
  model0_->apply_constraint();
  CgVector *b = this->create_b(); // requires model0_ first
  cq_ = new CgQuad(this, *b); // implements the CgQuadInterface
  delete b;
};

// solve quadratic problem
CgVector* CgTrans::solve(long number_iterations) const {
#ifndef NDEBUG
  adjoint_is_good();
#endif
  CgVector*answer = cq_->solve(number_iterations);
  if(add_reference_model_) {
    answer->add(*model0_);
  }
  return answer;
};

// Solve nonquadratic objective function with Gauss Newton iterations
CgVector* CgTrans::solve(int conjugate_grad_iterations,
                         int line_search_iterations,
                         int linearization_iterations,
                         double line_search_error) {
#ifndef NDEBUG
  adjoint_is_good();
#endif
  if(debug) printf("Begin nonlinear optimization\n");
  int iter;
  double alphamin, alphamax, okerror,okfraction;
  // require good accuracy so that specified iterations are used.
  alphamin =0.; alphamax=1.1; 
  okerror = line_search_error; okfraction=line_search_error;
  for(iter=0; iter<linearization_iterations; iter++) {
  // use parent solver to avoid adding reference model
    CgVector* perturbation = cq_->solve(conjugate_grad_iterations);
    double pp; pp = perturbation->dot(*perturbation);
    if(debug) perturbation->printme("linear perturbation");
    if(almost_zero(pp)) {
      if(debug) {
        printf("done early with linearization, iteration %d\n", iter+1);
      }
      delete perturbation; goto done;
    }
    double scale;
    if(debug) printf("Beginning line search\n");
    scale = this->line_search(*perturbation, alphamin,alphamax, okerror,
                              okfraction, line_search_iterations);
    if(debug) printf("Scale factor = %g\n",scale);
    if(almost_zero(scale)) {
       if(debug){printf("finished line search early, iteration %d\n",iter+1);}
       delete perturbation; goto done;
    }
    perturbation->scale(scale);
    if(debug) perturbation->printme("scaled perturbation");
    perturbation->add(*model0_);
    if(debug) perturbation->printme("scaled perturb'n plus reference model");
    perturbation->apply_constraint();
    if(debug) perturbation->printme("updated model with constraint");

    delete model0_;
    model0_  = perturbation;
    perturbation = 0;
    CgVector *b = this->create_b();
    cq_->set_b(*b);
    delete b;
  }
done:
  return model0_->clone();
}

CgVector* CgTrans::create_b() const {
  if(debug) model0_->printme("reference model to begin calculating b");
  CgVector * data  = data0_->clone_zero();    // data is data with zeros
  CgVector * model = model0_->clone();         // model is m
  CgVector * outmodel = model0_->clone_zero(); // outmodel is m with zeros
  cti_->forward_nonlinear(*data,*model);       // data is f(m) (defaults to Fm)
  if(debug) data->printme(" current modeled data: f(model)");
  data->scale(-1);                          // data is -f(m)
  data->add(*data0_);                        // data is e = d - f(m)
  if(debug) data->printme(" current error: data-f(model)");
  data->multiply_inverse_covariance();      // data is N e
  cti_->adjoint_linearized(*data,*outmodel,*model0_);   // outmodel is F'Ne
  outmodel->scale(-1.);                     // outmodel is -F'Ne
  if(!damp_perturb_) {
    model->multiply_inverse_covariance();     // model is M m
    outmodel->add(*model);                     // outmodel is -F'Ne + Mm
  }
  delete data;
  delete model;
  if(debug) printf("Done with calculating b\n");
  return outmodel;
}

void CgTrans::hessian(CgVector&x) const {
  if(debug) x.printme("x for hessian");
  CgVector * data  = data0_->clone_zero(); // data is full of zeros
  CgVector * model = x.clone_zero();      // model is full of zeros
  cti_->forward_linearized(*data, x, *model0_);    // data is Fx
  data->multiply_inverse_covariance();     // data is NFx
  cti_->adjoint_linearized(*data, *model, *model0_);     // model is F'NFx
  delete data;
  x.multiply_inverse_covariance();         // x is Mx
  x.add(*model);                            // x is (F'NF + M)x
  delete model;
  if(debug) printf("Done with hessian\n");
}

// evaluate objective function for nonquadratic linear search
double CgTrans::objective_function(const CgVector&x,
                                   int is_a_perturbation) const {
  CgVector * model = x.clone();             // model is x or m+x
  CgVector * data  = data0_->clone_zero();   // data is full of zeros
  if(is_a_perturbation) model->add(*model0_); // model is m+x
  model->apply_constraint();                 // model is constrained(m+x)
  cti_->forward_nonlinear(*data, *model);          // data is f(m+x)
  data->scale(-1);                          // data is -f(m+x)
  data->add(*data0_);                        // data is e = d - f(m+x)
  CgVector* datap = data->clone();          // datap is e
  data->multiply_inverse_covariance();      // data is Ne
  double eNe = datap->dot(*data);           // eNe is e'Ne
  delete data;
  delete datap;
  if(damp_perturb_) {                   // if requested, replace model by x
    model->scale(-1);                   // model is -(m+x)
    model->add(*model0_);                 // model is -x
    model->scale(-1);                   // model is x (after constraint)
  }                                        // model is m+x or x
  CgVector* modelp = model->clone();        // modelp is m+x or x
  model->multiply_inverse_covariance();     // model is M(m+x) or Mx
  double xMx = modelp->dot(*model);         // xMx is (m+x)'M(m+x) or x'Mx
  delete model;
  delete modelp;
  return (eNe+xMx);
}

int CgTrans::adjoint_is_good() const {
#ifndef NDEBUG
  if(debug) printf("Testing dot product\n"); fflush(stdout);
#endif
  CgVector* b = this->create_b();  // use as an arbitrary non-zero model
  CgVector* d = data0_->clone();  // use as arbitrary non-zero data
  CgVector* Fb = d->clone_zero(); // for forward transform of b
  CgVector* Ad = b->clone_zero(); // for adjoint of data
  cti_->forward_linearized(*Fb, *b, *model0_);
  cti_->adjoint_linearized(*d, *Ad, *model0_);
  double dFb = d->dot(*Fb);
  double Adb = Ad->dot(*b);
  int matches = almost_equal(dFb, Adb); // see if equality is very good.
  if(!matches) { // allow rough equality (magnitudes most important)
    matches = ((dFb*Adb >=0.) ? 1 : 0); // assert have the same sign
    double r1 = (dFb>0.) ? dFb : -dFb; // abs
    double r2 = (Adb>0.) ? Adb : -Adb; // abs
    double oneplus = 1.2;  // may still fail for degenerate b and data
    matches = matches && (almost_cmp(oneplus*r1,r2) >= 0);
    matches = matches && (almost_cmp(oneplus*r2,r1) >= 0);
  }
#ifndef NDEBUG
  if(!matches) {
    printf("\ndot(d,Fb)=%g dot(F'd,b)=%g\n",dFb,Adb);
    printf("Failed dot-product test.\n");
  }
  fflush(stdout);
#endif
  delete b; delete d; delete Fb; delete Ad;
  return matches;
}

// search scaled perturbation to minimize objective function
double CgTrans::line_search(const CgVector &x, double alphamin,
                            double alphamax, double okerror,
                            double okfraction, long nter) const {

  CgVector *xx=0;
  long iter;
  double fnew, xmin, xerr, xnew, alphabest, alphanew;

  // allocate space
  iter = 0;
  nter = (nter<10)? 10: nter;
  fnew = 0.;
  line_sample (&xmin, &xerr, &xnew, &iter, fnew);
  while(iter <= nter &&
        (iter < 5 || xerr > okerror || xerr >= okfraction * xmin)) {
    alphanew = (alphamax - alphamin) * xnew + alphamin;
    xx = x.clone();
    xx->scale(alphanew);
    fnew = this->objective_function(*xx,1);
    delete xx;
    line_sample (&xmin, &xerr, &xnew, &iter, fnew);
    alphabest = (alphamax - alphamin) * xmin + alphamin;
  }
  // remove temp space */
  return alphabest;
}

// search an interval (0,1) for a minimum, parabolas and golden section
// long iter = 0, nter = 20;
// float okerr = 0.001, factor = 1., xerr=1.;
// line_sample(&xmin, &xerr, &xnew, &iter, fnew);
// while(xerr>okerr*factor && iter<=nter) {
//    fnew = function(xnew)
//    line_sample(&xmin, &xerr, &xnew, &iter, fnew);
//    factor=xmin  // optional
// }
// No need to initialize xmin,xerr,xnew.  fnew not used
// in first iteration.  iter must be 0 in first iteration, and fnew
// must be updated for the returned xnew on later iterations.

// Originally patterned after a line search in 1st ed. of Numerical Recipes
// in 1988. This Ctran code could be cleaned up, but it has seen a lot of data
// and is well debugged already.  This is NOT REENTRANT.  Notice the
// internal static variables.
void CgTrans::line_sample(double *xmin, double *xerr, double *xnew, long *iter,
                          double fnew) const {
  static double x[4]={0, 0, 0, 0}, fx[4]={0, 0, 0, 0}, dx[3]={1., 1., 1.};
  static double r1, r2, rgold = 0;
  static int imin = 0, inew = 0, ix[4], ifx[4];
  int ip1,ip2,ip3;

  if(debug) printf("beginning iteration %d\n",*iter);
  if(*iter <= 3) {
    if(debug) printf("first three iter\n");
    if(*iter == 0) {
      int i;
      if(debug) printf("first iter\n");
      // first iteration initialization
      imin = 0; inew = 0;
      rgold = (sqrt (5.)-1.)*.5;
      // starting samping points
      x[0] = 0; x[1] = 1.-rgold; x[2] = rgold; x[3] = 1.;
      // dx are errors
      for(i=0; i<3; i++) dx[i] = 1.;
      // fx are function values at x's
      for(i=0; i<4; i++) fx[i] = 0.;
      // return
      goto done;
    }
    // 2nd and 3rd iteration, fill in fx
    // fill in function values
    fx[inew] = fnew;
    ++inew;
    // return
    goto done;
  }
  // four and more iterations, f's are full
  if(debug) printf("four and more\n");
  fx[inew] = fnew;                // store previously requested value of f
  // index by increasing x and by increasing fx
  index(ix, x, 4); index (ifx, fx, 4);
  if(debug) {
    for(int i = 0; i < 4; i++) {
      printf("x[%d]=%g fx[%d]=%g ", ix[i], x[ix[i]], ix[i], fx[ix[i]]);
      printf("ix[%d]=%d ifx[%d]=%d\n", i, ix[i], i, ifx[i]);
    }
  }
  imin = ifx[0];                // minimum value of function
  inew = ix[0];                        // x(inew) should be replaced
  if(imin == ix[0] || imin == ix[1]) {inew = ix[3];}
  // set flag if smallest x was original minimum of interval
  int ltenth; ltenth = almost_zero(x[imin]);
  // if we have a smallest or largest x, then we do not have enough
  // points for a parabolic method.  set flag for golden section
  int lgold; lgold = ((imin == ix[3]) || (imin == ix[0]) || ltenth)?1:0;
  // dx's are the successive errors, dx(2) last, dx(3) before last,
  // dx(1) to be for this one
  dx[2] = dx[1]; dx[1] = dx[0];
  // look at three points not to be replaced
  if(inew == ix[0] || inew == ix[1]) {ip1 = ix[1]; ip2 = ix[2]; ip3 = ix[3];}
  else {ip1 = ix[0]; ip2 = ix[1]; ip3 = ix[2];}
  // ip1, ip2, and ip3 are indices for three points to be saved, in
  // ascending order of x
  // calculate distance between two points that span minimum = error
  dx[0] = x[ip3] - x[ip1];
  // use golden section if errors have not been decreasing rapidly enough
  // (error must be less than half after two previous iterations)
  lgold = (lgold || dx[0] > dx[2] * .5)?1:0;
  // r1 is the span of left interval, r1 of the right.
  r1 = x[ip2] - x[ip1]; r2 = x[ip3] - x[ip2];
  // go to golden section search, if flag set
  if(debug) printf("ip1=%d ip2=%d ip3=%d\n", ip1, ip2, ip3);
  if(!lgold) {
    if(debug) printf("Use parabolic method\n");
    // here use parabolic method, if have no previous objections
    int igood = parabola_min(&x[inew], x[ip1], fx[ip1], x[ip2],
                             fx[ip2], x[ip3], fx[ip3]);
    if(debug) {
      printf("x[ip1]=%g fx[ip1]=%g ", x[ip1], fx[ip1]);
      printf("x[ip2]=%g fx[ip2]=%g ", x[ip2], fx[ip2]);
      printf("x[ip3]=%g fx[ip3]=%g\n", x[ip3], fx[ip3]);
      printf("parabolic x[inew]=%g\n", x[inew]);
    }
    // if parabolic search was numerically stable, then return.
    // (Straight lines have no interior minimum, for instance.)
    // If the new point to be searched is already the minimum point,
    // then might have already converged.  In case not, go on and
    // do golden section to reduce the width of the interval searched. u
    if(igood && x[inew] != x[imin]) {goto done;}
    // if parabolic search failed, do golden search
  }
  if(debug) printf("Use golden search\n");
  // Do golden search.  Divide larger section.
  int ll; ll = (r1 > r2)?1:0; // ll==1 if left span is largest.
  // find new x in larger left interval
  if(ll) x[inew] = x[ip1] + rgold * (x[ip2] - x[ip1]);
  // find new x in larger right interval
  if(!ll)  x[inew] = x[ip3] - rgold * (x[ip3] - x[ip2]);
  // if smallest x has the minimum, then scale down drastically
  // we may have orders of magnitude to go.
  if(ltenth) {
    if(debug) printf("Scale by tenth\n");
    x[inew] = x[ip1] + (x[ip2] - x[ip1]) * .1;
  }
  // the following is the only valid exit from this routine
  done:
  ++(*iter);                        // update iteration number
  *xmin = x[imin];                // note present best value of x
  *xerr = dx[0];                // note size of interval spanning minimum
  // give the value of x that needs to be searched, user will return fnew
  *xnew = x[inew];
  if(debug) {
    printf ("imin=%d inew=%d\n", imin, inew);
    printf ("current minimum x is %g with f(x)=%g\n",x[imin],fx[imin]);
  }
  return;
}

// slothsort
void CgTrans::index(int *ix, const double *rx, long n) {
  assert(n<20);
  long ixx,i,j;
  for(i=0; i<n; i++) {ix[i] = i;}
  for(i=0; i<n-1; i++) {
    for(j=0; j<n-1; j++) {
      if(rx[ix[j]] > rx[ix[j+1]]) {
        ixx = ix[j]; ix[j] = ix[j+1]; ix[j+1] = ixx;
      }
    }
  }
}

// find minimum x of parabola with values f(x1) = f1, f(x2) = f2, f(x3) = f3
// f(xmin) <= f(x) for all x.
// return 1 if successful; return 0 and minimum of 0.5 if unsucessful
// x1 < x2 < x3; f(x2) < f(x1), f(x2) < f(x3)
int CgTrans::parabola_min(double *xmin, double x1, double f1,
                          double x2, double f2, double x3, double f3)
{
  double xm, a, b;

  if(almost_equal (x3, x1)) goto middle;
  xm = (x2 - x1) / (x3 - x1);
  if(xm < 0.001 || xm > 0.999) goto middle;
  a = ((f3 - f1) * xm - (f2 - f1)) / (xm - xm * xm);
  b = f3 - f1 - a;
  if(a * b >= 0 || 0.5 * fabs (b) > fabs (a)) goto middle;
  xm = -0.5 * b / a;
  if(xm < 0. || xm > 1.) goto middle;
  *xmin = xm * (x3 - x1) + x1;
  return 1;
middle:
  xm = 0.5;
  *xmin = xm * (x3 - x1) + x1;
  return 0;
}

int CgTrans::almost_equal(double r1, double r2)
{
  return(!almost_cmp (r1, r2));
}

int CgTrans::almost_cmp(double r1, double r2)
// return 1 if r1>r2, -1 if r1<r2, 0 if r1==r2, within 10*precision
// always assume r1 == r2, if within precision
// Test "if(almost_cmp(r1,r2)>0)" for a strict r1>r2
{
  if(almost_zero(r1) && almost_zero(r2)) return 0;
  if(r1-(10.*FLT_EPSILON)*fabs(r1) > r2+(10.*FLT_EPSILON)*fabs(r2)) return 1;
  if(r1+(10.*FLT_EPSILON)*fabs(r1) < r2-(10.*FLT_EPSILON)*fabs(r2)) return -1;
  return 0;
}

int CgTrans::almost_zero(double r)
{
  if(fabs(r) < 10.*FLT_MIN) return 1;
  else return 0;
}


