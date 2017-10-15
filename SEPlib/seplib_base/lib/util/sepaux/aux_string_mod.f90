!<
!aux_string_mod
!
!Usage:
!call aux_string_mod (arg1, arg2 arg3)
!
!Input Parameters:
!
!
!Output Parameters:
!
!
!
!Description:
!
!
!
!>
!-------------------------------------------------
!
!Author: Robert Clapp, ESMB 463, 7230253
!
!Date Created:Wed Apr  8  0:10:56 PDT 1998
!
!Purpose:
!
!
module aux_string_mod 
  implicit none
  contains
  subroutine decode_list(key_list,key,nkeys)
    character(len=*) :: key_list
    character(len=128), pointer :: key(:)
    character(len=128) :: temp_ch
    integer nkeys,i1,i2,ic
    nkeys=1
    do ic=1,len_trim(key_list)
      if (key_list(ic:ic).eq.":") then
        nkeys= nkeys+1
      end if
    end do 
    allocate(key(nkeys))
    i2=1
    i1=1
    temp_ch="                                                      "
    do ic=1,len_trim(key_list)
      if (key_list(ic:ic).eq.":") then
        key(i2)=temp_ch(1:len_trim(temp_ch))
        temp_ch="                                                     &
          &         "
        i1=1
        i2= i2+1
      else
        temp_ch(i1:i1)=key_list(ic:ic)
        i1=                i1+1
      end if
    end do 
    key(i2)=temp_ch(1:len_trim(temp_ch))
  end subroutine 
end module 
