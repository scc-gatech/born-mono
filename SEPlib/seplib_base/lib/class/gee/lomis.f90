module lomis2  
  use mask1 
  use  helicon 
  use  polydiv 
  use  cgstep_mod 
  use  solver_mod 
  use  patch 
  use  mkwallwt
implicit none
  contains
! fill in missing data by minimizing power out of a given filter.
! by helix magic works in any number of dimensions
  subroutine lomis1( niter, xx,yy,aa, mask, doprec, npatch, nwall, nwind&
    &)
    logical,                      intent( in)  :: doprec
    integer,                      intent( in)  :: niter
    type( filter), dimension (:), intent( in)  :: aa
    real,          dimension( :), intent( in)  :: mask, xx
    real,          dimension( :), intent( out) :: yy        
    ! fitting variables
    real,          dimension( :), allocatable  :: dd, wall
    integer,       dimension( :), pointer      :: npatch, nwall, nwind
    real,          dimension( :), pointer      :: windata, winmask
    logical,       dimension( :), pointer      :: kk
    integer                                    :: nw, stat1, stat2, stat3&
      &, ip
    nw = product( nwind)
    allocate (windata (nw), winmask (nw), kk (nw))
    if ( doprec) then
!  preconditioned
      call mask1_init (kk)
    else
      allocate( dd( nw))
      dd = 0.
    end if 
    call patch_init( npatch, nwall, nwind)
    yy = 0.
    do ip = 1, product( npatch)
      stat1 = patch_lop (.false.,.false., xx,   windata)
      stat2 = patch_lop (.false.,.false., mask, winmask)
      kk = (winmask /= 0.)        
      if (count (kk) < nw .and. count (kk) > 0) then
        if (doprec) then
          call polydiv_init( nw, aa (ip))
          call solver_prec( mask1_lop, cgstep, niter= niter, x= windata&
            &,dat= windata,prec= polydiv_lop, nprec= nw, eps= 0.)
        else
!  regularized
          call helicon_init( aa (ip))
          call solver( helicon_lop, cgstep, niter= niter, x= windata&
            &,dat= dd, known = kk, x0= windata)
        end if 
        call cgstep_close( )
      end if
      stat3 = patch_lop (.true.,.true., yy, windata)
      call patch_close ()
!        call snap ("winout.H",nwind(1), nwind(2), windata)
!        call snap ("winmask.H",nwind(1), nwind(2), winmask)
    end do 
    allocate (wall (size (yy)))
    windata = 1.
    call wallwtn(npatch, nwall, nwind, windata, wall)
    yy = yy*wall
    if (doprec) then
      call polydiv_close()
    else
      deallocate (dd)
    end if 
    deallocate (windata, winmask, kk, wall)
  end subroutine 
end module 
