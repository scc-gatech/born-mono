module  wei_verb_mod
use sep
use sep3d_struct_mod
use wei_external_mod
implicit none
logical,private,save :: verb,debug,verb_all
integer, private, save :: jprint, ido, ndo,fprint,depth

contains

logical function init_wei_verbose()
  init_wei_verbose=.false.

  call from_param("debug",debug,.false.)
  call from_param("verb",verb_all,.false.)
  verb=.false.
 if(sep_thread_num()==0) call from_param("verb",verb,.false.)

 depth=2
  init_wei_verbose=.true.
end function

logical function my_debug()
my_debug=debug
end function

logical function my_verb()
my_verb=verb
end function

logical function init_print_progress(fsteps,nsteps) 
integer :: nsteps,fsteps
real    :: pct

if(sep_thread_num()==0) then
  call from_param("pct_master",pct,2.)
else
  call from_param("pct_slave",pct,10.)
end if
ndo=nsteps
ido=fsteps
jprint=real(ndo)*pct/100.

fprint=floor(real(ido)/real(jprint))
init_print_progress=.true.

end function

subroutine print_progress(string,ijump)
integer, optional  :: ijump
integer           :: iadd
character(len=*)  :: string
real :: pct
iadd=1
if(present(ijump)) iadd=ijump
if(verb_all) then
  ido=ido+iadd
  pct=real(nint(1000.*real(ido)/real(ndo)))/10.
  if(ido >= fprint) then
    write(0,*) sep_thread_num(),"=thread finished ",pct, "% of ",trim(string)
    fprint=fprint+jprint
  end if
end if
end subroutine

subroutine in(t)
    character(len=*), intent(in) :: t
    integer :: i
                                                                                           
    depth=depth+1
    write(0,"(a)",advance='no') '        '
    do i=1,depth
       write(0,"(a)",advance='no') '  '
    end do
    write(0,*) '>>> enter ',trim(t)
                                                                                           
  end subroutine in
  !----------------------------------------------------------------
  !! exit a function/subroutine
  subroutine out(t)
    character(len=*), intent(in) :: t
    integer :: i
                                                                                           
    write(0,"(a)",advance='no') '        '
    do i=1,depth
       write(0,"(a)",advance='no') '  '
    end do
    write(0,*) '<<< exit  ',trim(t)
    depth=depth-1
                                                                                           
  end subroutine out
  !-------------------------------
                                                                                           





end module
