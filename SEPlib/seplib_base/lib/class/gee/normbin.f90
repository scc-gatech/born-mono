module bin2_norm	
  use int2
  use interp

  implicit none
  integer,                          private :: n1, n2, nd
  real, dimension (:), allocatable, private :: count, ones

contains
  subroutine bin2_init (coord, o, d, n)
    real,    dimension (:,:), intent (in) :: coord
    real,    dimension (2),   intent (in) :: o, d
    integer, dimension (2),   intent (in) :: n

    integer                               :: stat

    nd = size (coord, 1) ; n1 = n (1) ; n2 = n (2)
    if (.not. allocated (count)) then
       allocate (count (n1*n2), ones (nd)) ; ones = 1.
    end if
    call int2_init (coord, o, d, n, bin2_int,  (/ 1, 1 /), nd)
    stat = int2_lop (.true.,.false.,count,ones)
  end subroutine bin2_init

  subroutine bin2 (ord, mod)
    real, dimension (nd),    intent (in)  :: ord
    real, dimension (n1*n2), intent (out) :: mod

    integer                               :: stat
    stat = int2_lop (.true.,.false.,mod,ord)
    where (count > 0.) mod = mod / count
  end subroutine bin2

  subroutine bin2_close ()
    deallocate (count, ones)
    call int2_close ()
  end subroutine bin2_close
end module bin2_norm
