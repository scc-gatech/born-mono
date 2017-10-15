module interpolate_mod 
  use sep_mod
  use sep_func_mod
  implicit none
  real,private,save :: oout,dout
  integer,private,save :: nout,boundary
  real,private,pointer,save :: weight(:)
  contains
!!$
!!$=head1 NAME
!!$
!!$interpolate_mod - Linear interpolation
!!$
!!$=head1 SYNOPSIS
!!$
!!$Initialized=C<call init_1d_linear_interp(o1out,d1out,n1out,b)>
!!$
!!$Operator=C<call interp_1d_linear(input,output,var)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over
!!$
!!$=item o1out - real
!!$
!!$      output axis begining
!!$
!!$=item d1out - real
!!$
!!$      output axis sampling
!!$
!!$=item n1out - integer
!!$
!!$      number of output points
!!$
!!$=item b -integer
!!$
!!$      boundary condition (0-zero outside var boundary, 1 replicate
!!$       last value)
!!$
!!$=item input -  C<real(:)>
!!$
!!$      Input values
!!$
!!$=item var -  C<real(:)>
!!$
!!$      Input sample location
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item output - real(:)
!!$
!!$      output linear interpolated
!!$
!!$=back
!!$
!!!$=head1 DESCRIPTION
!!$
!!$Interpolate an irregular sampled function to a regular function.
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepauxf90>
!!$
!!$=cut
!!>
  subroutine init_1d_linear_interp(o1out,d1out,n1out,b)
    integer n1out,b
    real o1out,d1out
    oout=o1out
    dout=d1out
    nout=n1out
    boundary=b
    allocate(weight(nout))
  end subroutine 
  subroutine interp_1d_linear(input,output,var)
    real :: input(:),output(:),var(:)
    integer i1,b1,e1,a1,z1,ia,posi,nin
    real :: pos,frac
!first go through and calculate the weights
    weight=0.
    output=0.
    nin=size(input)
    do i1=1,nin 
      pos=(var(i1)-oout)/dout + 1.
      posi=floor(pos)
      frac=real(pos-posi)
      if (pos >0.) then
        if (pos < 1.) then
          weight(1)=                        weight(1)+frac
          output(1)=                        output(1)+input(i1)*frac
        else if (pos < nout) then
          weight(posi)=                        weight(posi)+ 1. -frac
          output(posi)=                        output(posi)+input(i1)*(1.-frac)
          weight(1+posi)=                        weight(1+posi)+ frac
          output(1+posi)=                        output(1+posi)+input(i1)*frac
        else if (pos < nout +1) then
          weight(nout)=                        weight(nout)+1.-frac
          output(nout)=                        output(nout)+input(i1)*(1.-frac)
        end if
      end if
    end do 
    if (all(weight < .5)) then
      call erexit("interpolation failure output/input incompatible")
    end if
    where(weight.ne.0)
      output=output/weight
end where
    b1=1
    do while (weight(b1).eq.0. )
      b1=  b1+1
    end do 
    e1=nout
    do while (weight(e1).eq.0.)
      e1= e1-1
    end do 
!apply boundary conditon
! 0- zeros, 1-maintain last value
    if (b1>1) then
      if (boundary.eq.0) then
        output(1:b1-1)=0.
      else
        output(1:b1-1)=output(b1)
      end if
    end if
    if (e1<nout) then
      if (boundary.eq.0) then
        output(e1+1:nout)=0.
      else
        output(e1+1:nout)=output(e1)
      end if
    end if
!now let's fill anything missing in the middle
    i1=b1+1
    a1=b1
    do while (i1<=e1)
      if (weight(i1).ne.0.) then
        if (a1 .ne. i1+1) then
!we just found the end of an empty strip
          do ia=a1+1,i1-1 
            frac=real(ia-a1)/real(i1-a1)
            output(ia)=frac*output(i1)+(1.-frac)*output(a1)
          end do
        end if
        a1=i1
      end if
      i1=        i1+1
    end do
  end subroutine 
end module 
