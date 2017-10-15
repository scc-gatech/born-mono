!<
! init_iter_pars
!
!Usage:
! call init_iter_pars(fin,jin,nin,max_read)
!
!Input Parameters:
! fin - integer(:)  initial coordinates of dataset itterate over
! jin - integer(:)  skip  coordinates of dataset to itterate over
! nin - integer(:)  number of coordinates of dataset to itterate over
! max_read - integer maximum number of elements to read at one time
!
!
!Description
!
! Initialize itterator for a n dimensional dataset
!
!CATEGORY
! Lib:Aux:Itterate Pars
!
!COMPILE LEVEL
!LOCAL
!
!>
!-------------------------------------------------
!
!Author: Robert Clapp, ESMB 463, 7230253
!
!Date Created:Sat Mar 15 18:52:02 1997
!
!
module iter_pars_mod 
  implicit none
  integer,dimension(:),private,pointer :: num_read,to_read,f_in,j_in&
    &,max_axis
  integer,private,save :: pos,ndim
  contains
  subroutine init_iter_pars(fin,jin,nin,max_read)
    integer,dimension(:) :: fin,jin,nin,max_read
    integer i1
    ndim=size(fin)
    allocate(num_read(ndim),to_read(ndim),f_in(ndim),j_in(ndim),max_axis&
      &(ndim))
    num_read=0
    to_read=nin
    f_in=fin
    j_in=jin
    pos=1
    do i1=1,ndim 
      if (nin(i1) >1 .and. max_read(i1) .eq. nin(i1)) then
        pos=i1+1
      end if
    end do 
    pos=min(pos,ndim)
    max_axis(1:pos)=nin
    max_axis(pos:)=1
    max_axis(pos)=max_read(pos)
  end subroutine 
!<
! clean_iter_pars
!
!Usage:
! call clean_iter_pars()
!
!
!Description
! Deallocates memory for  n-dimensional itterator
!
!CATEGORY
! Lib:Aux:Itterate Pars
!
!COMPILE LEVEL
!LOCAL
!
!>
  subroutine clean_iter_pars()
    integer i1
    deallocate(num_read,to_read,f_in,j_in)
  end subroutine 
!<
! iter_pars_exe
!
!Usage:
! logical function iter_pars_exe(f_out,j_out,n_out)
!
!Output Parameters:
! f_out - integer(:)  f parameters to use in call
! j_out - integer(:)  skip  f parameters to use in call
! n_out - integer(:)  number of elements to use in call
!
!
! Return Values
!
! .true. = still have more to itterate
! .false.= Done itterating
!
!Description
!
! Itterates an n-dimensional dataset. Useful utility for efficient memory
! handling because will calculate maximum it can read given complex windowing
! parameters.
!
!CATEGORY
! Lib:Aux:Itterate Pars
!
!COMPILE LEVEL
!LOCAL
!>
  logical function iter_pars_exe(f_out,j_out,n_out)
    logical :: logic_it
    integer,dimension(:),intent(out) :: f_out,j_out,n_out
    integer :: i1
    j_out=j_in
    i1=pos
    logic_it=.true.
    iter_pars_exe=.true.
    do while (logic_it)
      if (num_read(i1) >= to_read(i1)) then
        if (i1.eq.ndim) then
          logic_it=.false.
          iter_pars_exe=.false.
        else
          num_read(i1)=0
          i1= i1+1
          num_read(i1)= num_read(i1)+1
        end if
      else
        f_out=f_in+ num_read * j_out        
        if (pos>1) then
          f_out(:pos-1)=0
        end if
        n_out(:pos) = max_axis
        n_out(pos:) = 1
        n_out(pos) = min(to_read(pos)-num_read(pos),max_axis(pos))
        do i1=1,pos 
          num_read(i1)= num_read(i1)+n_out(i1)
        end do 
!                num_read(1:pos)+=n_out(1:pos)
        logic_it=.false.
      end if
    end do
  end function 
end module 
