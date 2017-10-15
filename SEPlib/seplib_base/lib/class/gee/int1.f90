module int1  
! generic 1-d interpolation
  use classutil
  implicit none
  integer, private :: nd 
  integer, private :: nf 
  integer ,private,  dimension (:),    allocatable  :: x1 
  integer ,private,  dimension (:),    allocatable  :: x2 
  integer ,private,  dimension (:),    allocatable  :: y1 
  integer ,private,  dimension (:),    allocatable  :: y2 
  logical ,private,  dimension (:),    allocatable  :: m 
  real ,private,     dimension (:,:), allocatable  :: w 
  contains
  subroutine int1_init ( coord,o1,d1,n1,interp,nd_in,nf_in,weight,ww )
    integer    :: nd_in 
    integer    :: nf_in 
    real, dimension (:),   intent (in)  :: coord, weight
    real, dimension (:,:), intent (out) :: ww
    real,                  intent (in)  :: o1, d1
    integer,               intent (in)  :: n1
    optional                            :: weight, ww
    interface  
      integer function interp (x, w)
        real, intent (in)   :: x
        real, dimension (:) :: w
      end function
    end interface 
    integer                             :: id, ix, i1, i2, stat, j
    real                                :: rx
    nd = nd_in 
    nf = nf_in 
    if (.not. allocated(x1)) then
      allocate(x1 ( nd)) 
    end if
    if (.not. allocated(x2)) then
      allocate(x2 ( nd)) 
    end if
    if (.not. allocated(y1)) then
      allocate(y1 ( nd)) 
    end if
    if (.not. allocated(y2)) then
      allocate(y2 ( nd)) 
    end if
    if (.not. allocated(m)) then
      allocate(m ( nd)) 
    end if
    if (.not. allocated(w)) then
      allocate(w ( nf,nd)) 
    end if
    if (present (ww)) then
      ww = 0.
    end if
    do id = 1, nd  
      rx = (coord (id) - o1)/d1
      ix = rx - 0.5*nf + 1. 
      i1 = rx
      rx = rx - i1 
      i1 = ix + 1 
      i2 = ix + nf
      if (i2 < 1 .or. i1 > n1) then
        m (id) = .true. 
        w (:, id) = 0.
        cycle
      end if
      m (id) = .false. 
      stat = interp (rx, w (:,id))
      x1 (id) = max (i1,1)
      y1 (id) = x1 (id) - ix
      x2 (id) = min (i2,n1)
      y2 (id) = x2 (id) - ix   
      if (y1 (id) > 1) then
        w (1:y1(id)-1,id) = 0.
      end if
      if (y2 (id) < nf) then
        w (y2(id)+1:nf,id) = 0.    
      end if
      if (present (weight)) then
        w (:,id) = w (:,id) * weight (id)
      end if
      if (present (ww)) then
        do j = 1, nf
          ww (i1:i2+1-j,j) =             ww (i1:i2+1-j,j) + w (1:1+nf-j&
            &,id) * w (j:nf,id)
        end do
      end if
    end do
  end subroutine
  function int1_lop ( adj, add, mdl, ord) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mdl,ord 
    call adjnull (adj,add,mdl,ord )
    call int1_lop2(adj,add,mdl,ord )
    stat=0
  end function 
  subroutine int1_lop2(adj,add,mdl,ord)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: mdl 
    real, dimension (:)  :: ord 
    integer :: i
    do i = 1, nd  
      if (m (i)) then
        cycle 
      end if
      if (adj) then
        mdl (x1(i):x2(i)) =          mdl (x1(i):x2(i)) +  w (y1(i):y2(i&
          &),i)  * ord (i)
      else
        ord (i) =          ord (i) + sum (mdl (x1(i):x2(i)) * w (y1(i&
          &):y2(i),i))
      end if
    end do
  end subroutine
  subroutine  int1_close()
    deallocate( x1 ,x2,y1,y2,m,w)
  end subroutine 
end module 
