module nmisinput  
! find a mask of missing filter inputs
  use nhelicon
  implicit none
  contains
  subroutine find_mask( known, aa)
    logical,  intent( in)          :: known(:)
    type( nfilter)                 :: aa
    real, dimension( size (known)) :: rr, dfre
    integer                        :: stat, i
    where( known)
      dfre = 0.
    elsewhere
      dfre = 1.    
    end where 
    call nhelicon_init( aa)
    do i = 1, size( aa%hlx)
      aa%hlx( i)%flt = 1.
    end do 
    stat = nhelicon_lop( .false., .false., dfre, rr)
    where ( rr > 0.)
      aa%mis = .true.        
end where
    do i = 1, size( aa%hlx)
      aa%hlx( i)%flt = 0. 
    end do
  end subroutine 
end module 
