module rsf_regfile_mod
 use generic_file_mod
 use rsf


 

 implicit none
 
 type,extends(genericRegFile) :: rsfFile
   type(file) :: myf
 
   contains
       procedure::get_int=>sfGetInt
      procedure:: get_Float=>sfGetFloat
      procedure:: get_String=>sfGetString
      procedure:: get_Bool=>sfGetBool
      procedure:: get_Ints=>sfGetInts
      procedure:: get_Floats=>sfGetFloats
      procedure:: put_logical=>sfPutLogical
      procedure:: put_int=>sfputInt
      procedure:: put_real=>sfputReal
      procedure:: put_string=>sfputString
      procedure:: put_reals=>sfPutReals
      procedure:: put_ints=>sfPutInts
      procedure:: readDescription=>sfReadDescription
      procedure:: writeDescription=>sfWriteDescription
      procedure::  err=>sfError
      procedure:: initFile=>initFileRSF
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

subroutine initFileRSF(struct,tag,mode)
  character(len=*),intent(in) :: tag,mode
  class(rsfFile) :: struct
  integer :: ierr
  call struct%setFileName(tag)
  
  if( mode=="IN") then
     struct%myf=rsf_input(tag)
     call struct%readDescription()
   else if(trim(tag)=="OUT") then
     struct%myf=rsf_output(tag)
  end if
end subroutine 
      
    

