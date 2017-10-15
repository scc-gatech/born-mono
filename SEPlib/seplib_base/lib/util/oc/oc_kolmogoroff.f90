module oc_kolmogoroff_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_filter_mod
  implicit none

  integer, private :: npad   ! Filter padding factor

contains
  !----------------------------------------------------------------
  subroutine oc_kolfac_init(npad_in)
    integer, optional :: npad_in
    npad =2**5; if(present( npad_in)) npad = npad_in
  end subroutine oc_kolfac_init
  !----------------------------------------------------------------
  subroutine oc_kolfac(ss,ff)
    type(rfilter), intent(in)  :: ss
    type(cfilter), intent(out) :: ff

    integer :: i,n
    integer :: nk
    complex, dimension(:), allocatable :: ak

    nk=pad2(maxval(npad*ff%lag))
    allocate(ak(nk))

    !-------------------------
    ak=cmplx(0.)
    n=size(ss%lag)
    ak(1)=(1.,0.)
    do i=1+n/2,n
       ak(1+ss%lag(i))= (1.,0.) * (.999999*ss%flt(i))
    end do
    !-------------------------
    call kolmogoroff_factor(ak)
    !-------------------------
    n=size(ff%lag)
    do i=1,n
       ff%flt(i)= ak(1+ff%lag(i))
    end do

    deallocate(ak)
  end subroutine oc_kolfac
  !----------------------------------------------------------------  
  subroutine kolmogoroff_factor(ak)
    complex, dimension(:), intent(inout) :: ak
    integer :: n,i

    n=size(ak)
    do i=n,n/2,-1        !! make other side of autocorrelation
       ak(i) = ak(n-i+2)
    end do

    call fts(+1.,n,ak)   !! make spectrum
    do i=1,n
       ak(i)=clog(ak(i))
    end do
    call fts(-1.,n,ak)

    ak(1    ) = ak(1    ) / 2.
    ak(1+n/2) = ak(1+n/2) / 2.
    do i=1+n/2+1,n
       ak(i) = 0.
    end do

    call fts(+1.,n,ak)
    do i=1,n
       ak(i)=cexp(ak(i))
    end do
    call fts(-1.,n,ak)

    ak=ak/ak(1)
  end subroutine kolmogoroff_factor
  !----------------------------------------------------------------
  subroutine fts(signi,nx,cx)
    !   complex FT.  if (sign1>0) scale = 1; else scale=1/nx
    !
    !                      nx          signi*2*pi*i*(j-1)*(k-1)/nx
    !   cx(k)  =  scale * sum cx(j) * e
    !                     j=1             for k=1,2,...,nx=2**integer

    complex, dimension(:), intent(inout) :: cx
    real    :: signi
    integer :: nx

    integer :: i, j, k, m, istep
    real    :: arg
    complex :: cw, cdel, ct

    if(nx /= pad2(nx)) call seperr('fts: nx not a power of 2')

    if(signi < 0.) then
       do i= 1, nx
          cx(i) = cx(i) / nx
       end do
    end if

    j = 1
    do i= 1, nx
       if (i<=j) then
          ct = cx(j)
          cx(j) = cx(i)
          cx(i) = ct 
       end if

       m = nx/2
       do while (j>m .and. m>1)
          j = j-m; m = m/2 
       end do

       j = j+m
    end do

    k = 1
    do while (k<nx)
       istep = 2*k
       cw = 1.

       arg = signi*3.14159265/k
       cdel = cmplx( cos(arg), sin(arg))

       do m= 1, k
          do i= m, nx, istep
             ct=cw*cx(i+k)
             cx(i+k)=cx(i)-ct
             cx(i  )=cx(i)+ct 
          end do
          cw = cw * cdel
       end do

       k = istep
    end do

  end subroutine fts
  !----------------------------------------------------------------
  integer function pad2(n)
    integer :: n
    pad2 = 1
    do while(pad2<n)
       pad2=pad2*2
    end do
  end function pad2
  !----------------------------------------------------------------  
end module oc_kolmogoroff_mod
