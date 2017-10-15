module solver_mod
  use chain_mod
  use arnoldi_mod
  use llist_mod
  use ddot_mod

  implicit none
  private :: normlz
  logical, parameter, private  :: T = .true., F = .false.

contains
!!$=head1 NAME
!!$
!!$solver_prec - iteratively solve preconditioned system of equation
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call solver_prec(oper, solv, prec, nprec, x, dat, niter, eps,
!!$ x0,nmem,nfreq,err,res,xp,nloper,xmov,rmov,wght,verb,known,wt)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item oper   - function
!!$
!!$      operator (standard interface)
!!$
!!$=item solv   - step function
!!$
!!$      step mecanism
!!$
!!$=item prec   - function
!!$
!!$      preconditioning operator (standard interface)
!!$
!!$=item nprec  - integer
!!$
!!$      number of output pts when prec is applied to the model
!!$
!!$
!!$=item dat    - C<real(:)>
!!$
!!$      data
!!$
!!$=item niter  - integer
!!$
!!$      number of iterations
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  x0    - C<real(:)>
!!$
!!$       Initial model
!!$
!!$=item nmem   - integer
!!$
!!$      Change behavior after nmem itterations
!!$      use in conjunction with nfreq and/or wght
!!$
!!$=item nfreq  - integer
!!$
!!$      Restart step function every nfreq itterations
!!$      after nmem steps
!!$
!!$=item nloper - func
!!$
!!$      Non-linear operator to use for forward operator
!!$
!!$=item wght   - func(res,m)
!!$
!!$      Weighting funtion operator
!!$
!!$=item verb   - logical
!!$
!!$      Wheter or not to be verbose
!!$
!!$=item known  - logical(:)
!!$
!!$      Specify (.true.) model values that are knonw
!!$
!!$=item wt     - real(:)
!!$
!!$      Weighting operator to apply to residuals
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item x      - C<real(:)>
!!$
!!$      model
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item err    - real(niter)
!!$
!!$      Error sum((d-d~)**2)
!!$
!!$=item res    - C<real(:)>
!!$
!!$      Final residual (size of data)
!!$
!!$=item xp     - C<real(:)>
!!$
!!$      Precondited variable
!!$
!!$=item xmov   - C<real(:,niter)>
!!$
!!$      Movie of model values as a function of iter
!!$
!!$=item rmov   - C<real(:,niter)>
!!$
!!$      Movie of data residual
!!$
!!$=item mdiag  - C<real(:)>
!!$
!!$      Diagonal of model resolution matrix (see nort)
!!$
!!$=item mres  - C<real(:,:)>
!!$
!!$      Model resolution matrix (see nort)
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Generic  preconditioned solver.
!!$
!!$=head1 COMMENTS
!!$
!!$All operators must follow convetion
!!$
!!$integer function (logical adj, logical add, C<real(:)> model, C<real(:)> data)
!!$
!!$All solv routines must follow the convention
!!$
!!$C<integer function(logical forget,real(:) x,real(:) g, real(:) rr,real(:) gg)>
!!$
!!$=head1 SEE ALSO
!!$
!!$L<solver>,L<solver_prec>,L<nonlin_solver>,L<lsqr>
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$


  subroutine solver_prec (oper, solv, prec, nprec, x, dat, niter, eps &
  ,             x0,nmem,nfreq,err,res,xp,nloper,xmov,rmov,wght,verb,known,wt,mwt) 
    optional :: x0,nmem,nfreq,err,res,xp,nloper,xmov,rmov,wght,verb,known,wt,mwt
    interface
       function wght (res, w) result (stat)
         integer              :: stat
         real, dimension (:)  :: res, w
       end function wght
       function oper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function oper
       function solv (forget, x, g, rr, gg) result (stat)
         integer             :: stat
         logical             :: forget
         real, dimension (:) :: x, g, rr, gg
       end function solv
       function prec (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function prec
       function nloper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function nloper
    end interface
    real,    dimension (:),   intent (in)           :: dat, x0, wt, mwt
    logical, dimension (:),   intent (in)           :: known
    real,    dimension (:),   intent (out)          :: x, err, res, xp
    integer,                  intent (in)           :: niter, nmem, nfreq, nprec
    real,    dimension (:,:), intent (out)          :: xmov, rmov   
    real,                     intent (in)           :: eps
    logical,                  intent (in)           :: verb

    real,    dimension (size (dat) + nprec), target :: p, g
    real,    dimension (size (dat))                 :: rr, gg
    real,    dimension (:), pointer                 :: pm, pd, gm, gd, wht
    integer                                         :: iter, stat, nx, nd
    integer                                         :: dprr, dppd, dppm, dpgm
    logical                                         :: forget, v

    nd = size (dat)
    nx = size (x)

    pm => p (1 : nprec) ; pd => p (1 + nprec:) ; pd = 0.
    gm => g (1 : nprec) ; gd => g (1 + nprec:)

    if (present (wt) .or. present (wght)) then
       allocate (wht (nd))
       if (present (wt)) then
          wht = wt
       else
          wht = 1.
       end if
    end if

    rr = -dat
    if (present (x0)) then
       pm = x0 
       if (present (nloper)) then
          if (present (mwt)) then
             call chain (nloper, prec, F, T, pm*mwt, rr, x)
          else 
             call chain (nloper, prec, F, T, pm, rr, x)
          end if
       else
          if (present (mwt)) then
             call chain (  oper, prec, F, T, pm*mwt, rr, x)
          else
             call chain (  oper, prec, F, T, pm, rr, x)
          end if
       end if
    else
       pm = 0.
    end if

    v = F ; if (present (verb)) v = verb
    forget = F
    do iter = 1, niter
       if (present (nmem )) forget = (iter > nmem) ! restart
       if (present (wght) .and. forget) stat = wght (rr, wht)
       if (present (wght) .or. present (wt)) then
          rr = eps*pd + wht*rr 
          call chain (oper, prec, T, F, gm, rr*wht, x) 
       else
          call chain (oper, prec, T, F, gm, rr,    x)
       end if
       if (present (mwt)) gm = gm*mwt
       gd = eps*rr
       if (present (known)) where (known) gm = 0.
       if (present (mwt)) then
          call chain (oper, prec, F, F, gm*mwt, gg,   x)
       else
          call chain (oper, prec, F, F, gm, gg,   x)
       end if
       if (present (wght) .or. present (wt)) then
          gg = eps*gd + wht*gg
       else
          gg = eps*gd + gg
       end if
       if (forget .and. present (nfreq)) &
            forget = (mod (iter, nfreq) == 0) ! periodic restart
       if (v) then
          dprr = int (dot_product (rr, rr)/nd)
          dppd = int (dot_product (pd, pd)/nd)
          dppm = int (dot_product (pm, pm)/nprec)
          dpgm = int (dot_product (gm, gm)/nprec)
          write (0,*) "iteration ", iter, " res", dprr, &
          " prec dat", dppd, " prec mod", dppm, " grad", dpgm
       end if
       stat = solv (forget, p, g, rr, gg)
       if (present (nloper)) then
          rr = eps*pd -dat
          if (present (mwt)) then
             call chain (nloper, prec, F, T, pm*mwt, rr, x)
          else
             call chain (nloper, prec, F, T, pm, rr, x)
          end if
       else if (present (wght) .or. present (wt)) then
          rr =        -dat
          if (present (mwt)) then
             call chain (  oper, prec, F, T, pm*mwt, rr, x)
          else
             call chain (  oper, prec, F, T, pm, rr, x)
          end if
       else if (present (xmov) .or. iter == niter) then
          if (present (mwt)) then
             stat = prec (F, F, pm*mwt, x)
          else
             stat = prec (F, F, pm, x)
          end if
       end if
       if (present (xmov)) xmov (:,iter) =  x (1 : size (xmov, 1))
       if (present (rmov)) rmov (:,iter) =  pd * eps
       !       if (present (err)) err (iter) = dot_product (p, p) * eps*eps
       if (present (err)) err (iter) = dot_product (rr,rr) 
    end do

    if (present (xp)) then
       if (present (mwt)) then
          xp = pm*mwt
       else
          xp = pm
       end if
    end if
!    if (present (res)) res = pd * eps
    if (present (res)) res = rr
    if (present (wght) .and. .not. present (wt)) deallocate (wht)
  end subroutine solver_prec

!!$
!!$=head1 NAME
!!$
!!$solver_reg - iteratively solve regularized system of equation
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call solver_reg(oper, solv, reg, nreg, x, dat, niter, eps,
!!$ x0,nmem,nfreq,err,res,nloper,xmov,rmov,wght,verb,known,wt)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item oper   - function
!!$
!!$      operator (standard interface)
!!$
!!$=item solv   - step
!!$
!!$      step mecanism
!!$
!!$=item reg    - function
!!$
!!$      regularization operator operator(standard interface)
!!$
!!$=item nreg  - integer
!!$
!!$      number of output pts when reg is applied to the model
!!$
!!$
!!$=item dat    - C<real(:)>
!!$
!!$      data
!!$
!!$=item niter  - integer
!!$
!!$      number of iterations
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  x0    - C<real(:)>
!!$
!!$       Initial model
!!$
!!$=item nmem   - integer
!!$
!!$      Change behavior after nmem itterations
!!$      use in conjunction with nfreq and/or wght
!!$
!!$=item nfreq  - integer
!!$
!!$      Restart step function every nfreq itterations
!!$      after nmem steps
!!$
!!$=item nloper - func
!!$
!!$      Non-linear operator to use for forward operator
!!$
!!$=item wght   - func(res,m)
!!$
!!$      Weighting funtion operator
!!$
!!$=item verb   - logical
!!$
!!$      Wheter or not to be verbose
!!$
!!$=item known  - logical(:)
!!$
!!$      Specify (.true.) model values that are knonw
!!$
!!$=item wt     - real(:)
!!$
!!$      Weighting operator to apply to residuals
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item x      - C<real(:)>
!!$
!!$      model
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item err    - real(niter)
!!$
!!$      Error sum((d-d~)**2)
!!$
!!$=item res    - C<real(:)>
!!$
!!$      Final residual (size of data)
!!$
!!$=item xmov   - C<real(:,niter)>
!!$
!!$      Movie of model values as a function of iter
!!$
!!$=item rmov   - C<real(:,niter)>
!!$
!!$      Movie of data residual
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Generic regularized solver.
!!$
!!$=head1 COMMENTS
!!$
!!$All operators must follow convetion
!!$
!!$C<integer function (logical adj, logical add, real(:) model, real(:)data)>
!!$
!!$All solv routines must follow the convention
!!$
!!$C<integer function(logical forget,real(:) x,real(:) g, real(:) rr,real(:) gg)>
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<solver_prec>,L<solver>,L<nonlin_solver>,L<lsqr>
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$


  subroutine solver_reg (oper, solv, reg, nreg, x, dat, niter, eps &
  ,             x0,nmem,nfreq,err,res,resm,nloper,nlreg,xmov,rmov,wght,verb,known,wt)
    optional :: x0,nmem,nfreq,err,res,resm,nloper,nlreg,xmov,rmov,wght,verb,known,wt
    interface
       function wght (res, w) result (stat)
         integer              :: stat
         real, dimension (:)  :: res, w
       end function wght
       function oper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function oper
       function solv (forget, x, g, rr, gg) result (stat)
         integer             :: stat
         logical             :: forget
         real, dimension (:) :: x, g, rr, gg
       end function solv
       function reg (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function reg
       function nloper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function nloper
       function nlreg (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function nlreg
    end interface
    real, dimension (:),    intent (in)         :: dat, x0, wt
    real, dimension (:),    intent (out)        :: x, err, res, resm
    real,                   intent (in)         :: eps
    integer,                intent (in)         :: niter, nmem, nfreq, nreg
    real, dimension (:,:),  intent (out)        :: xmov, rmov   
    logical,                intent (in)         :: verb
    logical, dimension (:), intent (in)         :: known

    real, dimension (size (x))                  :: g
    real, dimension (size (dat) + nreg), target :: rr, gg
    real, dimension (:), pointer                :: rd, rm, gd, gm, wht
    integer                                     :: iter, stat, nd
    logical                                     :: forget, v
    integer                                     :: dprd, dprm, dpx, dpg

    nd = size (dat)
    rd => rr (1 : nd) ; rm => rr (1 + nd :)
    gd => gg (1 : nd) ; gm => gg (1 + nd :)

    if (present (wt) .or. present (wght)) then
       allocate (wht (nd))
       if (present (wt)) then
          wht = wt
       else
          wht = 1.
       end if
    end if

    rd = -dat
    if (present (x0)) then
       x = x0 
       if (present (nloper)) then
          stat = nloper (F, T, x, rd) 
       else
          stat =   oper (F, T, x, rd) 
       end if
       if (present (nlreg)) then
          stat = nlreg  (F, F, x, rm)
       else
          stat =   reg  (F, F, x, rm)            
       end if
       rm = rm*eps
    else
       x = 0. ; rm = 0.
    end if

    forget = F
    v = F ; if (present (verb)) v = verb
    do iter = 1, niter
       if (present (nmem )) forget = (iter > nmem) ! restart
       if (present (wght) .and. forget) stat = wght (rd, wht)
       if (present (wght) .or. present (wt)) then 
          rd = wht*rd
          call array (oper, reg, T, F, g, wht*rd, rm*eps)
       else
          call array (oper, reg, T, F, g,     rd, rm*eps) 
       end if
       if (present (known)) where (known) g = 0. 
       call array (oper, reg, F, F, g, gd, gm)
       gm = gm*eps
       if (present (wght) .or. present (wt))  gd = gd*wht
       if (forget .and. present (nfreq)) &  
       forget = (mod (iter, nfreq) == 0) ! periodic restart

       if (v) then
          dprd = int (dot_product (rd, rd))
          dprm = int (dot_product (rm, rm))
          dpx  = int (dot_product (x, x))
          dpg  = int (dot_product (g, g))
          write (0,*) "iteration ", iter,  &
          " res dat", dprd, " res mod", dprm,  &   
          " mod ", dpx, " grad", dpg
       end if

       stat = solv (forget, x, g, rr, gg)

       if (present (nloper)) then
          rd = -dat; stat = nloper (F, T, x, rd)
       endif
       if (present (nlreg)) then
          stat = nlreg  (F, F, x, rm); rm = rm*eps
       end if
       if (present (wght) .or. present (wt)) then
          rd = -dat; stat = oper (F, T, x, rd)
       end if
       if (present (xmov)) xmov (:,iter) =  x (:size (xmov, 1))
       if (present (rmov)) then
          rmov (:,iter) = rr (:size (rmov, 1))
       end if
       if (present (err)) err (iter) = dot_product (rd, rd)
    end do

    if (present (res)) res = rd
    if (present (resm)) resm = rm (:size (resm))
    if (present (wght) .or. present (wt)) deallocate (wht)
  end subroutine solver_reg




  subroutine solver_van (oper, solv, van, nvan, x, dat, niter, eps &
  ,             x0,nmem,nfreq,err,res,resm,nloper,xmov,rmov,wght,verb,known,wt)
    optional :: x0,nmem,nfreq,err,res,resm,nloper,xmov,rmov,wght,verb,known,wt
   interface
       function wght (res, w) result (stat)
         integer              :: stat
         real, dimension (:)  :: res, w
       end function wght    
       function oper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function oper
       function solv (forget, x, g, rr, gg) result (stat)
         integer             :: stat
         logical             :: forget
         real, dimension (:) :: x, g, rr, gg
       end function solv
       function van (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function van
       function nloper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function nloper
    end interface
    real, dimension (:),    intent (in)         :: dat, x0, wt
    real, dimension (:),    intent (out)        :: x, err, res, resm
    real,                   intent (in)         :: eps
    integer,                intent (in)         :: niter, nmem, nfreq, nvan
    real, dimension (:,:),  intent (out)        :: xmov, rmov   
    logical,                intent (in)         :: verb
    logical, dimension (:), intent (in)         :: known

    real, dimension (nvan)                      :: g, p
    real, dimension (size (dat) + nvan), target :: rr, gg
    real, dimension (:), pointer                :: rd, rm, gd, gm, wht
    integer                                     :: iter, stat, nd
    logical                                     :: forget, v
    integer                                     :: dprd, dprm, dpp, dpg

    nd = size (dat)
    rd => rr (1 : nd) ; rm => rr (1 + nd :)
    gd => gg (1 : nd) ; gm => gg (1 + nd :)

    if (present (wt) .or. present (wght)) then
       allocate (wht (nd))
       if (present (wt)) then
          wht = wt
       else
          wht = 1.
       end if
    end if

    rd = -dat
    if (present (x0)) then
       p = x0 
       if (present (nloper)) then
          call chain (nloper, van, F, T, p, rd, x) 
       else
          call chain (  oper, van, F, T, p, rd, x) 
       end if
       rm = p*eps
    else
       p = 0. ; rm = 0.
    end if

    forget = F
    v = F ; if (present (verb)) v = verb
    do iter = 1, niter
       if (present (nmem )) forget = (iter > nmem) ! restart
       if (present (wght) .and. forget) stat = wght (rd, wht)
       if (present (wght) .or. present (wt)) then 
          rd = wht*rd
          call chain (oper, van, T, F, g, wht*rd, x)
       else
          call chain (oper, van, T, F, g, rd, x)
       end if
       g = g + eps*rm
       if (present (known)) where (known) g = 0. 
       call chain (oper, van, F, F, g, gd, x)
       gm = eps*g
       if (present (wght) .or. present (wt))  gd = gd*wht
       if (forget .and. present (nfreq)) &  
            forget = (mod (iter, nfreq) == 0) ! periodic restart

       if (v) then
          dprd = int (dot_product (rd, rd))
          dprm = int (dot_product (rm, rm))   
          dpp  = int (dot_product (p, p))
          dpg  = int (dot_product (g, g))
          write (0,*) "iteration ", iter,   &
          " res dat", dprd,  " res mod", dprm,  &   
          " mod ", dpp, " grad", dpg
       end if

       stat = solv (forget, p, g, rr, gg)

       if (present (nloper)) then
          rd = -dat; call chain (nloper, van, F, T, p, rd, x)
       else if (present (wght) .or. present (wt)) then
          rd = -dat; call chain (  oper, van, F, T, p, rd, x)
       else if (present (xmov) .or. iter == niter) then
          stat = van (F, F, p, x)
       end if
        
       if (present (xmov)) xmov (:,iter) =  x (:size (xmov, 1))
       if (present (rmov)) rmov (:,iter) = rr (:size (rmov, 1))
       if (present (err)) err (iter) = dot_product (rd, rd)
    end do
    if (present (res)) res = rd
    if (present (resm)) resm = rm (:size (resm))
    if (present (wght) .or. present (wt)) deallocate (wht)
  end subroutine solver_van


!!$=head1 NAME
!!$
!!$solver - solve system of equation
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call solver(oper, solv, x, dat, niter, eps,x0,nmem,nfreq,
!!$  err,res,nloper,xmov,rmov,nort,mres,mdiag,wght,verb,known,wt)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item oper   - function
!!$
!!$      operator (standard interface)
!!$
!!$=item solv   - step
!!$
!!$      step mecanism
!!$
!!$
!!$=item dat    - C<real(:)>
!!$
!!$      data
!!$
!!$=item niter  - integer
!!$
!!$      number of iterations
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  x0    -
!!$
!!$       C<real(:)> Initial model
!!$
!!$=item nmem   - integer
!!$
!!$      Change behavior after nmem itterations
!!$      use in conjunction with nfreq and/or wght
!!$
!!$=item nfreq  - integer
!!$
!!$      Restart step function every nfreq itterations
!!$      after nmem steps
!!$
!!$=item nloper - func
!!$
!!$      Non-linear operator to use for forward operator
!!$
!!$=item wght   - func(res,m)
!!$
!!$      Weighting funtion operator
!!$
!!$=item verb   - logical
!!$
!!$      Wheter or not to be verbose
!!$
!!$=item known  - logical(:)
!!$
!!$      Specify (.true.) model values that are knonw
!!$
!!$=item wt     - real(:)
!!$
!!$      Weighting operator to apply to residuals
!!$
!!$=item nort   - integer
!!$
!!$      Number of steps to keep when doing model
!!$      model resolution (mdiag,mres)
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item x      - C<real(:)>
!!$
!!$      model
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item err    - real(niter)
!!$
!!$      Error sum((d-d~)**2)
!!$
!!$=item res    - C<real(:)>
!!$
!!$      Final residual (size of data)
!!$
!!$=item xmov   - C<real(:,niter)>
!1$
!!$      Movie of model values as a function of iter
!!$
!!$=item rmov   - C<real(:,niter)>
!!$
!!$      Movie of data residual
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$Generic  solver.
!!$
!!$=head1 COMMENTS
!!$
!!$All operators must follow convetion
!!$
!!$integer function (logical adj, logical add, C<real(:)> model, C<real(:)>data)
!!$
!!$All solv routines must follow the convention
!!$
!!$C<integer function(logical forget,real(:) x,real(:) g,real(:) rr, real(:) gg)>
!!$
!!$
!!$=head1 SEE ALSO
!!$
!!$L<solver_reg>,L<solver_prec>,L<nonlin_solver>,L<lsqr>
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$


  subroutine solver (oper, solv, x, dat, niter, &
       x0,nmem,nfreq,err,res,nloper,xmov,rmov,nort,mres,mdiag,wght,verb,known,wt,mwt) 
    optional :: &
       x0,nmem,nfreq,err,res,nloper,xmov,rmov,nort,mres,mdiag,wght,verb,known,wt,mwt
    interface
       function wght (res, w) result (stat)
         integer              :: stat
         real, dimension (:)  :: res, w
       end function wght
       function oper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function oper
       function solv (forget, x, g, rr, gg) result (stat)
         integer             :: stat
         logical             :: forget
         real, dimension (:) :: x, g, rr, gg
       end function solv
       function nloper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function nloper
    end interface
    real,    dimension (:),   intent (in)  :: dat, x0, wt, mwt
    real,    dimension (:),   intent (out) :: x, err, res, mdiag
    integer,                  intent (in)  :: niter, nmem, nfreq, nort
    real,    dimension (:,:), intent (out) :: xmov, rmov, mres
    logical,                  intent (in)  :: verb
    logical, dimension (:),   intent (in)  :: known

    real, dimension (:), pointer           :: g, wht
    real, dimension (size (dat))           :: rr, gg
    integer                                :: iter, stat
    logical                                :: forget, v
    type (list)                            :: gs
    integer                                :: dpr, dpx, dpg

    if (present (wt) .or. present (wght)) then
       allocate (wht (size(dat)))
       if (present (wt)) then
          wht = wt
       else
          wht = 1.
       end if
    end if

    rr = -dat
    if (present (x0)) then
       x = x0 
       if (present (nloper)) then
          stat = nloper (F, T, x, rr)
       else
          stat =   oper (F, T, x, rr)
       end if
       if (present (mwt)) then
          where (abs(mwt) > epsilon (mwt)) x = x0 / mwt
       end if
    else
       x = 0.
    end if

    if (present (nort)) gs =llist_init ()

    allocate (g (size (x)))

    v = F ; if (present (verb)) v = verb

    forget = F
    do iter = 1, niter
       if (present (nmem )) forget = (iter > nmem) ! restart
       if (present (wght) .and. forget) stat = wght (rr, wht) 
       if (present (wght) .or. present (wt)) then
          rr = wht*rr
          stat = oper (T, F, g, wht*rr)
       else 
          stat = oper (T, F, g, rr)
       end if
       if (present (mwt)) g = g * mwt
       if (forget .and. present (nfreq)) &
            forget = (mod (iter, nfreq) == 0)      ! periodic restart

       if (present (known)) where (known) g = 0. 
       if (present (nort)) call arnoldi (gs, g, iter <= nort) 

       if (v) then
          dpr = int (dot_product (rr, rr))
          dpx = int (dot_product (x, x))
          dpg = int (dot_product (g, g))        
          write (0,*) "iteration ", iter,    &
          " res", dpr, " mod ", dpx,         &
          " grad", dpg
       end if

       if (present (mwt)) then
          stat = oper (F, F, g * mwt, gg)
       else
          stat = oper (F, F, g, gg)
       end if
       if (present (wght) .or. present (wt)) gg = wht*gg
       stat = solv (forget, x, g, rr, gg)
       
       if (present (nort)) allocate (g (size (x)))
       if (present (nloper)) then
          if (present (mwt)) then
             rr = -dat; stat = nloper (F, T, x * mwt, rr)
          else
             rr = -dat; stat = nloper (F, T, x, rr)
          end if
       else if (present (wght) .or. present (wt)) then
          if (present (mwt)) then
             rr = -dat; stat =   oper (F, T, x * mwt, rr)
          else
             rr = -dat; stat =   oper (F, T, x, rr)
          end if
       end if
       if (present (mwt) .and. (present (xmov) .or. iter == niter)) x = x * mwt
       if (present (xmov)) xmov (:,iter) =  x (1 : size (xmov, 1))
       if (present (rmov)) rmov (:,iter) = rr (1 : size (rmov, 1))
       if (present (err)) err (iter) = dot_product (rr, rr)
    end do

    if (present (mres )) call arnoldi_mat  (gs, mres)
    if (present (mdiag)) call arnoldi_diag (gs, mdiag) 
    if (present (nort )) then
       call llist_close (gs)
    else
       deallocate (g)
    end if
    if (present (res)) res = rr
    if (present (wght) .or. present (wt)) deallocate (wht)
  end subroutine solver
!!$=head1 NAME
!!$
!!$lsqr - solve system of using lsqr method
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call lsqr(oper,x, dat, niter, eps,x0,nmem,nfreq,
!!$  err,res,nloper,xmov,rmov,nort,mres,mdiag,wght,verb,known,wt)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item oper   - function
!!$
!!$      operator (standard interface)
!!$
!!$=item dat    - C<real(:)>
!!$
!!$      data
!!$
!!$=item niter  - integer
!!$
!!$      number of iterations
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  x0    - C<real(:)>
!!$
!!$        Initial model
!!$
!!$=item nmem   - integer
!!$
!!$      Change behavior after nmem itterations
!!$      use in conjunction with nfreq and/or wght
!!$
!!$=item nfreq  - integer
!!$
!!$      Restart step function every nfreq itterations
!!$      after nmem steps
!!$
!!$=item nloper - func
!!$
!!$      Non-linear operator to use for forward operator
!!$
!!$=item wght   - func(res,m)
!!$
!!$      Weighting funtion operator
!!$
!!$=item verb   - logical
!!$
!!$      Wheter or not to be verbose
!!$
!!$=item known  - logical(:)
!!$
!!$      Specify (.true.) model values that are knonw
!!$
!!$=item wt     - real(:)
!!$
!!$      Weighting operator to apply to residuals
!!$
!!$=item nort   - integer
!!$
!!$      Number of steps to keep when doing model
!!$       model resolution (mdiag,mres)
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item x      - C<real(:)>
!!$
!!$      model
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item err    - real(niter)
!!$
!!$      Error sum((d-d~)**2)
!!$
!!$=item res    - C<real(:)>
!!$
!!$      Final residual (size of data)
!!$
!!$=item xmov   - C<real(:,niter)>
!!$
!!$      Movie of model values as a function of iter
!!$
!!$=item rmov   - C<real(:,niter)>
!!$
!!$      Movie of data residual
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Generic LSQR solver.
!!$
!!$=head1 COMMENTS
!!$
!!$All operators must follow convetion
!!$
!!$integer function (logical adj, logical add, C<real(:)> model, C<real(:)>data)
!!$
!!$=head1 SEE ALSO
!!$
!!$L<solver_reg>,L<solver_prec>,L<nonlin_solver>,L<solver>
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut


  subroutine lsqr (oper, x, dat, niter, &
  nloper, nort, mres, mdiag, dres, ddiag) 
    optional :: nloper, nort, mres, mdiag, dres, ddiag 
    interface
       function oper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function oper
       function nloper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function nloper
    end interface
    real, dimension (:), intent (in)    :: dat
    real, dimension (:), intent (out)   :: x, mdiag, ddiag
    integer, intent (in)                :: niter, nort
    real, dimension (:,:), intent (out) :: mres, dres

    real, dimension (size (x))          :: w
    real, dimension (:), pointer        :: u, v
    integer                             :: iter, stat, nu, nv
    double precision                    :: alfa, beta, rhobar, phibar
    double precision                    :: c, s, teta, rho, phi, t1, t2
    type (list)                         :: uu, vv

    nu = size (dat) ; nv = size (x)
    allocate (u (nu), v (nv))

    u = dat; x = 0.        ; call normlz (u, beta)
    stat = oper (T,F,v,u)  ; call normlz (v, alfa)

    if (present (nort)) then
       uu = llist_init ()
       vv = llist_init ()
    end if

    w = v
    rhobar = alfa
    phibar = beta

    do iter = 1, niter
       if (present (nort)) then
          call arnoldi (uu, u, iter <= nort)
          call arnoldi (vv, v, iter <= nort)
       end if

       u = - alfa * u ; stat = oper (F, T, v, u) ; call normlz (u, beta)
       v = - beta * v ; stat = oper (T, T, v, u) ; call normlz (v, alfa)

       rho = sqrt (rhobar*rhobar + beta*beta)
       c = rhobar/rho ; s = beta / rho ; teta = s * alfa
       rhobar = - c * alfa ; phi = c * phibar ; phibar = s * phibar
       t1 = phi /rho ; t2 = -teta/rho

       x = x + t1* w
       w = v + t2* w

       if (present (nort)) allocate (u (nu), v (nv))
    end do

    if (present (ddiag)) call arnoldi_diag (uu, ddiag)
    if (present (mdiag)) call arnoldi_diag (vv, mdiag)
    if (present (dres))  call arnoldi_mat  (uu, dres)
    if (present (mres))  call arnoldi_mat  (vv, mres)

    if (present (nort))  then
       call llist_close (uu)
       call llist_close (vv)
    else
       deallocate (u, v)
    end if
  end subroutine lsqr

  subroutine normlz (vector, size)
    real, dimension (:), intent (inout) :: vector
    double precision,    intent (out)   :: size

    size = sqrt (ddot (vector, vector))
    vector = vector / size
  end subroutine normlz
!!$=head1 NAME
!!$
!!$nonlin_solver - generic non-linear solver program
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call  nonlin_solver(oper,solv,x, dat, niter,x0,err,res,xmov,rmov)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item oper   - function
!!$
!!$      operator (standard interface)
!!$
!!$=item solv   - func
!!$
!!$      Step calculation routine
!!$
!!$=item dat    - C<real(:)>
!!$
!!$      data
!!$
!!$=item niter  - integer
!!$
!!$      number of iterations
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item  x0    - C<real(:)>
!!$
!!$       Initial model
!!$
!!$=back
!!$
!!$=head1 OUTPUT PARAMETERS
!!$
!!$=head2 REQUIRED PARAMETERS
!!$
!!$=over 4
!!$
!!$=item x      - C<real(:)>
!!$
!!$      model
!!$
!!$=back
!!$
!!$=head2 OPTIONAL PARAMETERS
!!$
!!$=over 4
!!$
!!$=item err    - real(niter)
!!$
!!$      Error sum((d-d~)**2)
!!$
!!$=item res    - C<real(:)>
!!$
!!$      Final residual (size of data)
!!$
!!$=item xmov   - C<real(:,niter)>
!!$
!!$      Movie of model values as a function of iter
!!$
!!$=item rmov   - C<real(:,niter)>
!!$
!!$      Movie of data residual
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Generic non-linear solver.
!!$
!!$=head1 COMMENTS
!!$
!!$All operators must follow convetion
!!$
!!$integer function (logical adj, logical add, C<real(:)> model, C<real(:)>data)
!!$
!!$All solv routines must follow the convention
!!$
!!$C<integer function(logical forget,real(:) x,real(:) g) >
!!$
!!$
!!$L<solver_reg>,L<solver_prec>,L<lsqr>,L<solver>
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$=cut
!!$


  subroutine nonlin_solver (oper, solv, x, dat, niter, &
       x0, err, res, xmov, rmov) 
    optional     :: x0, err, res, xmov, rmov
    interface
       function oper (adj, add, x, dat) result (stat)
         integer              :: stat
         logical, intent (in) :: adj, add
         real, dimension (:)  :: x, dat
       end function oper
       function solv (forget, x, g) result (stat)
         integer             :: stat
         logical             :: forget
         real, dimension (:) :: x, g
       end function solv
    end interface
    real, dimension (:), intent (in)    :: dat, x0
    real, dimension (:), intent (out)   :: x, err, res
    integer, intent (in)                :: niter
    real, dimension (:,:), intent (out) :: xmov, rmov   

    real, dimension (size (x))   :: g
    real, dimension (size (dat)) :: rr
    integer                      :: iter, stat
    logical                      :: forget 
    double precision             :: beta 

    rr = -dat
    if (present (x0)) then
       x = x0; stat = oper (F, T, x, rr)
    else
       x = 0. 
    end if

    forget = T
    do iter = 1, niter
       if (forget) then
          stat = oper (T, F, g, rr)            ! g = F' R 
          stat = oper (F, F, g, rr)            ! G = F  g
          beta = ddot (g, g) / ddot (rr, rr)   ! b = |g| / |G|
          x = x + beta * g                     ! x =   x + b g
          rr = -dat; stat = oper (F, T, x, rr) ! R = F x - D 
       end if

       stat = oper (T, F, g, rr)               ! g = F' R
       stat = solv (forget, x, g)
       rr = -dat; stat = oper (F, T, x, rr)    ! R = F x - D

       if (present (xmov)) xmov (:,iter) =  x (1 : size (xmov, 1))
       if (present (rmov)) rmov (:,iter) = rr (1 : size (rmov, 1))
       if (present (err)) err (iter) = dot_product (rr, rr)
    end do

    if (present (res)) then
       res = -dat; stat = oper (F, T, x, res)
    end if
  end subroutine nonlin_solver

end module solver_mod











