module tri_smooth_mod 
  implicit none
  contains
  subroutine tri_smooth(field,nwind)
    real  :: field(:,:,:)
    integer,intent(in) :: nwind(3)
    integer :: n1,n2,n3,i1,i2,i3
    real,pointer :: temp(:)
    n1=size(field,1)
    n2=size(field,2)
    n3=size(field,3)
    allocate(temp(max(n1,max(n2,n3))))
    if (n1>1) then
      do i3=1,n3 
        do i2=1,n2 
          call triangle(nwind(1),1,n1,field(:,i2,i3),temp)
          field(1:n1,i2,i3)=temp(1:n1)
        end do
      end do
    end if
    if (n2>1) then
      do i3=1,n3 
        do i1=1,n1 
          call triangle(nwind(2),1,n2,field(i1,:,i3),temp)
          field(i1,1:n2,i3)=temp(1:n2)
        end do
      end do
    end if
    if (n3>1) then
      do i2=1,n2 
        do i1=1,n1 
          call triangle(nwind(3),1,n3,field(i1,i2,:),temp)
          field(i1,i2,1:n3)=temp(1:n3)
        end do
      end do
    end if
  end subroutine 
end module 
