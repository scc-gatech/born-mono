module model_mask
! apply mask to model
  use classutil
  implicit none
  real ,private, dimension(:), pointer :: mask_in
  contains
  subroutine model_mask_init ( mask_in_in )
    real , dimension(:), pointer :: mask_in_in
    mask_in => mask_in_in
  end subroutine
  function model_mask_lop ( adj, add, model, masked_model) result(stat&
    &)
    integer :: stat
    logical,intent(in) :: adj,add
    real,dimension(:) :: model,masked_model
    call adjnull (adj,add,model,masked_model )
    call model_mask_lop2(adj,add,model,masked_model )
    stat=0
  end function
  subroutine model_mask_lop2(adj,add,model,masked_model)
    logical,intent(in) :: adj,add
    real, dimension (:) :: model
    real, dimension (:) :: masked_model
    if ( adj) then
      model= model+masked_model*mask_in
    else
      masked_model= masked_model+model*mask_in
    end if
  end subroutine
  subroutine model_mask_close()
  end subroutine
end module
