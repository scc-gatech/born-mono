
solver_pg::solver_pg(my_operator *o, my_vector *d,my_vector *minv, my_vector *maxv, double mintol){

  min_vec=minv;
  max_vec=maxv;
  mint=mintol;
  op=o;
  dat=d;
  my_vector *junk=max_vec->clone();
  junk->scale_add(1,min_vec,-1.);
  min_range=junk->min_val();
  delete junk;


}

my_vector *solver_pg::solve(my_vector *x0,int niter){


/*
function [x,histout,costdata] = projbfgs(x0,f,up,low,tol,maxit)
%
% C. T. Kelley, June 11, 1998
%
% This code comes with no guarantee or warranty of any kind.
%
% function [x,histout,costdata] = projbfgs(x0,f,up,low,tol,maxit)
%
% projected BFGS with Armijo rule, simple linesearch 
% 
%
% Input: x0 = initial iterate
%        f = objective function,
%            the calling sequence for f should be
%            [fout,gout]=f(x) where fout=f(x) is a scalar
%              and gout = grad f(x) is a COLUMN vector
%        up = vector of upper bounds
%        low = vector of lower bounds
%        tol = termination criterion norm(grad) < tol
%              optional, default = 1.d-6
%        maxit = maximum iterations (optional) default = 1000
%
% Output: x = solution
%         histout = iteration history   
%             Each row of histout is
%   [norm(projgrad), f, number of step length reductions, iteration count,
%            relative size of active set]
%         costdata = [num f, num grad, num hess] (for steep, num hess=0)
%
if nargin < 4
error(' projbfgs requires bounds ');
end
xc=x0; ndim=length(up); kku=zeros(ndim,1); kkl=zeros(ndim,1);
%
% list of active indices 
%
alist=zeros(ndim,1);
%
for i=1:ndim
        kku(i)=up(i); kkl(i)=low(i);
	if kkl(i) > kku(i)
        error('lower bound exceeds upper bound')
        end
end
%
% put initial iterate in feasible set
%
if norm(xc - kk_proj(xc,kku,kkl)) > 0
      disp(' initial iterate not feasibile ');
*/
 my_vector *xc=x0->clone();
 kk_proj(xc);
 
 /*
      xc=kk_proj(xc,kku,kkl);
end
alp=1.d-4;
nsmax=5; ystore=zeros(ndim,nsmax); sstore=ystore; ns=0;
*/
std::vector<my_vector *> ystore,sstore;
int nsmax=5, ns=0;
for(int i=0; i < nsmax; i++) {
  ystore.push_back(xc->clone_zero());
  sstore.push_back(xc->clone_zero());
}

/*


if nargin < 6
maxit=1000; 
end
if nargin < 5
tol=1.d-6;
end
itc=1; 
*/
my_vector *gc=xc->clone();
double fc=feval(x0,gc);




//[fc,gc]=feval(f,xc); 
//numf=1; numg=1; numh=0;
//ithist=zeros(maxit,5);
//pgc=xc - kk_proj(xc - gc,kku,kkl);
my_vector *pgc=xc->clone();
pgc->scale_add(1.,gc,-1.);
my_vector *alist=xc->clone_zero();
kk_proj(pgc,alist);


//ia=0; alist=zeros(ndim,1);
//tst=kku-kkl; lim1=.5*min(tst);
//epsilon=min(lim1,norm(pgc));
  double pgc_dot=sqrt(pgc->dot(pgc));
  double epsilon=min(min_range*.5,pgc_dot);
  

/*
for i=1:ndim; if(xc(i)==kku(i) | xc(i)==kkl(i)) ia=ia+1; end; end;
for i=1:ndim; 
         if( min(kku(i)-xc(i),xc(i)-kkl(i)) < epsilon) 
                 alist(i)=1; 
         end;
end;
*/



//ithist(1,5)=ia/ndim;
//ithist(1,1)=norm(pgc); ithist(1,2) = fc; ithist(1,4)=itc-1; ithist(1,3)=0; 

fprintf(stderr,"ON_BOUND %d functional=%g  grad_dot=%g\n",
   (int)alist->sum(), fc,pgc_dot);

int itc=0;

  double lambda;
  //while(norm(pgc) > tol & itc <= maxit)
   //     lambda=1;
  my_vector *dsd=gc->clone();
  my_vector *tmp=gc->clone();
  my_vector *xt=gc->clone();
  int numf=1,numg=1,numh=0,iarm;
  double fgoal,alp=1e-4;
  while(pgc_dot>tol && itc < niter){
    lambda=1.;
    dsd->scale_add(0.,gc,-1.); //dsd=-gc; 
    bfgrsp(dsd,ystore,sstore,ns,dsd,alist);//dsd=bfgsrp(ystore,sstore,ns,dsd,alist);
    tmp->add_scal(0.,gc,-1.); 
    proja(tmp,tmp,alist);//proja(-gc,alist);
    dsd->scale_add(1.,tmp,1.);//dsd=dsd+proja(-gc,alist);
    
    tmp=scale_add(0.,dsd,lambda);
    tmp->scale_add(1.,xc,1.);
    kk_proj(xt,tmp);//xt=kk_proj(xc+lambda*dsd,kku,kkl);
    
    ft=feval(xt,tmp); //ft=feval(f,xt);   
    /*numf=numf+1;
	iarm=0; itc=itc+1;
        pl=xc - xt; fgoal=fc-(gc'*pl)*alp;
%
%       simple line search
%
*/
    iarm=0; itc=itc+1;
    tmp->scale_add(0.,xc,1.);
    tmp->scale_add(1.,xt,-1.);
    fgoal=fc-gc->dot(tmp)*alp;
  /*
	while(ft > fgoal)
                lambda=lambda*.1;
		iarm=iarm+1;
                xta=xt;
		xt=kk_proj(xc+lambda*dsd,kku,kkl);
                pl=xc-xt;
		ft=feval(f,xt); numf = numf+1;
		if(iarm > 10) 
		disp(' Armijo error in steepest descent ')
                histout=ithist(1:itc,:); costdata=[numf, numg, numh];
                x=xc;
		return; end
                fgoal=fc-(gc'*pl)*alp;
	end
	*/
	while(ft>fgoal){
	   lambda=lambda*.1;
	   iarm=iarm+1;
	   tmp=scale_add(0.,xc,1.);
	   tmp->scale_add(1.,dsd,lambda);
	   kk_proj(xt,tmp);
	   tmp->scale_add(0.,xx,1.);
	   tmp->scale_add(1.,xt,-1.);
	   ft=feval(xt);
	   numf=numf+1;
	   if(iarm>10){
	     fprintf(stderr,"Armijo error in steepest descent\n");
	     x->scale_add(0.,xc,1.);
	     return;
	   }
	   fgoal=fc-gc->dot(tmp)*alp;
	 }
	/*   
	[fc,gp]=feval(f,xt); numf=numf+1; numg=numg+1;
        y=gp-gc; s=xt-xc;
        gc=gp; xc=xt; pgc=xc-kk_proj(xc-gc,kku,kkl); 
     */
    fc=feval(xt,gp);
    numf+=1; numg+=1;
    y->scale_add(1.,gp,-1.,gc);
    s->scale_add(1.,xt,-1.,xc);
    gc->scale_add(0.,gp,1.);
    xc->scale_add(0.,xt,1.);
    tmp->scale_add(1.,xc,-1.,gc);
    kk_proj(pgc,tmp);
    
    double epsilon=MIN(lim1,sqrt(pgc->dot(pgc)));
     
     
        epsilon=min(lim1,norm(pgc));
        alist=zeros(ndim,1); ial=0;
                for i=1:ndim; 
                     if( min(kku(i)-xc(i),xc(i)-kkl(i)) < epsilon) 
                         alist(i)=1; ial=ial+1; 
                     end;
                end;
        y=proji(y,alist); s=proji(s,alist); 
%
%   restart if y'*s is not positive or we're out of room
%
        yts=y'*s;
        if yts <=0 ns=0;
          end
        if ns==nsmax 
            ns=0;
        elseif yts > 0
                ns=ns+1;
                alpha=sqrt(yts);
                sstore(:,ns)=s/alpha; ystore(:,ns)=y/alpha;
        end
	ithist(itc,1)=norm(pgc); ithist(itc,2) = fc; 
	ithist(itc,4)=itc-1; ithist(itc,3)=iarm;
ia=0; for i=1:ndim; if(xc(i)==kku(i) | xc(i)==kkl(i)) ia=ia+1; end; end;
ithist(itc,5)=ia/ndim;
end
x=xc; 
histout=ithist(1:itc,:); costdata=[numf, numg, numh];

/*
%
% projection onto active set
%
function px = kk_proj(x,kku,kkl)
ndim=length(x);
px=zeros(ndim,1);
px=min(kku,x); 
px=max(kkl,px);
*/
void solver_pg::kk_proj(my_vector *x,my_vector *on_b){
  x->take_max(min_vec,on_b);
  x->take_min(max_vec,on_b);

}



/*
%
% bfgsrp
%
% C. T. Kelley, Dec 20, 1996
%
% This code comes with no guarantee or warranty of any kind.
%
% This code is used in projbfgs.m
%
% There is no reason to ever call this directly.
%
% form the product of the generalized inverse of the
% bfgs approximate Hessian
% with a vector using the recursive approach
%
function dnewt=bfgsrp(ystore,sstore,ns,dsd,alist)
dnewt=proji(dsd,alist);
if (ns==0)
return;
end;
sstore(:,ns)=proji(sstore(:,ns),alist);
ystore(:,ns)=proji(ystore(:,ns),alist);
beta=sstore(:,ns)'*dsd; dnewt=dsd-beta*ystore(:,ns);
ndim=length(dsd); xlist=zeros(ndim,1);
dnewt=bfgsrp(ystore,sstore,ns-1,dnewt,xlist);
dnewt=dnewt+(beta-ystore(:,ns)'*dnewt)*sstore(:,ns); 
dnewt=proji(dnewt,alist);
*/
my_vector *solver_pg::bfgsrp(my_vector *out,std::vector<my_vector*> ystore, std::vector<my_vector*> sstore,
  int ns, my_vector *dsd, my_vector *alist){
   if(ns==0){
     proj1(out,dsd,alist);
   }
 
   proji(sstore[ns],sstore[ns],alist);
   proji(ystore[ns],ystore[ns],alist);
   double beta=sstore[ns]->dot(dsd);
   out->scale_add(0.,dsd,1.);
   out->scale_add(1.,-beta,ystore[ns]);
   std::vector<my_vector *> xlist;
   xlist->push_back(dnewt->clone_zero());
   bfgsrp(out,ystore,sstore,ns-1,dnewt,xlist);
   double alpha=beta-ystore[0]->dot(out);
   out->scale_add(1.,sstore[ns],alpha);
   out=proji(out,alist);
   xlist->clear();
}


/*
%
% projection onto epsilon-inactive set
%
function px=proji(x,alist)
*/
void solver_pg::proj1(my_vector *out,my_vector *vec, my_vector *change){
 
//ndim=length(x); px=x;
//for k=1:ndim if alist(k) == 1 px(k)=0; end; end
  my_vector *junk=change->clone();
  junk->mult(vec);
  out->scale_add(0.,vec,1.);
  out->scale_add(1.,junk,-1.);
  delete junk;
  
}
/*
%
%  projection onto epsilon-active set
%
function px=proja(x,alist)
ndim=length(x); px=x;
for k=1:ndim if alist(k) == 0 px(k)=0; end; end
*/
void solver_pg::proja(my_vector *out, my_vector *vec, my_vector *change){
  my_vector *junk=change->clone();
  junk->set_val(1.);
  junk->scale_add(1.,change,-1.);
  proj1(out,vec,junk);
  delete junk;
}

