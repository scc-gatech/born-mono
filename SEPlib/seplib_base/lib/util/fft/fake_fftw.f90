module fftw_sep_fake_mod 
  use sep_mod
  implicit none
  integer, parameter, private :: dbl=selected_real_kind(10,50)
  real,pointer,private ::  jj(:)
  character(len=10) :: started
  interface fftwnd_f77_one 
    module procedure fft_1d_cc_internal
    module procedure fft_2d_cc_internal
    module procedure fft_3d_cc_internal
    module procedure fft_4d_cc_internal
    module procedure fft_5d_cc_internal
    module procedure fft_2d_cc_two
    module procedure fft_1_3d_cc_two
  end interface 
  interface rfftw_f77_one 
    module procedure rtoc_rc_1d
    module procedure ctor_cr_1d
    module procedure rc_rr_1d
  end interface 
  interface sfftw_plan_dft_r2c
   module procedure sfftw_plan_dft_r2c1
   module procedure sfftw_plan_dft_r2c2
  end interface
  interface sfftw_plan_dft_c2r
   module procedure sfftw_plan_dft_c2r1
   module procedure sfftw_plan_dft_c2r2
  end interface

!interface  sfftw_plan_dft{
!module procedure plan1
!}
  contains 
  subroutine plan1(plan,ndim,n,in,out,p1,p2)
    integer :: ndim,n(:)
    complex :: in,out
    integer :: p1,p2
    integer(kind=dbl) ::  plan
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine rfftw_f77_create_plan(plan,n,sign,junk)
    integer(kind=dbl) :: plan
    integer           :: n,junk,sign,k
    call erexit("you must  configure with fftw to run this program ")
!k=n*.5
!if(k*2!=n) {
!write(0,*) "axis length",n
!call erexit("complex-real axis transform must be divisiable by 2")
!}
!call check_prime(plan,1,(/k/),sign,junk)
!if(started(1:4)=="good"){
!  if(size(jj)< n+1) {
!    deallocate(jj)
!    allocate(jj(n+1))
!  }
!}
!else{
!        allocate(jj(n+1))
!  started="good"
!}
  end subroutine 
  subroutine sfftw_plan_dft_r2c2(plan,ndim,dims,r1,c1,f2)
    integer*8 :: plan
    integer :: f1,f2,dims,ndim
    complex :: c1,c2
    real :: r1
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine sfftw_plan_dft_r2c1(plan,ndim,dims,c1,r1,f1,f2)
    integer(kind=dbl) :: plan
    integer :: f1,f2,dims(:),ndim
    complex :: c1,c2
    real :: r1
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine sfftw_plan_dft_c2r1(plan,ndim,dims,c1,r1,f1,f2)
    integer(kind=dbl) :: plan
    integer :: f1,f2,dims,ndim
    complex :: c1
    real :: r1
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine sfftw_plan_dft_c2r2(plan,ndim,dims,c1,r1,f1,f2)
    integer(kind=dbl) :: plan
    integer :: f1,f2,dims(:),ndim
    complex :: c1
    real :: r1
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine sfftw_plan_dft(plan,ndim,dims,c1,c2,f1,f2)
    integer(kind=dbl) :: plan
    integer :: f1,f2,dims(:),ndim
    complex :: c1,c2
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine sfftw_destroy_plan(plan)
    integer(kind=dbl) :: plan
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine sfftw_execute(plan)
    integer(kind=dbl) :: plan
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine fftw2d_f77_create_plan(plan,n1,n2,sign,junk)
    integer(kind=dbl) :: plan
    integer           :: n1,n2,junk,sign
    call erexit("you must  configure with fftw to run this program ")
!call check_prime(plan,2,(/n1,n2/),sign,junk)
  end subroutine 
  subroutine fftwnd_f77_create_plan(plan,ndim,n,sign,junk)
    integer(kind=dbl) :: plan
    integer           :: ndim,n(:),junk,sign
    call erexit("you must  configure with fftw to run this program ")
!call check_prime(plan,ndim,n,sign,junk)
  end subroutine 
  subroutine fftw_f77_create_plan(plan,n,sign,junk)
    integer(kind=dbl) :: plan
    integer           :: n,junk,sign
    call erexit("you must  configure with fftw to run this program ")
!call check_prime(plan,1,(/n/),sign,junk)
  end subroutine 
  subroutine rtoc_rc_1d(flag_in,ba,carray)
    integer(kind=dbl) :: flag_in
    real :: ba(:)
    complex :: carray(:)
    integer :: sign
    sign=flag_iN
    call erexit("you must  configure with fftw to run this program ")
