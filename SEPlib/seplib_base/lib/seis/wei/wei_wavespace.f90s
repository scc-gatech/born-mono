module wei_wavespace_mod
use wei_basics_mod
implicit none
type(sep3d),private, save :: W,wpad
real, private,pointer, save :: omega(:),dstep(:)
integer,private, save :: opsign,npadx,npady
logical,private, save :: adj,add


contains

logical function init_reg_wave_space(win)
type(sep3d) :: win

init_reg_wave_space=.false.

call init_sep3d(win,"SCRATCH","COMPLEX","REGULAR",(/1,1,1,1,1,1,1,1/))
call from_param("amx_n",win%n(1));call from_param("amy_n",win%n(2))
call from_param("ahx_n",win%n(3));call from_param("ahy_n",win%n(4))
call from_param("az__n",win%n(5));call from_param("aw__n",win%n(6))

call from_param("amx_o",win%o(1));call from_param("amy_o",win%o(2))
call from_param("ahx_o",win%o(3));call from_param("ahy_o",win%o(4))
call from_param("az__o",win%o(5));call from_param("aw__o",win%o(6))

call from_param("amx_d",win%d(1));call from_param("amy_d",win%d(2))
call from_param("ahx_d",win%d(3));call from_param("ahy_d",win%d(4))
call from_param("az__d",win%d(5));call from_param("aw__d",win%d(6))

allocate(omega(my_nomp()))
allocate(dstep(my_nomp()))

call from_param("nzb",win%n(7),1);call from_param("nwb",win%n(8),1)

if(win%n(8) > win%n(6) .or. win%n(8) <1) then
  if(sep_thread_num()==0) write(0,*) "illegal nwb value",win%n(6)
  return
end if

if(win%n(7) > win%n(5) .or. win%n(5) <1) then
  if(sep_thread_num()==0) write(0,*) "illegal nzb value",win%n(7)
  return
end if

call sep3d_set_sep3d(win)

call from_param("npad_x",npadx,0)
call from_param("npad_y",npady,0)

init_reg_wave_space=.true.

end function

logical function check_space_logic(wsep,dsep,rsep,ssep)
type(sep3d) :: wsep
type(sep3d),optional :: dsep(:),rsep(:),ssep(:)
integer :: i,isep
real   :: f
logical :: ang,checkoff
check_space_logic=.false.

   call from_param("ray_par",ang,.false.)
   checkoff=.true.
   if(ang) checkoff=.false.

   if(present(dsep)) then
    do isep=1,size(dsep)
     if(.not. axis_check_param_file("m_x",1,1,wsep,dsep(isep),.false.)) then
      if(sep_thread_num()==0) write(0,*)  "data and wave space inconsistent"
      return
     end if
     if(.not. axis_check_param_file("m_y",2,2,wsep,dsep(isep),.false.)) then
      if(sep_thread_num()==0) write(0,*)  "data and wave space inconsistent"
      return
     end if
     if(.not. axis_check_param_file("h_x",3,3,wsep,dsep(isep),.false.)) then
      if(sep_thread_num()==0) write(0,*)  "data and wave space inconsistent"
      return
     end if
     if(.not. axis_check_param_file("h_y",4,4,wsep,dsep(isep),.false.)) then
      if(sep_thread_num()==0) write(0,*)  "data and wave space inconsistent"
      return
     end if
     if(.not. axis_check_param_file("w",5,6,dsep(isep),wsep,.false.)) then
      if(sep_thread_num()==0) write(0,*)  "data and wave space inconsistent"
      return
     end if
    end do
   end if

   if(present(rsep)) then
    do isep=1,size(rsep)
     if(.not. axis_check_param_file("z",5,5,wsep,rsep(isep),.false.)) then
      if(sep_thread_num()==0) write(0,*)  "image and wave space inconsistent"
      return
     end if
     if(.not. axis_check_param_file("m_x",1,1,wsep,rsep(isep),.false.)) then
      if(sep_thread_num()==0) write(0,*)  "image and wave space inconsistent"
      return
     end if
     if(.not. axis_check_param_file("m_y",2,2,wsep,rsep(isep),.false.)) then
      if(sep_thread_num()==0) write(0,*)  "image and wave space inconsistent"
      return
     end if
     if(.not. axis_check_param_file("h_x",3,3,wsep,rsep(isep),checkoff)) then
      if(sep_thread_num()==0) write(0,*)  "image and wave space inconsistent"
      return
     end if
     if(.not. axis_check_param_file("h_y",4,4,wsep,rsep(isep),checkoff)) then
      if(sep_thread_num()==0) write(0,*)  "image and wave space inconsistent"
      return
     end if
    end do
   end if


   f=-wsep%o(4)/wsep%d(4)
   i=nint(f)
   if(abs(f-i) > .01  .or. i < 0 .or. i >= wsep%n(4)) then
    if(sep_thread_num()==0)write(0,*) "wavespace does not include h_y=0.",f,i
    return
   end if
   f=-wsep%o(3)/wsep%d(3)
   i=nint(f)
   if(abs(f-i) > .01 .or. i < 0 .or. i >= wsep%n(3) ) then
    if(sep_thread_num()==0)write(0,*) "wavespace does not include h_x=0.",f,i
    return
   end if
     
   check_space_logic=.true.
