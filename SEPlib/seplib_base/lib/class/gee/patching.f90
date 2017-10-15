module patching  
! Apply existing operator in all patches
  use patch
  use mkwallwt
  implicit none
  contains
  subroutine patchingn( oper, modl, data, npatch, nwall, nwind, windwt&
    &)
    interface  
      integer function oper(adj, add, modl, data)
        logical, intent (in) :: adj, add 
        real, dimension( :)  :: modl, data
      end function
    end interface 
    real,    dimension( :), intent( in)         :: windwt, modl
    real,    dimension( :), intent( out) :: data
    integer, dimension( :), pointer         :: npatch, nwall, nwind
    real,    dimension( size( modl))         :: wallwt
    real,    dimension( size( windwt))   :: winmodl, windata
    integer                                 :: i, stat1, stat2
    data = 0.
    call patch_init( npatch, nwall, nwind)
    do i = 1, product( npatch)
      stat1 = patch_lop( .false., .false., modl, winmodl)
      stat2 = oper(      .false., .false., winmodl, windata)
      stat1 = patch_lop( .true. , .true. , data,  windwt * windata)
      call patch_close ()
    end do 
    call wallwtn( npatch, nwall, nwind, windwt, wallwt)
    data = data * wallwt
  end subroutine 
end module 