!call pfarc(sign,size(ba,1),ba,carray)
  end subroutine 
  subroutine rc_rr_1d(flag_in,ba,carray)
    integer(kind=dbl) :: flag_in
    real :: ba(:)
    real :: carray(:)
    integer :: sign
    integer :: i,k
    call erexit("you must  configure with fftw to run this program ")
!sign=flag_iN
!!REAL TO COMPLEX
!if(sign==-1) {
!  call pfarc(sign,size(ba,1),ba,jj)
!  k=1
!  do i=1,size(ba,1)/2{
!   carray(i)=jj(k)
!   k+=2
!  }
!  carray(size(ba,1)/2+1)=jj(size(ba,1)+1)
!  k=size(ba)
!  do i=size(ba,1)/2+2,size(ba){
!   carray(i)=jj(k)
!   k-=2
!  }
!}
!else{
!  k=1
!  do i=1,size(ba,1)/2{
!   jj(k)=ba(i)
!   k+=2
!  }
!  jj(size(ba)+1)=ba(size(ba,1)/2+1)
!  k=size(ba)
!  do i=size(ba,1)/2+2,size(ba){
!   jj(k)=ba(i)
!   k-=2
!  }
!  jj(2)=0.
!  call pfacr(sign,size(ba,1),jj,carray)
!}
  end subroutine 
  subroutine ctor_cr_1d(flag_in,car,rar)
    integer(kind=dbl) :: flag_in
    integer :: sign
    complex :: car(:)
    real :: rar(:)
    sign=flag_iN
    call erexit("you must  configure with fftw to run this program ")
!call pfacr(sign,size(rar,1),car,rar)
  end subroutine 
  subroutine fftw_f77_one(flag_in,input,output)
    integer(kind=dbl) :: flag_in
    integer :: sign,junk
    complex :: input(:)
    complex :: output(:)
    sign=flag_iN
    output=input
    call erexit("you must  configure with fftw to run this program ")
!call pfamcc(sign,size(input,1),1, 1,size(input,1),output)
  end subroutine 
  subroutine check_prime(plan,ndim,n,sign,junk)
    integer(kind=dbl) :: plan
    integer           :: ndim,n(:),junk,sign
    integer :: i,j
    call erexit("you must  configure with fftw to run this program ")
!plan=sign
!   do i=1,ndim{
!     j=npfao(n(i),n(i))
!     if(j != n(i)) {
!       write(0,"(a,i2,a,i4,a,i4)") "axis ",i," not a prime factor len=",
!         n(i)," try padding to ",j
!       call erexit("invalid axis length")
!      }
!   }
  end subroutine 
  subroutine fft_1d_cc_internal(flag_in,array,junk)
    integer(kind=dbl) :: flag_in
    integer :: sign,junk
    complex :: array(:)
    sign=flag_in
    call erexit("you must  configure with fftw to run this program ")
!call pfamcc(sign,size(array,1),1, 1,size(array,1),array)
  end subroutine 
  subroutine fft_1_3d_cc_two(flag_in,array,array2)
    integer(kind=dbl) :: flag_in
    integer :: sign
    complex :: array(:),array2(:,:,:,:)
    call erexit("you must  configure with fftw to run this program ")
!sign=flag_in
!array2=reshape(array,(/size(array2,1),size(array2,2),size(array2,3),size(array2,4)/))
!call fft_4d_cc_internal(flag_in,array2,4)
  end subroutine 
  subroutine fft_2d_cc_two(flag_in,array,array2)
    integer(kind=dbl) :: flag_in
    integer :: sign
    complex :: array(:,:),array2(:,:)
    call erexit("you must  configure with fftw to run this program ")
!sign=flag_in
!array2=array
!call pfamcc(sign,size(array,1),size(array,2), 1,size(array,1),array2)
!call pfamcc(sign,size(array,2),size(array,1),size(array,1),1,array2)
  end subroutine 
  subroutine fft_2d_cc_internal(flag_in,array,junk)
    integer(kind=dbl) :: flag_in
    integer :: sign,junk
    complex :: array(:,:)
    call erexit("you must  configure with fftw to run this program ")
!sign=flag_in
!call pfamcc(sign,size(array,1),size(array,2), 1,size(array,1),array)
!call pfamcc(sign,size(array,2),size(array,1),size(array,1),1,array)
  end subroutine 
  subroutine fft_3d_cc_internal(flag_in,array,junk)
    integer(kind=dbl) :: flag_in
    integer :: sign,junk,i3
    complex :: array(:,:,:)
    sign=flag_in
