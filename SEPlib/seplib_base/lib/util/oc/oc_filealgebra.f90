module oc_filealgebra_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use sep
  use oc_global_mod
  use oc_file_mod
  implicit none

  interface oc_assign
     module procedure R_oc_assign
     module procedure C_oc_assign
  end interface
  interface oc_linear
     module procedure R_oc_linear
     module procedure C_oc_linear
  end interface
  interface oc_random
     module procedure R_oc_random
     module procedure C_oc_random
  end interface

contains
  !----------------------------------------------------------------
  real(kind=db) function oc_rdp(t1_,t2_,maxmem) result(dp)
    !! computes the dot product of the files t1_ and t2_
    character(len=128), dimension(:), intent(in)  :: t1_,t2_
    integer,                          optional    :: maxmem
    integer                                       :: i,j,k,n, stat
    type(fileinfo)                                :: file1,  file2
    real,               dimension(:), allocatable :: rblov1, rblov2

    dp=rZERO
    n=size(t1_)

    do k=1,n
       call oc_allocatefile(file1, t1_(k), maxmem); call sseek(t1_(k),0,0)
       call oc_allocatefile(file2, t2_(k), maxmem); call sseek(t2_(k),0,0)
       call oc_checksimilarity(file1, file2, "oc_rdp")

       allocate(rblov1(file1%bloe),rblov2(file2%bloe))
       do i=1,file1%blon
          call sreed(t1_(k), rblov1, file1%blob)
          if (t1_(k).eq.t2_(k)) then
             stat=sseek_block(t1_(k),i-1,file1%blob,0)
          end if
          call sreed(t2_(k), rblov2, file2%blob)

          do j=1,file1%bloe
             dp = dp + rblov1(j) * rblov2(j)
          end do
       end do
       deallocate(rblov1,rblov2)

       call oc_deallocatefile( file1); call sseek(t1_(k),0,0)
       call oc_deallocatefile( file2); call sseek(t2_(k),0,0)
    end do
  end function oc_rdp
  !----------------------------------------------------------------
  complex(kind=db) function oc_cdp(t1_,t2_,maxmem) result(dp)
    !! computes the dot product of the files t1_ and t2_
    character(len=128), dimension(:), intent(in)  :: t1_,t2_
    integer,                          optional    :: maxmem
    integer                                       :: i,j,k,n, stat
    type(fileinfo)                                :: file1,  file2
    complex,            dimension(:), allocatable :: cblov1, cblov2

    dp=cZERO
    n=size(t1_)

    do k=1,n
       call oc_allocatefile(file1, t1_(k), maxmem); call sseek(t1_(k),0,0)
       call oc_allocatefile(file2, t2_(k), maxmem); call sseek(t2_(k),0,0)
       call oc_checksimilarity(file1, file2, "oc_cdp")

       allocate(cblov1(file1%bloe),cblov2(file2%bloe))
       do i=1,file1%blon
          call sreed(t1_(k), cblov1, file1%blob)
          if (t1_(k).eq.t2_(k)) then
             stat=sseek_block(t1_(k),i-1,file1%blob,0)
          end if
          call sreed(t2_(k), cblov2, file2%blob)

          do j=1,file1%bloe
             dp = dp + conjg(cblov1(j)) * cblov2(j)
          end do
       end do
       deallocate(cblov1,cblov2)

       call oc_deallocatefile( file1); call sseek(t1_(k),0,0)
       call oc_deallocatefile( file2); call sseek(t2_(k),0,0)
    end do
          write(0,*)'dp=',dp
  end function oc_cdp
  !----------------------------------------------------------------
  subroutine R_oc_assign(t_, sca, maxmem)
    !! assigns the scalar sca to the file t_
    character(len=128), intent(in)     :: t_
    integer,            optional       :: maxmem
    real,               intent(in)     :: sca
    integer                            :: i
    type(fileinfo)                     :: file
    real,    dimension(:), allocatable :: rblov
    complex, dimension(:), allocatable :: cblov

    call oc_allocatefile(file, t_, maxmem); call sseek(t_,0,0)

    if (file%esize /=8) then !! REAL FILE
       allocate(rblov(file%bloe))
       do i=1,file%blon
          rblov=sca
          call sep_write(rblov,t_)
       end do
       deallocate(rblov)
    else                     !! COMPLEX FILE
       allocate(cblov(file%bloe))
       do i=1,file%blon
          cblov=(1.,0.)*sca
          call sep_write(cblov,t_)
       end do
       deallocate(cblov)
    end if

    call oc_deallocatefile(file); call sseek(t_,0,0)
  end subroutine R_oc_assign
  !----------------------------------------------------------------
  subroutine C_oc_assign(t_, sca, maxmem)
    !! assigns the scalar sca to the file t_
    character(len=128), intent(in)     :: t_
    integer,            optional       :: maxmem
    complex,            intent(in)     :: sca
    integer                            :: i
    type(fileinfo)                     :: file
    complex, dimension(:), allocatable :: cblov

    call oc_allocatefile(file, t_, maxmem); call sseek(t_,0,0)

    allocate(cblov(file%bloe))
    do i=1,file%blon
       cblov=sca
       call sep_write(cblov,t_)
    end do
    deallocate(cblov)

    call oc_deallocatefile(file); call sseek(t_,0,0)
  end subroutine C_oc_assign
  !----------------------------------------------------------------
  subroutine R_oc_linear(t0_, ti_, scai, maxmem)
    !! linear operations
    !! writes in t0_ the sum of the products (scai*ti_)
    character(len=128),               intent(in)  :: t0_
    character(len=128), dimension(:), intent(in)  :: ti_
    real(kind=db),      dimension(:), intent(in)  :: scai
    integer,                          optional    :: maxmem
    type(fileinfo)                                :: file0, filei
    integer                                       :: ni,i, j, k, stat
    logical                                       :: do_sk
    real,               dimension(:), allocatable :: rblov0, rblovi
    complex,            dimension(:), allocatable :: cblov0, cblovi

    if (size(ti_) /= size(scai)) then 
       call seperr('impossible linear combination')
    else
       ni=size(ti_)
    end if

    call oc_allocatefile(file0, t0_, maxmem); call sseek(t0_,0,0)
    do i=1,ni
       call oc_allocatefile(filei, ti_(i), maxmem); call sseek(ti_(i),0,0)
       call oc_checksimilarity(file0, filei, "oc_linear_real")
       call oc_deallocatefile(filei)
    end do
    call oc_allocatefile(filei, ti_(1), maxmem)

    if(file0%esize /=8) then
       allocate(rblov0(file0%bloe),rblovi(filei%bloe))
    else
       allocate(cblov0(file0%bloe),cblovi(filei%bloe))
    end if

    blocks: do j=1,file0%blon
       if (file0%esize /=8) then
          rblov0=rZERO
       else
          cblov0=cZERO
       end if
       !! READ
       files: do i=1,ni
          do_sk=F
          do k=1,i-1
             if(ti_(k).eq.ti_(i)) do_sk=T
          end do !! was file i read before? seek back.
          if(do_sk) stat=sseek_block(ti_(i),j-1,filei%blob,0)

          if (file0%esize /=8) then
             call sreed(ti_(i), rblovi, filei%blob)
             rblov0 = rblov0 + scai(i)*rblovi
          else
             call sreed(ti_(i), cblovi, filei%blob)
             cblov0 = cblov0 + scai(i)*cblovi
          end if
       end do files

       !! WRITE
       do_sk=F
       do i=1,ni
          if(ti_(i).eq.t0_) do_sk=T
       end do    !! was file 0 read before? seek back.
       if (do_sk) stat=sseek_block(t0_,j-1,file0%blob,0)

       if(file0%esize /=8) then
          call sep_write(rblov0, t0_)
       else
          call sep_write(cblov0, t0_)
       end if
    end do blocks

    if(file0%esize /=8) then
       deallocate(rblov0, rblovi)
    else
       deallocate(cblov0, cblovi)
    end if

    call oc_deallocatefile(file0); call sseek(t0_,0,0)
    call oc_deallocatefile(filei)
    do i=1,ni
       call sseek(ti_(i),0,0)
    end do
  end subroutine R_oc_linear
  !----------------------------------------------------------------
  subroutine C_oc_linear(t0_, ti_, scai, maxmem)
    !! linear operations
    !! writes in t0_ the sum of the products (scai*ti_)
    character(len=128),               intent(in)  :: t0_
    character(len=128), dimension(:), intent(in)  :: ti_
    complex(kind=db),   dimension(:), intent(in)  :: scai
    integer,                          optional    :: maxmem
    type(fileinfo)                                :: file0, filei
    integer                                       :: ni, i, j, k, stat
    logical                                       :: do_sk
    complex,            dimension(:), allocatable :: cblov0, cblovi

    if (size(ti_) /= size(scai)) then 
       call seperr('impossible linear combination')
    else
       ni=size(ti_)
    end if

    call oc_allocatefile(file0, t0_, maxmem); call sseek(t0_,0,0)
    do i=1,ni
       call oc_allocatefile(filei, ti_(i), maxmem); call sseek(ti_(i),0,0)
       call oc_checksimilarity(file0, filei, "oc_linear_complex")
       call oc_deallocatefile(filei)
    end do
    call oc_allocatefile(filei, ti_(1), maxmem)
    allocate(cblov0(file0%bloe),cblovi(filei%bloe))

    blocks: do j=1,file0%blon
       cblov0=cZERO
       !! READ
       files: do i=1,ni
          do_sk=F
          do k=1,i-1
             if(ti_(k).eq.ti_(i)) do_sk=T
          end do !! was file i read before? seek back.
          if(do_sk) stat=sseek_block(ti_(i),j-1,filei%blob,0)

          call sreed(ti_(i), cblovi, filei%blob)
          cblov0 = cblov0 + scai(i)*cblovi
       end do files

       !! WRITE
       do_sk=F
       do i=1,ni
          if(ti_(i).eq.t0_) do_sk=T
       end do    !! was file 0 read before? seek back.
       if (do_sk) stat=sseek_block(t0_,j-1,file0%blob,0)
       call sep_write(cblov0, t0_)
    end do blocks

    deallocate(cblov0, cblovi)
    call oc_deallocatefile(file0); call sseek(t0_,0,0)
    call oc_deallocatefile(filei)
    do i=1,ni
       call sseek(ti_(i),0,0)
    end do
  end subroutine C_oc_linear
  !----------------------------------------------------------------
  subroutine R_oc_random(t_, scale, maxmem)
    character(len=128),    intent(in)  :: t_
    real,                  intent(in)  :: scale
    integer,               optional    :: maxmem

    type(fileinfo)                     :: file
    real,    dimension(:), allocatable :: rblov
    complex, dimension(:), allocatable :: cblov

    integer, dimension(:), allocatable :: seed,old
    integer                            :: i,k
    character(len=20)                  :: date, time, zone
    integer, dimension(8)              :: values
    integer                            :: s1,s2

    call oc_allocatefile(file, t_, maxmem); call sseek(t_, 0,0)

    if(file%esize /=8) then
       allocate(rblov(file%bloe))
    else
       allocate(rblov(file%bloe))
       allocate(cblov(file%bloe))
    end if

    !!init the random number generator
    call random_seed(size=k)
    allocate(seed(k),old(k))
    call date_and_time(date,time,zone,values)

    s1=product(values(5:6)); s1=min(s1,31328)
    s2=product(values(5:7)); s2=min(s2,30081)
    seed(1:2) = (/s1, s2/)
    call random_seed(put = seed(1:k))
    !!---------------------------------------

    do i=1,file%blon
       call random_number(rblov); rblov=2*scale*(rblov-.5)
       if(file%esize /=8) then
          call sep_write(rblov,t_)
       else
          cblov=            rblov
          call random_number(rblov); rblov=2*scale*(rblov-.5)
          cblov=cblov+(0,1)*rblov
          call sep_write(cblov,t_)
       end if
    end do

    if(file%esize /=8) then
       deallocate(rblov)
    else
       deallocate(rblov,cblov)
    end if
    call oc_deallocatefile(file); call sseek(t_,0,0)
  end subroutine R_oc_random
  !----------------------------------------------------------------
  subroutine C_oc_random(t_, scale, maxmem)
    character(len=128),    intent(in)  :: t_
    complex,               intent(in)  :: scale
    integer,               optional    :: maxmem
    type(fileinfo)                     :: file
    real,    dimension(:), allocatable :: rblov,iblov
    complex, dimension(:), allocatable :: cblov

    integer, dimension(:), allocatable :: seed,old
    integer                            :: i,k
    character(len=20)                  :: date, time, zone
    integer, dimension(8)              :: values
    integer                            :: s1,s2

    call oc_allocatefile(file, t_, maxmem); call sseek(t_, 0,0)
    allocate(rblov(file%bloe),iblov(file%bloe),cblov(file%bloe))

    !!init the random number generator
    call random_seed(size=k)
    allocate(seed(k),old(k))
    call date_and_time(date,time,zone,values)

    s1=product(values(5:6)); s1=min(s1,31328)
    s2=product(values(5:7)); s2=min(s2,30081)
    seed(1:2) = (/s1, s2/)
    call random_seed(put = seed(1:k))
    !!---------------------------------------

    do i=1,file%blon
       call random_number(rblov); rblov=2*(rblov-.5)
       call random_number(iblov); iblov=2*(iblov-.5)
       cblov= scale*(rblov + (0,1)*iblov)
       call sep_write(cblov,t_)
    end do

    deallocate(rblov,iblov,cblov)
    call oc_deallocatefile(file); call sseek(t_,0,0)
  end subroutine C_oc_random
  !----------------------------------------------------------------
  subroutine oc_complexify(t_, maxmem)
    character(len=128),   intent(in)   :: t_
    integer,              optional     :: maxmem
    integer                            :: i
    character(len=128)                 :: k_, name
    type(fileinfo)                     :: file
    complex, allocatable, dimension(:) :: cblov
    real,    allocatable, dimension(:) :: rblov

    call oc_allocatefile(file, t_, maxmem); call sseek(t_,0,0)

    if (file%esize /= 8) then
       !! generate temporary scratch file
       name = '_tmp'; k_ = oc_clone(T, t_,name,maxmem)

       call auxputch("esize","i",2*file%esize,k_)
       allocate (cblov(file%bloe), rblov(file%bloe))
       do i=1,file%blon
          call sreed(t_, rblov, file%blob)
          cblov=(1,0)*rblov
          call sep_write(cblov, k_)
       end do
       deallocate (rblov, cblov)
       call oc_deallocatefile(file); call sseek(t_,0,0)

       !! copy scratch to original
       call sseek(k_,0,0); call oc_allocatefile(file, k_, maxmem)
       call auxputch("esize","i",file%esize,t_)
       allocate (cblov(file%bloe))
       do i=1,file%blon
          call sreed(k_, cblov, file%blob)
          call sep_write(cblov, t_)
       end do
       deallocate (cblov)
       call oc_deallocatefile(file); call sseek(t_,0,0)
    end if
  end subroutine oc_complexify
  !----------------------------------------------------------------
  subroutine oc_mask(k_,t_,maxmem)
    character(len=128),    intent(in)  :: k_, t_
    integer,               optional    :: maxmem
    integer                            :: i,j
    type(fileinfo)                     :: filek, filet
    real,    dimension(:), allocatable :: rblov
    complex, dimension(:), allocatable :: cblov
    real,    dimension(:), allocatable :: kblov
    integer                            :: stat

    call oc_allocatefile(filek, k_, maxmem); call sseek(k_,0,0)
    call oc_allocatefile(filet, t_, maxmem); call sseek(t_,0,0)

    allocate(kblov(filek%bloe))
    if (filet%esize /=8) then !! REAL
       allocate(rblov(filet%bloe))
       do j=1,filet%blon
          call sreed(t_, rblov, filet%blob)
          call sreed(k_, kblov, filek%blob)
          do i=1,filek%bloe
             if (kblov(j) /=0.) rblov(j)=rZERO
          end do

          stat=sseek_block(t_,j-1,filet%blob,0)
          call sep_write(rblov, t_)
       end do
       deallocate(rblov)
    else                      !! COMPLEX
       allocate(cblov(filet%bloe))
       do j=1,filet%blon
          call sreed(t_, cblov, filet%blob)
          call sreed(k_, kblov, filek%blob)
          do i=1,filek%bloe
             if (kblov(j) /=0.) cblov(j)=cZERO
          end do

          stat=sseek_block(t_,j-1,filet%blob,0)
          call sep_write(cblov, t_)
       end do
       deallocate(cblov)
    end if
    deallocate(kblov)

    call oc_deallocatefile(filek); call sseek(k_,0,0)
    call oc_deallocatefile(filet); call sseek(t_,0,0)
  end subroutine oc_mask
  !----------------------------------------------------------------
  subroutine oc_product(t0_, ti_, maxmem)
    !! writes in t0_ the product of (ti_)s
    character(len=128),               intent(in)  :: t0_
    character(len=128), dimension(:), intent(in)  :: ti_
    integer,                          optional    :: maxmem
    type(fileinfo)                                :: file0, filei
    integer                                       :: ni
    complex,            dimension(:), allocatable :: cblov0, cblovi
    real,               dimension(:), allocatable :: rblov0, rblovi
    integer                                       :: i, j, k
    logical                                       :: do_sk
    integer                                       :: stat

    ni=size(ti_)

    call oc_allocatefile(file0, t0_, maxmem); call sseek(t0_,0,0)
    do i=1,ni
       call sseek(ti_(i),0,0)
       call oc_allocatefile(filei, ti_(i), maxmem)
       if (file0%esize .eq.4 .and. filei%esize .eq.8) then
          call seperr('cant write complex in real')
       end if
       call oc_checkspace(file0, filei,"oc_product")
       call oc_deallocatefile(filei)
    end do
    call oc_allocatefile(filei, ti_(1), maxmem)

    if (file0%esize /=8) then !! left is REAL: R = R R R R R...
       allocate(rblov0(file0%bloe), rblovi(filei%bloe))
       rblocks: do j=1,file0%blon
          rblov0=rONE
          !! READ
          rfiles: do i=1,ni
             do_sk=F
             do k=1,i-1
                if(ti_(k).eq.ti_(i)) do_sk=T
             end do !! was file i read before? seek back.
             if(do_sk) stat=sseek_block(ti_(i),j-1,filei%blob,0)

             call sreed(ti_(i), rblovi, filei%blob)
             rblov0 = rblov0 * rblovi
          end do rfiles

          !! WRITE
          do_sk=F
          do i=1,ni
             if(ti_(i).eq.t0_) do_sk=T
          end do    !! was file 0 read before? seek back.
          if (do_sk) stat=sseek_block(t0_,j-1,file0%blob,0)
          call sep_write(rblov0, t0_)
       end do rblocks
       deallocate(rblov0,rblovi)
    else                      !! left is COMPLEX: C = C R C C R...
       allocate(cblov0(file0%bloe))
       cblocks: do j=1,file0%blon
          cblov0=cONE
          !! READ
          cfiles: do i=1,ni
             do_sk=F
             do k=1,i-1
                if(ti_(k).eq.ti_(i)) do_sk=T
             end do !! was file i read before? seek back.
             if(do_sk) stat=sseek_block(ti_(i),j-1,filei%blob,0)

             call oc_allocatefile(filei, ti_(i), maxmem)
             if(filei%esize /=8) then
                allocate(rblovi(filei%bloe))
                call sreed(ti_(i), rblovi, filei%blob)
                cblov0 = cblov0 * rblovi
                deallocate(rblovi)
             else
                allocate(cblovi(filei%bloe))
                call sreed(ti_(i), cblovi, filei%blob)
                cblov0 = cblov0 * cblovi
                deallocate(cblovi)
             end if
             call oc_deallocatefile(filei)
          end do cfiles

          !! WRITE
          do_sk=F
          do i=1,ni
             if(ti_(i).eq.t0_) do_sk=T
          end do    !! was file 0 read before? seek back.
          if (do_sk) stat=sseek_block(t0_,j-1,file0%blob,0)
          call sep_write(cblov0, t0_)
       end do cblocks
       deallocate(cblov0)
    end if
    call oc_deallocatefile(file0); call sseek(t0_,0,0)
  end subroutine oc_product
  !----------------------------------------------------------------
  real(kind=db) function oc_norm(t_,maxmem) result(norm)
    character(len=128), dimension(:), intent(in) :: t_
    integer,                          optional   :: maxmem
    type(fileinfo)                               :: file

    call oc_allocatefile(file, t_(1), maxmem); call sseek(t_(1),0,0)
    
    if (file%esize /=8) then
       norm=sqrt(    oc_rdp(t_,t_, maxmem) )
    else
       norm=sqrt(abs(oc_cdp(t_,t_, maxmem)))
    end if

    call oc_deallocatefile(file)
  end function oc_norm
  !----------------------------------------------------------------
  real function oc_magnitude(t_,maxmem) result(magnitude)
    character(len=128), dimension(:), intent(in)  :: t_
    integer,                          optional    :: maxmem
    type(fileinfo)                                :: file
    integer                                       :: n, k, i
    real,               dimension(:), allocatable :: rblov
    real                                          :: maxloc

    n = size(t_)

    call oc_allocatefile(file, t_(1), maxmem); call sseek(t_(1),0,0)
    allocate(rblov(file%bloe))
    call oc_deallocatefile(file)

    magnitude=rZERO
    do k=1,n
       call oc_allocatefile(file, t_(k), maxmem); call sseek(t_(k),0,0)

       do i=1,file%blon
          call sreed(t_(k), rblov, file%blob)
          
          maxloc = maxval(abs(rblov))
          if(maxloc > magnitude) magnitude = maxloc
       end do

       call oc_deallocatefile(file); call sseek(t_(k),0,0)
    end do

    deallocate(rblov)
  end function oc_magnitude
  !----------------------------------------------------------------
  subroutine oc_normalize(t_, magnitude, maxmem)
    character(len=128), dimension(:), intent(in)  :: t_
    real(kind=db),                    intent(out) :: magnitude
    integer,                          optional    :: maxmem
    integer                                       :: esize, n,k
    type(fileinfo)                                :: file

    call oc_allocatefile(file, t_(1), maxmem); call sseek(t_(1),0,0)
    n=size(t_)

    magnitude = oc_norm(t_,maxmem)

    do k=1,n
       call oc_linear(t_(k),(/t_(k)/),(/1/magnitude/),maxmem)  !! t = t / ||t||
    end do

    call oc_deallocatefile(file)
  end subroutine oc_normalize
  !----------------------------------------------------------------
end module oc_filealgebra_mod






