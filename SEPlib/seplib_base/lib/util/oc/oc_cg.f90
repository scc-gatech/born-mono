!!$=head1 NAME
!!$
!!$oc_cg - perform one step of conjugate gradient 
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$ ierr=oc_cg(forget,x_,g_,rr_,gg_,s_,ss_,maxmem)
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$
!!$=item forget,x_,g_,rr_,gg_,s_,ss_ - Standard OC step parameters
!!$
!!$=item maxmem - integer, optional
!!$
!!$Maximum memory to use for blocking operation
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Performs one step of out of core conjugate gradient 
!!$
!!$=head1 SEE ALSO
!!$
!!$   L<oc_cd> L<oc_cgstep>    L<oc_solver>
!!$
!!$=head1 LIBRARY
!!$
!!$sepocf90
!!$
!!$=cut
!!$
!!$>




module oc_cg_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_filealgebra_mod
  implicit none

contains
  !----------------------------------------------------------------
  integer function oc_cg(forget,x_,g_,rr_,gg_,s_,ss_,maxmem)
    integer,                          optional   :: maxmem
    logical,                          intent(in) :: forget 
    character(len=128),               intent(in) ::  x_,  g_,  s_
    character(len=128), dimension(:), intent(in) :: rr_, gg_, ss_
    integer                                      :: esize, k,n
    real(kind=db)                                :: rn
    real(kind=db),   save                        :: rnp
    real(kind=db)                                :: ralfa, rbeta, rsds

    complex(kind=db)                             :: cn
    complex(kind=db),save                        :: cnp
    complex(kind=db)                             :: calfa, cbeta, csds

    call from_aux(x_,"esize",esize)
    n=size(rr_)

    if(esize /=8) then !! REAL
       !-------------
       rn = oc_rdp((/g_/),(/g_/), maxmem) !! g*.g
       !-------------
       if(forget .or. abs(rnp) < abs(rEPS)) then
          rbeta=0
       else
          rbeta=rn/rnp
       end if
       !-------------
       call    oc_linear( s_,    (/ g_   , s_   /), (/rONE,rbeta/),maxmem)    !! s= g+ b.s
       do k=1,n
          call oc_linear(ss_(k), (/gg_(k),ss_(k)/), (/rONE,rbeta/),maxmem)    !! S= G+ b.S 
       end do
       rsds = oc_rdp(ss_,ss_, maxmem) !! S*.S
       !-------------
       if(abs(rsds) > abs(rEPS)) then
          ralfa = - rn / rsds         !! a = g*.g/S*.S
          call    oc_linear( x_,    (/ x_   , s_   /), (/rONE,ralfa/),maxmem) !! x= x+ a.s
          do k=1,n
             call oc_linear(rr_(k), (/rr_(k),ss_(k)/), (/rONE,ralfa/),maxmem) !! R= R+ a.S
          end do
       end if
       !-------------
       rnp = rn !! remember g*.g
       !-------------
    else                !! COMPLEX
       !-------------
       cn = oc_cdp((/g_/),(/g_/), maxmem) !! g*.g
       !-------------
       if(forget .or. abs(cnp) < abs(cEPS)) then
          cbeta=0
       else
          cbeta=cn/cnp
       end if
       !-------------
       call    oc_linear( s_,    (/ g_   , s_   /), (/cONE,cbeta/),maxmem)    !! s= g+ b.s
       do k=1,n
          call oc_linear(ss_(k), (/gg_(k),ss_(k)/), (/cONE,cbeta/),maxmem)    !! S= G+ b.S 
       end do
       csds = oc_cdp(ss_,ss_, maxmem) !! S*.S
       !-------------
       if(abs(csds) > abs(cEPS)) then
          calfa = - cn / csds
          call    oc_linear( x_,    (/ x_   , s_   /), (/cONE,calfa/),maxmem) !! x= x+ a.s
          do k=1,n
             call oc_linear(rr_(k), (/rr_(k),ss_(k)/), (/cONE,calfa/),maxmem) !! R= R+ a.S
          end do
       end if
       !-------------
       cnp = cn !! remember g*.g
       !-------------
    end if
    oc_cg = 0
  end function oc_cg
  !----------------------------------------------------------------
end module oc_cg_mod











