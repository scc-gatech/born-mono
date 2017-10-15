!APPLY A COSINE TAPER
module cos_taper_mod 
  use sep_mod
  implicit none
  real,pointer :: cos_taper_filter(:)
  contains
  subroutine init_cos_taper(n1,o1,d1,min0,min1,max0,max1)
    integer :: n1
    real    :: o1,d1,min0,min1,max0,max1
    integer :: i1_x,i0_x,i1_n,i0_n,i1
    real    :: pi2
    allocate(cos_taper_filter(n1))
    pi2=atan(1.)*2. !pi/2.
    i0_n = min(n1,max(1,nint((min0-o1)/d1+1.)))
    i1_n = min(n1,max(1,nint((min1-o1)/d1+1.)))
    i0_x = min(n1,max(1,nint((max0-o1)/d1+1.)))
    i1_x = min(n1,max(1,nint((max1-o1)/d1+1.)))
    if ( i0_n<1 .or. i0_n>n1 ) then
      call erexit("invalid min0 to init_cos_taper")
    end if
    if ( i1_n<1 .or. i1_n>n1 ) then
      call erexit("invalid min1 to init_cos_taper")
    end if
    if ( i0_x<1 .or. i0_x>n1 ) then
      call erexit("invalid max0 to init_cos_taper")
    end if
    if ( i1_x<1 .or. i1_x>n1 ) then
      call erexit("invalid max1 to init_cos_taper")
    end if
    if ( i0_n > i1_n) then
      call erexit("min0 > min1")
    end if
    if ( i0_x < i1_x) then
      call erexit("max0 < max1")
    end if
    cos_taper_filter(1:i0_n)=0.
    cos_taper_filter(i0_x:)=0.
    cos_taper_filter(i1_n:i1_x)=1.
    do i1=i0_n,i1_n 
      cos_taper_filter(i1)=(1.-cos((real(i1-i0_n)/real(i1_n-i0_n))*pi2&
        &))
    end do 
    do i1=i1_x,i0_x 
      cos_taper_filter(i1)=(1.-cos(pi2-(real(i1-i1_x)/real(i0_x-i1_x))&
        &*pi2))
    end do
  end subroutine 
  integer function cos_taper_op(adj,add,model,data)
    logical, intent(in) :: adj,add
    complex :: model(:),data(:)
    if (.not. add) then
      if (adj) then
        model=0.
      else
        data=0.
      end if
    end if
    if (.not. adj) then
      data= data+model*cos_taper_filter
    else
      model=  model+data*cos_taper_filter
    end if 
    cos_taper_op=0
  end function 
  subroutine cos_taper_clean()
    deallocate(cos_taper_filter)
  end subroutine 
end module 
