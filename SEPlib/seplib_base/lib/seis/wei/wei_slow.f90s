module wei_slow_mod
use wei_wavespace_mod
implicit none
real, private, save, pointer :: slow_block_s(:,:,:),slow_block_g(:,:,:),slow_block(:,:,:)
integer, private, pointer, save :: nref_s(:),nref_g(:)
real, private, pointer, save :: ref_slow_s(:,:),ref_slow_g(:,:)
real, private, pointer, save :: bound_s(:,:),bound_g(:,:)
integer, private, pointer ,save :: izs(:),iref(:),iz_slow(:)
logical, private, pointer, save :: source(:)
integer, private, save, pointer :: ixg(:,:),iyg(:,:)
integer, private, save, pointer :: ixs(:,:),iys(:,:),imaps_g(:,:,:),imaps_s(:,:,:),imaps(:,:,:)
integer, private, save :: nvels
type(sep3d) :: slow_master(2),slow,slow_both(2)
character(len=128) :: slow_master_tag(2)


contains

!initialize slowness tag
logical function init_slow()
  type(sep3d) :: wsep
  nullify(slow_block,nref_s,nref_g,ref_slow_s,ref_slow_g,bound_s,bound_g)
  call grab_current_wave_space_pad(wsep)

  nullify(slow_block_s,slow_block_g,iz_slow)
  allocate(bound_s(1,1),bound_g(1,1),ref_slow_g(1,1),ref_slow_s(1,1))
  allocate(nref_s(1),nref_g(1))
  allocate(iref(my_nomp()))
  allocate(izs(my_nomp()))
  allocate(iyg(wsep%n(4),my_nomp()))
  allocate(ixg(wsep%n(3),my_nomp()))
  allocate(source(my_nomp()))
  allocate(iys(wsep%n(2),my_nomp()))
  allocate(ixs(wsep%n(1),my_nomp()))
  call add_mem(size(ixs)+size(iys)+size(source)+size(ixg)+size(iyg)+size(izs)+size(iref))
  call add_mem(size(nref_s)+size(nref_g)+size(bound_s)+size(bound_g)+size(ref_slow_g))
  call add_mem(size(ref_slow_s))
  init_slow=.true.
end function


!initialize a slowness tag
logical function init_slow_tag(tag_in,tag_out,slow_out)
type(sep3d) :: slow_out
character(len=*)  :: tag_in,tag_out
character(len=30) :: name
character(len=1024) :: fi
init_slow_tag=.false.


if(0==getch(tag_in,"s",name)) then
  if(sep_thread_num()==0) write(0,*) "slowness (",trim(tag_in),") not provided"
  return
end if
if(sep3d_file_exist(tag_in)) then
  call init_sep3d(tag_in,slow_out,"SCRATCH")
!  if(sep3d_ndims(slow_out)/=3)   then
!     if(sep_thread_num()==0) write(0,*) "slowness file must be 3 dimensions"
!     return
!  end if
else
  call from_param(trim(tag_in),fi)
  write(0,*) trim(fi)
  if(sep_thread_num()==0) write(0,*)  "trouble opening up slowness file ",trim(tag_in)
  return
end if
tag_out=tag_in
init_slow_tag=.true.
end function


!map  the midpoint  vs slowness axis
subroutine slow_map_it(h,wsep,iw,ssep,is,ib)
type(sep3d) :: wsep,ssep
integer :: i,iloc,ib(:),iw,is
real :: floc,fpos,h
floc=h+wsep%o(iw)-wsep%d(iw)
do i=1,wsep%n(iw)
  floc=floc+wsep%d(iw)
  fpos=(floc-ssep%o(is))/ssep%d(is)+1.
  ib(i)=floor(fpos)
  if(ib(i) <1)  then
   ib(i)=1
  else if(ib(i) >ssep%n(is))  then
   ib(i)=ssep%n(is)
  end if
end do
end subroutine



!---------------------------------------------------
!                                                  !
!         STORE AND GRAB SLOWNESS INFO             !
!                                                  !
!---------------------------------------------------

!store the current reference velocity
subroutine store_current_iref(i)
integer :: i
iref(my_iomp())=i
end subroutine

!store the current reference velocity
subroutine grab_current_iref(i)
integer :: i
i=iref(my_iomp())
end subroutine

!grab the current reference velocity
subroutine grab_current_izs(i)
integer :: i
i=izs(my_iomp())
end subroutine


!store the current depth step
subroutine store_current_izs(iz)
integer :: iz,i1,i2,iuse
type(sep3d) :: wsep
integer,pointer :: ix(:),iy(:)
integer, pointer :: imap(:,:),n_ref(:)
real, pointer :: rslow(:,:),bound(:,:)

