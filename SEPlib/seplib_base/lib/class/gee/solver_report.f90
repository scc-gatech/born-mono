module solver_report_mod 
  use ddot_mod
  implicit none
  real, private  :: scale
  contains
  subroutine solver_report(iter,x,g,rd,rm)
    integer,            intent(in) :: iter
    real, dimension(:), intent(in) :: x,g
    real, dimension(:), pointer    :: rd,rm
    optional                       ::    rm
    double precision               :: dd,dm,dx,dg
    dd=dot_product(rd,rd)
    dx=dot_product( x, x)
    dg=dot_product( g, g)
    dm=0.
    if (present(rm)) then
      dm=dot_product(rm,rm)
    end if
    if (iter.eq.1) then
      scale=max(dd,dm)/1000.
      write(0,*) "DOT PRODUCT VALUES SCALED BY ",scale
    end if
    write (0,*) "iteration ", iter     ,           "   res dat", int(dd&
      &/scale)     ,           "   res mod", int(dm/scale)     ,    &
      &       "       mod", int(dx/scale)     ,           "      grad", int(dg/scale)
  end subroutine 
end module 
