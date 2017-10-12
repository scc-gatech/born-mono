module hypercubeGen_mod
  use axis_mod
  implicit none
  integer,parameter :: longlong = selected_int_kind(15)
  type hypercube
    
      integer(kind=longlong),private :: n123
      type(axis),dimension(:),allocatable,private :: axes
    contains
    procedure, pass :: setAxes
    procedure, pass :: setAxis
    procedure, pass :: getAxis
    procedure, pass :: getAxes
    procedure, pass :: getN123
    procedure, pass :: getNdim
    procedure, pass :: sameSize
    procedure, pass :: returnNs
    procedure, pass :: getNdimG1
    procedure, pass :: setN123
  end type hypercube
  
  interface createHypercube
    module procedure hyp_hyp,hyp_ns,hyp_axes
  end interface
  
  
   
  contains
  function hyp_hyp(str)
    type(hypercube) :: hyp_hyp,str
    type(axis), allocatable :: axes(:)
    allocate(axes(str%getNdim()))
    call str%getAxes(axes)
    call hyp_hyp%setAxes(axes)
    deallocate(axes)
  end function
  function hyp_axes(axes)
    type(hypercube) :: hyp_axes
    type(axis) :: axes(:)
    call hyp_axes%setAxes(axes)
  end function
  function hyp_ns(ns)
    type(hypercube) :: hyp_ns
    integer :: ns(:),i
    type(axis),dimension(:),allocatable :: axes
    allocate(axes(size(ns)))
    do i=1,size(ns)
      axes(i)=createaxis(ns(i))
    end do
    call hyp_ns%setAxes(axes)
    deallocate(axes)
  end function
    
  
  subroutine destroyH(self)
    type(hypercube) self
    deallocate(self%axes)
  end subroutine
  
  subroutine setN123(self)
    class(hypercube) :: self
    integer :: i
    integer(kind=longlong) :: n
    self%n123=1
    do i=1,size(self%axes)
      n=self%axes(i)%n
      self%n123=self%n123*n
    end do
  end subroutine
  
  function getNdimG1(self)
    class(hypercube) :: self
    integer :: getNdimG1
    integer :: i
    logical :: found
    getNdimG1=size(self%axes)
    found=.false.
    do while(getNdimG1 >=1 .and. .not. found)
      if(self%axes(getNdimG1)%n >1) then
        found=.true.
      else
        getNdimG1=getNdimG1-1
      end if
    end do
    if(getNdimG1 <1 ) getNdimG1=1
  end function
  
  subroutine returnNs(self,ns)
    class(hypercube) :: self
    integer,dimension(:) :: ns
    integer :: i
    if(size(ns) /= size(self%axes)) then
       write(0,*) "returnNs n not the right size"
       stop 1
    end if
    do i=1,size(ns)
      ns(i)=self%axes(i)%n
    end do
  end subroutine
  
  function sameSize(self,hyper2)
    class(hypercube) :: self,hyper2
    logical :: sameSize
    integer :: i
    sameSize=.true.
    if(size(self%axes) == size(hyper2%axes)) then
      i=1
      do while(i/=size(self%axes) .and. sameSize) 
        sameSize=self%axes(i)%same_axis(hyper2%axes(i))
        i=i+1
      end do
    else
      sameSize=.false.
    end if
  end function
    
    
  function getNdim(self)
    class(hypercube) :: self
    integer :: getNdim
    getNdim=size(self%axes)
  end function
  
  function getN123(self)
    class(hypercube) :: self
    integer(kind=longlong) :: getN123
    getN123=self%n123
  end function
  
  subroutine getAxes(self,axes)
    class(hypercube) self
    integer :: i
    type(axis), dimension(:) :: axes
    if(size(axes)/=size(self%axes)) then
       write(0,*) "get axes axes wrong size"
       stop 1
    end if
    do i=1,size(self%axes)
      axes(i)=self%getAxis(i)
    end do
  end subroutine

  function getAxis(self,iax)
    class(hypercube) :: self
    integer :: iax
    type(axis) :: getAxis
    
    getAxis=createaxis(self%axes(iax))
  end function
    
  
  subroutine setAxis(self,iax,axs)
    class(hypercube) :: self
    integer :: iax
    type(axis) :: axs
    
    self%axes(iax)=createaxis(axs)
    call self%setN123()
  end subroutine
  

  subroutine setAxes(self,axes)
    class(hypercube) :: self
    type(axis), dimension(:) ::  axes
    integer :: i
    if(allocated(self%axes)) deallocate(self%axes)
    allocate(self%axes(size(axes)))
    do i=1,size(axes)
      call self%setAxis(i,axes(i))
    end do
  end subroutine
  

end module