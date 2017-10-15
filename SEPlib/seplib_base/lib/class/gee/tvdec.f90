module tvdec  
! Linear operator for time-variable deconvolution
  use classutil
  use leakint2
  use nicaf1
  implicit none
  real ,private,  dimension(:), allocatable  :: aa 
  contains
  subroutine tvdec_init ( yy,ny,na,rho )
    real, dimension( :), pointer :: yy
    integer, intent( in)         :: ny, na
    real,    intent( in)         :: rho 
    if (.not. allocated(aa)) then
      allocate(aa (  ny*na)) 
    end if
    call nicaf1_init( yy, ny, na)
    call leakint2_init( ny, na, rho)
  end subroutine
  function tvdec_lop ( adj, add, mod, dat) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mod,dat 
    call adjnull (adj,add,mod,dat )
    call tvdec_lop2(adj,add,mod,dat )
    stat=0
  end function 
  subroutine tvdec_lop2(adj,add,mod,dat)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: mod 
    real, dimension (:)  :: dat 
    integer stat1, stat2
    if ( adj) then
      stat2 = nicaf1_lop  ( adj, .false., aa, dat)
      stat1 = leakint2_lop( adj, .true.,  mod, aa)
    else
      stat1 = leakint2_lop( adj, .false., mod, aa)
      stat2 = nicaf1_lop  ( adj, .true.,  aa, dat)
    end if
  end subroutine 
  subroutine tvdec_close()
    deallocate( aa )
  end subroutine 
end module 
