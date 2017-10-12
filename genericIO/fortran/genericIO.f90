module generic_io_mod
 use generic_file_mod 
 implicit none
 
 type,abstract :: genericIO
    class(genericRegFile),pointer :: regFile
    class(paramObj),pointer       :: param
    character(len=128) :: myType
   contains
     procedure(genericInit),deferred,pass :: init
     procedure, pass ::  initRegFile
 end type
  abstract interface
    subroutine genericInit(struct,IO)
      import genericIO
      class(genericIO) :: struct
      character(len=*) :: IO
    end subroutine

  end  interface
  contains
    subroutine initRegFile(struct,fle)
      class(genericRegFile) :: fle
      class(genericIO)      :: struct
        fle%myIOName=struct%myType
    end subroutine
end module