call grab_fract_map(ix,iy,imap)
call grab_slowness_refs(n_ref,rslow,bound)
imap=>imaps(:,:,my_iomp())

izs(my_iomp())=iz
iuse=iz_slow(iz)
do i2=1,size(slow_block,2)
  do i1=1,size(slow_block,1)
   imap(i1,i2)=1
   do while(slow_block(i1,i2,iuse) > bound(imap(i1,i2),iz)) 
     imap(i1,i2)=imap(i1,i2)+1
   end do
  end do
end do

end subroutine


!store the current offset information
subroutine store_current_ihx_ihy(ihx,ihy)
integer :: ihx,ihy,imy,imx
real :: hx,hy,mx,my,scale
type(sep3d) :: wsep
call grab_current_wave_space_pad(wsep)
hx=wsep%o(3)+wsep%d(3)*(ihx-1)
hy=wsep%o(4)+wsep%d(4)*(ihy-1)
scale=1
if(source(my_iomp())) scale=-1
call slow_map_it(scale*hx,wsep,1,slow_both(1),1,ixs(:,my_iomp()))
call slow_map_it(scale*hy,wsep,2,slow_both(1),2,iys(:,my_iomp()))
if(nvels==2) then
  call slow_map_it(scale*hx,wsep,1,slow_both(2),1,ixg(:,my_iomp()))
  call slow_map_it(scale*hy,wsep,2,slow_both(2),2,iyg(:,my_iomp()))
end if

end subroutine

!grab the current irefence slowness map
subroutine grab_iref_mid_map(iref_mxmy)
integer :: iref_mxmy(:,:)
integer :: i1,i2,i
integer,pointer :: ix(:),iy(:)
integer, pointer :: imap(:,:),n_ref(:)
real, pointer :: rslow(:,:),bound(:,:)

call grab_fract_map(ix,iy,imap)

do i2=1,size(iref_mxmy,2)
  do i1=1,size(iref_mxmy,1)
    iref_mxmy(i1,i2)=imap(ix(i1),iy(i2))
  end do
end do


end subroutine






!grab a map of the slowness in terms of midpoint 
subroutine grab_slow_mid_map(slow_mxy)
real :: slow_mxy(:,:)
integer :: i1,i2,iz
integer,pointer :: ix(:),iy(:)
integer, pointer :: imap(:,:)
call grab_fract_map(ix,iy,imap)
iz=iz_slow(izs(my_iomp()))

do i2=1,size(slow_mxy,2)
  do i1=1,size(slow_mxy,1)
    slow_mxy(i1,i2)=slow_block(ix(i1),iy(i2),iz)
  end do
end do

!iz= srite("mycheck.H",slow_block,size(slow_block)*4)
!iz= srite("mycheck2.H",slow_mxy,size(slow_mxy)*4)

end subroutine




!grab the mapping from slowness to wavefield mx,my
subroutine grab_fract_map(ix,iy,imap)
integer,pointer :: iy(:),ix(:)
integer, pointer :: imap(:,:)
if(source(my_iomp())) then
 ix=>ixs(:,my_iomp())
 iy=>iys(:,my_iomp())
 imap=>imaps_s(:,:,my_iomp())
else
 ix=>ixg(:,my_iomp())
 iy=>iyg(:,my_iomp())
 imap=>imaps_g(:,:,my_iomp())
end if
end subroutine




!store the slowness files for the downward continuation
subroutine store_slowness(s1,s2)
type(sep3d) :: s1,wsep
type(sep3d),optional :: s2
slow_both(1)=s1
allocate(imaps_s(s1%n(1),s1%n(2),my_nomp()))
call add_mem(size(imaps_s))
call grab_current_wave_space_pad(wsep)
if(associated(slow_block_s)) then
  call del_mem(size(slow_block_s))
  deallocate(slow_block_s)
end if
allocate(slow_block_s(s1%n(1),s1%n(2),wsep%n(7)))
call add_mem(size(slow_block_s))
if(present(s2)) then
   call add_mem(size(imaps_g))
   slow_both(2)=s2
   allocate(imaps_g(s2%n(1),s2%n(2),my_nomp()))
   call add_mem(size(imaps_g))
   nvels=2
   if(associated(slow_block_g)) then
     call del_mem(size(slow_block_g))
     deallocate(slow_block_g)
   end if
   allocate(slow_block_g(s1%n(1),s1%n(2),wsep%n(7)))
   call add_mem(size(slow_block_s))
else
   nvels=1
   ixg=>ixs
   iyg=>iys
   imaps_g=>imaps_s
   slow_block_g=>slow_block_s
end if
end subroutine

