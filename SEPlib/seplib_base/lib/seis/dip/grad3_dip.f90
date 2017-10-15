module grad3_dip_mod 
  use classutil
  implicit none
  integer,private,save :: n1,n2,n3,ndim
  contains
  subroutine grad3_init(n)
    integer :: n(3)
    ndim=0
    n1=n(1)
    if (n1>1) then
      ndim= ndim+1
    end if
    n2=n(2)
    if (n2>1) then
      ndim= ndim+1
    end if
    n3=n(3)
    if (n3>1) then
      ndim= ndim+1
    end if
  end subroutine 
  integer function grad3_op(adj,add,model,data)
    logical, intent(in) :: adj,add
    real :: model(:),data(:)
    call adjnull(adj,add,model,data)
    call grad3_sub(adj,model,data)
    grad3_op=0
  end function 
  subroutine grad3_sub(adj,model,data)
    logical :: adj
    real :: model(n1,n2,n3),data(n1,n2,n3,ndim)
    integer :: i1,i2,i3,idim
    idim=0
    if (n1>1) then
      idim=  idim+1
      do i3=1,n3 
        do i2=1,n2 
          call grad_op(adj,n1,model(:,i2,i3),data(:,i2,i3,idim))
        end do
      end do
    end if
    if (n2>1) then
      idim=  idim+1
      do i3=1,n3 
        do i1=1,n1 
          call grad_op(adj,n2,model(i1,:,i3),data(i1,:,i3,idim))
        end do
      end do
    end if
    if (n3>1) then
      idim=  idim+1
      do i2=1,n2 
        do i1=1,n1 
          call grad_op(adj,n3,model(i1,i2,:),data(i1,i2,:,idim))
        end do
      end do
    end if
  end subroutine 
  subroutine grad_op(adj,n,model,data)
    logical :: adj
    integer :: n
    real :: model(n),data(n)
    if (.not. adj) then
      data(2:n)=   data(2:n)+model(2:n)-model(1:n)
      data(1)=   data(1)+model(2)-model(1)
    else
      model(2:n)=  model(2:n)+data(2:n)
      model(1:n)=  model(1:n)-data(2:n)
      model(2)=  model(2)+data(1)
      model(1)=  model(1)-data(2)
    end if
  end subroutine 
  subroutine grad3_close()
  end subroutine 
end module 
