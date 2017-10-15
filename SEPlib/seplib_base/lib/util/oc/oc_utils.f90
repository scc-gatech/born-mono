module oc_utils_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  implicit none

  interface message
     module procedure print_integer
     module procedure print_real
     module procedure print_character
     module procedure print_logical
     module procedure print_info
  end interface

contains
  !----------------------------------------------------------------
  integer function prime(nn, n) result(m)
    !! returns m so that N/m=integer
    integer, intent(in) :: nn,n
    integer             ::  p,i
    p=nn/n
    do m=n,1,-1
       i=p
       do while(i*m<nn)
          i=i+1
       end do
       if (i*m .eq. nn) exit
    end do
  end function prime
  !----------------------------------------------------------------
  subroutine print_integer(n,v)
    character(len=*), intent(in) :: n
    integer,          intent(in) :: v
    call ocline()
    write (0,'(" OC ",a,"=",i8)') trim(n),v
    call ocline()
  end subroutine print_integer
  subroutine print_real(n,v)
    character(len=*), intent(in) :: n
    real,             intent(in) :: v
    call ocline()
    write (0,'(" OC ",a,"=",f15.6)') trim(n),v
    call ocline()
  end subroutine print_real
  subroutine print_character(n,v)
    character(len=*), intent(in) :: n,v
    call ocline()
    write (0,'(" OC ",a,"=",a)') trim(n),trim(v)
    call ocline()
  end subroutine print_character
  subroutine print_info(n)
    character(len=*), intent(in) :: n
    call ocline()
    write (0,'(" OC ",a)') trim(n)
    call ocline()
  end subroutine print_info
  subroutine print_logical(n,v)
    character(len=*), intent(in) :: n
    logical,          intent(in) :: v
    call ocline()
    write (0,'(" OC ",a,"=",l2)') trim(n),v
    call ocline()
  end subroutine print_logical
  !----------------------------------------------------------------
  subroutine ocline()
    write (0,*) 'OC-------------------------------------------'
  end subroutine ocline
  !----------------------------------------------------------------
end module oc_utils_mod








