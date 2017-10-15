module oc_helocut_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_filter_mod
  use oc_laplacian_mod
  use oc_helicon_mod
  use oc_polydiv_mod
  implicit none

  type(cfilter),    private :: caa,cbb
  type(rfilter),    private ::  aa, bb

  integer,          private :: maxmem

contains
  !----------------------------------------------------------------
  subroutine oc_helocut_init(t_,nf,niter,eps,maxmem_in)
    character(len=128)  ::t_
    real,    optional   ::            eps
    integer, optional   ::   nf,niter,maxmem_in

    maxmem = 0; if(present(maxmem_in)) maxmem=maxmem_in

    call oc_laplacian_factor(caa,t_,nf,niter,0.0,maxmem)
    allocate(aa%lag(size(caa%lag))); aa%lag=     caa%lag
    allocate(aa%flt(size(caa%flt))); aa%flt=real(caa%flt)

    call oc_laplacian_factor(cbb,t_,nf,niter,eps,maxmem)
    allocate(bb%lag(size(cbb%lag))); bb%lag=     cbb%lag
    allocate(bb%flt(size(cbb%flt))); bb%flt=real(cbb%flt)

  end subroutine oc_helocut_init
  !----------------------------------------------------------------
  integer function oc_helocut(adj,add, x_,yy_, op1,op2,op3,op4,op5,op6,op7,op8,op9) result(stat)
    character(len=128)                :: x_,yy_,t_,name
    logical,intent(in)                :: adj,add 
    integer, external, optional       :: op1,op2,op3,op4,op5,op6,op7,op8,op9
    type(fileinfo)                    :: filex,filey

    call oc_adjnull(adj,add,x_,yy_)

    call oc_allocatefile(filex, x_,  maxmem); call sseek(x_, 0,0)
    call oc_allocatefile(filey, yy_, maxmem); call sseek(yy_,0,0)
    call oc_checksimilarity(filex,filey, "oc_laplacian")

    name='_t.helocut'; t_ = oc_clone(T, x_ ,name ,maxmem)
    call oc_assign(t_,0.,maxmem)

    call oc_helicon_init(aa, maxmem)
    call oc_polydiv_init(bb, maxmem)

    if(adj) then
       stat = oc_polydiv(T,  F,t_,yy_)
       stat = oc_helicon(T,add,x_,t_ )
    else
       stat = oc_helicon(F,  F,x_,t_ )
       stat = oc_polydiv(F,add,t_,yy_)
    end if

    call oc_deallocatefile(filex); call sseek(x_, 0,0)
    call oc_deallocatefile(filey); call sseek(yy_,0,0)
    stat = 0
  end function oc_helocut
  !----------------------------------------------------------------
end module oc_helocut_mod
