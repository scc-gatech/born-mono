module sep_regfile_mod
 use generic_file_mod
 use sep_mod


 

 implicit none
 
 type,extends(genericRegFile) :: sepFile
 
   contains
       procedure::get_int=>sGetInt
      procedure:: get_Float=>sGetFloat
      procedure:: get_String=>sGetString
      procedure:: get_Bool=>sGetBool
      procedure:: get_Ints=>sGetInts
      procedure:: get_Floats=>sGetFloats
      procedure:: put_logical=>sfPutLogical
      procedure:: put_int=>sfputInt
      procedure:: put_real=>sfputReal
      procedure:: put_string=>sfputString
      procedure:: put_reals=>sfPutReals
      procedure:: put_ints=>sfPutInts
      procedure:: readDescription=>sfReadDescription
      procedure:: writeDescription=>sfWriteDescription
      procedure::  err=>sfError
      procedure:: initFile=>initFileSEP
      procedure:: readF1=>sfreadF1
      procedure:: readF2=>sfreadF2
      procedure:: readF3=>sfreadF3
      procedure:: readF4=>sfreadF4
      procedure:: readF5=>sfreadF5
      
      procedure:: writeF1=>sfwriteF1
      procedure:: writeF2=>sfwriteF2
      procedure:: writeF3=>sfwriteF3
      procedure:: writeF4=>sfwriteF4
      procedure:: writeF5=>sfwriteF5
      
      procedure:: rfwind1=>sfrwind1
      procedure:: rfwind2=>sfrwind2
      procedure:: rfwind3=>sfrwind3
      procedure:: rfwind4=>sfrwind4
      procedure:: rfwind5=>sfrwind5
      
      procedure:: wfwind1=>sfwwind1
      procedure:: wfwind2=>sfwwind2
      procedure:: wfwind3=>sfwwind3
      procedure:: wfwind4=>sfwwind4
      procedure:: wfwind5=>sfwwind5
      
      
      procedure:: readc1=>screadF1
      procedure:: readc2=>screadF2
      procedure:: readc3=>screadF3
      procedure:: readc4=>screadF4
      procedure:: readc5=>screadF5
      
      procedure:: writec1=>scwriteF1
      procedure:: writec2=>scwriteF2
      procedure:: writec3=>scwriteF3
      procedure:: writec4=>scwriteF4
      procedure:: writec5=>scwriteF5
      
      procedure:: rcwind1=>scrwind1
      procedure:: rcwind2=>scrwind2
      procedure:: rcwind3=>scrwind3
      procedure:: rcwind4=>scrwind4
      procedure:: rcwind5=>scrwind5
      
      procedure:: wcwind1=>scwwind1
      procedure:: wcwind2=>scwwind2
      procedure:: wcwind3=>scwwind3
      procedure:: wcwind4=>scwwind4
      procedure:: wcwind5=>scwwind5
end type


contains 

subroutine initFileSEP(struct,tag,mode)
  character(len=*),intent(in) :: tag,mode
  class(sepFile) :: struct
  integer :: ierr
  call struct%setFileName(tag)
  
  if( trim(tag)=="IN") then
        call struct%readDescription()
   else if(trim(tag)=="OUT") then
     write(0,*) "IN TAG OUT"
   else
    if(trim(mode)=="IN") then
      ierr= auxin(trim(tag))
      call struct%readDescription()
    else if(trim(mode)=="OUT") then
      call auxout(trim(tag))
    else if(trim(mode)=="INOUT") then
      call auxinout(trim(tag))
      call struct%readDescription()

    else if(trim(mode)=="SCRATCH") then
      call auxtmp(trim(tag))
    else 
      write(0,*) "UNKNOWN MODE",trim(mode)
      call seperr("")
    end if
  end if
end subroutine 
      
    

