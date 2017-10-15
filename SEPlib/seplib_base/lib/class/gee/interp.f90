module interp

  implicit none

contains

  function bin_int (x, w) result (stat)
    integer             :: stat
    real, intent (in)   :: x
    real, dimension (:) :: w
    w = 0.; w (1) = 1. ; stat = 0 
  end function bin_int

  function bin2_int (x, w) result (stat)
    integer                           :: stat
    real, dimension (2), intent (in)  :: x
    real, dimension (:,:)             :: w
    w = 0.; w (1,1) = 1. ; stat = 0 
  end function bin2_int

  function lin_int (x, w) result (stat)
    integer             :: stat
    real, intent (in)   :: x
    real, dimension (:) :: w
    w = 1.
    if (size (w) == 2) then
       w (2) = x 
       w (1) = 1. - x 
    end if
    stat = 0
  end function lin_int

  function lin2_int (x, w) result (stat)
    integer                          :: stat
    real, dimension (2), intent (in) :: x
    real, dimension (:,:)            :: w

    w = 1.
    if (size (w,1) == 2) then
       w (2,:) = x (1)
       w (1,:) = 1. - x (1) 
    end if
    if (size (w,2) == 2) then
       w (:,2) = w (:,2) * x (2)
       w (:,1) = w (:,1) * (1. - x (2)) 
    end if
    stat = 0
  end function lin2_int

end module interp
















