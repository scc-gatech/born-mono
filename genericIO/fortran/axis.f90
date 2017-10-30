module axis_mod
  implicit none
  type axis
      integer :: n
      real    :: o
      real    :: d
      character(len=1024) ::  label
    contains
      procedure, pass :: print_info
      procedure, pass :: same_axis
      procedure, pass :: basic_init
  end type axis


  interface createAxis
   module procedure axisn,axisnod,axisnodl,axisaxis
  end interface

  contains
  
  
    subroutine basic_init(self,n,o,d,label)
    class(axis) :: self
    integer    :: n
    real       :: o,d
    character(len=*) :: label
    self%n=n
    self%o=o
    self%d=d
    self%label=label
  end subroutine
  
    logical function  same_axis(ax1,ax2)
    class(axis) :: ax1,ax2
    
    if(ax1%d < 1e-7) then
      write(0,*) "d of axis is to small"
      stop 9
    end if
    
    same_axis=.true.
    if(ax1%n/=ax2%n) same_axis=.false.
    if(abs((ax1%o-ax2%o)/ax1%d) >.01) same_axis=.false.
    if(abs(ax1%d/ax2%d-1.) > .01) same_axis=.false.
  end function
  
  type(axis) function axisn(n) 
    integer :: n
    call axisn%basic_init(n,0.,1.,"none")
  end function
  type(axis) function axisnod(n,o,d)
    integer :: n
    real :: o,d
    call axisnod%basic_init(n,o,d,"none")
  end function
  type(axis) function axisnodl(n,o,d,label)
    integer :: n
    real :: o,d
    character(len=*) :: label
    call axisnodl%basic_init(n,o,d,label)
  end function
  type(axis) function axisaxis(ax)
    type(axis) :: ax
    call axisaxis%basic_init(ax%n,ax%o,ax%d,ax%label)
  end function


  subroutine print_info(self,iax)
    class(axis) :: self
    integer :: iax
    write(0,*) "AXIS",iax,self%n,self%o,self%d,trim(self%label)
  end subroutine
end module
    
      