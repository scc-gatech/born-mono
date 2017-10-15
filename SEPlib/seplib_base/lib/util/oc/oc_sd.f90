module oc_sd_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_filealgebra_mod
  use oc_llist_mod
  implicit none

contains
  !----------------------------------------------------------------
  integer function oc_sd(forget,x_,g_,rr_,gg_,s_,ss_,maxmem)
    integer,                          optional   :: maxmem
    logical,                          intent(in) :: forget 
    character(len=128),               intent(in) ::  x_,  g_,  s_
    character(len=128), dimension(:), intent(in) :: rr_, gg_, ss_
    integer                                      :: esize, k,n
    real(kind=db)    :: ralfa, rgdg, rgdr
    complex(kind=db) :: calfa, cgdg, cgdr

    call from_aux(x_,"esize",esize)

    n=size(rr_)

    if(esize /=8) then !! REAL
       rgdg = oc_rdp(gg_,gg_,maxmem)
       if(rgdg < rEPS) then
          call erexit('cgstep: g.g == 0')
       else
          rgdr = oc_rdp(gg_,rr_,maxmem)
          ralfa = - rgdr / rgdg         !! a = (G*.R)/(G*.G)
          call    oc_linear( x_   ,(/ x_   , g_   /),(/rONE,ralfa/),maxmem)  !! x = x + a g
          do k=1,n
             call oc_linear(rr_(k),(/rr_(k),gg_(k)/),(/rONE,ralfa/),maxmem)  !! R = R + a G
          end do
       end if

    else                !! COMPLEX
       cgdg = oc_cdp(gg_,gg_,maxmem)
       if(abs(cgdg) < abs(cEPS)) then
          call erexit('cgstep: g.g == 0')
       else
          cgdr = oc_cdp(gg_,rr_,maxmem)
          calfa = - cgdr / cgdg         !! a = (G*.R)/(G*.G)
          call    oc_linear( x_   ,(/ x_   , g_   /),(/cONE,calfa/),maxmem)  !! x = x + a g
          do k=1,n
             call oc_linear(rr_(k),(/rr_(k),gg_(k)/),(/cONE,calfa/),maxmem)  !! R = R + a G
          end do
       end if
    end if

    oc_sd = 0
  end function oc_sd
  !----------------------------------------------------------------
end module oc_sd_mod











