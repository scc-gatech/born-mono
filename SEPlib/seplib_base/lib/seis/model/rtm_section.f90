module rtm_section_mod
use sep

contains

subroutine section_info(section,nsect,master_data,type)
integer :: nsect,ierr
character(len=*) :: type
logical :: master_data,section

  nsect=2;
  section=.false.
 call from_param("section",section,section)
 call from_param("nsect",nsect,nsect)
 call from_param("section_type",type,"BLOCK")
 call from_param("master_data",master_data,.true.)

end subroutine


end module
