module generic_file_mod
  use hypercubeGen_mod
  use param_func_mod
  implicit none
  type,extends(genericParamObj),abstract :: genericRegFile
    private
      type(hypercube),pointer :: hyper=>null()
      character(len=1024) :: fle
  contains
    procedure(readDescription),deferred :: readDescription
    procedure(writeDescription),deferred :: writeDescription
    procedure :: setFileName=>set_file
    procedure :: getFileName=>get_filename
    procedure(putReal),deferred :: put_Real
    procedure(putInt),deferred :: put_Int
    procedure(putString),deferred :: put_String
    procedure(putLogical),deferred :: put_Logical
    procedure(putReals),deferred :: put_Reals
    procedure(putInts),deferred :: put_Ints
    procedure:: setHyper=>set_hyper
    procedure:: getHyper=>get_hyper
    procedure :: putAxis=>putAxis_d
    procedure(initFileBase)  ,deferred :: initFile
    procedure(readF1),deferred :: readF1
    procedure(readF2),deferred :: readF2
    procedure(readF3),deferred :: readF3
    procedure(readF4),deferred :: readF4
    procedure(readF5),deferred :: readF5
    procedure(writeF1),deferred :: writeF1
    procedure(writeF2),deferred :: writeF2
    procedure(writeF3),deferred :: writeF3
    procedure(writeF4),deferred :: writeF4
    procedure(writeF5),deferred :: writeF5
    procedure(rfwind1),deferred :: rfwind1
    procedure(rfwind2),deferred :: rfwind2
    procedure(rfwind3),deferred :: rfwind3
    procedure(rfwind4),deferred :: rfwind4
    procedure(rfwind5),deferred :: rfwind5
    procedure(wfwind1),deferred :: wfwind1
    procedure(wfwind2),deferred :: wfwind2
    procedure(wfwind3),deferred :: wfwind3
    procedure(wfwind4),deferred :: wfwind4
    procedure(wfwind5),deferred :: wfwind5
    
    procedure(readc1),deferred :: readc1
    procedure(readc2),deferred :: readc2
    procedure(readc3),deferred :: readc3
    procedure(readc4),deferred :: readc4
    procedure(readc5),deferred :: readc5
    procedure(writec1),deferred :: writec1
    procedure(writec2),deferred :: writec2
    procedure(writec3),deferred :: writec3
    procedure(writec4),deferred :: writec4
    procedure(writec5),deferred :: writec5
    procedure(rcwind1),deferred :: rcwind1
    procedure(rcwind2),deferred :: rcwind2
    procedure(rcwind3),deferred :: rcwind3
    procedure(rcwind4),deferred :: rcwind4
    procedure(rcwind5),deferred :: rcwind5
    procedure(wcwind1),deferred :: wcwind1
    procedure(wcwind2),deferred :: wcwind2
    procedure(wcwind3),deferred :: wcwind3
    procedure(wcwind4),deferred :: wcwind4
    procedure(wcwind5),deferred :: wcwind5
    generic :: readRealStream=>readF1,readF2,readF3,readF4,readF5,&
       readc1,readc2,readc3,readc4,readc5
    generic :: writeRealStream=>writeF1,writeF2,writeF3,writeF4,writeF5,&
      writec1,writec2,writec3,writec4,writec5
    generic:: readRealWindow=>RFwind1,RFwind2,rFwind3,rfwind4,rfwind5,&
      rcwind1,rcwind2,rcwind3,rcwind4,rcwind5
    generic:: writeRealWindow=>WFwind1,wFwind2,wFwind3,wFwind4,wfwind5,&
      wcwind1,wcwind2,wcwind3,wcwind4,wcwind5
  end type