!Do First axis tranfrom
    call erexit("you must  configure with fftw to run this program ")
!call pfamcc(sign,size(array,1),size(array,2)*size(array,3),
!   1,size(array,1),array)
!
!!Second axis transform
!do i3=1,size(array,3)
!  call pfamcc(sign,size(array,2),size(array,1), size(array,1),1,array(:,:,i3))
!
!!Third axis transform
!  call pfamcc(sign,size(array,3),size(array,1)*size(array,2),
!      size(array,1)*size(array,2),1,array)
  end subroutine 
  subroutine fft_4d_cc_internal(flag_in,array,junk)
    integer(kind=dbl) :: flag_in
    integer :: sign,junk,i3,i4
    complex :: array(:,:,:,:)
    sign=flag_in
!Do First axis tranfrom
    call erexit("you must  configure with fftw to run this program ")
!call pfamcc(sign,size(array,1),size(array,2)*size(array,3)*size(array,4),
!   1,size(array,1),array)
!
!!Second axis transform
!do i4=1,size(array,4)
!  do i3=1,size(array,3)
!    call pfamcc(sign,size(array,2),size(array,1), size(array,1),1,
!       array(:,:,i3,i4))
!
!!Third axis transform
!do i4=1,size(array,4)
!    call pfamcc(sign,size(array,3),size(array,1)*size(array,2),
!      size(array,1)*size(array,2),1,array(:,:,:,i4))
!
!!Fourth axis transform
!call pfamcc(sign,size(array,4),size(array,3)*size(array,1)*size(array,2),
!    size(array,1)*size(array,2)*size(array,3),1,array)
!
  end subroutine 
  subroutine fft_5d_cc_internal(flag_in,array,junk)
    integer(kind=dbl) :: flag_in
    integer :: sign,junk,i3,i4,i5
    complex :: array(:,:,:,:,:)
    sign=flag_in
!Do First axis tranfrom
    call erexit("you must  configure with fftw to run this program ")
!call pfamcc(sign,size(array,1),size(array,2)*size(array,3)*size(array,4)*
!   size(array,5), 1,size(array,1),array)
!
!!Second axis transform
!do i5=1,size(array,5)
! do i4=1,size(array,4)
!  do i3=1,size(array,3)
!    call pfamcc(sign,size(array,2),size(array,1), size(array,1),1,
!       array(:,:,i3,i4,i5))
!
!!Third axis transform
!do i5=1,size(array,5)
! do i4=1,size(array,4)
!    call pfamcc(sign,size(array,3),size(array,1)*size(array,2),
!      size(array,1)*size(array,2),1,array(:,:,:,i4,i5))
!
!!Fourth axis transform
!do i5=1,size(array,5)
!    call pfamcc(sign,size(array,4),size(array,1)*size(array,2)*size(array,3),
!      size(array,1)*size(array,2)*size(array,3),1,array(:,:,:,:,i5))
!
!!Fifth axis transform
!call pfamcc(sign,size(array,5),
!    size(array,4)*size(array,3)*size(array,1)*size(array,2),
!    size(array,1)*size(array,2)*size(array,3)*size(array,4),1,array)
!
  end subroutine 
  subroutine rfftw_f77_destroy_plan(flag_in)
    integer(kind=dbl) :: flag_in
!if(associated(jj)) deallocate(jj)
    call erexit("you must  configure with fftw to run this program ")
!nullify(jj)
  end subroutine 
  subroutine fftw_f77_destroy_plan(flag_in)
    integer(kind=dbl) :: flag_in
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine fftwnd_f77_destroy_plan(flag_in)
    integer(kind=dbl) :: flag_in
    call erexit("you must  configure with fftw to run this program ")
  end subroutine 
  subroutine sfftw_plan_r2r_3d(plan,n1,n2,n3,ar1,ar2,f1,f2,f3,f4)
    integer(kind=dbl) :: plan
    integer :: n1,n2 ,f1,f2,f3,f4,n3
    real :: ar1(:,:,:)
    real :: ar2(:,:,:)
    call erexit("you must  configure with fftw to run this program ")
  end subroutine

  subroutine sfftw_plan_r2r_2d(plan,n1,n2,ar1,ar2,f1,f2,f3,f4)
    integer(kind=dbl) :: plan
    integer :: n1,n2 ,f1,f2,f3,f4
    real :: ar1(:,:)
    real :: ar2(:,:)
    call erexit("you must  configure with fftw to run this program ")
  end subroutine

end module 
