module wavelet_mod
use extrapolation_types
use sep3d_struct_mod
use sep


implicit none
type(s_sep3d),private::wavelet
real,private,pointer:: wavelet_block(:,:)

contains
logical function wavelet_init(extrapolation,i_node)
type(extrapolation_type)::extrapolation
integer ::i_node
type(sep3d)::wave_let
real :: tdelay
integer, external:: auxin, sreed
integer ::sizeofreal,sizeofint,sizeofwavelet,ierr,nh
nullify(wavelet_block)
call s_sep3d_init(wavelet)
!if (i_node==0) then
!  if(-1==auxin("wavelet")) then
!    write(0,*)"Must provide Wavelet file"
!    return
!  end if
  !write(0,*) "a"
  call init_sep3d("wavelet",wave_let,"INPUT")
  allocate(wavelet_block(wave_let%n(1), wave_let%n(2)) )
  call sep3d_grab_headers("wavelet",wave_let,nh)
   if(.not. sep3d_read_Data("wavelet",wave_let,wavelet_block)) &
     call seperr("trouble reading data")
     
!  if (4*product(wave_let%n) /= sreed("wavelet",wavelet_block,size(wavelet_block)*4) ) then
!     write(0,*)"trouble reading wavelet"
!     wavelet_init=.false.
!  end if
  !write(0,*) "b"
  call from_aux("wavelet", "tdelay", tdelay, 0.)
  extrapolation%t_delay=tdelay
  wavelet%n(1:2)=wave_let%n(1:2) 
  wavelet%d(1:2)=wave_let%d(1:2)
  wavelet%o(1:2)=wave_let%o(1:2) 
!endif
!call MPI_BCAST(wavelet,wavelet%MPI_s_sep3d_Size,MPI_BYTE,0,MPI_COMM_WORLD,ierr)
!write(*,*) wavelet%n(1),wavelet%n(2),"tttttttttt"
!if (i_node /=0 ) then
!   allocate(wavelet_block(wavelet%n(1), wavelet%n(2)) )
!end if
!call MPI_BCAST(wavelet_block,wavelet%n(1)*wavelet%n(2),MPI_REAL,0,MPI_COMM_WORLD,ierr)
!write(*,*) wavelet_block(65,:)

wavelet_init=.true.
end function wavelet_init

subroutine wavelet_addsource(extrapolation,force_block)
real :: force_block(:)
type(extrapolation_type):: extrapolation

integer it_source,ix_source,x0_source

it_source=nint((extrapolation%t-wavelet%o(1)+extrapolation%t_delay)/wavelet%d(1))+1 ! wavelet 1: t
force_block=0.
if (it_source < 1) then
   call seperr("it_source<1")
end if
if (it_source < wavelet%n(1) ) then
  x0_source=nint((extrapolation%sx+wavelet%o(2)-extrapolation%x_min)/extrapolation%dx)+1
  if ( x0_source <1 ) call seperr("x0_source too small")
  do ix_source=1, wavelet%n(2)
    force_block(x0_source+ix_source-1)=wavelet_block(it_source,ix_source)
  end do
end if
  
end subroutine wavelet_addsource


subroutine wavelet_clean()
if (associated(wavelet_block)) then
  deallocate(wavelet_block)
end if
nullify(wavelet_block)
end subroutine wavelet_clean


end module 
