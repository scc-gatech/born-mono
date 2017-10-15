!!$=head1 NAME
!!$
!!$ddot - calculate double precission dot product
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<product=ddot(arr1,arr2)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item arr1,arr2 - C<real(:)>  
!!$
!!$      arrays to calcuate dot products of
!!$
!!$=back   
!!$
!!$=head1 RETURN VALUE
!!$
!!$=over 4
!!$
!!$=item product  - double precission 
!!$
!!$      result of dot product
!!$
!!$=back
!!$
!!$=head1  DESCRIPTION 
!!$
!!$Calculate a dout product of two real vectors.
!!$
!!$=head1 LIBRARY
!!$
!!$B<geef90>
!!$
!!$
!!$=cut
!!$

module ddot_mod
  implicit none
contains
  function ddot (arr1, arr2) result (product)
    double precision 			:: product
    real, dimension(:), intent(in) 	:: arr1, arr2
    product = sum(dprod(arr1, arr2))
  end function ddot
end module ddot_mod


