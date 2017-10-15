module oc_file_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use sep
  use oc_utils_mod
  implicit none

  type, public :: fileinfo
     character(len=128)                :: name
     integer                           :: nd, esize
     integer, dimension(:), pointer    :: n
     real,    dimension(:), pointer    :: o,d
     integer                           :: blon ! number of blocks
     integer                           :: bloe ! elements in a block
     integer                           :: blob ! bytes in a block
  end type fileinfo

contains

  !----------------------------------------------------------------
  subroutine oc_allocatefile(file, t_, maxmem)
    type(fileinfo),     intent(inout) :: file
    character(len=128), intent(in)    :: t_
    integer,            optional      :: maxmem  !! in bytes
    integer                           :: prodn

    file%name=t_

    file%nd = sep_dimension(t_)
    allocate (file%n(file%nd)); file%n = 1
    allocate (file%o(file%nd)); file%o = 0.
    allocate (file%d(file%nd)); file%d = 1.
    call from_aux(t_, "n"    ,file%n    ,file%n)
    call from_aux(t_, "o"    ,file%o    ,file%o)
    call from_aux(t_, "d"    ,file%d    ,file%d)
    call from_aux(t_, "esize",file%esize,4)

    prodn=product(file%n)
    if (present(maxmem) .and. maxmem /=0 .and. prodn>maxmem/file%esize) then
       file%bloe=prime(prodn,maxmem/file%esize)
       file%blon=prodn/file%bloe
    else
       file%bloe=prodn
       file%blon=1
    end if
    file%blob=file%bloe*file%esize
  end subroutine oc_allocatefile
  !----------------------------------------------------------------
  subroutine oc_deallocatefile(file)
    type(fileinfo) :: file

    deallocate( file%n, file%o, file%d)
  end subroutine oc_deallocatefile
  !----------------------------------------------------------------
  subroutine oc_infofile(file)
    type(fileinfo), intent(in) :: file
    integer                    :: i
    character(len=4)           :: bufn,bufo,bufd

    write (0,*) '======================================='
    write (0,'(" ",a,"= ",a)') 'name        ',trim(file%name)
    write (0,'(" ",a,"=",i2)') 'esize       ',file%esize
    write (0,'(" ",i4," blocks",i7," elements",i7," bytes")') file%blon,file%bloe,file%blob
    write (0,'(" ",a,"=",i2)') 'dim',file%nd
    do i=1,file%nd
       write (bufn, "(a,i1)" ) 'n',i
       write (bufo, "(a,i1)" ) 'o',i
       write (bufd, "(a,i1)" ) 'd',i
       write (0,'(" ",a,"=",i4,"    ",a,"=",f7.2,"    ",a,"=",f7.2)') &
       trim(bufn),file%n(i),trim(bufo),file%o(i),trim(bufd),file%d(i)
    end do
    write (0,*) '======================================='
  end subroutine oc_infofile
  !----------------------------------------------------------------
  subroutine oc_checksimilarity(file1,file2,caller_in)
    type(fileinfo),   intent(in) :: file1,file2
    character(len=*), optional   :: caller_in
    character(len=128)           :: caller
    integer                      :: i

    caller="unknown"; if(present(caller_in)) caller=caller_in

    if      (file1%nd /= file2%nd) then
       call oc_infofile(file1);call oc_infofile(file2)
       call message("file spaces dont match: caller",caller)
       call seperr('is dead !')
    else if (file1%esize /= file2%esize) then
       call oc_infofile(file1);call oc_infofile(file2)
       call message("file esize dont match: caller",caller)
       call seperr('is dead !')
    else
       do i=1,file1%nd
          if(file1%n(i) /= file2%n(i)) then
             call oc_infofile(file1);call oc_infofile(file2)
             call message("file Ns dont match: caller",caller)
             call seperr('is dead !')
          end if
       end do
    end if
  end subroutine oc_checksimilarity
  !----------------------------------------------------------------
  subroutine oc_checkspace(file1,file2,caller_in)
    type(fileinfo), intent(in) :: file1,file2
    character(len=*), optional :: caller_in
    character(len=128)         :: caller
    integer                    :: i

    caller="unknown"; if(present(caller_in)) caller=caller_in

    if      (file1%nd /= file2%nd) then
       call oc_infofile(file1);call oc_infofile(file2)
       call message("file spaces dont match: caller",caller)
       call seperr('is dead !')
    else
       do i=1,file1%nd
          if(file1%n(i) /= file2%n(i)) then
             call oc_infofile(file1);call oc_infofile(file2)
             call message("file Ns dont match: caller",caller)
             call seperr('is dead !')
          end if
       end do
    end if
  end subroutine oc_checkspace
  !----------------------------------------------------------------

  !----------------------------------------------------------------
  character(len=128) function oc_allocate(tmp,name,esize,n,o,d) result(t_)
    logical, intent(in)                :: tmp
    character(len=128),    intent(in)  :: name
    integer,               optional    :: esize
    integer, dimension(:), optional    :: n
    real,    dimension(:), optional    :: o,d
    integer                            :: i
    character (len=10)                 :: buffer

    t_=trim(name)
    if(tmp) then
       call auxtmp(trim(name))
    else
       call auxscr(trim(name))
    end if

    if (present(n)) then
       do i=1,size(n)
          write (buffer, "(a,i1)" ) 'n',i
          call auxputch(buffer,'i',n(i),t_)
          write (buffer, "(a,i1)" ) 'o',i
          call auxputch(buffer,'f', 0.0,t_)
          write (buffer, "(a,i1)" ) 'd',i
          call auxputch(buffer,'f', 1.0,t_)
       end do
    end if

    if (present(o)) then
       do i=1,size(o)
          write (buffer, "(a,i1)" ) 'o',i
          call auxputch(buffer,'f',o(i),t_)
       end do
    end if

    if (present(d)) then
       do i=1,size(d)
          write (buffer, "(a,i1)" ) 'd',i
          call auxputch(buffer,'f',d(i),t_)
       end do
    end if

    if (present(esize)) then
       call auxputch('esize','i', esize,t_)
    end if
  end function oc_allocate
  !----------------------------------------------------------------
  character(len=128) function oc_clone(tmp,t0_,name, maxmem) result(t_)
    !! allocate a file (handle=t_, name=name) w/ the same structure as t0_ 
    logical,            intent(in)     :: tmp
    character(len=128), intent(in)     :: t0_
    character(len=128), intent(in)     :: name
    integer,            optional       :: maxmem
    integer                            :: i
    character (len=10)                 :: buffer
    type(fileinfo)                     :: file
    real,    dimension(:), allocatable :: rblov
    complex, dimension(:), allocatable :: cblov

    call oc_allocatefile(file, t0_, maxmem)
    t_=trim(name)
    if(tmp) then
       call auxtmp(trim(name))
    else
       call auxscr(trim(name))
    end if

    do i=1,file%nd
       write (buffer, "(a,i1)" ) 'n',i
       call auxputch(buffer,'i',file%n(i),t_)
       write (buffer, "(a,i1)" ) 'o',i
       call auxputch(buffer,'f',file%o(i),t_)
       write (buffer, "(a,i1)" ) 'd',i
       call auxputch(buffer,'f',file%d(i),t_)
    end do
    write (buffer, "(a,i1)" ) 'n',file%nd+1
    call auxputch(buffer,'i',1,t_)
    call auxputch("esize","i",file%esize,t_)

    if (file%esize /=8) then !! REAL
       allocate(rblov(file%bloe))
       do i=1,file%blon
          rblov=0.0
          call sep_write(rblov,t_)
       end do
       deallocate(rblov)
    else                     !! COMPLEX
       allocate(cblov(file%bloe))
       do i=1,file%blon
          cblov=(0.0,0.0)
          call sep_write(cblov,t_)
       end do
       deallocate(cblov)
    end if

    call sseek(t_,0,0)
    call oc_deallocatefile(file); call sseek(t0_,0,0)
  end function oc_clone
  !----------------------------------------------------------------
  subroutine oc_append(t_, s_, maxmem)
    character(len=128), intent(in)     :: t_, s_
    integer,            optional       :: maxmem
    type(fileinfo)                     :: file
    integer                            :: i
    real,    dimension(:), allocatable :: rblov
    complex, dimension(:), allocatable :: cblov

    call sseek(s_, 0,0); call oc_allocatefile(file, s_, maxmem)

    if (file%esize /=8) then !! REAL
       allocate(rblov(file%bloe))
       do i=1,file%blon
          call sreed(s_, rblov, file%blob)
          call sep_write(rblov,t_)
       end do
       deallocate(rblov)
    else                     !! COMPLEX
       allocate(cblov(file%bloe))
       do i=1,file%blon
          call sreed(s_, cblov, file%blob)
          call sep_write(cblov,t_)
       end do
       deallocate(cblov)
    end if

    call oc_deallocatefile(file); call sseek(s_,0,0)
  end subroutine oc_append
  !----------------------------------------------------------------
  subroutine oc_adddim(t_, nnew)
    character(len=128), intent(in) :: t_
    integer,            intent(in) :: nnew
    type(fileinfo)                 :: file
    character (len=10)             :: buffer

    call oc_allocatefile(file, t_)
    write (buffer, "(a,i1)" ) 'n',file%nd+1
    call auxputch(buffer,'i',nnew,t_)

    write (buffer, "(a,i1)" ) 'o',file%nd+1
    call auxputch(buffer,'i',0,t_)

    write (buffer, "(a,i1)" ) 'd',file%nd+1
    call auxputch(buffer,'i',1,t_)

    call oc_deallocatefile(file)
  end subroutine oc_adddim
  !----------------------------------------------------------------
  subroutine oc_shapeheader(t_,esize,n,o,d)
    character(len=128),    intent(in) :: t_
    integer,               intent(in) :: esize
    integer, dimension(:), intent(in) :: n
    real,    dimension(:), optional   :: o,d
    integer                           :: i
    character (len=10)                :: buffer

    call auxputch("esize",'i',esize,t_)

    do i=1,size(n)
       write (buffer, "(a,i1)" ) 'n',i
       call auxputch(buffer,'i',n(i),t_)
    end do

    if (present(o)) then
       do i=1,size(o)
          write (buffer, "(a,i1)" ) 'o',i
          call auxputch(buffer,'f',o(i),t_)
       end do
    end if

    if (present(d)) then
       do i=1,size(d)
          write (buffer, "(a,i1)" ) 'd',i
          call auxputch(buffer,'f',d(i),t_)
       end do
    end if

  end subroutine oc_shapeheader
  !----------------------------------------------------------------
  subroutine oc_print(t_, maxmem)
    !! print the contents of a file
    character(len=128), intent(in)     :: t_
    integer,            optional       :: maxmem
    integer                            :: i
    type(fileinfo)                     :: file
    real,    dimension(:), allocatable :: rblov
    complex, dimension(:), allocatable :: cblov

    call sseek(t_,0,0); call oc_allocatefile(file, t_, maxmem)

    if (file%esize /=8) then !! REAL
       allocate(rblov(file%bloe))
       do i=1,file%blon
          call sreed(t_, rblov, file%blob)
          write(0,*) rblov
       end do
       deallocate(rblov)
    else                     !! COMPLEX
       allocate(cblov(file%bloe))
       do i=1,file%blon
          call sreed(t_, cblov, file%blob)
          write(0,*) cblov
       end do
       deallocate(cblov)
    end if
    call oc_deallocatefile(file); call sseek(t_,0,0)
  end subroutine oc_print
  !----------------------------------------------------------------
end module oc_file_mod
