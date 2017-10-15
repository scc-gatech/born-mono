module slow_mod
use extrapolation_types
use sep3d_struct_mod

implicit none
type(s_sep3d),private:: slow
real,private,allocatable::slow_cube(:,:)

contains

!-----------------------------------------------------------------------
logical function slow_init(i_node)
integer :: i_node
type(sep3d)::tmp_slow
integer, external :: auxin, sreed
integer :: ierr,nh



call s_sep3d_init(slow)


!if (i_node==0) then
!  if (-1==auxin("Slow")) then
!     write(0,*) "Must provide slow  file"
!     return
!  end if
  call init_sep3d("Slow",tmp_slow,"INPUT")
  slow%n(1:2)=tmp_slow%n(1:2)
  slow%d(1:2)=tmp_slow%d(1:2)
  slow%o(1:2)=tmp_slow%o(1:2)
  allocate(slow_cube(slow%n(1),slow%n(2)))
  slow_cube=0.0
  call sep3d_grab_headers("Slow",tmp_slow,nh)
  if(.not. sep3d_read_data("Slow",tmp_slow,slow_cube))&
    call seperr("trouble reading slowness") 
  
!  if (4*slow%n(1)*slow%n(2) /= &
!   sreed("Slow",slow_cube,slow%n(1)*slow%n(2)*4) ) then
!    write(0,*) "trouble reading slow"
!    slow_init=.false.
!  end if
!end if
!call MPI_BCAST(slow,slow%MPI_s_sep3d_Size,MPI_BYTE,0,MPI_COMM_WORLD,ierr)
!if (i_node /=0 ) then
!   allocate(slow_cube(slow%n(1),slow%n(2)))
!end if
!call MPI_BCAST(slow_cube,slow%n(1)*slow%n(2),MPI_REAL,0,MPI_COMM_WORLD,ierr)
slow_init=.true.
end function slow_init
!----------------------------------------------------------------------

!subroutine slow_get_structure(slow_outside)
!type(sep3d):: slow_outside
!slow_outside=slow
!end subroutine slow_get_structure

subroutine slow_read(extrapolation,slow_block)
type(extrapolation_type)::extrapolation
real :: slow_block(:,:)

real :: extrapolation_x,wx_slow,extrapolation_z,wz_slow
integer:: ix,ix_slow,iz,iz_slow
real :: w_z1x1,w_z1x2,w_z2x1,w_z2x2

do ix=1,extrapolation%nx
   extrapolation_x=(ix-1)*extrapolation%dx+extrapolation%x_min
   extrapolation_x=(extrapolation_x-slow%o(2))/slow%d(2)+1
   ix_slow=int(extrapolation_x)
   wx_slow=extrapolation_x-ix_slow
   if (ix_slow<1 ) then
     ix_slow=1
     wx_slow=0.5
   end if
   if (ix_slow >= slow%n(2) ) then
     ix_slow=slow%n(2)-1
     wx_slow=0.5
   end if
   do iz=1,extrapolation%nz
     extrapolation_z=(iz-1)*extrapolation%dz+extrapolation%z0
     extrapolation_z=(extrapolation_z-slow%o(1))/slow%d(1)+1
     iz_slow=int(extrapolation_z) 
     wz_slow=extrapolation_z-iz_slow
     if (iz_slow <1 ) then
       iz_slow=1
       wz_slow=0.5
     end if
     if (iz_slow >= slow%n(1) ) then
       iz_slow= slow%n(1)-1
       wz_slow=0.5
     end if
     w_z1x1=(1-wz_slow)*(1-wx_slow); w_z1x2=(1-wz_slow)*wx_slow
     w_z2x1=wz_slow*(1-wx_slow);     w_z2x2=wz_slow*wx_slow

     slow_block(iz,ix)=slow_cube(iz_slow  ,ix_slow  )*w_z1x1 &
                      +slow_cube(iz_slow+1,ix_slow  )*w_z2x1 &
                      +slow_cube(iz_slow  ,ix_slow+1)*w_z1x2 &
                      +slow_cube(iz_slow+1,ix_slow+1)*w_z2x2
   end do
end do
end subroutine slow_read

subroutine slow_clean()
if (allocated(slow_cube)) deallocate(slow_cube)
end subroutine slow_clean
!--------------------------------------------------------------------------
end module 
