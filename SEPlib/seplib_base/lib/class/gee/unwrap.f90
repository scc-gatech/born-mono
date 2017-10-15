module unwrap  
  use cgstep_mod
  use igrad2
  use solver_smp_mod
  implicit none
  contains
  subroutine grad2init( z, n1,n2, rt )
    integer  i, j,           n1,n2
    real                 rt( n1,n2,2)
    complex               z( n1,n2  ),             a,b,c
    rt = 0.
    do i= 1, n1-1  
      do j= 1, n2-1  
        a =  z(i  ,j  )        
        c =  z(i+1,j  )
        rt(i,j,1) =  aimag( clog( c * conjg( a) ) )
        b =  z(i,  j+1)
        rt(i,j,2) =  aimag( clog( b * conjg( a) ) )
      end do
    end do
  end subroutine 
! Phase unwraper.   Starting from phase hh, improve it.
  subroutine unwraper( zz, hh, niter)
    integer  n1,n2,              niter
    complex              zz(:,:)
    real                 hh(:)
    real, allocatable ::  rt(:)
    n1 = size( zz, 1) 
    n2 = size( zz, 2)
    allocate( rt( n1*n2*2))
    call grad2init( zz,n1,n2, rt)
    call igrad2_init(  n1,n2)
    call solver_smp( m=hh, d=rt, Fop=igrad2_lop, stepper=cgstep, niter=niter&
      &, m0=hh)
    call cgstep_close ()
    deallocate( rt)
  end subroutine 
end module 
