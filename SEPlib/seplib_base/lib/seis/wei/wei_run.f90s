module wei_run_mod
use sep
implicit none
logical, private, save :: restart
character(len=128),private :: operation
contains


logical function init_wei_run()

    call from_param("restart",restart,.false.)

    call from_param("operation",operation,"migration.")


init_wei_run=.true.

end function

logical function my_restart()
  my_restart=restart
end function



subroutine my_operation(o)
character(len=*) :: o
  o=operation
end subroutine











end module
