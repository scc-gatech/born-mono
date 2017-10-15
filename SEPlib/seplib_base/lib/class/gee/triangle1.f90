!!$=head1 NAME
!!$
!!$triangle1 - triangle smoothing
!!$
!!$=head1 SYNOPSIS
!!$
!!$initializer- C<call triangle1_init(nbox,ndat)>
!!$
!!$operator  - C<int=triangle1_lop(adj,add,x,y)>
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$=item nbox - int
!!$
!!$      size of the box
!!$
!!$=item ndat - int
!!$
!!$      number of data points
!!$
!!$=item adj  - logical
!!$
!!$      whether or not run adjoint
!!$
!!$=item add  - logical
!!$
!!$      wherther or not to add output to input
!!$
!!$=item mod    - C<real(ndat)>
!!$
!!$      model
!!$
!!$=item dat    - C<real(ndat)>
!!$
!!$      data
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Triangle smoothing using helix filters
!!$
!!$=head1 SEE ALSO
!!$
!!$L<triangle2>,L<helicon>,L<polydiv>
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
module triangle1  
! triangle smoothing
  use classutil
  use helicon
  use polydiv
  implicit none
  logical,   parameter, private              :: T = .true. 
  logical,   parameter, private              :: F = .false. 
  type( filter),        private              :: aa 
  type( filter),        private              :: bb 
  real ,private,     dimension (:), allocatable   :: tmp1 
  real ,private,     dimension (:), allocatable   :: tmp2 
  contains
  subroutine triangle1_init ( nbox,ndat )
    integer nbox, ndat
    if (.not. allocated(tmp1)) then
      allocate(tmp1 ( ndat)) 
    end if
    if (.not. allocated(tmp2)) then
      allocate(tmp2 ( ndat)) 
    end if
    call allocatehelix( aa, 1)
    call allocatehelix( bb, 1)
    aa%flt = -1.
    aa%lag = nbox
    bb%flt = -1.
    bb%lag = 1
    call helicon_init( aa)
    call polydiv_init(ndat, bb)
  end subroutine
  function triangle1_lop ( adj, add, mod, dat) result(stat)
    integer            :: stat 
    logical,intent(in) :: adj,add 
    real,dimension(:)  :: mod,dat 
    call adjnull (adj,add,mod,dat )
    call triangle1_lop2(adj,add,mod,dat )
    stat=0
  end function 
  subroutine triangle1_lop2(adj,add,mod,dat)
    logical,intent(in) :: adj,add 
    real, dimension (:)  :: mod 
    real, dimension (:)  :: dat 
    integer stat1, stat2
    if (adj) then
      stat1 = helicon_lop (T, F, tmp1, dat)
      stat2 = polydiv_lop (T, F, tmp2, tmp1)
      stat2 = polydiv_lop (F, F, tmp2, tmp1)
      stat1 = helicon_lop (F, T, tmp1, mod)
    else
      stat1 = helicon_lop (T, F, tmp1, mod)
      stat2 = polydiv_lop (T, F, tmp2, tmp1)
      stat2 = polydiv_lop (F, F, tmp2, tmp1)
      stat1 = helicon_lop (F, T, tmp1, dat)
    end if
  end subroutine
  subroutine  triangle1_close()
    deallocate( tmp1 ,tmp2)
    call deallocatehelix( aa)
    call deallocatehelix( bb)
    call polydiv_close()
  end subroutine 
end module 
