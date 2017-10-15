!!$=head1 NAME
!!$
!!$weight - simple weighting operator
!!$
!!$=head1 SYNOPSIS
!!$ 
!!$C<call weight_init(w)>
!!$      
!!$C<ierr=weight_lop(adj,add,model,data)>
!!$    
!!$=head1 PARAMETERS
!!$      
!!$=over 4
!!$    
!!$=item w  - C<real(:)>  
!!$      
!!$      Weight to apply 
!!$
!!$=item adj,add,model,data -
!!$ 
!!$      Standard operator interfaces 
!!$ 
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$ 
!!$Simple weighting operator.  Model and data must be the same size.
!!$
!!$=head1 LIBRARY
!!$ 
!!$B<geef90>
!!$ 
!!$=cut

module weight

  real, dimension (:), pointer, private :: w

contains

  subroutine weight_init (wt)
    real, dimension(:), pointer :: wt
    w => wt
  end subroutine weight_init

  function weight_lop (adj, add, x, y) result (stat)
    integer              :: stat
    logical, intent (in) :: adj, add
    real, dimension (:)  :: x, y

    if (adj) then
       if (add) then
          x = x + w * y
       else
          x =     w * y
       end if
    else
       if (add) then
          y = y + w * x
       else
          y =     w * x
       end if
    end if

    stat = 0
  end function weight_lop

end module weight
