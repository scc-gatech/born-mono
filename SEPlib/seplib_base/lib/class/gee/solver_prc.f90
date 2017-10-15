module solver_prc_mod 
! 0 = W (F S J p - d)
  use chain0_mod 
  use  solver_report_mod           ! 0 =      I   p
  implicit none
  logical, parameter, private  :: T = .true., F = .false.
  contains
  subroutine solver_prc( m,d, Fop, Sop, stepper, nSop, niter,eps   ,  &
    &           Wop,Jop,p0,rm0,err,resd,resm,mmov,rmov,verb)
    optional :: Wop,Jop,p0,rm0,err,resd,resm,mmov,rmov,verb
    interface  
!-------------------------- begin definitions -----------
      integer function Fop(adj,add,m,d)
        real::m(:),d(:)
        logical,intent(in)::adj,add
      end function 
      integer function Sop(adj,add,m,d)
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
    real, dimension(:),    intent(in)          :: d, p0,rm0
    integer,               intent(in)          :: niter, nSop
    logical,               intent(in)          :: verb
    real,                  intent(in)          :: eps
    real, dimension(:),    intent(out)         :: m,err, resd,resm
    real, dimension(:,:),  intent(out)         ::        rmov,mmov   
    real, dimension(size( m))                  :: p ,  g
    real, dimension(size( d) + nSop), target   :: rr, gg, tt
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
    if (present( Wop)) then
      stat=Wop(F,F,-d,rd) ! begin initialization --------
    else
      rd = -d                                                
      !Rd  = -W    d
    end if 
    rm = 0.
    if (present(rm0)) then
      rm=rm0                                !Rm  =      Rm0
    end if
    if (present( p0)) then
      p=p0                                        ! p  =       p0
      if (present( Wop)) then
        call chain0(Wop,Fop,Sop,F,T,p,rd,tm,td)
      else
        call chain0(    Fop,Sop,F,T,p,rd,tm   )!Rd +=  WFS  p0
      end if 
      rm = rm + eps*p                                          
      !Rm +=   e I p0
    else
      p=0
    end if 
    forget = T
!-------------------------- begin iterations ------------
    do iter = 1,niter  
      if (present(Wop)) then
        call chain0(Wop,Fop,Sop,T,F,g,rd,tm,td)
      else
        call chain0(    Fop,Sop,T,F,g,rd,tm   ) !g  = (WFS)'Rd
      end if 
      g = g + eps*rm                                           
      !g +=   e I'Rm
      if (present(Jop)) then
        tm=g
        stat=Jop(F,F,tm,g      )
      end if
!g  =     J g
      if (present(Wop)) then
        call chain0(Wop,Fop,Sop,F,F,g,gd,tm,td)
      else
        call chain0(    Fop,Sop,F,F,g,gd,tm   ) !Gd = (WFS) g
      end if 
      gm   =  eps*g                                            
      !Gm =   e I g
      stat = stepper(forget, p,g, rr,gg)                       
      !m+=dm; R+=dR
      if (stat .eq.1) then
        exit ! got stuck descending
      end if
      stat = Sop(F,F,p,m)                                      
      !m  = S p
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
