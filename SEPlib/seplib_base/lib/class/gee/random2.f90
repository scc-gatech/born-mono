module random_mod

  implicit none
  integer, private   :: seed = 1996

  interface random
     module procedure rand_int
     module procedure rand_real
     module procedure rand_real_array_1d
     module procedure rand_real_array_2d
     module procedure rand_real_array_3d
  end interface

contains

  subroutine random_init (iseed)
    integer, intent (in) :: iseed
    seed = iseed
  end subroutine random_init

  subroutine rand_int (rand, lo, hi)
    integer, intent (out) :: rand
    integer, intent (in)  :: lo, hi
    
    real                  :: r

    call rand_real (r)
    rand = lo + int (r*(hi-lo+1))
  end subroutine rand_int

  subroutine rand_real (rand, amp, pow)
    real,    intent (out)           :: rand
    real,    intent (in), optional  :: amp
    integer, intent (in), optional  :: pow

    integer, parameter              :: ia = 727, im = 524287

    seed = mod (seed*ia,im)
    rand = (float(seed) - 0.5)/float(im - 1)

    if (present (amp)) then
       rand = (rand - 0.5) * amp
    else 
       if (present (pow)) rand = (rand - 0.5) ** pow
    end if
  end subroutine rand_real

  subroutine rand_real_array_1d (rand, amp, pow)
    real, dimension (:), intent (out)           :: rand
    real,                intent (in), optional  :: amp
    integer,             intent (in), optional  :: pow

    integer                                     :: i

    do i = 1, size (rand)
       if (present (amp)) then
          call rand_real (rand (i), amp)
       else if (present (pow)) then
          call rand_real (rand (i), pow=pow)
       else
          call rand_real (rand (i))
       end if
    end do
  end subroutine rand_real_array_1d

  subroutine rand_real_array_2d (rand, amp, pow)
    real, dimension (:,:), intent (out)           :: rand
    real,                  intent (in), optional  :: amp
    integer,               intent (in), optional  :: pow

    integer                                       :: i1, i2

    do i2 = 1, size (rand, 2)
       do i1 = 1, size (rand, 1)
          if (present (amp)) then
             call rand_real (rand (i1, i2), amp)
          else if (present (pow)) then
             call rand_real (rand (i1, i2), pow=pow)
          else
             call rand_real (rand (i1, i2))
          end if
       end do
    end do
  end subroutine rand_real_array_2d

  subroutine rand_real_array_3d (rand, amp, pow)
    real, dimension (:,:,:), intent (out)           :: rand
    real,                    intent (in), optional  :: amp
    integer,                 intent (in), optional  :: pow

    integer                                         :: i1, i2, i3

    do i3 = 1, size (rand, 3)
       do i2 = 1, size (rand, 2)
          do i1 = 1, size (rand, 1)
             if (present (amp)) then
                call rand_real (rand (i1, i2, i3), amp)
             else if (present (pow)) then
                call rand_real (rand (i1, i2, i3), pow=pow)
             else
                call rand_real (rand (i1, i2, i3))
             end if
          end do
       end do
    end do
  end subroutine rand_real_array_3d

end module random_mod
