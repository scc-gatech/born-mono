module oc_llist_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_global_mod
  use oc_file_mod
  use oc_filealgebra_mod

  implicit none

  !--------------------------------
  type entry
     character(len=128)                        :: g_
     character(len=128), dimension(:), pointer :: gg_
     real(kind=db)         :: rgn
     complex(kind=db)      :: cgn
     type(entry), pointer  :: next
  end type entry
  !--------------------------------
  type list
     type(entry), pointer :: first
     type(entry), pointer :: current
     integer              :: depth
  end type list
  !--------------------------------

  interface oc_llist_add
     module procedure oc_llist_add_real
     module procedure oc_llist_add_complex
  end interface

  interface oc_llist_down
     module procedure oc_llist_down_real
     module procedure oc_llist_down_complex
  end interface

contains
  !----------------------------------------------------------------
  type(list) function oc_llist_init(n) result(l)
    integer, intent(in) :: n
    allocate(l%first)
    l%depth    =0
    allocate(l%first%gg_(n))
    call oc_llist_rewind(l)
    call oc_llist_entry_init(l%current)
  end function oc_llist_init
  !----------------------------------------------------------------
  subroutine oc_llist_entry_init(e)
    type(entry), intent(inout) :: e
    e%g_=""
    e%gg_=""
    e%rgn=0
    e%cgn=0
  end subroutine oc_llist_entry_init
  !----------------------------------------------------------------
  subroutine oc_llist_report(e)
    type(entry), intent(in) :: e
    integer                 :: i
    write(0,*) '-----------------------'
    write(0,*) trim(e%g_)
    do i=1,size(e%gg_)
       write(0,*) trim(e%gg_(i))
    end do
    write(0,*) e%rgn
    write(0,*) e%cgn
    write(0,*) '-----------------------'
  end subroutine oc_llist_report
  !----------------------------------------------------------------
  subroutine oc_llist_rewind(l)
    type(list), intent(inout) :: l
    l%current => l%first
  end subroutine oc_llist_rewind
  !----------------------------------------------------------------
  integer function oc_llist_depth(l) result(n)
    type(list), intent(in) :: l
    n = l%depth
  end function oc_llist_depth
  !----------------------------------------------------------------
  subroutine oc_llist_forward(l)
    type(list), intent(inout) :: l
    l%current => l%current%next
  end subroutine oc_llist_forward
  !----------------------------------------------------------------
  subroutine oc_llist_add_real(l, g_,gg_, gn,maxmem)
    type(list),                       intent(inout) :: l
    character(len=128),               intent(in)    :: g_
    character(len=128), dimension(:), intent(in)    :: gg_
    real(kind=db),                    intent(in)    :: gn
    integer,                          optional      :: maxmem
    character(len=128)                              :: l_
    character(len=128), dimension(:), allocatable   :: ll_
    integer                                         :: i,n
    character(len=128)                              :: buffer

    l%depth = l%depth + 1

    if     (l%depth>=100 .and. l%depth<=999) then
       write(buffer, "(a,i3)" ) '_l',l%depth
    else if(l%depth>=10  .and. l%depth<=99 ) then
       write(buffer, "(a,i2)" ) '_l',l%depth
    else
       write(buffer, "(a,i1)" ) '_l',l%depth
    end if
    l_=oc_clone(F, g_,buffer,maxmem)
    call oc_linear(l_,(/g_/),(/rONE/),maxmem)

    n=size(gg_)
    allocate(ll_(n))
    do i=1,n
       if     (l%depth>=100 .and. l%depth<=999) then
          write(buffer, "(a,i3,a,i1)" ) '_L',l%depth,'.',i
       else if(l%depth>=10  .and. l%depth<=99 ) then
          write(buffer, "(a,i2,a,i1)" ) '_L',l%depth,'.',i
       else
          write(buffer, "(a,i1,a,i1)" ) '_L',l%depth,'.',i
       end if
       ll_(i)=oc_clone(F, gg_(i),buffer,maxmem)
       call oc_linear(ll_(i),(/gg_(i)/),(/rONE/),maxmem)
    end do

    l%current%g_ = l_
    allocate(l%current%gg_(n))
    do i=1,n
       l%current%gg_(i) = ll_(i)
    end do
    deallocate(ll_)

    l%current%rgn = gn

    allocate(l%current%next)
    allocate(l%current%next%gg_(n))
    call oc_llist_entry_init(l%current%next)
  end subroutine oc_llist_add_real
  !----------------------------------------------------------------
  subroutine oc_llist_add_complex(l, g_,gg_, gn,maxmem)
    type(list),                       intent(inout) :: l
    character(len=128),               intent(in)    :: g_
    character(len=128), dimension(:), intent(in)    :: gg_
    complex(kind=db),                 intent(in)    :: gn
    integer,                          optional      :: maxmem
    character(len=128)                              :: l_
    character(len=128), dimension(:), allocatable   :: ll_
    integer                                         :: i,n
    character(len=128)                              :: buffer

    l%depth = l%depth + 1

    if     (l%depth>=100 .and. l%depth<=999) then
       write(buffer, "(a,i3)" ) '_l',l%depth
    else if(l%depth>=10  .and. l%depth<=99 ) then
       write(buffer, "(a,i2)" ) '_l',l%depth
    else
       write(buffer, "(a,i1)" ) '_l',l%depth
    end if
    l_=oc_clone(F, g_,buffer,maxmem)
    call oc_linear(l_,(/g_/),(/rONE/),maxmem)

    n=size(gg_)
    allocate(ll_(n))
    do i=1,n
       if     (l%depth>=100 .and. l%depth<=999) then
          write(buffer, "(a,i3,a,i1)" ) '_L',l%depth,'.',i
       else if(l%depth>=10  .and. l%depth<=99 ) then
          write(buffer, "(a,i2,a,i1)" ) '_L',l%depth,'.',i
       else
          write(buffer, "(a,i1,a,i1)" ) '_L',l%depth,'.',i
       end if
       ll_(i)=oc_clone(F, gg_(i),buffer,maxmem)
       call oc_linear(ll_(i),(/gg_(i)/),(/rONE/),maxmem)
    end do

    l%current%g_ = l_
    allocate(l%current%gg_(n))
    do i=1,n
       l%current%gg_(i) = ll_(i)
    end do
    deallocate(ll_)

    l%current%cgn = gn

    allocate(l%current%next)
    allocate(l%current%next%gg_(n))
    call oc_llist_entry_init(l%current%next)
  end subroutine oc_llist_add_complex
  !----------------------------------------------------------------
  subroutine oc_llist_down_real(l, g_,gg_, gn)
    type(list),                       intent(inout) :: l
    character(len=128),               intent(inout) :: g_
    character(len=128), dimension(:), intent(inout) :: gg_
    real(kind=db),                    intent(inout) :: gn

    g_ = l%current%g_
    gg_= l%current%gg_
    gn = l%current%rgn
    call oc_llist_forward(l)
  end subroutine oc_llist_down_real

  subroutine oc_llist_down_complex(l, g_,gg_, gn)
    type(list),                       intent(inout) :: l
    character(len=128),               intent(inout) :: g_
    character(len=128), dimension(:), intent(inout) :: gg_
    complex(kind=db),                 intent(inout) :: gn

    g_ = l%current%g_
    gg_= l%current%gg_
    gn = l%current%cgn
    call oc_llist_forward(l)
  end subroutine oc_llist_down_complex
  !----------------------------------------------------------------
  subroutine oc_llist_chop(l)
    type(list), intent(inout) :: l

    call oc_llist_rewind(l)
    l%first   => l%current%next
    l%current => l%first
    l%depth   =  l%depth-1
  end subroutine oc_llist_chop
  !----------------------------------------------------------------
  subroutine oc_llist_close(l)
    type(list), intent(inout) :: l
    integer                   :: i
    do i = 1,l%depth
       call oc_llist_chop(l)
    end do
  end subroutine oc_llist_close
  !----------------------------------------------------------------
end module oc_llist_mod






