!<
! catch_up_grid
!
! USAGE
! call  catch_up_grid(last_location,current_location,n_g,f,n,grid)
!
! INPUT PARAMETERS
! last_local    - integer(:)  last local with n-d space
! current_local - integer(:)  current local with n-d space
! n_g           - integer(:)  size of grid
! n             - integer(:)  size of grid
! f             - integer(:)  first element of grid
! grid          - integer(:,:,:,:,:,:,:) grid
!
! DESCRIPTION
! Writes out grid slices (used in Sort3d)
!
! CATEGORY
! Lib:Aux
!
! COMPILE LEVEL
! LOCAL
!
!>
module sort_subs_mod 
  use sorting_mod
  use sep_3d_mod
  implicit none
  contains
  subroutine catch_up_grid(last_location,current_location,n_g,f,n,grid&
    &)
    integer, dimension(:) :: last_location,current_location,f,n,n_g
    integer, dimension(:,:,:,:,:,:,:) :: grid
    integer :: i1,ng_out(2),n_out(2),f_out(2),j_out(2),current,last&
      &,shift
    shift=size(n)-size(last_location)
    ng_out(2)=1
    ng_out(2)=1
    last=0
    current=0
    do i1=1,size(last_location)
      last=        last+(last_location(i1)-1)        * ng_out(2)
      current=        current+(current_location(i1)-1)*ng_out(2)
      ng_out(2)=ng_out(2)*n_g(shift+i1)
    end do 
    n_out=(/size(grid),1/)
    j_out=1
    f_out(1)=0
    ng_out(1)=n_out(1)
    if (current.ne.last) then
      do i1=last,current-1 
        f_out(2)=i1
        if(0.ne. sep_put_grid_window("out",3,ng_out,n_out,f_out,j_out,grid&
          &)) call seperr("trouble putting grid window")
        grid=-1
      end do 
      last_location=current_location
      f(shift+1:size(f))=current_location(1:size(n))-1
    end if
  end subroutine 
!<
! sort_it
!
! USAGE
! call  sort_it(array,nkeys,n2)
!
! INPUT PARAMETERS
! array    - integer(:,:)  array to sort
! nkeys - integer(:)  number of keys to sort
! n2           - integer(:)  size of grid n2
!
! DESCRIPTION
! Recursively sorts a 2-D array
!
! CATEGORY
! Lib:Aux
!
! COMPILE LEVEL
! LOCAL
!
!>
  subroutine sort_it(array,nkeys,n2)
    integer,dimension(:,:) :: array
    integer :: nkeys,n2,beg_i
    integer :: i2,i1,old_val(nkeys)
    logical :: first
    call quick_sort(array,nkeys,nkeys+1,1,n2)
    do i1=nkeys-1,1,-1 
      old_val=array(1:nkeys,1)
      beg_i=1
      i2=2
      do while (i2 <= n2)
        if (any(old_val(i1+1:) .ne. array(i1+1:nkeys,i2))) then
          if (i2-beg_i-1 > 0) then
            call quick_sort(array,i1,nkeys+1,beg_i,i2-1)
          end if
          beg_i=i2
          old_val=array(1:nkeys,i2)
        end if
        i2=                        i2+1
      end do 
      if (n2 > beg_i) then
        call quick_sort(array,i1,nkeys+1,beg_i,n2)
      end if
    end do
  end subroutine 
end module 
