!Sergey's
module divide  
  use weight
  use solver_mod
  use cgstep_mod
  use tridiag
  implicit none
  contains
  subroutine divide_apply_tri(n,num,den,rat,eps1,eps2)
    integer :: n(3),n1,n2,n3
    real :: num(:),den(:),eps1,rat(:),eps2
    n1=n(1)
    n2=n(2)
    n3=n(3)
    call divide_apply_tri2(n1,n2,n3,num,den,rat,eps1,eps2)
  end subroutine 
  subroutine divide_apply_tri2(n1,n2,n3,num,den,rat,eps1,eps2)
    integer :: i2,n1,n2,i3,n3
    real :: num(n1,n2,n3),eps1
    real  :: den(n1,n2,n3),eps2,rat(n1,n2,n3)
    real, allocatable  :: d(:),a(:),b(:),c(:)
    integer :: ifirst,ilast
    allocate(d(n1),a(n1),b(n1),c(n1))
!first solve
!
!  d = L m  ---> num*den = den*den num/den
!  0 = e A m  ---> (-1 2 -1) in 1-D
!
    rat=0
    call rtris_init(n1)
    do i3=1,n3 
      ifirst=1
      do while (sum(abs(den(:,ifirst,i3)))<.001 .and. ifirst < n2-1)
        ifirst= ifirst+1
      end do 
      ilast=n2
      do while (sum(abs(den(:,ilast,i3)))<.001 .and. ilast > 1)
        ilast= ilast-1
      end do 
      if (ifirst <ilast) then
        d=num(:,ifirst,i3)*den(:,ifirst,i3)  
        b=den(:,ifirst,i3)*den(:,ifirst,i3)+ 2 *eps1
        a=-eps1
        c=a
        b(1)=    b(1)-eps1
        b(n1)= b(n1)-eps1 !boundary condition
        call rtris_back(a,b,c,d,rat(:,ifirst,i3))
        do i2=ifirst+1,ilast 
!NEXT add Extra reg
!
! m(:,i2-1) *  eps2 =  eps2
!
          if (i3.eq.1) then
            d=num(:,i2,i3)*den(:,i2,i3)+eps2*rat(:,i2-1,i3)
            b=den(:,i2,i3)*den(:,i2,i3)+ 2 *eps1 + eps2
          else
            d=num(:,i2,i3)*den(:,i2,i3)+eps2*(rat(:,i2,i3-1)+rat(:,i2-1&
              &,i3))
            b=den(:,i2,i3)*den(:,i2,i3)+ 2 *eps1 +2 * eps2
          end if 
!      else d=num(:,i2,i3)*den(:,i2,i3)+eps2*(rat(:,i2-1,i3));
          b(1)=      b(1)-eps1
          b(n1)= b(n1)-eps1 !boundary condition
          call rtris_back(a,b,c,d,rat(:,i2,i3))
        end do
      end if
    end do 
    deallocate(a,b,c,d)
    call rtris_close()
  end subroutine 
  subroutine divide_apply_cg (doprec, nd, num, den, rat, eps, niter,&
    & prec, nprec)
    logical,              intent (in)  :: doprec
    integer,              intent (in)  :: nd, nprec, niter
    real,                 intent (in)  :: eps
    real, dimension (nd), intent (in)  :: num, den
    real, dimension (nd), intent (out) :: rat
    interface  
      integer function prec (adj, add, x, dat)
        logical, intent (in) :: adj, add
        real, dimension (:)  :: x, dat
      end function
    end interface 
    real, dimension (:), pointer       :: wt
    allocate (wt (nd))
    wt = den
    call weight_init (wt)
    if (doprec) then
      call solver_prec (weight_lop, cgstep, prec, nprec,x = rat, dat =&
        & num, niter = niter, eps = eps)
    else
      call solver_reg (weight_lop, cgstep, prec, nprec,x = rat, dat =&
        & num, niter = niter, eps = eps)
    end if 
    call cgstep_close ()
    deallocate (wt)
  end subroutine 
end module 
