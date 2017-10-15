module interp_new

contains

  function lg_int (x, w) result (stat) 
    ! lagrange interpolation
    integer             :: stat
    real, intent (in)   :: x
    real, dimension (:) :: w

    integer             :: i, j, nf, nc
    real                :: f, xi

    nf = size (w); nc = (nf+1)*0.5
    do i = 1, nf
       f = 1.
       xi = x + nc - i
       do j = 1, nf
          !write (0,*) j, nf, f
          if (i /= j) f = f * (1. + xi / (i - j))
       end do
       w (i) = f
    end do
    stat = 0
  end function lg_int

  function taylor (x, w) result (stat) 
    ! lagrange interpolation
    integer             :: stat
    real, intent (in)   :: x
    real, dimension (:) :: w

    integer             :: i
    real                :: xi

    xi = 1.
    do i = 1, size (w)
       if (i > 1) xi = xi*(x + 2. -i)/(i-1)
       w (i) = xi
    end do
    stat = 0
  end function taylor

  function lg_der (x, w) result (stat) 
    ! lagrange interpolation derivative
    integer             :: stat
    real, intent (in)   :: x
    real, dimension (:) :: w

    integer             :: i, j, nf, k
    real                :: f, fk, xi

    nf = size (w)
    do i = 1, nf
       xi = x + 0.5*nf - i
       fk = 0.
       do k = 1, nf
          if (k == i) cycle 
          f = 1.
          do j = 1, nf
             if (i /= j .and. k /= j) f = f * (1. + xi / (i - j))
          end do
          fk = fk + f/(k - i)
       end do
       w (i) = fk
    end do
    stat = 0
  end function lg_der

  function lg2_int (x, w) result (stat)
    ! lagrange interpolation
    integer                           :: stat
    real, dimension (2), intent (in)  :: x
    real, dimension (:,:)             :: w

    integer                           :: i, j, n1, n2
    real                              :: f, xi

    n1 = size (w, 1)
    n2 = size (w, 2)
    do i = 1, n1
       f = 1.
       xi = x (1) + 1. - i
       do j = 1, n1
          if (i /= j) f = f * (1. + xi / (i - j))
       end do
       w (i,:) = f
    end do
    do i = 1, n2
       f = 1.
       xi = x (2) + 1. - i
       do j = 1, n2
          if (i /= j) f = f * (1. + xi / (i - j))
       end do
       w (:,i) = w (:,i) * f
    end do

    stat = 0
  end function lg2_int

  function sinc_int (x, w) result (stat) 
    ! sinc interpolation, UNTESTED
    integer             :: stat
    real, intent (in)   :: x
    real, dimension (:) :: w

    integer             :: i, nw, nc, sw
    real                :: pi, sx, xi

    nw = size (w); nc = (nw+1)*0.5; sw = 1 + mod(nc,2)
    pi = acos (-1.) ; sx = sin (pi*x) 
    
    w = (/ ((x + nc - i) * pi , i = 1, nw) /)
    w (sw:nw:2) = - w (sw:nw:2)
    where (abs(w) > epsilon (w) )
       w  = sx / w
    elsewhere
       w = 1.
    end where
    stat = 0 !?    w = w / sum (w) 
  end function sinc_int

  function muir_int (x, w) result (stat) 
    ! Muir's interpolation, UNTESTED
    integer             :: stat
    real, intent (in)   :: x
    real, dimension (:) :: w

    integer             :: i, nw, nc, sw
    real                :: pi, sx, pw

    nw = size (w); nc = (nw+1)*0.5; sw = 1 + mod(nc,2)
    pi = acos (-1.) ; sx = sin (pi*x); pw = pi/nw
    w = (/ (nw *tan (pw * (x + nc - i)), i = 1, nw) /)
    w (sw:nw:2) = - w (sw:nw:2)
    where (abs(w) > epsilon (w))
       w  = sx / w
    elsewhere
       w = 1.
    end where
    stat = 0 
  end function muir_int

  function muir2_int (x, w) result (stat) 
    ! Muir's interpolation, UNTESTED
    integer                          :: stat
    real, dimension (2), intent (in) :: x
    real, dimension (:,:)            :: w

    integer                          :: i, nw1, nw2
    real                             :: pi, sx, pw1, pw2

    pi = acos (-1.) ; sx = sin (pi*x (1)) * sin (pi*x(2))
    nw1 = size (w, 1) ; pw1 = pi/nw1
    nw2 = size (w, 2) ; pw2 = pi/nw2

    do i = 1, nw1
       w (i,:) = nw1 * tan (pw1 * (x (1) + 1. - i))
    end do
    w (2:nw1:2,:) = - w (2:nw1:2,:)
    do i = 1, nw2
       w (:,i) =  nw2 * tan (pw2 * (x (1) + 1. - i)) *  w (:,i)
    end do
    w (:,2:nw2:2) = - w (:,2:nw2:2)

    where (abs(w) > 1.e-6 )
       w  = sx / w
    elsewhere
       w = 1.
    end where
    stat = 0 
  end function muir2_int

  function cubic_int (x, w) result (stat) 
    ! cubic convolution interpolation, UNTESTED
    integer             :: stat
    real, intent (in)   :: x
    real, dimension (:) :: w

    real                :: x1

    x1 = 1. - x

    w (1) = -0.5*x*x1*x1
    w (2) = 0.5*x1*(2.+x*(2.-3.*x))
    w (3) = 0.5*x*(1+x*(4.-3.*x))
    w (4) = -0.5*x*x*x1

    stat = 0
  end function cubic_int

end module interp_new












