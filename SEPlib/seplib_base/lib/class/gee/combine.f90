module combine  
! combination filter
  use patch
  use gauss
  use mkwallwt
  implicit none
  contains
  subroutine combinen( data, data0, comb, x, n, nwall, nwind, wind)
    integer, dimension( :),    pointer      :: n, nwall, nwind
    real,    dimension( :, :), intent( in)  :: data
    real,    dimension( :),    intent( in)  :: data0, wind
    real,    dimension( :, :), intent( out) :: x
    real,    dimension( :),    intent( out) :: comb
    real,    dimension( :, :), allocatable  :: windata, a
    real,    dimension( :),    allocatable  :: windata0, b
    real,    dimension( size (comb))        :: wall
    integer                                    :: k, stat, i,j, ns, nw
    logical                                 :: lstat
    ns = size (data, 2)
    nw = product (nwind)
    allocate( windata( nw, ns), windata0 (nw), a (ns, ns), b (ns))
    call gauss_init (ns)
    call patch_init (n, nwall, nwind)
    comb = 0.
    do k = 1, product( n)
      stat = patch_lop( .false., .false., data0, windata0)       
      do i = 1, ns  
        stat = patch_lop( .false., .false., data (:,i), windata (:,i))
        a (i,i) = dot_product (windata (:,i), windata (:,i))
        do j = 1, i-1  
          a (i,j) = dot_product (windata (:,i), windata (:,j))
          a (j,i) = a (i,j)
        end do 
        b (i) = dot_product (windata (:,i), windata0)
      end do 
      lstat = gauss_solve(a, b, x (k,:))
      do i = 1, ns  
        windata0 = windata0 - x (k,i) * windata (:,i)
      end do 
      stat = patch_lop( .true., .true., comb, windata0*wind) 
      call patch_close ()
    end do 
    call wallwtn(n, nwall, nwind, wind, wall) 
    comb = comb * wall
    call gauss_close ()
    deallocate( windata, windata0, a, b)
  end subroutine 
end module 