end function

   logical function axis_check_param_file(axisdes,iax1,iax2,psep,fsep,checkod)
    type(sep3d) :: psep,fsep
    character(len=*) axisdes
    logical :: stat
    integer :: iax1,iax2
    logical  :: exact,checkod
    real   :: mye
    mye=epsilon(mye)*100.
    axis_check_param_file=.true.
                                                                                           
    if(psep%n(iax1) .ne. fsep%n(iax2)) then
      if(sep_thread_num()==0) write(0,*) "n of axis",iax1,&
        " of ",trim(fsep%tag),"(",fsep%n(iax2),&
      ") and ",trim(axisdes),"(",psep%n(iax1),") are inconsistent"
      axis_check_param_file=.false.
    end if
    if(.not. checkod) return
    if(psep%n(iax1) >1) then
      if(abs(psep%o(iax1) - fsep%o(iax2)) > mye) then
         if(sep_thread_num()==0) write(0,*) "o of axis",&
           iax1," of ",trim(fsep%tag),"(",fsep%o(iax2),&
        ") and ",trim(axisdes),"(",psep%o(iax1),") are inconsistent"
        axis_check_param_file=.false.
       end if
      if(abs(psep%d(iax1) - fsep%d(iax2)) > mye) then
         if(sep_thread_num()==0) write(0,*) "d of axis",&
          iax1," of ",trim(fsep%tag),"(",fsep%d(iax2),&
        ") and ",trim(axisdes),"(",psep%d(iax1),") are inconsistent"
         axis_check_param_file=.false.
       end if
    end if
   end function
!
! Grab and store
!

subroutine store_current_dstep(d)
real :: d
dstep(my_iomp())=d
end subroutine

subroutine grab_current_dstep(d)
real :: d
d=dstep(my_iomp())
end subroutine



subroutine grab_current_opsign(o)
integer :: o
o=opsign
end subroutine

subroutine store_current_omega(o)
real :: o
omega(my_iomp())=o
end subroutine

subroutine grab_current_omega(o)
real :: o
o=omega(my_iomp())*2.*pi
end subroutine

subroutine store_current_add(a)
logical :: a
add=a
end subroutine

logical function grab_current_add()
logical :: a
grab_current_add=add
end function


subroutine store_current_adj(a)
logical :: a
adj=a
opsign = goB2T
if(adj) opsign = goT2B
end subroutine

logical function grab_current_adj()
grab_current_adj=adj
end function


subroutine grab_current_wave_space(win)
type(sep3d) :: win
win=W
end subroutine

integer function grab_current_npady()
grab_current_npady=npady
end function

integer function grab_current_npadx()
grab_current_npadx=npadx
end function


subroutine store_current_wave_space(win)
type(sep3d) :: win
W=win
call init_sep3d(win,wpad,"SCRATCH")
wpad%n(1)=wpad%n(1)+npadx*2
wpad%n(2)=wpad%n(2)+npady*2
wpad%o(1)=wpad%o(1)-npadx*wpad%d(1)
wpad%o(2)=wpad%o(2)-npady*wpad%d(2)
call sep3d_set_sep3d(wpad)
end subroutine

subroutine grab_current_wave_space_pad(win)
type(sep3d) :: win
win=Wpad
end subroutine


                                                                                           

end module
