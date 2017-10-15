module sinc_interp_mod
  use classutil
  use sep_mod
  implicit none
  real, pointer :: table(:,:)
  integer,pointer :: iloc(:,:),itable(:)
  integer,private,save:: n_pts,n_table,n1


  contains


  subroutine sinc_1d_init(loc)
    real    :: loc(:)
    integer :: i1,i2,i
    real    :: f,d

    n1=size(loc)
    allocate(iloc(n_pts,n1),itable(n1))
    d=1./(real(n_table-1))
    do i2=1,n1
      loc(i2)=max(0.,loc(i2))
      f=loc(i2)
      i=f;f=f-i
      itable(i2)=nint(f/d)+1
      do i1=1,n_pts
        if(i - n_pts/2+i1 < 1) then
          iloc(i1,i2)=1
        else  if(i-n_pts/2 +i1 >n1) then
          iloc(i1,i2)=n1
        else
          iloc(i1,i2)=i-n_pts/2+i1
        end if
      end do
    end do
  end subroutine

  subroutine sinc_1d_clean()
    deallocate(iloc,itable)
  end subroutine

  subroutine sinc_build_table(npts,ntable)
  use sep_filter_mod
    integer, optional :: npts,ntable
    real              :: d
    integer           :: i
    real, pointer     :: temp(:)

    n_pts=8
    n_table=512
    if(present(npts)) n_pts=8
    if(mod(n_pts,2)/=0) call seperr("sinc table must be even")
    if(present(ntable)) n_table=513

    allocate(temp(n_pts*3))
    allocate(table(n_pts,n_table))

    d=1./real(n_table-1)
    do i=1,n_table
      call mksinc(table(:,i),n_pts,(i-1)*d,temp)
    end do

    deallocate(temp)
  end subroutine


  integer function interp_1d_axis_op(adj,add,model,data)
    logical :: adj,add
    real :: model(:),data(:)
    real :: pt1,val,d,f
    integer :: i1,i2,ibeg,i,iend,it,ilo,ihi,iin,iv

    call adjnull(adj,add,model,data)


    d=1./real(size(table,2))
    if(.not. adj) then
      do i1=1,size(iloc,2)
        do i=1,size(iloc,1)
          data(i1)=data(i1)+table(i,itable(i1))*model(iloc(i,i1))
!           f=(itable(i1)-1)*d
!          data(i1)=data(i1)+(1.-f)*model(iloc(4,i1))+f*model(iloc(5,i1))
        end do
      end do
    else
      do i1=1,size(iloc,2)
        do i=1,size(iloc,1)
          model(iloc(i,i1))=model(iloc(i,i1))+data(i1)*table(i,itable(i1))
        end do
      end do
    end if

    interp_1d_axis_op=0


  end function
end module