subroutine sfrwind1(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real,intent(out) :: vals(:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/= srw_f_1(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine

subroutine sfrwind2(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real ,intent(out):: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine
subroutine sfrwind3(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real ,intent(out):: vals(:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine

subroutine sfrwind4(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real,intent(out) :: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine

subroutine sfrwind5(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real,intent(out):: vals(:,:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine

subroutine sfwwind1(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real,intent(in) :: vals(:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine


subroutine sfwwind2(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real,intent(in) :: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine


subroutine sfwwind3(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real ,intent(in):: vals(:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine


subroutine sfwwind4(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real ,intent(in):: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine


subroutine sfwwind5(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  real,intent(in) :: vals(:,:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine

subroutine sfwriteF1(struct,vals,sz)
  class(sepFile) :: struct
  real,intent(in) :: vals(:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=srite(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine

subroutine sfwriteF2(struct,vals,sz)
  class(sepFile) :: struct
  real,intent(in) :: vals(:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=srite(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine

subroutine sfwriteF3(struct,vals,sz)
  class(sepFile) :: struct
  real ,intent(in):: vals(:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=srite(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine

subroutine sfwriteF4(struct,vals,sz)
  class(sepFile) :: struct
  real,intent(in) :: vals(:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=srite(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine

subroutine sfwriteF5(struct,vals,sz)
  class(sepFile) :: struct
  real,intent(in) :: vals(:,:,:,:,:)
  integer(kind=8),optional :: sz
  integer (kind=8):: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=srite(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine


subroutine sfreadF1(struct,vals,sz)
  class(sepFile) :: struct
  real ,intent(out):: vals(:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  integer(kind=8) :: done,todo,blk
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  blk=1000*1000*100;
  done=0
  do while(done/=sz2) 
    todo=min(sz2-done,blk)
    if(sz2*4/=sreed(trim(struct%getFileName()),vals,sz2*4)) then
         write(0,*) "FILE=",trim(struct%getFileName())
       call seperr("trouble reading data")
    end if
    done=done+todo
  end do
end subroutine


subroutine sfreadF2(struct,vals,sz)
  class(sepFile) :: struct
  real,intent(out) :: vals(:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=sreed(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine



subroutine sfreadF3(struct,vals,sz)
  class(sepFile) :: struct
  real,intent(out) :: vals(:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=sreed(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine


subroutine sfreadF4(struct,vals,sz)
  class(sepFile) :: struct
  real ,intent(out):: vals(:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=sreed(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine


subroutine sfreadF5(struct,vals,sz)
  class(sepFile) :: struct
  real,intent(out) :: vals(:,:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*4/=sreed(trim(struct%getFileName()),vals,sz2*4)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine


subroutine sfPutLogical(struct,arg,val)
  class(sepFile) :: struct
  character(len=*),intent(in) :: arg
  logical,intent(in)  :: val
  integer :: ierr
   ierr=auxputch(arg,"l",val,struct%getFileName())
   if(ierr<1) then
     write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing parameter")
   end if 
end subroutine

subroutine sfPutString(struct,arg,val)
  class(sepFile) :: struct
  character(len=*) ,intent(in):: arg
  character(len=*) ,intent(in):: val
  integer :: ierr
   ierr=auxputch(arg,"s",val,struct%getFileName())
   if(ierr<1) then
     write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing parameter")
   end if 
end subroutine


subroutine sfPutInt(struct,arg,val)
  class(sepFile) :: struct
  character(len=*),intent(in) :: arg
  integer ,intent(in):: val
  integer :: ierr
   ierr=auxputch(arg,"d",val,struct%getFileName())
   if(ierr<1) then
     write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing parameter")
   end if 
end subroutine

subroutine sfPutReal(struct,arg,val)
  class(sepFile) :: struct
  character(len=*),intent(in) :: arg
  real ,intent(in):: val
  integer :: ierr
   ierr=auxputch(arg,"f",val,struct%getFileName())
   if(ierr<1) then
     write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing parameter")
   end if 
end subroutine

subroutine sfPutReals(struct,arg,val)
  class(sepFile) :: struct
  character(len=*),intent(in) :: arg
  real ,intent(in):: val(:)
  integer :: ierr
   ierr=auxpu_f_f_a (arg,"f",val,struct%getFileName())
   if(ierr<1) then
     write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing parameter")
   end if 
end subroutine

subroutine sfPutInts(struct,arg,val)
  class(sepFile) :: struct
  character(len=*),intent(in) :: arg
  integer,intent(in) :: val(:)
  integer :: ierr
   ierr=auxputch(arg,"d",val,struct%getFileName())
   if(ierr<1) then
     write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing parameter")
   end if 
end subroutine

   subroutine sfError(struct,arg)
   class(sepFile) :: struct
   character(len=*),intent(in) :: arg
   call seperr(arg)
 end subroutine

subroutine sfWriteDescription(struct)
 class(sepFile) :: struct
 type(hypercube) :: hy,hy2
 type(axis) :: ax
 type(axis), pointer ::axes(:)
 character(len=1024) :: fle
 integer :: iax,ierr
 hy=struct%getHyper()
 allocate(axes(hy%getNdim()))
 call hy%getAxes(axes)
! axes=hyper%getAxes()
 fle=struct%getFileName()
 do iax=1,hy%getNdim()
    if(0/=sep_put_data_axis_par(fle,iax,axes(iax)%n,axes(iax)%o,axes(iax)%d,axes(iax)%label)) then
      write(0,*) "FILE=",trim(fle)," axis=",iax
      call seperr("trouble writing axis")
    end if
 end do
 deallocate(axes)
 end subroutine


subroutine sfReadDescription(struct)
 class(sepFile) :: struct
 type(hypercube) :: hyper
 type(axis), pointer :: axes(:)
 integer :: ndim,iax,ierr
 ierr=sep_get_number_data_axes(trim(struct%getFileName()),ndim)
 if(ierr/=0) then
   write(0,*) "FILE=",trim(struct%getFileName())
   call seperr("trouble getting the number of dimensions")
 end if
 allocate(axes(ndim))
 do iax=1,ndim 
   if(0/=sep_get_data_axis_par(trim(struct%getFileName()),iax,axes(iax)%n,axes(iax)%o,axes(iax)%d,axes(iax)%label)) then
     write(0,*) "FILE",trim(struct%getFileName())," axis=",iax
     call seperr("trouble getting axis")
   end if
 end do
 hyper=hyp_axes(axes)
 call struct%setHyper(hyper)
 deallocate(axes)
end subroutine

subroutine sGetInt(struct,arg,val,def)
   class(sepFile) :: struct
   character(len=*),intent(in) :: arg
   integer,intent(inout):: val
   integer,optional :: def
   integer :: ierr 
   
   ierr=auxpar(arg,"d",val,trim(struct%getFileName()))
   if(ierr==0 .and. present(def)) then
     val=def
   else if(ierr==0) then
     call seperr(arg//" param unspecified in parameters")
   end if
 end subroutine
 
 subroutine sGetFloat(struct,arg,val,def)
   class(sepFile) :: struct
   character(len=*),intent(in) :: arg
   real ,intent(inout):: val
   real, optional :: def
   integer :: ierr
   ierr=auxpar(arg,"d",val,trim(struct%getFileName()))
   if(ierr==0 .and. present(def)) then
     val=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   end if
 end subroutine



subroutine sGetString(struct,arg,val,def)
   class(sepFile) :: struct
   character(len=*),intent(in) :: arg
   character(len=*),intent(inout) ::val
   character(len=*), optional :: def
   integer :: ierr 
   ierr=auxpar(arg,"s",val,trim(struct%getFileName()))
   if(ierr==0 .and. present(def)) then
     val=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   end if
 end subroutine
 
 subroutine sGetBool(struct,arg,val,def)
   class(sepFile) :: struct
   character(len=*),intent(in) :: arg
   logical,intent(inout) :: val
   logical, optional :: def
   integer :: ierr
   ierr=auxpar(arg,"l",val,trim(struct%getFileName()))
      if(ierr==0 .and. present(def)) then
     val=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   end if
 end subroutine


 subroutine sGetInts(struct,arg,val,def)
   class(sepFile) :: struct
   character(len=*) ,intent(in):: arg
   integer, dimension(:),intent(inout):: val
   integer, optional :: def(:)
   integer :: ierr 
   integer :: tmp(10000)
   ierr=auxpar(arg,"d",tmp,trim(struct%getFileName()))
   if(ierr==0 .and. present(def)) then
     val(1:size(def))=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   else
     val(1:ierr)=tmp(1:ierr)
   end if

 end subroutine
 
 
 
 subroutine sGetFloats(struct,arg,val,def)
   class(sepFile) :: struct
   character(len=*),intent(in) :: arg
   real,intent(inout) :: val(:)
   real :: tmp(1000)
   real,optional ::  def(:)
   integer :: ierr
   ierr=auxpar(arg,"f",tmp,trim(struct%getFileName()))
   if(ierr==0 .and. present(def)) then
     val(1:size(def))=def
   else if(ierr==0)then
     call seperr(arg//" param unspecified in parameters")
   else
     val(1:ierr)=tmp(1:ierr)
   end if
 end subroutine
 

 
 
 subroutine scrwind1(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(out) :: vals(:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine

subroutine scrwind2(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(out) :: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine
subroutine scrwind3(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(out) :: vals(:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine

subroutine scrwind4(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(out) :: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine

subroutine scrwind5(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(out) :: vals(:,:,:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=sreed_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
  deallocate(ng)
end subroutine

subroutine scwwind1(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(in) :: vals(:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine


subroutine scwwind2(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(in) :: vals(:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine


subroutine scwwind3(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(in) :: vals(:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine


subroutine scwwind4(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex,intent(in) :: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine


subroutine scwwind5(struct,vals,nw,fw,jw)
  class(sepFile) :: struct
  complex ,intent(in):: vals(:,:,:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   hy=struct%getHyper()
   allocate(ng(hy%getNdim())); call hy%returnNs(ng)
  if(0/=srite_window(trim(struct%getFileName()),size(nw), &
    ng,nw,fw,jw,4,vals)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
  deallocate(ng)
end subroutine

subroutine scwriteF1(struct,vals,sz)
  class(sepFile) :: struct
  complex,intent(in) :: vals(:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=srite(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine

subroutine scwriteF2(struct,vals,sz)
  class(sepFile) :: struct
  complex,intent(in) :: vals(:,:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=srite(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine

subroutine scwriteF3(struct,vals,sz)
  class(sepFile) :: struct
  complex ,intent(in):: vals(:,:,:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=srite(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine

subroutine scwriteF4(struct,vals,sz)
  class(sepFile) :: struct
  complex,intent(in) :: vals(:,:,:,:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=srite(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine

subroutine scwriteF5(struct,vals,sz)
  class(sepFile) :: struct
  complex ,intent(in):: vals(:,:,:,:,:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=srite(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble writing data")
  end if
end subroutine


subroutine screadF1(struct,vals,sz)
  class(sepFile) :: struct
  complex ,intent(out):: vals(:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=sreed(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine


subroutine screadF2(struct,vals,sz)
  class(sepFile) :: struct
  complex ,intent(out) :: vals(:,:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=sreed(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine



subroutine screadF3(struct,vals,sz)
  class(sepFile) :: struct
  complex ,intent(out) :: vals(:,:,:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=sreed(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine


subroutine screadF4(struct,vals,sz)
  class(sepFile) :: struct
  complex ,intent(out) :: vals(:,:,:,:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=sreed(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine


subroutine screadF5(struct,vals,sz)
  class(sepFile) :: struct
  complex ,intent(out) :: vals(:,:,:,:,:)
  integer(kind=8),optional :: sz
  integer :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  if(sz2*8/=sreed(trim(struct%getFileName()),vals,sz2*8)) then
       write(0,*) "FILE=",trim(struct%getFileName())
     call seperr("trouble reading data")
  end if
end subroutine


 
 
 
end module

      
