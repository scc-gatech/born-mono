module generic_c_stubs
use, intrinsic :: iso_c_binding
implicit none


interface
subroutine initializeIO(nargs,nlens,argv) bind(c,name="initializeIO")
import
integer(c_int),intent(in),value :: nargs
integer(c_int),intent(in),dimension(*) :: nlens
character(c_char),dimension(*),intent(in) :: argv

end subroutine

integer function strlen(strin) bind(c,name="strlen")
  import
  character(C_CHAR),dimension(*) :: strin
end function
subroutine openRegFile(ioType,tag,usg) bind(c,name="openRegFile")
 import

 character(c_char), dimension(*),intent(in) :: tag,usg,ioType
end subroutine
   
subroutine openIrregFile(ioType,tag,usg) bind(c,name="openIrregFile")
import

  character(c_char), dimension(*),intent(in) :: tag,usg,ioType
end subroutine
 
subroutine closeIO() bind(c,name="closeIO")
import

end subroutine

subroutine getParamIntC(ioType,par,val,def) bind(c,name="getParamInt")
import

  character(c_char), dimension(*),intent(in) :: par,ioType
  integer(c_int),intent(inout) :: val
  integer(c_int) ,value,intent(in) :: def
end subroutine

subroutine getParamFloatC(ioType,par,val,def) bind(c,name="getParamFloat")
import

  character(c_char), dimension(*),intent(in) :: par,ioType
  real(c_float) ,intent(inout):: val
  integer(c_int),value,intent(in)  :: def
end subroutine

subroutine getParamBoolC(ioType,par,val,def) bind(c,name="getParamBool")
import

  character(c_char), dimension(*),intent(in) :: par,ioType
  integer(c_int) :: val
  integer(c_int),intent(in),value  :: def
end subroutine

subroutine getParamStringC(ioType,par,val,def) bind(c,name="getParamString")
import

  character(c_char), intent(in),dimension(*) :: par,ioType
  character(c_char) ,dimension(*),intent(inout):: val
  integer(c_int) ,value,intent(in) :: def
end subroutine

subroutine getParamIntsC(ioType,par,val,def) bind(c,name="getParamInts")
import

  character(c_char), dimension(*),intent(in) :: par,ioType
  integer(c_int),intent(inout),dimension(*) :: val
  integer(c_int) ,value,intent(in) :: def
end subroutine

subroutine getParamFloatsC(ioType,par,val,def) bind(c,name="getParamFloats")
import

  character(c_char), dimension(*),intent(in) :: par,ioType
  real(c_float) ,intent(inout),dimension(*):: val
  integer(c_int),value,intent(in)  :: def
end subroutine

subroutine getFileInt(ioType,myf,par,val,def) bind(c,name="getFileInt")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  integer(c_int),intent(inout) :: val
  integer(c_int) ,value,intent(in) :: def
end subroutine

subroutine getFileFloat(ioType,myf,par,val,def) bind(c,name="getFileFloat")

import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  real(c_float) ,intent(inout):: val
  integer(c_int),value,intent(in)  :: def
end subroutine

subroutine getFileBool(ioType,myf,par,val,def) bind(c,name="getFileBool")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  integer(c_int),intent(in) :: val
  integer(c_int),value  :: def
end subroutine

subroutine getDefaultIOName(ioType) bind(c,name="getDefaultIOName")
import
  character(c_char), dimension(*),intent(out) :: ioType
end subroutine

subroutine getFileString(ioType,myf,par,val,def) bind(c,name="getFileString")
import
  character(c_char), intent(in),dimension(*) :: myf,par,ioType
  character(c_char) ,dimension(*),intent(inout):: val
  integer(c_int) ,value,intent(in) :: def
end subroutine

subroutine getFileInts(ioType,myf,par,nsz,val,def) bind(c,name="getFileInts")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  integer(c_int),intent(inout),dimension(*) :: val
  integer,intent(in) :: nsz
  integer(c_int) ,value,intent(in) :: def
end subroutine

subroutine getFileFloats(ioType,myf,par,nsz,val,def) bind(c,name="getFileFloats")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  integer,intent(in) :: nsz
  real(c_float) ,intent(inout),dimension(*):: val
  integer(c_int),value,intent(in)  :: def
end subroutine

subroutine putFileInt(ioType,myf,par,val) bind(c,name="putFileInt")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  integer(c_int),intent(in) :: val
end subroutine

subroutine putFileFloat(ioType,myf,par,val) bind(c,name="putFileFloat")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  real(c_float) ,intent(in):: val
end subroutine

subroutine putFileBool(ioType,myf,par,val) bind(c,name="putFileBool")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  integer(c_int),intent(in) :: val
end subroutine

subroutine putFileString(ioType,myf,par,val) bind(c,name="putFileString")
import
  character(c_char), intent(in),dimension(*) :: myf,par,ioType
  character(c_char) ,dimension(*),intent(in):: val
end subroutine

subroutine putFileInts(ioType,myf,par,num,val) bind(c,name="putFileInts")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  integer(c_int),intent(in),dimension(*) :: val
  integer,intent(in),value :: num
