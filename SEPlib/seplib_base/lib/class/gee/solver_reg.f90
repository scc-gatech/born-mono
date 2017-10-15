module solver_reg_mod 
! 0 = W (F J m - d)
  use chain0_mod 
  use  solver_report_mod          ! 0 =    A   m
  implicit none
  logical, parameter, private  :: T = .true., F = .false.
  contains
  subroutine solver_reg( m,d, Fop, Aop, stepper, nAop, niter,eps   ,  &
    &           Wop,Jop,m0,rm0,err,resd,resm,mmov,rmov,verb)
    optional :: Wop,Jop,m0,rm0,err,resd,resm,mmov,rmov,verb
    interface  
!-------------------------- begin definitions -----------
      integer function Fop(adj,add,m,d)
        real::m(:),d(:)
        logical,intent(in)::adj,add
      end function 
      integer function Aop(adj,add,m,d)
        real::m(:),d(:)
        logical,intent(in)::adj,add
      end function 
      integer function Wop(adj,add,m,d)
        real::m(:),d(:)
        logical,intent(in)::adj,add
      end function 
      integer function Jop(adj,add,m,d)
        real::m(:),d(:)
        logical,intent(in)::adj,add
      end function 
      integer function stepper(forget,m,g,rr,gg)
        real, dimension(:)  ::        m,g,rr,gg    
        logical             :: forget
      end function
    end interface 
    real, dimension(:),    intent(in)          :: d, m0,rm0
    integer,               intent(in)          :: niter, nAop
    logical,               intent(in)          :: verb
    real,                  intent(in)          :: eps
    real, dimension(:),    intent(out)         :: m,err, resd,resm
    real, dimension(:,:),  intent(out)         ::        rmov,mmov
    real, dimension(size( m))                  :: g
    real, dimension(size( d) + nAop), target   :: rr, gg, tt
    real, dimension(:), pointer                :: rd, gd, td
    real, dimension(:), pointer                :: rm, gm, tm
    integer                                    :: iter, stat
    logical                                    :: forget
    rd => rr(1:size(d))
    rm => rr(1+size(d):)
    gd => gg(1:size(d))
    gm => gg(1+size(d):)
    td => tt(1:size(d))
    tm => tt(1+size(d):)
    if (present(Wop)) then
      stat=Wop(F,F,-d,rd) ! begin initialization ---------
    else
      rd = -d                                           !Rd  = -W  d
    end if 
    rm = 0.
    if (present(rm0)) then
      rm=rm0                       !Rm  =    Rm0
    end if
    if (present( m0)) then
      m=m0                                 !m   =     m0
      if (present(Wop)) then
        call chain0(Wop,Fop,F,T,m,rd,td)
      else
        stat=           Fop(F,T,m,rd   )   !Rd +=  WF m0
      end if 
      stat = Aop(F,T,eps*m0,rm)                           
      !Rm += e A m0
    else
      m=0
    end if 
    forget = T
!--------------------------- begin iterations -----------
    do iter = 1,niter  
      if (present( Wop)) then
        call chain0(Wop,Fop,T,F,g,rd,td)
      else
        stat =           Fop(T,F,g,rd   )  !g  = (WF)'Rd
      end if 
      stat =  Aop(T,T,g,eps*rm)                           
      !g +=  e A'Rm
      if (present( Jop)) then
        tm=g
        stat=Jop(F,F,tm,g   )
      end if
!g  =   J  g
      if (present( Wop)) then
        call chain0(Wop,Fop,F,F,g,gd,td)
      else
        stat =           Fop(F,F,g,gd   )  !Gd  = (WF) g
      end if 
      stat =  Aop(F,F,eps*g,gm)                           
      !Gm  =  e A g
      stat = stepper(forget, m,g, rr,gg)                  
      !m+=dm; R+=dR
      if (stat .eq.1) then
        exit ! got stuck descending
      end if
      if (present( mmov)) then
        mmov(:,iter) =  m(:size(mmov,1)) ! report -----
      end if
      if (present( rmov)) then
        rmov(:,iter) = rr(:size(rmov,1))
      end if
      if (present( err )) then
        err(  iter) = dot_product(rd,rd)
      end if
      if (present( verb)) then
        if (verb) then
          call solver_report(iter,m,g,rd,rm)
        end if
      end if
      forget=F
    end do 
    if (present( resd)) then
      resd = rd
    end if
    if (present( resm)) then
      resm = rm(:size(resm))
    end if
  end subroutine 
end module 
