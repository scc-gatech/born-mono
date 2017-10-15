module int2  
! generic 2-d interpolation
  use classutil
  implicit none
  integer, private :: nd 
  integer, private :: n1 
  integer, private :: n2 
  integer ,private,  dimension (2)  :: n 
  integer ,private,  dimension (2)  :: nf 
  integer ,private,  dimension (:,:),   allocatable  :: nxy 
  logical ,private,  dimension (:),     allocatable  :: m 
  real ,private,     dimension (:,:,:), allocatable  :: w 
  contains
  subroutine int2_init ( coord,o,d,n_in,interp,nf_in,nd_in,weight )
    integer ,   dimension (2) :: n_in 
    integer ,   dimension (2) :: nf_in 
    integer    :: nd_in 
    real,    dimension (:,:), intent (in)           :: coord
    real,    dimension (:),  optional :: weight
    real, dimension (2) ::     o, d, x
    integer  id, ix (2), stat
    interface  
      integer function interp (x, w)
        real, dimension (2), intent (in)     :: x
        real, dimension (:,:)                :: w
      end function
    end interface 
    n = n_in 
    nf = nf_in 
    nd = nd_in 
    if (.not. allocated(nxy)) then
      allocate(nxy ( nd,2)) 
    end if
    if (.not. allocated(m)) then
      allocate(m ( nd)) 
    end if
    if (.not. allocated(w)) then
      allocate(w ( nf(1),nf(2),nd)) 
    end if
    n1=n(1)
    n2=n(2)
    do id = 1, nd  
      x = (coord (id,:) - o)/d 
      ix = x
      x = x - ix 
      nxy (id,:) = ix + 1 - 0.5*nf
      m (id) = .true. 
      w (:,:, id) = 0. 
      if (  all((nxy (id,:) + 1  >= 1))           .and. all((nxy (id,:&
        &) + nf <= n))) then
        m (id) = .false. 
        stat = interp (x, w (:,:,id))
        if (present (weight)) then
          w (:,:,id) = w (:,:,id) * weight (id)
        end if
      end if
    end do
  end subroutine
  function int2_lop ( adj, add, x, ord) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,ord 
    call adjnull (adj,add,x,ord )
    call int2_lop2(adj,add,x,ord )
    stat=0
  end function 
  subroutine int2_lop2(adj,add,x,ord)
    logical,intent(in) :: adj,add 
    real, dimension (n1, n2)  :: x 
    real, dimension (:)  :: ord 
    integer                          :: i
    integer, dimension (2)           :: i1, i2 
    do i = 1, size (ord)
      if (m (i)) then
        cycle
      end if
      i1 = nxy (i,:) + 1
      i2 = nxy (i,:) + nf
      if ( adj) then
        x (i1(1):i2(1),i1(2):i2(2)) =          x (i1(1):i2(1),i1(2):i2&
          &(2)) + ord (i) * w (:,:,i)
      else
        ord (i) =          ord (i) + sum (x (i1(1):i2(1),i1(2):i2(2)) &
          &* w (:,:,i))
      end if
    end do
  end subroutine 
  subroutine int2_close()
    deallocate( nxy ,m,w)
  end subroutine 
end module 
