module solver_tiny_mod  
! 0 = F m - d
  implicit none
  contains
  subroutine solver_tiny( m,d, Fop, stepper, niter, m0,resd)
    optional ::                                     m0,resd
    interface  
!--------------------begin definitions -------
      integer function Fop(adj,add,m,d)
        real,dimension(:) :: m,d
        logical,intent(in):: adj,add
      end function 
      integer function stepper(forget,m,g,rr,gg)
        real,dimension(:) ::          m,g,rr,gg    
        logical           ::   forget
      end function
    end interface 
    real, dimension(:),    intent(in)  :: d       ! data
    real, dimension(:),    intent(out) :: m       ! model
    real, dimension(:),    intent(in)  :: m0      ! initial model
    integer,               intent(in)  :: niter   
    ! number of iterations
    integer                            :: iter    ! iteration number
    real, dimension(size( m))          :: g       ! gradient (dm)
    real, dimension(size( d)),target   :: rr      
    ! data residual ( vector)
    real, dimension(:),       pointer  :: rd      
    ! data residual (pointer)
    real, dimension(size( d)),target   :: gg      
    ! conj grad     ( vector)
    real, dimension(:),       pointer  :: gd      
    ! conj grad     (pointer)
    integer                            :: stat    ! status flag
    real, dimension(:),    intent(out) :: resd    ! residual
    rd => rr(1:size( d))
    gd => gg(1:size( d))
!--------------------------begin initialization -----------
    rd = -d                               ! Rd =      - d
    m  = 0
    if (present( m0)) then
      m = m0      !  m =        m0
      stat = Fop(.false.,.true.,m,rd)   ! Rd = Rd + F m0
    end if
    do iter = 1,niter  
!--------- begin iterations -----------
      stat = Fop( .true.,.false.,g,rd)   !  g =  F' Rd
      stat = Fop(.false.,.false.,g,gd)   ! Gd =  F  g
      stat = stepper(.false., m,g, rr,gg)! m = m+dm; R = R + dR
    end do 
    if (present( resd)) then
      resd = rd
    end if
  end subroutine 
end module 