!grab the current slowness structure
subroutine grab_current_slowness(s)
type(sep3d) :: s
s=slow
end subroutine
subroutine grab_slowness_masters(s,ns,tags)
type(sep3d) :: s(:)
integer     :: ns
character(len=*) :: tags(:)
s(1)=slow_master(1)
tags(1)=slow_master_tag(1)
if(nvels==2) then
 s(2)=slow_master(2)
 tags(2)=slow_master_tag(2)
end if 
ns=nvels
end subroutine

subroutine store_slowness_masters(s,tags)
type(sep3d) :: s(:)
character(len=*) :: tags(:)
nvels=size(s)
slow_master(1)=s(1)
slow_master_tag(1)=tags(1)
if(nvels==2) then
  slow_master(2)=s(2)
  slow_master_tag(2)=tags(2)
end if
end subroutine



!store the current slowness type (source vs receiver)
subroutine store_current_s_r(s)
logical :: s
source(my_iomp())=s
if(.not. source(my_iomp()) .and. nvels>1 ) then
  slow=slow_both(2)
  slow_block=>slow_block_g
  imaps=>imaps_g
else 
  slow=slow_both(1)
  slow_block=>slow_block_s
  imaps=>imaps_s
end if
end subroutine

!grab the current slowness type (source vs receiver)
subroutine grab_current_s_r(s)
logical :: s
s=source(my_iomp())
end subroutine

!gab the current slowness block
subroutine grab_current_slow_block(sss)
real, pointer :: sss(:,:,:)
if(source(my_iomp())) then
  sss=>slow_block_s
else
  sss=>slow_block_g
end if
end subroutine

!store the current slowness block
subroutine store_current_slow_block(sss)
real, pointer :: sss(:,:,:)

if(source(my_iomp())) then
  slow_block_s=>sss
else
  slow_block_g=>sss
end if
end subroutine


!grab reference slowness info
subroutine store_slowness_refs(n,r,b)
integer :: n(:)
real :: r(:,:),b(:,:)



if(source(my_iomp())) then
  call del_mem(size(nref_s)+size(ref_slow_s)+size(bound_s))
  deallocate(nref_s); allocate(nref_s(size(n)))
  deallocate(ref_slow_s); allocate(ref_slow_s(size(r,1),size(r,2)))
  deallocate(bound_s); allocate(bound_s(size(b,1),size(b,2)))
  call add_mem(size(nref_s)+size(ref_slow_s)+size(bound_s))
  nref_s=n
  ref_slow_s=r
  bound_s=b
else
  call del_mem(size(nref_g)+size(ref_slow_g)+size(bound_g))
  deallocate(nref_g); allocate(nref_g(size(n)))
  deallocate(ref_slow_g); allocate(ref_slow_g(size(r,1),size(r,2)))
  deallocate(bound_g); allocate(bound_g(size(b,1),size(b,2)))
  call add_mem(size(nref_g)+size(ref_slow_g)+size(bound_g))
  nref_g=n
  ref_slow_g=r
  bound_g=b
end if
end subroutine


!store reference slowness info
subroutine grab_slowness_refs(n,r,b)
integer,pointer :: n(:)
real,pointer :: r(:,:),b(:,:)

if(source(my_iomp()).or. nvels==1) then
  n=>nref_s
  r=>ref_slow_s
  b=>bound_s
else
  n=>nref_g
  r=>ref_slow_g
  b=>bound_g
end if
end subroutine

subroutine grab_current_nref(nref)
integer :: nref
integer,pointer :: n(:)
real,pointer :: r(:,:),b(:,:)

call grab_slowness_refs(n,r,b)
!nref=n(my_iomp())
nref=n(izs(my_iomp()))
end subroutine


subroutine grab_current_slow0(s)
real :: s
integer,pointer :: n(:)
real,pointer :: r(:,:),b(:,:)

call grab_slowness_refs(n,r,b)
s=r(iref(my_iomp()),izs(my_iomp()))
end subroutine

subroutine grab_slowness_block(slowb1,slowb2)
real, pointer :: slowb1(:,:,:)
real, pointer,optional :: slowb2(:,:,:)

slowb1=>slow_block_s
if(present(slowb2)) slowb2=>slow_block_g

end subroutine

subroutine store_slowness_block(slowb1,slowb2)
real, pointer :: slowb1(:,:,:)
real, pointer,optional :: slowb2(:,:,:)

slow_block_s=>slowb1
if(present(slowb2)) slow_block_g=>slowb2

end subroutine

subroutine store_slow_iz(iz)

integer :: iz(:)
if(associated(iz_slow)) deallocate(iz_slow)
allocate(iz_slow(size(iz)))
iz_slow=iz
end subroutine

end module
