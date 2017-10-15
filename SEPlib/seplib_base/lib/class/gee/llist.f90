module llist_mod
  implicit none

  type, private :: entry
     real, dimension (:), pointer  :: g
     double precision              :: gn
     type(entry),         pointer  :: next
  end type entry

  type list
     private
     type(entry), pointer :: first
     type(entry), pointer :: current
     integer              :: depth
  end type list

contains

  function llist_init() result(l)
    type(list)  :: l
    
    allocate(l%first)
    l%depth = 0
    call llist_rewind(l)
  end function llist_init

  subroutine llist_rewind(l)
    type(list)  :: l
    l%current => l%first
  end subroutine llist_rewind
  
  function llist_depth(l) result(n)
    type(list)  :: l
    integer     :: n

    n = l%depth
  end function llist_depth

  subroutine llist_add(l, g, gn)
    type(list)                   :: l
    real, dimension(:), pointer  :: g
    double precision, intent(in) :: gn

    l%current%g => g
    l%current%gn = gn
    allocate(l%current%next)
    l%depth =  l%depth + 1
  end subroutine llist_add
  
  subroutine llist_down(l, g, gn)
    type(list)                   :: l
    real, dimension(:), pointer  :: g
    double precision             :: gn

    g  => l%current%g
    gn =  l%current%gn
    l%current => l%current%next
  end subroutine llist_down

  subroutine llist_close(l)
    type(list)  :: l
    integer     :: i

    do i = 1, l%depth
       call llist_chop(l)
    end do

    deallocate(l%first)
  end subroutine llist_close

  subroutine llist_chop(l)
    type(list) :: l

    call llist_rewind(l)
    l%first =>    l%current%next
    if(associated(l%current%g)) &
    deallocate(l%current%g)
    deallocate(l%current)
    l%depth = l%depth-1
  end subroutine llist_chop
end module llist_mod




