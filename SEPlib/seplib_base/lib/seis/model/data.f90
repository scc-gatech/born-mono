module data_mod
use extrapolation_types
use rtm_section_mod
use sep3d_struct_mod
use sep_io_mod
use sep_mod

implicit none
type(sep3d),private ::data
integer, private ::block_size
integer :: data_nt, data_nx, data_nsx
real :: data_ot, data_ox, data_osx,data_dt, data_dx,data_dsx
contains
!-------------------------------------------------------
logical function  data_init(extrapolation)
type(extrapolation_type)::extrapolation
! integer,external::auxin
logical :: section,master_Data,restart
integer :: nsect
character(len=128) :: type
data_init=.false.
!if (-1 .eq. auxin("Data")) then
!  write(0,*) "Must provide Data file"
!  return
!end if

call from_param("restart",restart,.false.)
if (extrapolation%morm(1:9)=='migration') then
  call init_sep3d("Data", data, "INPUT")
  if (.not. valid_structure(data) ) then
    write(0,*) "invalid Data init"
    return
  end if
  !data: 1:x  2:t 3:sx
  if (sep3d_ndims(data)==2) then 
    data%n(3)=1
  end if
  data_nx=data%n(1); data_nt=data%n(2); data_nsx=data%n(3)
  data_ox=data%o(1); data_ot=data%o(2); data_osx=data%o(3)
  data_dx=data%d(1); data_dt=data%d(2); data_dsx=data%d(3)
  extrapolation%hx0=data_ox; extrapolation%dx=data_dx; extrapolation%nhx=data_nx
                             extrapolation%dt=data_dt; extrapolation%nt=data_nt
  extrapolation%sx0=data_osx;extrapolation%dsx=data_dsx;extrapolation%nsx=data_nsx
else
  if(restart) then
    call init_sep3d("Data", data, "SCRATCH")
  else
    call init_sep3d(data,"OUTPUT","FLOAT","REGULAR",ctag="Data")
    call axis_allocate(data,3)
    !1:x, 2:t, 3:sx
    data%o(1)=extrapolation%hx0 ; data%d(1)=extrapolation%dx ;  data%n(1)=extrapolation%nhx 
    data%o(2)=0.0               ; data%d(2)=extrapolation%dt  ; data%n(2)=extrapolation%nt 
    data%o(3)=extrapolation%sx0 ; data%d(3)=extrapolation%dsx ; data%n(3)=extrapolation%nsx
    data_nx=data%n(1); data_nt=data%n(2); data_nsx=data%n(3)
    data_ox=data%o(1); data_ot=data%o(2); data_osx=data%o(3)
    data_dx=data%d(1); data_dt=data%d(2); data_dsx=data%d(3)
    call sep3d_set_sep3d(data)
    call section_info(section,nsect,master_data,type)
    call sep3d_write_description("Data",data)
        end if
end if
block_size=data_nx*data_nt*4

data_init=.true.
end function data_init


!-----------------------------------------------------
subroutine data_write(isx,shot_block)
real::shot_block(:,:)
integer :: isx
integer :: n_block
! integer, external :: srite,sseek_block
n_block=isx-1
if (sseek_block("Data",n_block,block_size,0)/=n_block) call seperr("Error seeking")
if (srite("Data",shot_block,block_size) /= block_size ) call seperr("Error writing data")
end subroutine data_write
!------------------------------------------------------



!-------------------------------------------------------
subroutine data_read(isx,shot_block)
real:: shot_block(:,:)
integer ::isx
integer ::n_block,ret_seek,ix0_rec
! integer, external :: sreed, sseek_block
n_block=isx-1 
if (sseek_block("Data",n_block,block_size,0)/= n_block) call seperr("Error block seeking data")
if (sreed("Data",shot_block, block_size)/=block_size)call seperr("problems with reading data file")
end subroutine data_read

!-------------------------------------------------------

subroutine grab_data(data_out)
type(sep3d) :: data_out
data_out=data
end subroutine

subroutine data_clean()
end subroutine data_clean

end module 
