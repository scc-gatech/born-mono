!!$
!!$=head1 NAME
!!$
!!$nmo_mod - Perform nmo
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initializer-C<call nmo_init(slowness,offset,o1,d1)>
!!$
!!$operator-C<call nmo_nearest(adj,tt,zz)>
!!$
!!$operator-C<call nmo_linear(adj,tt,zz)>
!!$
!!$=head1 INITIALIZER
!!$
!!$=head2 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item slowness - C<real(:)>
!!$
!!$           Slowness(t)
!!$
!!$=item offset   - C<real(:)>
!!$
!!$           Offset
!!$
!!$=item o1       - real
!!$
!!$           First sample in time
!!$
!!$=item d1       - real
!!$
!!$           Sampling in time
!!$
!!$=item inverse  - logical
!!$
!!$           Wheter or not to do inverse
!!$
!!$=back
!!$
!!$=head1  OPERATORS
!!$
!!$=over 4
!!$
!!$=item adj   - logical
!!$
!!$        Whether or not do adjoint
!!$
!!$=item tt    - C<real(:,:)>
!!$
!!$        Input
!!$
!!$=item zz    - C<real(:,:)>
!!$
!!$        Output
!!$
!!$=back
!!$
!!$
!!$=head1 DESCRIPTION
!!$Nmo by nearest or linear interpolation neighbor for a gather
!!$
!!$=head1 COMMENTS
!!$
!!$NMO, its transpose, inverse, and inverse transpose by nearest neighbor.
!!$
!!$adj=.false. inv=.false.:        zz(iz) = [NMO]                   tt(it)
!!$
!!$adj=.true. inv=.false.:        tt(it) = [NMO adj]         zz(iz)
!!$
!!$adj=.false. inv=.true.:        tt(it) = [NMO inverse]           zz(iz)
!!$
!!$adj=.true. inv=.true.:        zz(iz) = [NMO inverse transpose] tt(it)
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<Nmo3d>
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepvelanf90>
!!$
!!$=cut
module nmo_mod 
  implicit none
  real,save, pointer,private :: slow(:)
  real,save, pointer,private :: x(:) !offset
  real,save,private :: t0,dt
  integer,save,private :: nt,n2
  logical, save,private :: inv
  contains
  subroutine nmo_init(slowness,offset,o1,d1,inverse,first)
    real,target :: slowness(:),offset(:)
    real :: o1,d1
    logical :: inverse,first
    inv=inverse
    t0=o1
    dt=d1
    slow=>slowness
    if (.not. first) then
      deallocate(x)
    end if
    allocate(x(size(offset)))
    x=offset
    first=.false.
    nt=size(slow)
    n2=size(x)
  end subroutine 
!>
!
  subroutine nmo_nearest( adj, tt, zz )
    logical :: adj
    integer   it, iz, off, last,i2
    real   tt(:,:), zz(:,:)
    real t, z, xs, arg
    integer :: itab(nt)
    real :: count(nt)
    if (inv) then
      count=0.
    end if
    do i2=1,n2 
      z = t0 + nt * dt
      t = z
      off = nt + 1
      do iz = nt, 1, -1   
        xs = x(i2) * slow(iz)
        arg = z * z + xs * xs
! next line replaceable by:        t = sqrt ( arg )
        t = (arg + t * t) / (t + t)
        it = 0.5 + (t - t0) / dt
        if ( it <= nt  ) then
          count(it)=                        count(it)+ 1.
          itab(iz) = it
        else
          off = iz
        end if 
        z = z - dt
      end do 
      last = off - 1
      if ( adj  .and. inv) then
        where(count.ne.0.)
          tt(:,i2)=tt(:,i2)/count
end where
      end if
      if ( (adj .and. inv) .or. (.not. adj .and. .not. inv) ) then
        do iz = 1, last  
          it = itab(iz)
          zz(iz,i2) =                        zz(iz,i2) + tt(it,i2)
        end do
      else
        do iz = 1, last 
          it = itab(iz)
          tt(it,i2)=                        tt(it,i2)+ zz(iz,i2)
        end do
      end if 
      if ( .not. adj .and. inv) then
        where(count.ne.0.)
          tt(:,i2)=tt(:,i2)/count
