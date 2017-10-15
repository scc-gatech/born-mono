!!$<
!!$=head1 NAME
!!$
!!$oc_cd - perform one step of conjugate directions
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$ierr= oc_cd (forget,x,g,rr,gg,s,ss,maxmem)
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$
!!$=item forget,x,gg,rr,gg,s,ss - Standard  step parameters
!!$
!!$=item maxmem - integer, optional
!!$
!!$Maximum memory to use for blocking operation
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ One step of out of core conjugate directions 
!!$
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$   L<oc_cgstep> L<oc_cg> L<oc_solver>
!!$
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$sepocf90
!!$
!!$=cut
!!$
!!$>


module oc_cd_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================

  !! conjugate directions with full memory
  !! TODO: add limited memory
  use oc_global_mod
  use oc_filealgebra_mod
  use oc_llist_mod
  implicit none

  type(list), public  :: steps

contains
  !----------------------------------------------------------------
  integer function oc_cd(forget,x_,g_,rr_,gg_,s_,ss_,maxmem)
    integer,                          optional   :: maxmem
    logical,                          intent(in) :: forget 
    character(len=128),               intent(in) ::  x_,  g_,  s_
    character(len=128), dimension(:), intent(in) :: rr_, gg_, ss_
    integer                                      :: esize, k,n,i
    character(len=128)                           ::  si_
    character(len=128), dimension(:), pointer    :: ssi_
    character(len=128)                           :: name

    real(kind=db)    :: ralfa, rbeta, rsds, rsdr
    complex(kind=db) :: calfa, cbeta, csds, csdr

    call from_aux(x_,"esize",esize)

    n=size(rr_)
    allocate(ssi_(n))
    if(forget) steps=oc_llist_init(n)

    if(esize /=8) then !! REAL
       call    oc_linear( s_   ,(/ g_   /),(/rONE/),maxmem)    !! s=g - initiate SUM
       do k=1,n
          call oc_linear(ss_(k),(/gg_(k)/),(/rONE/),maxmem)    !! S=G - initiate SUM
       end do

       call oc_llist_rewind(steps)
       do i=1,oc_llist_depth(steps)
          call oc_llist_down(steps, si_,ssi_, rsds)
          rbeta = -oc_rdp(gg_,ssi_,maxmem)/rsds                               !! bi = -(G*.Si)/(Si*.Si)
          call    oc_linear(s_    ,(/ s_   , si_   /),(/rONE, rbeta/),maxmem) !! s = g + SUM(bi si)
          call auxclose(si_)
          do k=1,n
             call oc_linear(ss_(k),(/ss_(k),ssi_(k)/),(/rONE, rbeta/),maxmem) !! S = G + SUM(bi Si)
             call auxclose(ssi_(k))
          end do
       end do

       rsds = oc_rdp(ss_,ss_,maxmem)    !! S*.S
       if(rsds > rEPS) then
          call oc_llist_add(steps,s_,ss_,rsds,maxmem)
          if(forget .and. steps%depth>1) call oc_llist_chop(steps)

          rsdr = oc_rdp(ss_,rr_,maxmem) !! S*.R
          ralfa = - rsdr / rsds                                              !! a = (S*.R)/(S*.S)
          call    oc_linear( x_   ,(/ x_   , s_   /),(/rONE,ralfa/),maxmem)  !! x = x + a s
          do k=1,n
             call oc_linear(rr_(k),(/rr_(k),ss_(k)/),(/rONE,ralfa/),maxmem)  !! R = R + a S
          end do
       end if
    else                !! COMPLEX
       call    oc_linear( s_   ,(/ g_   /),(/cONE/),maxmem)    !! s=g - initiate SUM
       do k=1,n
          call oc_linear(ss_(k),(/gg_(k)/),(/cONE/),maxmem)    !! S=G - initiate SUM
       end do

       call oc_llist_rewind(steps)
       do i=1,oc_llist_depth(steps)
          call oc_llist_down(steps, si_,ssi_, csds)
          cbeta = -oc_cdp(gg_,ssi_,maxmem)/csds                               !! bi = -(G*.Si)/(Si*.Si)
          call    oc_linear(s_    ,(/ s_   , si_   /),(/cONE, cbeta/),maxmem) !! s = g + SUM(bi si)
          call auxclose(si_)
          do k=1,n
             call oc_linear(ss_(k),(/ss_(k),ssi_(k)/),(/cONE, cbeta/),maxmem) !! S = G + SUM(bi Si)
             call auxclose(ssi_(k))
          end do
       end do

       csds = oc_cdp(ss_,ss_,maxmem)    !! S*.S
       if(abs(csds) > abs(cEPS)) then
          call oc_llist_add(steps,s_,ss_,csds,maxmem)
          if(forget .and. steps%depth>1) call oc_llist_chop(steps)

          csdr = oc_cdp(ss_,rr_,maxmem) !! S*.R
          calfa = - csdr / csds                                              !! a = (S*.R)/(S*.S)
          call    oc_linear( x_   ,(/ x_   , s_   /),(/cONE,calfa/),maxmem)  !! x = x + a s
          do k=1,n
             call oc_linear(rr_(k),(/rr_(k),ss_(k)/),(/cONE,calfa/),maxmem)  !! R = R + a S
          end do
       end if
    end if

    deallocate(ssi_)
    oc_cd = 0
  end function oc_cd
  !----------------------------------------------------------------
end module oc_cd_mod