end subroutine

subroutine putFileFloats(ioType,myf,par,num,val) bind(c,name="putFileFloats")
import
  character(c_char), dimension(*),intent(in) :: myf,par,ioType
  real(c_float) ,intent(in),dimension(*):: val
  integer,intent(in) ,value:: num
end subroutine


subroutine readFloatStream(ioType,myf,nsz,val) bind(c,name="readFloatStream")
import
  character(c_char), dimension(*),intent(in) :: myf,ioType
  real(c_float) ,intent(out),dimension(*):: val
 integer(c_long_long) ,value, intent(in) :: nsz
end subroutine

subroutine writeFloatStream(ioType,myf,nsz,val) bind(c,name="writeFloatStream")
import
  character(c_char), dimension(*),intent(in) :: myf,ioType
  real(c_float) ,intent(in),dimension(*):: val
 integer(c_long_long) ,value, intent(in) :: nsz
end subroutine

subroutine writeFloatWindow(ioType,myf,nw,fw,jw,val) bind(c,name="writeFloatWindow")
import
  character(c_char), dimension(*),intent(in) :: myf,ioType
  real(c_float) ,intent(in),dimension(*):: val
  integer(c_int),intent(in),dimension(*) :: nw,fw,jw
end subroutine

subroutine readFloatWindow(ioType,myf,nw,fw,jw,val) bind(c,name="readFloatWindow")
import
  character(c_char), dimension(*),intent(in) :: myf,ioType
  real(c_float) ,intent(out),dimension(*):: val
  integer(c_int),intent(in),dimension(*) :: nw,fw,jw
end subroutine



subroutine readComplexStream(ioType,myf,nsz,val) bind(c,name="readComplexStream")
import
  character(c_char), dimension(*),intent(in) :: myf,ioType
  complex(C_FLOAT_COMPLEX) ,intent(out),dimension(*):: val
 integer(c_long_long) ,value, intent(in) :: nsz
end subroutine

subroutine writeComplexStream(ioType,myf,nsz,val) bind(c,name="writeComplexStream")
import
  character(c_char), dimension(*),intent(in) :: myf,ioType
  complex(C_FLOAT_COMPLEX) ,intent(in),dimension(*):: val
 integer(c_long_long),value , intent(in) :: nsz
end subroutine

subroutine writeComplexWindow(ioType,myf,nw,fw,jw,val) bind(c,name="writeComplexWindow")
import
  character(c_char), dimension(*),intent(in) :: myf,ioType
  complex(C_FLOAT_COMPLEX) ,intent(in),dimension(*):: val
  integer(c_int),intent(in),dimension(*) :: nw,fw,jw
end subroutine

subroutine readComplexWindow(ioType,myf,nw,fw,jw,val) bind(c,name="readComplexWindow")
import
  character(c_char), dimension(*),intent(in) :: myf,ioType
  complex(C_FLOAT_COMPLEX) ,intent(out),dimension(*):: val
  integer(c_int),intent(in),dimension(*) :: nw,fw,jw
end subroutine





subroutine readDescriptionC(ioType,name) bind(c,name="readDescription")
import
 character(c_char), dimension(*),intent(in) :: name,ioType
end subroutine


subroutine writeDescriptionC(ioType,name) bind(c,name="writeDescription")
import
 character(c_char), dimension(*),intent(in) :: name,ioType
end subroutine

subroutine getDataType(ioType,name,typ) bind(c,name="getDataType")
import
 character(c_char), dimension(*),intent(in) :: name,ioType
  character(c_char), dimension(*),intent(out) :: typ

end subroutine

subroutine setDataType(ioType,name,typ) bind(c,name="setDataType")
import
 character(c_char), dimension(*),intent(in) :: name,typ,ioType
end subroutine
subroutine cerror(ioName,er) bind(c,name="cError")
import
 character(c_char), dimension(*),intent(in) :: er,ioName
end subroutine


subroutine getHyper(ioType,tag,ndimin,ndimout,n,o,d,nlens,label) bind(c,name="getHyper")
import
character(c_char),intent(in) :: tag,ioType
character(c_char),dimension(*),intent(out) :: label
real(c_float),dimension(*),intent(out) :: o,d
integer(c_int),dimension(*), intent(out) :: n,nlens
integer(c_int),intent(out)  ::ndimout
integer(c_int),value,intent(in) :: ndimin
end subroutine



subroutine setHyper(ioType,tag,ndimout,n,o,d,nlens,label) bind(c,name="setHyper")
import
character(c_char),dimension(*),intent(in) :: label,tag,ioType
real(c_float),dimension(*),intent(in) :: o,d
integer(c_int),dimension(*), intent(in) :: n,nlens
integer(c_int),value,intent(in)  ::ndimout
end subroutine
end interface

contains 
subroutine c2forstr2(str)
 character(len=*) :: str
 integer ::n,i

 n=strlen(str)
   do i=n+1,len(str)
     str(i:i)=" "
   end do
end subroutine



end module
