module extrapolation_types
type extrapolation_type
  character(len=128) :: oper
  character(len=128) :: morm  ! migration or modeling
  character(len=128) :: shot_or_rec ! extrapolation shot or receiver
  logical ::shot_in_memory
  logical ::wave_write
  logical ::input_image

  integer nx
  integer nz
  integer nop
  integer iz_sou
  integer iz_rec

  integer it
  integer isx
  integer nsx
  integer nt
  integer nhx

  integer img_nhx
  integer img_nhz
  integer img_nht
  integer nbtx  !taper boundary for x
  integer nbtz  !taper boundary for z

  integer nbdx  !boundary x
  integer nbdz  !boundary z

  integer MPI_extrapolation_type_Size

  real sx0
  real hx0
  real z0
  real z0_image
  real dsx

  real t
  real sx
  real dx
  real dz
  real z_sou

  real z_rec
  real dt
  real img_dt
  real x_min
  real x_max
  real image_x_min

  real image_x_max
  real image_z_min
  real ztapco
  real xtapco
  real t_delay
end type extrapolation_type

type s_sep3d
integer:: n(10)
real   :: o(10)
real   :: d(10)
integer:: MPI_s_sep3d_Size
end type s_sep3d

contains

subroutine s_sep3d_init(mysep3d)
type(s_sep3d)::mysep3d
integer :: sizeofint,sizeofreal,ninteger,nreal,ierr
ninteger=11; nreal=20
mysep3d%n=1
mysep3d%o=0.0
mysep3d%d=1.0
mysep3d%MPI_s_sep3d_Size=11*sizeofint+20*sizeofreal
end subroutine s_sep3d_init

subroutine extrapolation_type_init(extrapolation)
type(extrapolation_type):: extrapolation
integer:: ncharacher,nlogical,ninteger,nreal
integer::sizeofint,sizeofreal,sizeoflogical,sizeofch

extrapolation%oper='fft';extrapolation%morm='migration';extrapolation%shot_or_rec='shot'
extrapolation%shot_in_memory=.true.; extrapolation%wave_write=.true.
extrapolation%nx=200; extrapolation%nz=200; extrapolation%nop=3
extrapolation%iz_sou=1; extrapolation%iz_rec=1; extrapolation%it=1
extrapolation%isx=1; extrapolation%nsx=1; extrapolation%nt=1; extrapolation%nhx=1 
extrapolation%img_nhx=1; extrapolation%img_nhz=1; extrapolation%img_nht=1
extrapolation%nbtx=1; extrapolation%nbtz=1; extrapolation%nbdx=1; extrapolation%nbdz=1
extrapolation%sx0=0.0; extrapolation%hx0=0.0; extrapolation%z0=0.0; extrapolation%z0_image=0.0 
extrapolation%dsx=0.0;  extrapolation%t=0.0
extrapolation%sx=0.0; extrapolation%dx=10.0; extrapolation%dz=10.0 
extrapolation%z_sou=0.0; extrapolation%z_rec=0.0; extrapolation%dt=0.002 
extrapolation%x_min=0.0; extrapolation%x_max=0.0; extrapolation%image_x_min=0.0 
extrapolation%image_x_max=0.0; extrapolation%image_z_min=0.0
 extrapolation%ztapco=0.0; extrapolation%xtapco=0.0 
ncharacher=3;nlogical=3;ninteger=18;nreal=21
extrapolation%MPI_extrapolation_type_Size=ncharacher*128*sizeofch&
        +nlogical*sizeoflogical+ninteger*sizeofint+nreal*sizeofreal
end subroutine extrapolation_type_init

end module