end where
      end if
    end do
  end subroutine 
!<
! nmo_l
!
! USAGE
! call nmo_l(adj, tt, zz)
!
! DESCRIPTION
! Nmo by linear interpolation for a gather
!
! INPUT PARAMETERS
!  adj    -  logical  .true.=adjoint, .false.=forward
!  tt     -  real(:,:)  data
!  zz     -  real(:,:)  model
!
! CATEGORY
! Lib:Velan
!
! COMMENTS
! NMO, adjoint NMO, and inverse NMO by linear interpolation.
! adj=.false. inv=.false.:        zz(iz) = [NMO]                   tt(it)
! adj=.true. inv=.false.:        tt(it) = [NMO adj]         zz(iz)
! adj=.false. inv=.true.:        tt(it) = [NMO inverse]           zz(iz)
! adj=.true. inv=.true.:        zz(iz) = [NMO inverse adjoint] tt(it)
!
! COMPILE LEVEL
! DISTR
!>
!
  subroutine nmo_linear(adj,tt,zz)
    logical :: adj
    integer  it, i2, iz
    real  tt(:,:), zz(:,:),t,  z, tm, tpart, xs, arg
    integer :: itab(nt)
    real :: bb(nt),cc(nt),w0(nt),w1(nt),ts(nt)
    do i2=1,n2 
      cc=0.
      bb=0.                 ! initialize tridiagonal coefs.
      z = t0 + nt * dt
      t = z
      do iz = nt, 1, -1   
        xs = x(i2) * slow(iz)
        arg = z * z + xs * xs
! next line replaceable by:        t = sqrt ( arg )
        t = (arg + t * t) / (t + t)
        it = (t-t0) / dt + .00001
        tm = t0 + it * dt
        tpart = t - tm
        w0(iz) = (dt - tpart) / dt
        w1(iz) = 1. - w0(iz)
        if (it >0 .and.  it+1 <= nt ) then
! interior
          itab(iz)  = it
          bb(it)  =                        bb(it)  +  w0(iz) * w0(iz)
          cc(it)  =                        cc(it)  +  w1(iz) * w0(iz)
          bb(it+1)=                        bb(it+1)+  w1(iz) * w1(iz)
        else if ( it <= nt) then
! at edge
          itab(iz) =  - it
          bb(it)  =                        bb(it)  + w0(iz) * w0(iz)
          cc(it)  =                        cc(it)  + w1(iz) * w0(iz)
        else
! off end
          itab(iz) = 0
        end if 
        z = z - dt
      end do 
      ts=tt(:,i2)  ! avoid destruction of input.
      if ( adj .and. inv ) then
! transpose pseudoinverse
        where(bb.eq.0.)
          ts=0.
end where
        call vtris(nt, cc, bb, cc, ts, ts)   ! vtris allows overlay.
      end if
      if ((adj .and. inv).or.(.not. adj .and. .not. inv)) then
! Operator itself or transpose pseudoinverse
        do iz = 1, nt  
          it = itab(iz)
          if ( it > 0 ) then
            zz(iz,i2) =                                zz(iz,i2) + w0(iz) * ts(it)
            zz(iz,i2) =                                zz(iz,i2) + w1(iz) * ts(it+1)
          else if ( it < 0) then
            it = -it
            zz(iz,i2) =                                zz(iz,i2) + w0(iz) * ts(it)
          end if
        end do
      else
! transpose or inverse
        do iz = 1, nt  
          it = itab(iz)
          if ( it > 0 ) then
            ts(it)  =                                ts(it)  + w0(iz) * zz(iz,i2)
            ts(it+1)=                                ts(it+1)+ w1(iz) * zz(iz,i2)
          else if ( it < 0 ) then
            it = -it
            ts(it)   =                                ts(it)   + w0(iz) * zz(iz,i2)
          end if
        end do
      end if 
      if ( .not. adj .and.  inv) then
! pseudoinverse
        do it = 1, nt 
          if ( bb(it) .eq. 0.0) then
            bb(it) = 1.0
            ts(it) = 0.0
          end if
        end do 
        call vtris(nt, cc, bb, cc, ts, tt(:,i2))
      else
        tt(:,i2)=ts
      end if
    end do
  end subroutine 
end module 
