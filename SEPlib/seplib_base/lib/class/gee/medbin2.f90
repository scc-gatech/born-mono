module medbin2_mod	
  use gee_quantile_mod

  implicit none
  integer,                               private :: n1, n2, nd
  integer, dimension (:,:), allocatable, private :: ct, pt
  integer, dimension (:),   allocatable, private :: j1, j2
  logical, dimension (:),   allocatable, private :: m
  real,    dimension (:),   allocatable, private :: bd

contains
  subroutine medbin2_init (coord, o, d, n)
    real,    dimension (:,:), intent (in) :: coord
    real,    dimension (2),   intent (in) :: o, d
    integer, dimension (2),   intent (in) :: n

    integer                               :: id, i1, i2, start

    nd = size (coord, 1) ; n1 = n (1) ; n2 = n (2)
    allocate (ct (n1,n2), pt (n1, n2))
    allocate (j1 (nd), j2 (nd), m (nd))
    ct = 0
    do id = 1, nd
       i1 = 1.5 + (coord (id,1) - o (1))/d (1)
       i2 = 1.5 + (coord (id,2) - o (2))/d (2)
       m (id) = (i1 >= 1) .and. (i1 <= n1) &
       .and.    (i2 >= 1) .and. (i2 <= n2)
       if (m (id)) then
          ct (i1, i2) = ct (i1, i2) + 1
          j1 (id) = i1 ; j2 (id) = i2
       end if
    end do
    start = 1
    do i2 = 1, n2
       do i1 = 1, n1
          pt (i1, i2) = start
          start = start + ct (i1, i2)
       end do
    end do
  end subroutine medbin2_init

  subroutine medbin2 (ord, model)
    real, dimension (nd),     intent (in)  :: ord
    real, dimension (n1, n2), intent (out) :: model

    integer                                :: i1, i2, id, p1, p2, np

    allocate (bd (nd))

    do id = 1, nd 
       if (m (id)) then
          i1 = j1 (id)
          i2 = j2 (id)
          bd (pt (i1, i2)) = ord (id) 
          pt (i1, i2) = pt (i1, i2) + 1
       end if
    end do

    do i2 = 1, n2   
       do i1 = 1, n1
          np = ct (i1, i2) 
          if (np > 0) then
             p2 = pt (i1, i2) ; p1 = p2 - np + 1          
             if(i1+i2<3) write (0,*) p1,p2,np ! stupid SGI compiler
             model (i1, i2) = (gee_quantile ((np+1)/2, bd (p1:p2)) &
             +               gee_quantile ((np+2)/2, bd (p1:p2)))*0.5
             if(i1+i2<3) write (0,*) i1,i2, model(i1,i2) ! stupid SGI compiler
          else
             model (i1, i2) = 0.
          end if
       end do
    end do
  end subroutine medbin2

  subroutine medbin2_close ()
    deallocate (ct, pt, j1, j2, m, bd)
  end subroutine medbin2_close
end module medbin2_mod




