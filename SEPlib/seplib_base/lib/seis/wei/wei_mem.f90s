module wei_mem_mod
use sep
use wei_verb_mod
use sep3d_struct_mod
use wei_par_mod
use wei_run_mod
implicit none
integer, private, save :: mem_usage
logical, private, save :: verb_mem_usage,verb,my_reuse
integer, private, save :: ntot_fk,ntot_fx,ntot_wc,ntot_im
integer, private, save :: ntot_nomp,ntot_nzb,ntot_nwb
integer, private, save :: nfreq_save,nthread_save
logical, private, save :: nzb_avail(6),nwb_avail(6),nomp_avail(6)
integer, private, save :: fk_use(3),fx_use(3),wc_use(3),im_use(3)
integer, private, save :: wa_use(3),rs_use(3)
complex, allocatable, target :: wei_mem_nomp_1(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nomp_2(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nomp_3(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nomp_4(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nomp_5(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nomp_6(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nzb_1(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nwb_1(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nzb_2(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nwb_2(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nzb_3(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nwb_3(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nzb_4(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nwb_4(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nzb_5(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nwb_5(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nzb_6(:,:,:,:,:)
complex, allocatable, target :: wei_mem_nwb_6(:,:,:,:,:)

contains

  logical function init_mem_usage()

    call from_param('mem_monitor', verb_mem_usage, .false.)
    mem_usage=0
    ntot_wc=0
    ntot_im=0
    ntot_fk=0
    ntot_fx=0
    ntot_nzb=0
    ntot_nwb=0
    ntot_nomp=0
    fk_use=-1
    fx_use=-1
    wc_use=-1
    im_use=-1
    wa_use=-1
    rs_use=-1
    init_mem_usage=.true.
  end function

  subroutine set_fk_space(n)
   integer :: n
     ntot_fk=n
  end subroutine

  subroutine set_wc_space(n)
   integer :: n
     ntot_wc=n
  end subroutine


  subroutine set_fx_space(n)
   integer :: n
     ntot_wc=n
  end subroutine

  subroutine set_imaging_space(n)
   integer :: n
   ntot_im=n+ntot_im
  end subroutine

   logical function wem_allocate_memory(wsep,n_omp)
    integer :: ierr,ntot,n_omp
    type(sep3d) :: wsep
    character(len=128) :: operation
    integer :: stat(6),nomp
    integer :: n1,n2,n3,n4

    wem_allocate_memory=.false.

    nomp=n_omp
    call my_operation(operation)

    my_reuse=.false.

    if(operation(1:6)=="migrat" .and. wsep%n(8)== nomp .and. wsep%n(7)==nomp) then
       call from_param("save_mem",my_reuse,.false.)
    end if
    
    ntot_nomp=ntot_fk+ntot_fk+ntot_wc+ntot_im
    ntot_nwb=ntot_wc
    ntot_nzb=1
    if(my_reuse) then
       ntot_nomp=min(ntot_nomp,ntot_nzb)+ntot_nwb+ntot_nzb-wsep%n(7)
       ntot_nwb=0 ; ntot_nzb=0
    end if

    stat=0
    n1=wsep%n(1)
    n2=wsep%n(2)
    n3=wsep%n(3)
    n4=wsep%n(4)
    if(ntot_nomp>0) allocate(wei_mem_nomp_1(n1,n2,n3,n4,nomp),stat=stat(1))
    if(ntot_nomp>1) allocate(wei_mem_nomp_2(n1,n2,n3,n4,nomp),stat=stat(2))
    if(ntot_nomp>2) allocate(wei_mem_nomp_3(n1,n2,n3,n4,nomp),stat=stat(3))
    if(ntot_nomp>3) allocate(wei_mem_nomp_4(n1,n2,n3,n4,nomp),stat=stat(4))
    if(ntot_nomp>4) allocate(wei_mem_nomp_5(n1,n2,n3,n4,nomp),stat=stat(5))
    if(ntot_nomp>5) allocate(wei_mem_nomp_6(n1,n2,n3,n4,nomp),stat=stat(6))
    if(ntot_nomp>6) call seperr("only setup to deal with 6 nomp size buffers")
    if(any(stat/=0)) then
        write(0,*)  "trouble allocating nomp buffers"
        return
    end if
    nomp_avail=.false.; nomp_avail(1:ntot_nomp)=.true.

    if(ntot_nzb>0) allocate(wei_mem_nzb_1(n1,n2,n3,n4,wsep%n(7)),stat=stat(1))
    if(ntot_nzb>1) allocate(wei_mem_nzb_2(n1,n2,n3,n4,wsep%n(7)),stat=stat(2))
    if(ntot_nzb>2) allocate(wei_mem_nzb_3(n1,n2,n3,n4,wsep%n(7)),stat=stat(3))
    if(ntot_nzb>3) allocate(wei_mem_nzb_4(n1,n2,n3,n4,wsep%n(7)),stat=stat(4))
    if(ntot_nzb>4) allocate(wei_mem_nzb_5(n1,n2,n3,n4,wsep%n(7)),stat=stat(5))
    if(ntot_nzb>5) allocate(wei_mem_nzb_6(n1,n2,n3,n4,wsep%n(7)),stat=stat(6))
    if(ntot_nzb>6) call seperr("only setup to deal with 6 nzb size buffers")
    if(any(stat/=0)) then
        write(0,*)  "trouble allocating nzb buffers"
        return
    end if
    nzb_avail=.false.; nzb_avail(1:ntot_nzb)=.true.


    if(ntot_nwb>0) allocate(wei_mem_nwb_1(n1,n2,n3,n4,wsep%n(8)),stat=stat(1))
    if(ntot_nwb>1) allocate(wei_mem_nwb_2(n1,n2,n3,n4,wsep%n(8)),stat=stat(2))
    if(ntot_nwb>2) allocate(wei_mem_nwb_3(n1,n2,n3,n4,wsep%n(8)),stat=stat(3))
    if(ntot_nwb>3) allocate(wei_mem_nwb_4(n1,n2,n3,n4,wsep%n(8)),stat=stat(4))
    if(ntot_nwb>4) allocate(wei_mem_nwb_5(n1,n2,n3,n4,wsep%n(8)),stat=stat(5))
    if(ntot_nwb>5) allocate(wei_mem_nwb_6(n1,n2,n3,n4,wsep%n(8)),stat=stat(6))
    if(ntot_nwb>6) call seperr("only setup to deal with 6 nwb size buffers")
    if(any(stat/=0)) then
        write(0,*)  "trouble allocating nwb buffers"
        return
    end if
    nwb_avail=.false.; nwb_avail(1:ntot_nwb)=.true.
    call add_mem(n1*n2*n3*n4+(ntot_nwb*wsep%n(8)+ntot_nzb*wsep%n(7)+nomp*ntot_nomp))

    if(my_verb()) write(0,*) "  BUFFER ALLOCATION nomp,nzb,nwb",ntot_nomp,ntot_nzb,ntot_nwb
       wem_allocate_memory=.true.
   end function

  subroutine assign_wave_space(array)
   complex, pointer :: array(:,:,:,:,:)

   if(.not. my_reuse) then
     call assign_nwb_space(array,avail(wa_use(1),nwb_avail))
   else
     call assign_nomp_space(array,avail(wa_use(1),nomp_avail))
   end if
  end subroutine

  subroutine clear_wave_space()
    if(.not. my_reuse) then
     call clear_space(wa_use,nwb_avail)
    else
     call clear_space(wa_use,nomp_avail)
    end if
  end subroutine

  subroutine assign_image_space(array1)
   complex, pointer          :: array1(:,:,:,:,:)
   if(.not. my_reuse) then
     call assign_nzb_space(array1,avail(rs_use(1),nzb_avail))
   else
     call assign_nomp_space(array1,rs_use(1))
   end if
  end subroutine

  subroutine clear_image_space()
    if(.not. my_reuse) then
     call clear_space(rs_use,nzb_avail)
    else
     call clear_space(rs_use,nomp_avail)
    end if
  end subroutine


  subroutine assign_imaging_space(array1,array2,array3)
   complex, pointer          :: array1(:,:,:,:,:)
   complex, pointer,optional :: array2(:,:,:,:,:)
   complex, pointer,optional :: array3(:,:,:,:,:)
    call assign_nomp_space(array1,avail(im_use(1),nomp_avail))
    if(present(array2))call assign_nomp_space(array2,avail(im_use(2),nomp_avail))
    if(present(array3))call assign_nomp_space(array3,avail(im_use(3),nomp_avail))
  end subroutine

  subroutine clear_imaging_space()
     call clear_space(im_use,nomp_avail)
  end subroutine


  subroutine assign_fx_space(array1,array2,array3)
   complex, pointer          :: array1(:,:,:,:,:)
   complex, pointer,optional :: array2(:,:,:,:,:)
   complex, pointer,optional :: array3(:,:,:,:,:)
     call assign_nomp_space(array1,avail(fx_use(1),nomp_avail))
     if(present(array2))call assign_nomp_space(array2,avail(fx_use(2),nomp_avail))
     if(present(array3))call assign_nomp_space(array3,avail(fx_use(3),nomp_avail))
  end subroutine

  subroutine clear_fx_space()
     call clear_space(fx_use,nomp_avail)
  end subroutine

  subroutine assign_wc_space(array1,array2,array3)
   complex, pointer          :: array1(:,:,:,:,:)
   complex, pointer,optional :: array2(:,:,:,:,:)
   complex, pointer,optional :: array3(:,:,:,:,:)
     call assign_nomp_space(array1,avail(wc_use(1),nomp_avail))
     if(present(array2))call assign_nomp_space(array2,avail(wc_use(2),nomp_avail))
     if(present(array3))call assign_nomp_space(array3,avail(wc_use(3),nomp_avail))
  end subroutine

  subroutine clear_wc_space()
     call clear_space(wc_use,nomp_avail)
  end subroutine
  subroutine assign_fk_space(array1,array2,array3)
   complex, pointer          :: array1(:,:,:,:,:)
   complex, pointer,optional :: array2(:,:,:,:,:)
   complex, pointer,optional :: array3(:,:,:,:,:)
     call assign_nomp_space(array1,avail(fx_use(1),nomp_avail))
     if(present(array2))call assign_nomp_space(array2,avail(fx_use(2),nomp_avail))
     if(present(array3))call assign_nomp_space(array3,avail(fx_use(3),nomp_avail))
  end subroutine
  subroutine clear_fk_space()
     call clear_space(fk_use,nomp_avail)
  end subroutine

  subroutine clear_space(used,avail)
   integer :: used(:)
   logical :: avail(:)
   integer :: i

   do i=1,size(used)
    if(i>0 .and. i < size(avail)) then
       avail(i)=.true.
       used(i)=-1
    end if
  end do
  end subroutine


  integer function avail(ibuf,stat_array)
    logical :: stat_array(:)
    integer :: ibuf
    ibuf=1
    avail=-1
    do while(ibuf < size(stat_array))
      if(stat_array(ibuf))  then
        stat_array(ibuf)=.false.
        avail=ibuf
        return
      end if
      ibuf=ibuf+1
    end do
    call seperr("out of buffer space")
  end function

  subroutine assign_nomp_space(array,inum)
    integer :: inum
    complex,pointer :: array(:,:,:,:,:)
    if(inum==1) array=>wei_mem_nomp_1
    if(inum==2) array=>wei_mem_nomp_2
    if(inum==3) array=>wei_mem_nomp_3
    if(inum==4) array=>wei_mem_nomp_4
    if(inum==5) array=>wei_mem_nomp_5
    if(inum==6) array=>wei_mem_nomp_6
  end subroutine
  subroutine assign_nzb_space(array,inum)
    integer :: inum
    complex,pointer :: array(:,:,:,:,:)
    if(inum==1) array=>wei_mem_nzb_1
    if(inum==2) array=>wei_mem_nzb_2
    if(inum==3) array=>wei_mem_nzb_3
    if(inum==4) array=>wei_mem_nzb_4
    if(inum==5) array=>wei_mem_nzb_5
    if(inum==6) array=>wei_mem_nzb_6
  end subroutine
  subroutine assign_nwb_space(array,inum)
    integer :: inum
    complex,pointer :: array(:,:,:,:,:)
    if(inum==1) array=>wei_mem_nwb_1
    if(inum==2) array=>wei_mem_nwb_2
    if(inum==3) array=>wei_mem_nwb_3
    if(inum==4) array=>wei_mem_nwb_4
    if(inum==5) array=>wei_mem_nwb_5
    if(inum==6) array=>wei_mem_nwb_6
  end subroutine

 subroutine del_mem(iadd)
  integer  :: iadd
    mem_usage=mem_usage-iadd
 end subroutine

   subroutine show_mem(str)
    character(len=*) :: str
     if(my_Verb() .and. verb_mem_usage) &
      write(0,*) "memory used (mb) ",trim(str),":",real(mem_usage)*8./1000000.
   end subroutine

   subroutine add_mem(iadd)
    integer  :: iadd
      mem_usage=mem_usage+iadd
   end subroutine

   logical function reuse_mem()
     reuse_mem=my_reuse
   end function


end module
