module wei_attr
  !! attribute subroutines

  !---------------
  implicit none

  interface weiattr
     module procedure weiattrR1
     module procedure weiattrR2
     module procedure weiattrR3
     module procedure weiattrR4
     module procedure weiattrR5
     module procedure weiattrR6

     module procedure weiattrC1
     module procedure weiattrC2
     module procedure weiattrC3
     module procedure weiattrC4
     module procedure weiattrC5
     module procedure weiattrC6

  end interface
contains
  !----------------------------------------------------------------  
  subroutine weiattrR1(array)
    real, dimension(:), pointer :: array
    real    :: meanv,maxv,minv
    integer :: ntot, nnoz
    integer :: n1
    integer :: i1

    n1 = size(array,1)

    ntot=n1

    nnoz=0
    do i1=1,n1
       if(array(i1) /=0) nnoz=nnoz+1
    end do

    meanv=   sum(array)/ntot
    maxv =maxval(array)
    minv =minval(array)

    write(0,*)'             ===================================================='
    write(0,'(a,i3)       ')       "             |     n: ",n1
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6)    ')       "             |  mean: ",meanv
    write(0,'(a,f15.6)    ')       "             |   max: ",maxv
    write(0,'(a,f15.6)    ')       "             |   min: ",minv
    write(0,*)'             ===================================================='

  end subroutine weiattrR1
  !----------------------------------------------------------------
  subroutine weiattrR2(array)
    real, dimension(:,:), pointer :: array
    real    :: meanv,maxv,minv
    integer :: ntot, nnoz
    integer :: n1,n2
    integer :: i1,i2

    n1 = size(array,1)
    n2 = size(array,2)

    ntot=n1*n2

    nnoz=0
    do i2=1,n2
       do i1=1,n1
          if(array(i1,i2) /=0) nnoz=nnoz+1
       end do
    end do

    meanv=   sum(array)/ntot
    maxv =maxval(array)
    minv =minval(array)

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3)    ')       "             |     n: ",n1,n2
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6)    ')       "             |  mean: ",meanv
    write(0,'(a,f15.6)    ')       "             |   max: ",maxv
    write(0,'(a,f15.6)    ')       "             |   min: ",minv
    write(0,*)'             ===================================================='
  end subroutine weiattrR2
  !----------------------------------------------------------------
  subroutine weiattrR3(array)
    real, dimension(:,:,:), pointer :: array
    real    :: meanv,maxv,minv
    integer :: ntot, nnoz
    integer :: n1,n2,n3
    integer :: i1,i2,i3

    n1 = size(array,1)
    n2 = size(array,2)
    n3 = size(array,3)

    ntot=n1*n2*n3

    nnoz=0
    do i3=1,n3
       do i2=1,n2
          do i1=1,n1
             if(array(i1,i2,i3) /=0) nnoz=nnoz+1
          end do
       end do
    end do

    meanv=   sum(array)/ntot
    maxv =maxval(array)
    minv =minval(array)

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3,i3) ')       "             |     n: ",n1,n2,n3
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6)    ')       "             |  mean: ",meanv
    write(0,'(a,f15.6)    ')       "             |   max: ",maxv
    write(0,'(a,f15.6)    ')       "             |   min: ",minv
    write(0,*)'             ===================================================='
  end subroutine weiattrR3
  !----------------------------------------------------------------  
  subroutine weiattrR4(array)
    real, dimension(:,:,:,:), pointer :: array
    real    :: meanv,maxv,minv
    integer :: ntot, nnoz
    integer :: n1,n2,n3,n4
    integer :: i1,i2,i3,i4

    n1 = size(array,1)
    n2 = size(array,2)
    n3 = size(array,3)
    n4 = size(array,4)

    ntot=n1*n2*n3*n4

    nnoz=0
    do i4=1,n4
       do i3=1,n3
          do i2=1,n2
             do i1=1,n1
                if(array(i1,i2,i3,i4) /=0) nnoz=nnoz+1
             end do
          end do
       end do
    end do

    meanv=   sum(array)/ntot
    maxv =maxval(array)
    minv =minval(array)

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3,i3,i3) ')    "             |     n: ",n1,n2,n3,n4
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6)    ')       "             |  mean: ",meanv
    write(0,'(a,f15.6)    ')       "             |   max: ",maxv
    write(0,'(a,f15.6)    ')       "             |   min: ",minv
    write(0,*)'             ===================================================='
  end subroutine weiattrR4
  !----------------------------------------------------------------
  subroutine weiattrR5(array)
    real, dimension(:,:,:,:,:), pointer :: array
    real    :: meanv,maxv,minv
    integer :: ntot, nnoz
    integer :: n1,n2,n3,n4,n5
    integer :: i1,i2,i3,i4,i5

    n1 = size(array,1)
    n2 = size(array,2)
    n3 = size(array,3)
    n4 = size(array,4)
    n5 = size(array,5)

    ntot=n1*n2*n3*n4*n5

    nnoz=0
    do i5=1,n5
       do i4=1,n4
          do i3=1,n3
             do i2=1,n2
                do i1=1,n1
                   if(array(i1,i2,i3,i4,i5) /=0) nnoz=nnoz+1
                end do
             end do
          end do
       end do
    end do

    meanv=   sum(array)/ntot
    maxv =maxval(array)
    minv =minval(array)

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3,i3,i3,i3) ') "             |     n: ",n1,n2,n3,n4,n5
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6)    ')       "             |  mean: ",meanv
    write(0,'(a,f15.6)    ')       "             |   max: ",maxv
    write(0,'(a,f15.6)    ')       "             |   min: ",minv
    write(0,*)'             ===================================================='
  end subroutine weiattrR5
  !----------------------------------------------------------------
  subroutine weiattrR6(array)
    real, dimension(:,:,:,:,:,:), pointer :: array
    real    :: meanv,maxv,minv
    integer :: ntot, nnoz
    integer :: n1,n2,n3,n4,n5,n6
    integer :: i1,i2,i3,i4,i5,i6

    n1 = size(array,1)
    n2 = size(array,2)
    n3 = size(array,3)
    n4 = size(array,4)
    n5 = size(array,5)
    n5 = size(array,6)

    ntot=n1*n2*n3*n4*n5*n6

    nnoz=0
    do i6=1,n6
       do i5=1,n5
          do i4=1,n4
             do i3=1,n3
                do i2=1,n2
                   do i1=1,n1
                      if(array(i1,i2,i3,i4,i5,i6) /=0) nnoz=nnoz+1
                   end do
                end do
             end do
          end do
       end do
    end do
    meanv=   sum(array)/ntot
    maxv =maxval(array)
    minv =minval(array)

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3,i3,i3,i3,i3) ') "             |     n: ",n1,n2,n3,n4,n5,n6
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6)    ')       "             |  mean: ",meanv
    write(0,'(a,f15.6)    ')       "             |   max: ",maxv
    write(0,'(a,f15.6)    ')       "             |   min: ",minv
    write(0,*)'             ===================================================='
  end subroutine weiattrR6
  !----------------------------------------------------------------

  subroutine weiattrC1(array)
    complex, dimension(:), pointer :: array
    real    :: rmeanv,rmaxv,rminv
    real    :: imeanv,imaxv,iminv
    integer :: ntot, nnoz
    integer :: n1
    integer :: i1

    n1 = size(array,1)

    ntot=n1

    nnoz=0
    do i1=1,n1
       if(real(array(i1)) /=0 .or. aimag(array(i1)) /=0) nnoz=nnoz+1
    end do

    rmeanv=   sum(real(array))/ntot
    rmaxv =maxval(real(array))
    rminv =minval(real(array))

    imeanv=   sum(aimag(array))/ntot
    imaxv =maxval(aimag(array))
    iminv =minval(aimag(array))

    write(0,*)'             ===================================================='
    write(0,'(a,i3)       ')       "             |     n: ",n1
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6,f15.6)    ') "             |  mean: ",rmeanv, imeanv
    write(0,'(a,f15.6,f15.6)    ') "             |   max: ",rmaxv,  imaxv
    write(0,'(a,f15.6,f15.6)    ') "             |   min: ",rminv,  iminv
    write(0,*)'             ===================================================='

  end subroutine weiattrC1
  !----------------------------------------------------------------
  subroutine weiattrC2(array)
    complex, dimension(:,:), pointer :: array
    real    :: rmeanv,rmaxv,rminv
    real    :: imeanv,imaxv,iminv
    integer :: ntot, nnoz
    integer :: n1,n2
    integer :: i1,i2

    n1 = size(array,1)
    n2 = size(array,2)

    ntot=n1*n2

    nnoz=0
    do i2=1,n2
       do i1=1,n1
          if(real(array(i1,i2)) /=0 .or. aimag(array(i1,i2)) /=0) nnoz=nnoz+1
       end do
    end do

    rmeanv=   sum(real(array))/ntot
    rmaxv =maxval(real(array))
    rminv =minval(real(array))

    imeanv=   sum(aimag(array))/ntot
    imaxv =maxval(aimag(array))
    iminv =minval(aimag(array))

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3)    ')       "             |     n: ",n1,n2
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6,f15.6)    ') "             |  mean: ",rmeanv, imeanv
    write(0,'(a,f15.6,f15.6)    ') "             |   max: ",rmaxv,  imaxv
    write(0,'(a,f15.6,f15.6)    ') "             |   min: ",rminv,  iminv
    write(0,*)'             ===================================================='
  end subroutine weiattrC2
  !----------------------------------------------------------------
  subroutine weiattrC3(array)
    complex, dimension(:,:,:), pointer :: array
    real    :: rmeanv,rmaxv,rminv
    real    :: imeanv,imaxv,iminv
    integer :: ntot, nnoz
    integer :: n1,n2,n3
    integer :: i1,i2,i3

    n1 = size(array,1)
    n2 = size(array,2)
    n3 = size(array,3)

    ntot=n1*n2*n3

    nnoz=0
    do i3=1,n3
       do i2=1,n2
          do i1=1,n1
             if(real(array(i1,i2,i3)) /=0 .or. aimag(array(i1,i2,i3)) /=0) nnoz=nnoz+1
          end do
       end do
    end do

    rmeanv=   sum(real(array))/ntot
    rmaxv =maxval(real(array))
    rminv =minval(real(array))

    imeanv=   sum(aimag(array))/ntot
    imaxv =maxval(aimag(array))
    iminv =minval(aimag(array))

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3,i3) ')       "             |     n: ",n1,n2,n3
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6,f15.6)    ') "             |  mean: ",rmeanv, imeanv
    write(0,'(a,f15.6,f15.6)    ') "             |   max: ",rmaxv,  imaxv
    write(0,'(a,f15.6,f15.6)    ') "             |   min: ",rminv,  iminv
    write(0,*)'             ===================================================='
  end subroutine weiattrC3
  !----------------------------------------------------------------
  subroutine weiattrC4(array)
    complex, dimension(:,:,:,:), pointer :: array
    real    :: rmeanv,rmaxv,rminv
    real    :: imeanv,imaxv,iminv
    integer :: ntot, nnoz
    integer :: n1,n2,n3,n4
    integer :: i1,i2,i3,i4

    n1 = size(array,1)
    n2 = size(array,2)
    n3 = size(array,3)
    n4 = size(array,4)

    ntot=n1*n2*n3*n4

    nnoz=0
    do i4=1,n4
       do i3=1,n3
          do i2=1,n2
             do i1=1,n1
                if(real(array(i1,i2,i3,i4)) /=0 .or. aimag(array(i1,i2,i3,i4)) /=0) nnoz=nnoz+1
             end do
          end do
       end do
    end do

    rmeanv=   sum(real(array))/ntot
    rmaxv =maxval(real(array))
    rminv =minval(real(array))

    imeanv=   sum(aimag(array))/ntot
    imaxv =maxval(aimag(array))
    iminv =minval(aimag(array))

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3,i3,i3) ')    "             |     n: ",n1,n2,n3,n4
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6,f15.6)    ') "             |  mean: ",rmeanv, imeanv
    write(0,'(a,f15.6,f15.6)    ') "             |   max: ",rmaxv,  imaxv
    write(0,'(a,f15.6,f15.6)    ') "             |   min: ",rminv,  iminv
    write(0,*)'             ===================================================='
  end subroutine weiattrC4
  !----------------------------------------------------------------
  subroutine weiattrC5(array)
    complex, dimension(:,:,:,:,:), pointer :: array
    real    :: rmeanv,rmaxv,rminv
    real    :: imeanv,imaxv,iminv
    integer :: ntot, nnoz
    integer :: n1,n2,n3,n4,n5
    integer :: i1,i2,i3,i4,i5

    n1 = size(array,1)
    n2 = size(array,2)
    n3 = size(array,3)
    n4 = size(array,4)
    n5 = size(array,5)

    ntot=n1*n2*n3*n4*n5

    nnoz=0
    do i5=1,n5
       do i4=1,n4
          do i3=1,n3
             do i2=1,n2
                do i1=1,n1
                   if(real(array(i1,i2,i3,i4,i5)) /=0 .or. aimag(array(i1,i2,i3,i4,i5)) /=0) nnoz=nnoz+1
                end do
             end do
          end do
       end do
    end do

    rmeanv=   sum(real(array))/ntot
    rmaxv =maxval(real(array))
    rminv =minval(real(array))

    imeanv=   sum(aimag(array))/ntot
    imaxv =maxval(aimag(array))
    iminv =minval(aimag(array))

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3,i3,i3,i3) ') "             |     n: ",n1,n2,n3,n4,n5
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6,f15.6)    ') "             |  mean: ",rmeanv, imeanv
    write(0,'(a,f15.6,f15.6)    ') "             |   max: ",rmaxv,  imaxv
    write(0,'(a,f15.6,f15.6)    ') "             |   min: ",rminv,  iminv
    write(0,*)'             ===================================================='
  end subroutine weiattrC5
  !----------------------------------------------------------------
  subroutine weiattrC6(array)
    complex, dimension(:,:,:,:,:,:), pointer :: array
    real    :: rmeanv,rmaxv,rminv
    real    :: imeanv,imaxv,iminv
    integer :: ntot, nnoz
    integer :: n1,n2,n3,n4,n5,n6
    integer :: i1,i2,i3,i4,i5,i6

    n1 = size(array,1)
    n2 = size(array,2)
    n3 = size(array,3)
    n4 = size(array,4)
    n5 = size(array,5)
    n6 = size(array,6)

    ntot=n1*n2*n3*n4*n5*n6

    nnoz=0
    do i6=1,n6
       do i5=1,n5
          do i4=1,n4
             do i3=1,n3
                do i2=1,n2
                   do i1=1,n1
                      if(real(array(i1,i2,i3,i4,i5,i6)) /=0 .or. aimag(array(i1,i2,i3,i4,i5,i6)) /=0) nnoz=nnoz+1
                   end do
                end do
             end do
          end do
       end do
    end do

    rmeanv=   sum(real(array))/ntot
    rmaxv =maxval(real(array))
    rminv =minval(real(array))

    imeanv=   sum(aimag(array))/ntot
    imaxv =maxval(aimag(array))
    iminv =minval(aimag(array))

    write(0,*)'             ===================================================='
    write(0,'(a,i3,i3,i3,i3,i3,i3) ') "             |     n: ",n1,n2,n3,n4,n5,n6
    write(0,'(a,i9)       ')       "             |  nnoz: ",nnoz
    write(0,'(a,f15.6,f15.6)    ') "             |  mean: ",rmeanv, imeanv
    write(0,'(a,f15.6,f15.6)    ') "             |   max: ",rmaxv,  imaxv
    write(0,'(a,f15.6,f15.6)    ') "             |   min: ",rminv,  iminv
    write(0,*)'             ===================================================='
  end subroutine weiattrC6
  !----------------------------------------------------------------
end module wei_attr










