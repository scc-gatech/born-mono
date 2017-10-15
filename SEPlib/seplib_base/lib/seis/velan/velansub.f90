module velan_subs_mod 
use sep_mod
  implicit none
  integer,private :: n_v,n_off,off_type,smooth
  real,private :: d_v,v_0,mute,d1,dtout,o1
  contains
!!$
!!$=head1 NAME
!!$
!!$velan_subs_mod - Do semblance analsysis
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call init_velan(vel0,delv,numv,o_1,d_1,d_tout,sm,mu)>
!!$
!!$C<call velan_exe(input,output,offsq)>
!!$
!!$=head1 INITIALIZER init_velan
!!$
!!$=head2 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item vel0 -  real
!!$
!!$        initial velocity
!!$
!!$=item delv -  real
!!$
!!$        sampling in velocity
!!$
!!$=item numv -  integer
!!$
!!$        number of velocities
!!$
!!$=item o_1  -  real
!!$
!!$        first time
!!$
!!$=item d_1  -  real
!!$
!!$        sampling in time
!!$
!!$=item d_tout -real
!!$
!!$        sampling of output times
!!$
!!$=item sm    - real
!!$
!!!$        smoothing window
!!$
!!$=item mu    - real
!!$
!!$        muting parameter to apply
!!$
!!$=back
!!$
!!$=head1 OPERATOR
!!$
!!$=head2 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item input - C< real(:,:)>
!!$
!!$        input  traces
!!$
!!$=item offsq - C< real(:)>
!!$
!!$        Offset for traces
!!$
!!$=back
!!$
!!$=head2 OUTPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item output - C<real(:,:)>
!!$
!!$         output velocity
!!$
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!!$
!!$Module to make semblance panels given irregular traces
!!$
!!$=head1 COMMENTS
!!$
!!$Used by Velan90
!!$
!!$=head1 SEE ALSO
!!$
!!$L<nmo_mod>,L<Nmo3d>
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepvelanf90>
!!$
!!$=cut
!!$
  subroutine init_velan(vel0,delv,numv,o_1,d_1,d_tout,sm,mu)
    integer :: numv,sm
    real :: delv,vel0,mu,d_1,d_tout,o_1
    d_v=delv
    v_0=vel0
    n_v=numv
    smooth=sm
    mute=mu
    d1=d_1
    dtout=d_tout
    o1=o_1
  end subroutine 
  subroutine velan_exe(input,output,offsq)
    real,dimension(:,:) :: input,output
    real, dimension(:) :: offsq
    integer :: iv,i2,nin,nout,n2,i1
    real :: velocity
    real,allocatable,dimension(:) :: num,denom
    integer,allocatable,dimension(:)::  count
integer:: ierr
    real :: wt,xs,tempr,tau,time,fract,numsum,densum
    integer :: mutetime,tuse,begi,endi,i0
    nin=size(input,1)
    n2=size(input,2)
    nout=size(output,1)
    allocate(num(nin),denom(nin),count(nin))
    output=0.
    do iv=1,n_v 
      velocity=v_0 + (iv -1 )* d_v        
      num=0
      denom=0
      count=0
      do i2=1,n2 
        xs=offsq(i2)/velocity**2
        tempr=sqrt(xs/(mute*mute-1))
        mutetime=max(1,nint((tempr-o1)/d1 +1.5))
        do i1=mutetime,nin 
          tau=o1 + dtout * (i1-1)
          tau=o1 +d1 * (i1-1)
          time=sqrt(xs+tau**2)
          tuse=1+(time-o1)/d1
          fract=((time-o1)/d1+1)-tuse
          if (tuse > 0 .and. tuse <= nin .and. time >0) then
            wt = tau/time * (1./sqrt(time))
            if (tuse < nin) then
              tempr=(input(tuse,i2)*(1-fract)+fract*input(tuse+1,i2))
            else if (tuse.eq.nin) then
              tempr=input(tuse,i2)
            end if
            if (tempr.ne.0) then
              num(i1)=num(i1) + tempr
              denom(i1)=denom(i1) + tempr**2
              count(i1)=count(i1) + 1
            end if
          end if
        end do
      end do 
      do i1=1,nout 
        numsum=0
        densum=0
        begi= (nin/nout)*i1 -smooth/2
        endi= (nin/nout)*i1 +smooth/2
        if (begi<1) then
          begi=1
        end if
        if (endi>nin) then
          endi=nin
        end if
        do i0=begi,endi     
          numsum=numsum+num(i0)**2
          densum=densum+denom(i0)*count(i0)
        end do 
        if (densum.eq.0) then
          output(i1,iv)=0.
        else
          output(i1,iv)=numsum/densum
        end if
      end do
    end do 
    ierr= srite("juna.H",output,size(output)*4)
  end subroutine 
end module 
