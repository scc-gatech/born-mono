module oc_gmres_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_filealgebra_mod
  use oc_llist_mod
  implicit none

  type(list), public:: steps  

contains
  !----------------------------------------------------------------
  integer function oc_gmres(forget,x_,g_,rr_,gg_,s_,ss_,maxmem)
    integer,                          optional   :: maxmem
    logical,                          intent(in) :: forget 
    character(len=128),               intent(in) ::  x_,  g_,  s_
    character(len=128), dimension(:), intent(in) :: rr_, gg_, ss_
    integer                                      :: esize, k,n,i
    character(len=128)                           ::  gi_
    character(len=128), dimension(:), pointer    :: ggi_
    character(len=128)                           :: name

    real(kind=db)    :: ralfa, rbeta, rgama, rgdg, rsds, rgn
    complex(kind=db) :: calfa, cbeta, cgama, cgdg, csds, cgn

    call from_aux(x_,"esize",esize)

    n=size(rr_)
    allocate(ggi_(n))
    if(forget) steps=oc_llist_init(n)

    if(esize /=8) then !! REAL

       call oc_llist_rewind(steps)
       do i=1,oc_llist_depth(steps)
          call oc_llist_down(steps, gi_,ggi_, rgdg)
          rbeta = -oc_rdp((/g_/),(/gi_/),maxmem)/rgdg   !! bi = -(g*.gi)/(gi*.gi)
          call    oc_linear(g_    ,(/ g_   , gi_   /),(/rONE, rbeta/),maxmem) !! g = g + SUM(bi gi)
          call auxclose(gi_)
          do k=1,n
             call oc_linear(gg_(k),(/gg_(k),ggi_(k)/),(/rONE, rbeta/),maxmem) !! G = G + SUM(bi Gi)
             call auxclose(ggi_(k))
          end do
       end do

       rgn = oc_rdp((/g_/),(/g_/),maxmem)
       if(oc_llist_depth(steps)>0) then
          rgama = rgn/rgdg                     !! c = (g*.g)n+1 / (g*.g)n
       else
          rgama = rZERO
       end if
       call    oc_linear( s_   ,(/ g_   , s_   /),(/rONE,rgama/),maxmem) !! s = g + c.s
       do k=1,n
          call oc_linear(ss_(k),(/gg_(k),ss_(k)/),(/rONE,rgama/),maxmem) !! S = G + c.S
       end do

       rsds = oc_rdp(ss_,ss_,maxmem)
       if(rsds >rEPS) then
          ralfa = -rgn / rsds                   !! a = g*.g / S*.S
          call    oc_linear( x_   ,(/ x_   , s_   /),(/rONE, ralfa/),maxmem) !! x = x + a.s
          do k=1,n
             call oc_linear(rr_(k),(/rr_(k),ss_(k)/),(/rONE, ralfa/),maxmem) !! R = R + a.S
          end do
       end if

       call oc_llist_add(steps,g_,gg_,rgn,maxmem)
       if(forget) call oc_llist_chop(steps)

    else                !! COMPLEX

       call oc_llist_rewind(steps)
       do i=1,oc_llist_depth(steps)
          call oc_llist_down(steps, gi_,ggi_, cgdg)
          cbeta = -oc_cdp((/g_/),(/gi_/),maxmem)/cgdg   !! bi = -(g*.gi)/(gi*.gi)
          call    oc_linear(g_    ,(/ g_   , gi_   /),(/cONE, cbeta/),maxmem) !! g = g + SUM(bi gi)
          call auxclose(gi_)
          do k=1,n
             call oc_linear(gg_(k),(/gg_(k),ggi_(k)/),(/cONE, cbeta/),maxmem) !! G = G + SUM(bi Gi)
             call auxclose(ggi_(k))
          end do
       end do

       cgn = oc_cdp((/g_/),(/g_/),maxmem)
       if(oc_llist_depth(steps)>0) then
          cgama = cgn/cgdg                     !! c = (g*.g)n+1 / (g*.g)n
       else
          cgama = cZERO
       end if
       call    oc_linear( s_   ,(/ g_   , s_   /),(/cONE,cgama/),maxmem) !! s = g + c.s
       do k=1,n
          call oc_linear(ss_(k),(/gg_(k),ss_(k)/),(/cONE,cgama/),maxmem) !! S = G + c.S
       end do

       csds = oc_cdp(ss_,ss_,maxmem)
       if(abs(csds) > abs(cEPS)) then
          calfa = -cgn / csds                   !! a = g*.g / S*.S
          call    oc_linear( x_   ,(/ x_   , s_   /),(/cONE, calfa/),maxmem) !! x = x + a.s
          do k=1,n
             call oc_linear(rr_(k),(/rr_(k),ss_(k)/),(/cONE, calfa/),maxmem) !! R = R + a.S
          end do
       end if

       call oc_llist_add(steps,g_,gg_,cgn,maxmem)
       if(forget) call oc_llist_chop(steps)

    end if
    deallocate(ggi_)
    oc_gmres = 0
  end function oc_gmres
  !----------------------------------------------------------------
end module oc_gmres_mod







