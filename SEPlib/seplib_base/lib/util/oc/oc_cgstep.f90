!!$=head1 NAME
!!$
!!$oc_cgtep - perform one step of conjugate gradient
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$ ierr=oc_cgstep(forget,x_,g_,rr_,gg_,s_,ss_,maxmem)
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
!!$   L<oc_cd> L<oc_cg>    L<oc_solver>
!!$
!!$=head1 LIBRARY
!!$
!!$sepocf90
!!$
!!$=cut
!!$
!!$>



module oc_cgstep_mod
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
  integer function oc_cgstep(forget,x_,g_,rr_,gg_,s_,ss_,maxmem)
    integer,                          optional   :: maxmem
    logical,                          intent(in) :: forget 
    character(len=128),               intent(in) ::  x_,  g_,  s_
    character(len=128), dimension(:), intent(in) :: rr_, gg_, ss_
    integer                                      :: esize, k,n
    complex(kind=db) :: csds, cgdg, cgds, csdg, cdeterm, cgdr, csdr, calfa, cbeta
    real(kind=db)    :: rsds, rgdg, rgds, rsdg, rdeterm, rgdr, rsdr, ralfa, rbeta

    !! search plane by solving 2-by-2
    !!  G* . (R - G.alfa - S.beta) = 0
    !!  S* . (R - G.alfa - S.beta) = 0

    call from_aux(x_,"esize",esize)
    n=size(rr_)

    if(esize /=8) then !! REAL
       if(forget) then
          rbeta = rZERO
          rgdg  = oc_rdp(gg_,gg_, maxmem)
          write(0,*)'rgdg=',rgdg
          if(rgdg < rEPS) call erexit('cgstep: g.g == 0')
          ralfa = - oc_rdp(gg_,rr_, maxmem)/rgdg
       else
          rgdg = oc_rdp(gg_,gg_, maxmem) !! G*.G
          rsds = oc_rdp(ss_,ss_, maxmem) !! S*.S
          rgds = oc_rdp(gg_,ss_, maxmem) !! G*.S
          rsdg = oc_rdp(ss_,gg_, maxmem) !! S*.G
          if( rgdg==0. .or. rsds==0.)  then
             oc_cgstep = 1 !! something is screwed
          write(0,*)'rgdg=',rgdg
          write(0,*)'rsds=',rsds
             return        !! stop iterations
          end if
          rdeterm = max(abs( rgdg*rsds - rsdg*rgds ),1.d-12)
          rgdr = oc_rdp(gg_,rr_, maxmem) !! G*.R
          rsdr = oc_rdp(ss_,rr_, maxmem) !! S*.R
          ralfa = -( rsds * rgdr - rsdg * rsdr ) / rdeterm
          rbeta = -(-rgds * rgdr + rgdg * rsdr ) / rdeterm
       end if

       call oc_linear( s_, (/g_ , s_ /), (/ralfa, rbeta/), maxmem)             !! s = a*g + b*s
       call oc_linear( x_, (/x_ , s_ /), (/rONE , rONE /), maxmem)             !! x =   x +   s
       do k=1,n
          call oc_linear(ss_(k), (/gg_(k), ss_(k)/), (/ralfa, rbeta/), maxmem) !! S = a*G + b*S
          call oc_linear(rr_(k), (/rr_(k), ss_(k)/), (/rONE , rONE /), maxmem) !! R =   R +   S
       end do
    else                !! COMPLEX
       if(forget) then
          cbeta = cZERO
          cgdg  = oc_cdp(gg_,gg_, maxmem)
          write(0,*)'abs(cgdg)=',abs(cgdg)
          if(abs(cgdg) < abs(cEPS)) call erexit('cgstep: g.g == 0')
          calfa = - oc_cdp(gg_,rr_, maxmem)/cgdg
       else
          cgdg = oc_cdp(gg_,gg_, maxmem) !! G*.G
          csds = oc_cdp(ss_,ss_, maxmem) !! S*.S
          cgds = oc_cdp(gg_,ss_, maxmem) !! G*.S
          csdg = oc_cdp(ss_,gg_, maxmem) !! S*.G
          cdeterm = max(abs( cgdg*csds - csdg*cgds ),1.d-12)
          cgdr = oc_cdp(gg_,rr_, maxmem) !! G*.R
          csdr = oc_cdp(ss_,rr_, maxmem) !! S*.R
          calfa = -( csds * cgdr - csdg * csdr ) / cdeterm
          cbeta = -(-cgds * cgdr + cgdg * csdr ) / cdeterm
       end if

       if( abs(calfa)<1.d-12 .and. abs(cbeta)<1.d-12) then
          write(0,*)'abs(calfa)=',abs(calfa)
          write(0,*)'abs(cbeta)=',abs(cbeta)
          oc_cgstep = 1 !! convergence?
          return        !! stop iterations
       end if

       call oc_linear( s_, (/g_ , s_ /), (/calfa, cbeta/), maxmem)
       call oc_linear( x_, (/x_ , s_ /), (/cONE , cONE /), maxmem)

       do k=1,n
          call oc_linear(ss_(k), (/gg_(k), ss_(k)/), (/calfa, cbeta/), maxmem)
          call oc_linear(rr_(k), (/rr_(k), ss_(k)/), (/cONE , cONE /), maxmem)
       end do
    end if
    oc_cgstep = 0
  end function oc_cgstep
  !----------------------------------------------------------------
end module oc_cgstep_mod
