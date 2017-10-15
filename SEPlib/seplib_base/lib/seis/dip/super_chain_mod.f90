module super_chain_mod
  use chain0_mod
  use igrad2_2d_reg
  use igrad3_epst
  use model_mask
  integer, save          :: ndata
  real, save,pointer, dimension(:)       :: temp1,temp2,temp3,wt
contains
  subroutine wt_init(wt_in)
    real ::wt_in(:)
    ndata = ndat_in				
    !   allocate( temp1(ndata))
    allocate( wt(size(wt_in)))
wt=wt_in
write(0,*) 'max/min wt',maxval(wt),minval(wt)
  end subroutine wt_init
 subroutine wt_close()
   deallocate( wt)
 end subroutine wt_close

  subroutine flat_chain_init( ndat_in)
    integer ndat_in
    ndata = ndat_in				
 !   allocate( temp1(ndata))
  end subroutine flat_chain_init
  integer function flat_chain_exec( adj, add, model, data) ! W Grad K
    integer       ::stat
    logical  ,intent(in)     :: adj, add
    real          :: data(:), model(:)
if (adj) then
data=data*wt
   call chain20( igrad2_2d_reg_lop,model_mask_lop,adj, add, model, data,temp2) ! using temp2 instead of temp1 to save on memory....possibly dangerous
else
   call chain20( igrad2_2d_reg_lop,model_mask_lop,adj, add, model, data,temp2) ! using temp2 instead of temp1 to save on memory....possibly dangerous
data=data*wt
end if
    flat_chain_exec=0
  end function flat_chain_exec
  subroutine flat_chain_close()
    deallocate(temp1)
  end subroutine flat_chain_close

  subroutine flat_chain_adj_init( ndat_in)
    integer ndat_in
    ndata = ndat_in				
    !  allocate( temp3(ndata)) !using temp1 instead of temp3 in hopes of saving ram
  end subroutine flat_chain_adj_init
  integer function flat_chain_adj_exec( adj, add, model, data) ! K' Grad' W'
    integer       ::stat
    logical,intent(in)       :: adj, add
    logical :: adj_new
    real          :: data(:), model(:)

    adj_new=.true.
    if (adj) adj_new=.false.

if(adj_new) then   ! forward operator 
model=model*wt
    call chain20( igrad2_2d_reg_lop,model_mask_lop,adj_new, add, data,model,temp2) !using temp1 instead of temp3 in hopes of saving ram ! using temp2 instead of temp1 to save on memory....possibly dangerous
else
    call chain20( igrad2_2d_reg_lop,model_mask_lop,adj_new, add, data,model,temp2) !using temp1 instead of temp3 in hopes of saving ram ! using temp2 instead of temp1 to save on memory....possibly dangerous
model=model*wt
end if

    flat_chain_adj_exec=0
  end function flat_chain_adj_exec
  subroutine flat_chain_adj_close()
    deallocate(temp3)
  end subroutine flat_chain_adj_close

  subroutine super_chain_init( ndat_in)
    integer ndat_in
    ndata = ndat_in				
    allocate( temp2(ndata))
  end subroutine super_chain_init
  integer function super_chain_exec( adj, add, model, data)
    integer       ::stat
    logical       :: adj, add
    real          :: data(:), model(:)
    call chain20( flat_chain_adj_exec,flat_chain_exec,adj, add, model, data,temp2)
    super_chain_exec=0
  end function super_chain_exec
  subroutine super_chain_close()
    deallocate(temp2)
  end subroutine super_chain_close



  subroutine super3_chain_init( ndat_in)
    integer ndat_in
    ndata = ndat_in				
    allocate( temp2(ndata))
  end subroutine super3_chain_init
  integer function super3_chain_exec( adj, add, model, data)
    integer       ::stat
    logical       :: adj, add
    real          :: data(:), model(:)
adj=.false. ! self adjoint, adjoint=forward
    call chain20( flat3_chain_adj_exec,flat3_chain_exec,adj, add, model, data,temp2)
    super3_chain_exec=0
  end function super3_chain_exec
  subroutine super3_chain_close()
    deallocate(temp2)
  end subroutine super3_chain_close


  subroutine flat3_chain_init( ndat_in)
    integer ndat_in
    ndata = ndat_in				
    allocate( temp1(ndata))
  end subroutine flat3_chain_init
  integer function flat3_chain_exec( adj, add, model, data)
    integer       ::stat
    logical ,intent(in)      :: adj, add
    real          :: data(:), model(:)
if (adj) then
data=data*wt
    call chain20( igrad3_epst_lop,model_mask_lop,adj, add, model, data,temp1)
else
   call chain20( igrad3_epst_lop,model_mask_lop,adj, add, model, data,temp1)
data=data*wt
end if

    flat3_chain_exec=0
  end function flat3_chain_exec

  subroutine flat3_chain_close()
    deallocate(temp1)
  end subroutine flat3_chain_close


  subroutine flat3_chain_adj_init( ndat_in)
    integer ndat_in
    ndata = ndat_in				
    allocate( temp3(ndata))
  end subroutine flat3_chain_adj_init

  integer function flat3_chain_adj_exec( adj, add, model, data)

    integer       ::stat
    logical,intent(in)       :: adj, add
    logical       :: adj_new
    real          :: data(:), model(:)
    adj_new=.true.

    if (adj) adj_new=.false.
    if(adj_new) then   ! forward operator 
       model=model*wt
       call chain20( igrad3_epst_lop,model_mask_lop,adj_new, add, data,model,temp3)
    else
       call chain20( igrad3_epst_lop,model_mask_lop,adj_new, add, data,model,temp3)
       model=model*wt
    end if
    flat3_chain_adj_exec=0
  end function flat3_chain_adj_exec

  subroutine flat3_chain_adj_close()
    deallocate(temp3)
  end subroutine flat3_chain_adj_close



end module super_chain_mod


