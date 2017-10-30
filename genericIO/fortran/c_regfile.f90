module C_regfile_mod
 use generic_file_mod
 use generic_c_stubs


 

 implicit none
 
 type,extends(genericRegFile) :: cFile
 
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
      procedure:: getHyper=>sGetHyper
      procedure:: setHyper=>sSetHyper
      procedure:: readDescription=>sfReadDescription
      procedure:: writeDescription=>sfWriteDescription
      procedure::  err=>sfError
      procedure:: initFile=>initFileC
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

subroutine initFileC(struct,tag,mode)
  character(len=*),intent(in) :: tag,mode
  class(cFile) :: struct
  integer :: ierr
  
  call struct%setFileName(tag)
  call openRegFile(trim(struct%myIOName)//C_NULL_CHAR,trim(tag)//C_NULL_CHAR,mode)
  if(trim(mode)=="IN") call struct%readDescription()
  

end subroutine 
      
    

subroutine sfrwind1(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real,intent(out) :: vals(:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   call readFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)
end subroutine


subroutine sfrwind2(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real ,intent(out):: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   call readFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)
end subroutine
subroutine sfrwind3(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real ,intent(out):: vals(:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call readFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)
end subroutine

subroutine sfrwind4(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real,intent(out) :: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call readFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)
end subroutine

subroutine sfrwind5(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real,intent(out):: vals(:,:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call readFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)
end subroutine

subroutine sfwwind1(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real,intent(in) :: vals(:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  writeFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)
end subroutine


subroutine sfwwind2(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real,intent(in) :: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   call  writeFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine


subroutine sfwwind3(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real ,intent(in):: vals(:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   call  writeFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine


subroutine sfwwind4(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real ,intent(in):: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  writeFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine


subroutine sfwwind5(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  real,intent(in) :: vals(:,:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  writeFloatWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine

subroutine sfwriteF1(struct,vals,sz)
  class(cFile) :: struct
  real,intent(in) :: vals(:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)

end subroutine

subroutine sfwriteF2(struct,vals,sz)
  class(cFile) :: struct
  real,intent(in) :: vals(:,:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine

subroutine sfwriteF3(struct,vals,sz)
  class(cFile) :: struct
  real ,intent(in):: vals(:,:,:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine

subroutine sfwriteF4(struct,vals,sz)
  class(cFile) :: struct
  real,intent(in) :: vals(:,:,:,:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine

subroutine sfwriteF5(struct,vals,sz)
  class(cFile) :: struct
  real,intent(in) :: vals(:,:,:,:,:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine sfreadF1(struct,vals,sz)
  class(cFile) :: struct
  real ,intent(out):: vals(:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine sfreadF2(struct,vals,sz)
  class(cFile) :: struct
  real,intent(out) :: vals(:,:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine



subroutine sfreadF3(struct,vals,sz)
  class(cFile) :: struct
  real,intent(out) :: vals(:,:,:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine sfreadF4(struct,vals,sz)
  class(cFile) :: struct
  real ,intent(out):: vals(:,:,:,:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine sfreadF5(struct,vals,sz)
  class(cFile) :: struct
  real,intent(out) :: vals(:,:,:,:,:)
  integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readFloatStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine sfPutLogical(struct,arg,val)
  class(cFile) :: struct
  character(len=*),intent(in) :: arg
  logical ,intent(in) :: val
  integer ::x 
  x=0
  if(val) x=1
  call putFileBool(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,x)

end subroutine

subroutine sfPutString(struct,arg,val)
  class(cFile) :: struct
  character(len=*) ,intent(in):: arg
  character(len=*),intent(in) :: val
  call putFileString(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,val)

end subroutine


subroutine sfPutInt(struct,arg,val)
  class(cFile) :: struct
  character(len=*),intent(in) :: arg
  integer,intent(in) :: val
  call putFileInt(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,val)

end subroutine

subroutine sfPutReal(struct,arg,val)
  class(cFile) :: struct
  
  character(len=*),intent(in) :: arg
  real,intent(in) :: val
  call putFileFloat(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,val)

end subroutine

subroutine sfPutReals(struct,arg,val)
  class(cFile) :: struct
  character(len=*) ,intent(in):: arg
  real,intent(in) :: val(:)
  integer :: nsz
  nsz=size(val)
  call putFileFloats(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,nsz,val)

end subroutine

subroutine sfPutInts(struct,arg,val)
  class(cFile) :: struct
  character(len=*),intent(in) :: arg
  integer,intent(in) :: val(:)
  integer :: nsz
  nsz=size(val)
  call putFileInts(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,nsz,val)

end subroutine

   subroutine sfError(struct,arg)
   class(cFile) :: struct
   character(len=*),intent(in) :: arg
   call cError(trim(struct%myIOName)//C_NULL_CHAR,arg)
 end subroutine

subroutine sfWriteDescription(struct)
 class(cFile) :: struct

 call writeDescriptionC(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR)
 

 end subroutine


subroutine sfReadDescription(struct)
 class(cFile) :: struct
 type(hypercube) :: hyper

  call readDescriptionC(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR)
end subroutine

subroutine sGetInt(struct,arg,val,def)
   class(cFile) :: struct
   character(len=*) ,intent(in):: arg
   integer,intent(inout):: val
   integer,optional :: def
   integer :: x
   x=0
   if(present(def)) x=1
   if(present(def)) val=def
   call getFileInt(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,val,x)

 end subroutine
 
 subroutine sGetFloat(struct,arg,val,def)
   class(cFile) :: struct
   character(len=*) ,intent(in) :: arg
   real,intent(inout) :: val
   real, optional :: def
   integer ::x
   x=0;
   if(present(def)) then  
      val=def
      x=1
   end if

   call getFileFloat(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,val,x)
 end subroutine



subroutine sGetString(struct,arg,val,def)
   class(cFile) :: struct
   character(len=*),intent(in) :: arg
   character(len=*),intent(inout) :: val
   character(len=*), optional :: def
   integer :: x
   x=0;
   if(present(def)) then
     x=1
      val=def
   end if
   call getFileString(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,val,x)
     call c2forstr2(val)
 end subroutine
 
 subroutine sGetBool(struct,arg,val,def)
   class(cFile) :: struct
   character(len=*),intent(in) :: arg
   logical ,intent(inout):: val
   logical, optional :: def
   integer :: x,x2
   x=0;
   if(present(def)) then
     x=1
     x2=0
     if(def) x2=1;
      val=def
   end if
   call getFileBool(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,x2,x)
   val=.false.
   if(x2==1) val=.true.
   
 end subroutine


 subroutine sGetInts(struct,arg,val,def)
   class(cFile) :: struct
   character(len=*) ,intent(in):: arg
   integer, dimension(:),intent(inout):: val
   integer, optional :: def(:)
   integer :: sz
      integer :: x
   x=0;
   if(present(def)) then
     x=1
      val=def
   end if
   sz=size(val)
   call getFileInts(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,trim(arg)//C_NULL_CHAR,&
     sz,val,x)

 end subroutine
 
 
 
 subroutine sGetFloats(struct,arg,val,def)
   class(cFile) :: struct
   character(len=*),intent(in) :: arg
   real ,intent(inout):: val(:)
   real,optional ::  def(:)
   integer :: x
   x=0;
   if(present(def)) then
     x=1
      val=def
   end if
   call getFileFloats(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,&
     trim(arg)//C_NULL_CHAR,size(val),val,x)
 end subroutine
 

 
 
 subroutine scrwind1(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(out) :: vals(:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
  
   call  readComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,&
     trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)
  

end subroutine

subroutine scrwind2(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(out) :: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  readComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine
subroutine scrwind3(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(out) :: vals(:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   call  readComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine

subroutine scrwind4(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(out) :: vals(:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  readComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine

subroutine scrwind5(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(out) :: vals(:,:,:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  readComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine

subroutine scwwind1(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(in) :: vals(:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  writeComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine


subroutine scwwind2(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(in) :: vals(:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
  integer,allocatable :: ng(:)
   call  writeComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine


subroutine scwwind3(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(in) :: vals(:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  writeComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine


subroutine scwwind4(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex,intent(in) :: vals(:,:,:,:)
  integer ,intent(in):: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  writeComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine


subroutine scwwind5(struct,vals,nw,fw,jw)
  class(cFile) :: struct
  complex ,intent(in):: vals(:,:,:,:,:)
  integer,intent(in) :: nw(:),fw(:),jw(:)
  type(hypercube) :: hy
   call  writeComplexWindow(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,nw,fw,jw,vals)

end subroutine

subroutine scwriteF1(struct,vals,sz)
  class(cFile) :: struct
  complex,intent(in) :: vals(:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine

subroutine scwriteF2(struct,vals,sz)
  class(cFile) :: struct
  complex,intent(in) :: vals(:,:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine

subroutine scwriteF3(struct,vals,sz)
  class(cFile) :: struct
  complex ,intent(in):: vals(:,:,:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine

subroutine scwriteF4(struct,vals,sz)
  class(cFile) :: struct
  complex,intent(in) :: vals(:,:,:,:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine

subroutine scwriteF5(struct,vals,sz)
  class(cFile) :: struct
  complex ,intent(in):: vals(:,:,:,:,:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  writeComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine screadF1(struct,vals,sz)
  class(cFile) :: struct
  complex ,intent(out):: vals(:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine screadF2(struct,vals,sz)
  class(cFile) :: struct
  complex ,intent(out) :: vals(:,:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine



subroutine screadF3(struct,vals,sz)
  class(cFile) :: struct
  complex ,intent(out) :: vals(:,:,:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine screadF4(struct,vals,sz)
  class(cFile) :: struct
  complex ,intent(out) :: vals(:,:,:,:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine


subroutine screadF5(struct,vals,sz)
  class(cFile) :: struct
  complex ,intent(out) :: vals(:,:,:,:,:)
 integer(kind=8), optional :: sz
  integer(kind=8) :: sz2
   if(present(sz)) then 
     sz2=sz
   else
     sz2=size(vals)
   end if
   call  readComplexStream(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,sz2,vals)
end subroutine

  subroutine to1DCharArray(two,nlens,nelem,one)
    integer,intent(in) :: nelem
    character(len=*),dimension(:),intent(in) :: two
    integer,intent(out) :: nlens(:)
    character(len=*) :: one
    integer i2,ipos
    ipos=0
    do i2=1,nelem
      nlens(i2)=len_trim(two(i2))
      one(ipos+1:ipos+nlens(i2))=trim(two(i2))
      ipos=ipos+nlens(i2)
    end do
    
  end subroutine
  
  subroutine from1DCharArray(one,nelem,nlens,two)
    integer,intent(in) :: nelem
    integer,intent(in) :: nlens(:)
    integer :: ipos,i2
    character(len=*),dimension(:),intent(out) :: two
    character(len=*), intent(in) :: one
    
    ipos=0;
    do i2=1,nelem
      two(i2)=one(ipos+1:ipos+nlens(i2))
        call c2forstr2(two(i2))
      ipos=ipos+nlens(i2)
   end do

 end subroutine 
 function sGetHyper(struct) result(hyper)
    class(cFile) :: struct
    type(hypercube) :: hyper
    integer :: n(15)
    real,dimension(15) :: o,d
    character(len=1000), dimension(15) :: label
    type(axis),dimension(:),allocatable :: axes
    integer :: lens(99)
    character(len=15000) :: tmp
    integer ::ndimout
    integer :: i


    call getHyper(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,15,ndimout,n,o,d,lens,tmp)
    call from1DCharArray(tmp,ndimout,lens,label)
    allocate(axes(ndimout))
    do i=1,ndimout
      axes(i)=createaxis(n(i),o(i),d(i),label(i))
    end do
    hyper=createHypercube(axes)
    deallocate(axes)

    
  end function
  subroutine sSetHyper(struct,hyp) 
    class(cFile) :: struct
    type(hypercube),intent(in) :: hyp
    integer :: ndim
     integer :: n(15),nlens(99)
    real,dimension(15) :: o,d
    integer :: i
    type(axis) :: ax
    character(len=1000), dimension(15) :: label
     character(len=15000) :: tmp
     
     ndim=hyp%getNdim()
     do i=1,hyp%getNdim()
         ax=hyp%getaxis(i)
         n(i)=ax%n
         o(i)=ax%o
         d(i)=ax%d
         label(i)=ax%label
    end do
    call to1DCharArray(label,nlens,hyp%getNdim(),tmp)
    call setHyper(trim(struct%myIOName)//C_NULL_CHAR,trim(struct%getFileName())//c_NULL_CHAR,ndim,n,o,d,nlens,tmp)
  end subroutine    
     
 
 
 
end module

      