subroutine sfrwind1(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real,intent(out) :: vals(:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window read not implemented")
    vals=0

end subroutine

subroutine sfrwind2(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real ,intent(out):: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window read not implemented")
  vals=0

end subroutine
subroutine sfrwind3(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real ,intent(out):: vals(:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window read not implemented")
  vals=0

end subroutine

subroutine sfrwind4(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real,intent(out) :: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window read not implemented")
    vals=0


end subroutine

subroutine sfrwind5(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real,intent(out):: vals(:,:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window read not implemented")
  vals=0

end subroutine

subroutine sfwwind1(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real,intent(in) :: vals(:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine


subroutine sfwwind2(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real,intent(in) :: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine


subroutine sfwwind3(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real ,intent(in):: vals(:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine


subroutine sfwwind4(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real ,intent(in):: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine


subroutine sfwwind5(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  real,intent(in) :: vals(:,:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)

end subroutine

subroutine sfwriteF1(struct,vals,sz)
  class(rsfFile) :: struct
  real,intent(in) :: vals(:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
  end subroutine

subroutine sfwriteF2(struct,vals,sz)
  class(rsfFile) :: struct
  real,intent(in) :: vals(:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
  end subroutine

subroutine sfwriteF3(struct,vals,sz)
  class(rsfFile) :: struct
  real ,intent(in):: vals(:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
  end subroutine

subroutine sfwriteF4(struct,vals,sz)
  class(rsfFile) :: struct
  real,intent(in) :: vals(:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
end subroutine

subroutine sfwriteF5(struct,vals,sz)
  class(rsfFile) :: struct
  real,intent(in) :: vals(:,:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
end subroutine


subroutine sfreadF1(struct,vals,sz)
  class(rsfFile) :: struct
  real ,intent(out):: vals(:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine


subroutine sfreadF2(struct,vals,sz)
  class(rsfFile) :: struct
  real,intent(out) :: vals(:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine



subroutine sfreadF3(struct,vals,sz)
  class(rsfFile) :: struct
  real,intent(out) :: vals(:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine


subroutine sfreadF4(struct,vals,sz)
  class(rsfFile) :: struct
  real ,intent(out):: vals(:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine


subroutine sfreadF5(struct,vals,sz)
  class(rsfFile) :: struct
  real,intent(out) :: vals(:,:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine


subroutine sfPutLogical(struct,arg,val)
  class(rsfFile) :: struct
  character(len=*),intent(in) :: arg
  logical,intent(in)  :: val
  integer :: ierr
   !call to_par(struct%myf,arg,val)
   call sf_error("put logical not implemented in RSF Fortran")
end subroutine

subroutine sfPutString(struct,arg,val)
  class(rsfFile) :: struct
  character(len=*),intent(in) :: arg
  character(len=*),intent(in) :: val
   call to_par(struct%myf,arg,val)

end subroutine


subroutine sfPutInt(struct,arg,val)
  class(rsfFile) :: struct
  character(len=*),intent(in) :: arg
  integer,intent(in) :: val
  integer :: ierr
   call to_par(struct%myf,arg,val)

end subroutine

subroutine sfPutReal(struct,arg,val)
  class(rsfFile) :: struct
  character(len=*),intent(in) :: arg
  real,intent(in) :: val
  integer :: ierr
   call to_par(struct%myf,arg,val)

end subroutine

subroutine sfPutReals(struct,arg,val)
  class(rsfFile) :: struct
  character(len=*),intent(in) :: arg
  real,intent(in) :: val(:)
  integer :: ierr
  ! call to_par(struct%myf,arg,val)
   call sf_error("put reals not implemented in RSF Fortran")

end subroutine

subroutine sfPutInts(struct,arg,val)
  class(rsfFile) :: struct
  character(len=*) ,intent(in):: arg
  integer ,intent(in):: val(:)
  integer :: ierr
   call to_par(struct%myf,arg,val)

end subroutine

   subroutine sfError(struct,arg)
   class(rsfFile) :: struct
   character(len=*),intent(in) :: arg
   call seperr(arg)
 end subroutine

subroutine sfWriteDescription(struct)
 class(rsfFile) :: struct
 type(hypercube) :: hy,hy2
 type(axis) :: ax
 type(axis), pointer ::axes(:)
 character(len=1024) :: fle
  character(len=20) :: name

 integer :: iax,ierr
 hy=struct%getHyper()
 allocate(axes(hy%getNdim()))
 call hy%getAxes(axes)
! axes=hyper%getAxes()
 fle=struct%getFileName()
 do iax=1,hy%getNdim()
  write (name,"(a,i1)") "n",iax
   call to_par(struct%myf,name,axes(iax)%n)
      write (name,"(a,i1)") "o",iax
   call to_par(struct%myf,name,axes(iax)%n)
      write (name,"(a,i1)") "d",iax
   call to_par(struct%myf,name,axes(iax)%d)
      write (name,"(a,i1)") "label",iax
   call to_par(struct%myf,name,axes(iax)%label)
 end do
 deallocate(axes)
 end subroutine


subroutine sfReadDescription(struct)
 class(rsfFile) :: struct
 type(hypercube) :: hyper
 type(axis), pointer :: axes(:)
 integer :: ndim,iax,ierr,i
 character(len=20) :: name
 do i=1,9
     write (name,"(a,i1)") "n",i
    call from_par(struct%myf,name,ierr,1)
    if(ierr/=1) ndim=i
 end do
 allocate(axes(ndim))
 do iax=1,ndim 
   write (name,"(a,i1)") "n",i
   call from_param(struct%myf,name,axes(iax)%n,1)
      write (name,"(a,i1)") "o",i
   call from_param(struct%myf,name,axes(iax)%n,0.)
      write (name,"(a,i1)") "d",i
   call from_param(struct%myf,name,axes(iax)%d,1.)
      write (name,"(a,i1)") "label",i
   call from_param(struct%myf,name,axes(iax)%label,"")
 end do
 hyper=hyp_axes(axes)
 call struct%setHyper(hyper)
 deallocate(axes)
end subroutine




subroutine sfGetInt(struct,arg,val,def)
   class(rsfFile) :: struct
   character(len=*),intent(in) :: arg
   integer,intent(inout):: val
   integer,optional :: def
  call from_par(struct%myf,arg,val,def)
 end subroutine

 
 subroutine sfGetFloat(struct,arg,val,def)
   class(rsfFile) :: struct
   character(len=*),intent(in) :: arg
   real,intent(inout) :: val
   real, optional :: def
  call from_par(struct%myf,arg,val,def)
 end subroutine



subroutine sfGetString(struct,arg,val,def)
   class(rsfFile) :: struct
   character(len=*),intent(in) :: arg
   character(len=*) ,intent(inout):: val
   character(len=*), optional :: def
   integer :: ierr 
  call from_par(struct%myf,arg,val,def)
 end subroutine
 

 subroutine sfGetBool(struct,arg,val,def)
   class(rsfFile) :: struct
   character(len=*) ,intent(in):: arg
   logical,intent(inout) :: val
   logical, optional :: def
   integer :: ierr
   call sf_error("get bool not implemented RSF fortran")
  !call from_par(struct%myf,arg,val,def)

 end subroutine


 subroutine sfGetInts(struct,arg,val,def)
   class(rsfFile) :: struct
   character(len=*),intent(in) :: arg
   integer,intent(inout) :: val(:)
   integer, optional :: def(:)
   integer :: ierr 
   integer :: tmp(10000)
  call from_par(struct%myf,arg,val,def)

 end subroutine

 
 subroutine sfGetFloats(struct,arg,val,def)
   class(rsfFile) :: struct
   character(len=*) ,intent(in):: arg
   real, dimension(:),intent(inout) :: val
   real,optional ::  def(:)
   call sf_error("get floats not implemented RSF fortran")

  !call from_par(struct%myf,arg,val,def)

 end subroutine
 
 
 subroutine scrwind1(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(out) :: vals(:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")
  vals=0
end subroutine

subroutine scrwind2(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(out) :: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")
  vals=0

end subroutine
subroutine scrwind3(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(out) :: vals(:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")
  vals=0

end subroutine

subroutine scrwind4(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(out) :: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")
  vals=0

end subroutine

subroutine scrwind5(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(out) :: vals(:,:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  vals=0
  call sf_error("Window write not implemented")

end subroutine

subroutine scwwind1(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(in) :: vals(:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine


subroutine scwwind2(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(in) :: vals(:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine


subroutine scwwind3(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(in) :: vals(:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine


subroutine scwwind4(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex,intent(in) :: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine


subroutine scwwind5(struct,vals,nw,fw,jw)
  class(rsfFile) :: struct
  complex ,intent(in):: vals(:,:,:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  call sf_error("Window write not implemented")

end subroutine

subroutine scwriteF1(struct,vals,sz)
  class(rsfFile) :: struct
  complex,intent(in) :: vals(:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
end subroutine

subroutine scwriteF2(struct,vals,sz)
  class(rsfFile) :: struct
  complex,intent(in) :: vals(:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
end subroutine

subroutine scwriteF3(struct,vals,sz)
  class(rsfFile) :: struct
  complex ,intent(in):: vals(:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
end subroutine

subroutine scwriteF4(struct,vals,sz)
  class(rsfFile) :: struct
  complex,intent(in) :: vals(:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
end subroutine

subroutine scwriteF5(struct,vals,sz)
  class(rsfFile) :: struct
  complex ,intent(in):: vals(:,:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_write(struct%myf,vals)
end subroutine


subroutine screadF1(struct,vals,sz)
  class(rsfFile) :: struct
  complex ,intent(out):: vals(:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine


subroutine screadF2(struct,vals,sz)
  class(rsfFile) :: struct
  complex ,intent(out) :: vals(:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine



subroutine screadF3(struct,vals,sz)
  class(rsfFile) :: struct
  complex ,intent(out) :: vals(:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine


subroutine screadF4(struct,vals,sz)
  class(rsfFile) :: struct
  complex ,intent(out) :: vals(:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine


subroutine screadF5(struct,vals,sz)
  class(rsfFile) :: struct
  complex ,intent(out) :: vals(:,:,:,:,:)
  integer(kind=8),optional :: sz
  integer(kind=8) :: sz2
  if(present(sz)) then
    sz2=sz
  else
    sz2=size(vals)
  end if
  call rsf_read(struct%myf,vals)
end subroutine


 
 
 
end module

      
