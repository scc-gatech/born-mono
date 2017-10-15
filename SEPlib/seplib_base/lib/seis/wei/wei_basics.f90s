module wei_basics_mod
use wei_timers_mod
use wei_mem_mod
use wei_verb_mod
use wei_external_mod
use wei_par_mod
implicit none

contains

logical function init_wei_basics()

init_wei_basics=.false.
    if(.not. init_wei_verbose()) then
      if(sep_thread_num()==0) write(0,*) "trouble initializing verbosity"
      return 
    end if
                                                                                           
    if(.not. init_wei_run()) then
      if(sep_thread_num()==0) write(0,*) "trouble initializing run parameters"
      return 
    end if
                                                                                           
    if(.not. init_par()) then
      if(sep_thread_num()==0) write(0,*) "trouble initializing parrallel status"
      return 
    end if

    if(.not. init_mem_usage()) then
      if(sep_thread_num()==0) write(0,*) "trouble initializing memory usage"
      return 
    end if
!    if(.not. init_wei_timers()) then
!      if(sep_thread_num()==0) write(0,*) "trouble initializing timers"
!      return 
!    end if



init_wei_basics=.true.
end function

logical function operation_check(adj,cd,cr) 
logical :: cr,cd,adj
character(len=128) :: method,logfile

operation_check=.false.
call my_operation(method)
if(adj ) then
   if(cd) then
     if(sep_thread_num()==0) &
      write(0,*) "when doing r inversion the data (D) must be provided"
     return
   end if
   call from_param("logfile",logfile,"R")
else if(.not. adj) then
   if(cR) then
     if(sep_thread_num()==0)&
      write(0,*) "when doing modeling or inversion the image (R) must be provided"
     return
   end if
   logfile="D"
   call from_param("logfile",logfile,"D")
else 
  if(sep_thread_num()==0) write(0,*) "unknown operation",trim(method)
  return
end if

call set_alternate_putch(logfile)
if(sep_thread_num()==0) call set_yes_putch()
operation_check=.true.
end function

end module
