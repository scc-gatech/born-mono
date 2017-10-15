module triple  
! put a triple impulse
  use classutil
  implicit none
  integer, private :: nb 
  integer, private :: nx 
  integer, private :: wt 
  contains
  subroutine triple_init ( nb_in,nx_in,wt_in )
    integer    :: nb_in 
    integer    :: nx_in 
    integer    :: wt_in 
    nb = nb_in 
    nx = nx_in 
    wt = wt_in
  end subroutine
  function triple_lop ( adj, add, x, y) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: x,y 
    call adjnull (adj,add,x,y )
    call triple_lop2(adj,add,x,y )
    stat=0
  end function 
  subroutine triple_lop2(adj,add,x,y)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: x 
    real, dimension (:)  :: y 
    if (adj) then
      x (1:nx) = x (1:nx) + (y (1       :nx       )        - 2 *      &
        &            y (1 +   nb:nx +   nb)        +                &
        &      y (1 + 2*nb:nx + 2*nb)) /wt
    else
      y (1       :nx       ) = y (1       :nx       ) +   x (1:nx) /wt
      y (1 +   nb:nx +   nb) = y (1 +   nb:nx +   nb) - 2*x (1:nx) /wt
      y (1 + 2*nb:nx + 2*nb) = y (1 + 2*nb:nx + 2*nb) +   x (1:nx) /wt
    end if
  end subroutine 
  subroutine triple_close()
  end subroutine 
end module 
