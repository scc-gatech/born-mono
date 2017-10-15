module wave_mod
use extrapolation_types
use rtm_section_mod
use sep3d_struct_mod
use sep_mod
use sep_io_mod

implicit none
type(sep3d),private :: shot_wave,rec_wave
integer, private:: block_size
contains

subroutine wave_init(extrapolation,n_node)
type(extrapolation_type)::extrapolation
integer::n_node
integer::img_jt,skip
integer :: jsamp,nsect
character(len=128) :: type

call init_sep3d(shot_wave, "OUTPUT","FLOAT", "REGULAR", ctag="ShotWave")
call axis_allocate(shot_wave,6)
skip=n_node


img_jt=nint(extrapolation%img_dt/extrapolation%dt)

! 5 is sx and 6 is sy
shot_wave%o(6)=0
shot_wave%d(6)=10
shot_wave%n(6)=1
shot_wave%o(5)=extrapolation%sx0
shot_wave%d(5)=extrapolation%dsx*skip
shot_wave%n(5)=extrapolation%nsx/skip

! 4 is t
shot_wave%o(4)=0
shot_wave%d(4)=extrapolation%dt*img_jt
shot_wave%n(4)=extrapolation%nt/img_jt
!1 is z
shot_wave%o(1)=extrapolation%z0
shot_wave%d(1)=extrapolation%dz
shot_wave%n(1)=extrapolation%nz
! 2 is x and 3 leave for y
shot_wave%o(2)=extrapolation%x_min
shot_wave%d(2)=extrapolation%dx
shot_wave%n(2)=extrapolation%nx
shot_wave%o(3)=0
shot_wave%d(3)=10
shot_wave%n(3)=1


call sep3d_set_sep3d(shot_wave)
call sep3d_write_description("ShotWave",shot_wave)

if (extrapolation%morm(1:9)=='migration') then
  call init_sep3d(shot_wave, rec_wave, "OUTPUT", "RecWave")
  call sep3d_set_sep3d(rec_wave)
  call sep3d_write_description("RecWave",rec_wave)
end if
block_size=shot_wave%n(1)*shot_wave%n(2)*shot_wave%n(3)*4
end subroutine wave_init

subroutine grab_wave_block(mywave)
type(sep3d) :: mywave

mywave=shot_wave

end subroutine
subroutine wave_seek(extrapolation,n_node)
type(extrapolation_type)::extrapolation
integer :: n_node
integer :: n_block, ret_seek
! integer, external :: sseek_block
if (extrapolation%shot_or_rec(1:4)=='shot') then
  n_block=(extrapolation%isx/n_node-1)*shot_wave%n(4)
  ret_seek=sseek_block("ShotWave",n_block,block_size,0)
else
  n_block=(extrapolation%isx/n_node-1)*rec_wave%n(4)
  ret_seek=sseek_block("RecWave",n_block,block_size,0)
end if
if (ret_seek .ne. n_block) call seperr("Error seeking wave")
end subroutine wave_seek



subroutine wave_output(extrapolation,wave_block)
type(extrapolation_type):: extrapolation
real :: wave_block(:,:)
! integer, external :: srite

if (extrapolation%shot_or_rec(1:4)=='shot') then
  if ( srite("ShotWave",wave_block(:,:),block_size) /= block_size ) then
    write(0,*) "trouble in writing shot wave"
  end if
else
  if ( srite("RecWave",wave_block(:,:),block_size) /= block_size ) then
    write(0,*) "trouble in writing rec wave"
  end if
end if
end subroutine wave_output


end module
