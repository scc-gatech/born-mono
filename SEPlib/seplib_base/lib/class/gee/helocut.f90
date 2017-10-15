module helocut  
  use classutil
  use lapfac
  use helicon
  use polydiv
  implicit none
  type( filter),   private                :: aa 
  type( filter),   private                :: bb 
  real ,private,  dimension (:), allocatable  :: tt 
  contains
  subroutine helocut_init ( eps,n1,n2,na )
    real,    intent (in) :: eps
    integer, intent (in) :: n1, n2, na
    if (.not. allocated(tt)) then
      allocate(tt ( n1*n2)) 
    end if
    aa = lapfac2( 0.001, n1, na) 
    bb = lapfac2(   eps, n1, na) 
    call helicon_init( aa)
    call polydiv_init( n1*n2, bb)
  end subroutine
  function helocut_lop ( adj, add, pp, qq) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: pp,qq 
    call adjnull (adj,add,pp,qq )
    call helocut_lop2(adj,add,pp,qq )
    stat=0
  end function 
  subroutine helocut_lop2(adj,add,pp,qq)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: pp 
    real, dimension (:)  :: qq 
    integer stat1, stat2
    if ( adj) then
      stat2 = polydiv_lop( adj, .false., tt, qq)
      stat1 = helicon_lop( adj, .true.,  pp, tt)
    else
      stat1 = helicon_lop( adj, .false., pp, tt)
      stat2 = polydiv_lop( adj, .true.,  tt, qq)
    end if
  end subroutine
  subroutine  helocut_close()
    deallocate( tt )
    call deallocatehelix( aa)
    call deallocatehelix( bb)
    call polydiv_close()
  end subroutine 
end module 