! procedure(getBoolD),deferred,pass :: get_boolD
 !     procedure(getIntsN),deferred,pass :: get_IntsN
  !    procedure(getIntsD),deferred,pass :: get_IntsD
   !   procedure(getFloatsN),deferred,pass :: get_floatsN
    !  procedure(getFloatsD),deferred,pass :: get_floatsD
   !   procedure(error), deferred,pass:: err
   !   generic:: getInt=>get_intN,get_intD
   !   generic:: getReal=>get_floatN,get_floatD
   !   generic:: getLogical=>get_boolN,get_boolD  

  abstract interface
    subroutine initFileBase(struct,tag,mode)
      import genericRegFile
      class(genericRegFile) :: struct
      character(len=*) ,intent(in) :: tag,mode
    end subroutine
    subroutine readDescription(struct)
      import genericRegFile
      class(genericRegFile) :: struct
    end subroutine
     subroutine writeDescription(struct)
      import genericRegFile
      class(genericRegFile) :: struct
    end subroutine 
    subroutine rfwind1(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real, intent(out),dimension(:) :: vals
      integer :: sz
    end subroutine
    subroutine rfwind2(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real,intent(out), dimension(:,:) :: vals
      integer :: sz
    end subroutine
    subroutine rfwind3(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real, intent(out),dimension(:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine rfwind4(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real, intent(out),dimension(:,:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine rfwind5(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real, intent(out),dimension(:,:,:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine wfwind1(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real,intent(in), dimension(:) :: vals
      integer :: sz
    end subroutine
    subroutine wfwind2(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real, intent(in),dimension(:,:) :: vals
      integer :: sz
    end subroutine
    subroutine wfwind3(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real, intent(in),dimension(:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine wfwind4(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real,intent(in), dimension(:,:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine wfwind5(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      real, intent(in),dimension(:,:,:,:,:) :: vals
      integer :: sz
    end subroutine
  
  
  
    subroutine readF1(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real, intent(out),dimension(:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine readF2(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real, intent(out),dimension(:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine readF3(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real,intent(out), dimension(:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine readF4(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real, intent(out),dimension(:,:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine readF5(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real, intent(out),dimension(:,:,:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writeF1(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real, intent(in),dimension(:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writeF2(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real,intent(in), dimension(:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writeF3(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real, intent(in),dimension(:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writeF4(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real, intent(in),dimension(:,:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writeF5(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      real, intent(in),dimension(:,:,:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine putInts(struct,arg,val)
      import genericRegFile
      class(genericRegFile) :: struct
      character(len=*),intent(in) :: arg
      integer,dimension(:),intent(in) :: val
    end subroutine
      subroutine putReals(struct,arg,val)
      import genericRegFile
      class(genericRegFile) :: struct
      character(len=*),intent(in) :: arg
      real,dimension(:),intent(in) :: val
    end subroutine
      subroutine putString(struct,arg,val)
      import genericRegFile
      class(genericRegFile) :: struct
      character(len=*),intent(in) :: arg
      character(len=*),intent(in) :: val
    end subroutine
    subroutine putReal(struct,arg,val)
      import genericRegFile
      class(genericRegFile) :: struct
      character(len=*),intent(in) :: arg
      real,intent(in) :: val
    end subroutine
    subroutine putLogical(struct,arg,val)
      import genericRegFile
      class(genericRegFile) :: struct
      character(len=*) ,intent(in):: arg
      logical,intent(in) :: val
    end subroutine
    subroutine putInt(struct,arg,val)
      import genericRegFile
      class(genericRegFile) :: struct
      character(len=*),intent(in) :: arg
      integer ,intent(in):: val
    end subroutine
    
    
    subroutine rcwind1(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex,intent(out), dimension(:) :: vals
      integer :: sz
    end subroutine
    subroutine rcwind2(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex,intent(out), dimension(:,:) :: vals
      integer :: sz
    end subroutine
    subroutine rcwind3(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex,intent(out), dimension(:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine rcwind4(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex,intent(out), dimension(:,:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine rcwind5(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex,intent(out), dimension(:,:,:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine wcwind1(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex,intent(in), dimension(:) :: vals
      integer :: sz
    end subroutine
    subroutine wcwind2(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex, intent(in),dimension(:,:) :: vals
      integer :: sz
    end subroutine
    subroutine wcwind3(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex, intent(in),dimension(:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine wcwind4(struct,vals,nw,fw,jw)
      import genericRegFile
      integer,intent(in) :: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex,intent(in), dimension(:,:,:,:) :: vals
      integer :: sz
    end subroutine
    subroutine wcwind5(struct,vals,nw,fw,jw)
      import genericRegFile
      integer ,intent(in):: nw(:),fw(:),jw(:)
      class(genericRegFile) :: struct
      complex, intent(in),dimension(:,:,:,:,:) :: vals
      integer :: sz
    end subroutine
  
  
  
    subroutine readc1(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(out),dimension(:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine readc2(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(out),dimension(:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine readc3(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(out),dimension(:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine readc4(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(out),dimension(:,:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine readc5(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(out),dimension(:,:,:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writec1(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(in),dimension(:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writec2(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(in),dimension(:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writec3(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex,intent(in), dimension(:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writec4(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(in),dimension(:,:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
    subroutine writec5(struct,vals,sz)
      import genericRegFile
      class(genericRegFile) :: struct
      complex, intent(in),dimension(:,:,:,:,:) :: vals
      integer(kind=8),optional :: sz
    end subroutine
end interface
contains
  function get_Hyper(struct) result(hyper)
    class(genericRegFile) :: struct
    type(hypercube) :: hyper
    type(axis) :: ax
    if(.not. associated(struct%hyper)) then
       call struct%err("no hypercube associated with file")
    end if
    hyper=createHypercube(struct%hyper)
  end function
  subroutine set_Hyper(struct,hyp) 
    class(genericRegFile) :: struct
    type(hypercube),intent(in) :: hyp
      if(associated(struct%hyper)) deallocate(struct%hyper)
      allocate(struct%hyper)
      struct%hyper=createHypercube(hyp)
  end subroutine
  function get_fileName(struct)
   class(genericRegFile) :: struct
   character(len=1024) :: get_fileName
   get_fileName=struct%fle
  end function
  subroutine set_File(struct,fle) 
    class(genericRegFile) :: struct
    character(len=*) :: fle
    struct%fle=fle;
  end subroutine


    
    subroutine putaxis_d(struct,iax,axD)
      class(genericRegFile) :: struct
      integer,intent(in) :: iax
      type(axis) :: axD
      character(len=10) :: str
      
       write(str,"(a,i1)") "n",iax
       call struct%put_Int(str,axD%n)
       write(str,"(a,i1)") "o",iax
        call struct%put_Real(str,axd%o)
       write(str,"(a,i1)") "d",iax
        call struct%put_Real(str,axd%d)    
       write(str,"(a,i1)") "label",iax
        call struct%put_String(str,axd%label)
    end subroutine

end module
