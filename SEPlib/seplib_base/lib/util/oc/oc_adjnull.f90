module oc_adjnull_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================
  use oc_file_mod
  use oc_filealgebra_mod

  interface adjnull
     module procedure R_adjnull
     module procedure C_adjnull
     module procedure oc_adjnull
  end interface

contains
  !----------------------------------------------------------------
  subroutine R_adjnull(adj,add, x,yy)
    logical,               intent(in)    :: adj,add
    real,    dimension(:), intent(inout) :: x, yy
    if( .not. add) then 
       if( adj) then
          x = 0.
       else
          yy = 0.
       end if
    end if
  end subroutine R_adjnull
  !----------------------------------------------------------------
  subroutine C_adjnull(adj,add, x,yy)
    logical,               intent(in)    :: adj,add
    complex, dimension(:), intent(inout) :: x, yy
    if( .not. add) then 
       if( adj) then
          x = (0.,0.)
       else
          yy = (0.,0.)
       end if
    end if
  end subroutine C_adjnull
!!$<
!!$=head1 NAME
!!$
!!$oc_adjnull - perform adjnull on tags
!!$
!!$
!!$=head1 SYNOPSIS
!!$
!!$call oc_adjnull (adj,add,mod,data,maxmem)
!!$
!!$=head1 PARAMETERS
!!$
!!$=over 4
!!$
!!$
!!$=item adj,add,mod,data - Standatad gee adj,add parameters (x,y tag)
!!$
!!$=item maxmem - integer, optional 
!!$
!!$Maximum memory to use for blocking operation
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ Zeros output (defined by  adj) if add=.false.
!!$
!!$=head1 LIBRARY
!!$
!!$sepocf90
!!$
!!$=cut
!!$
!!$>
  !----------------------------------------------------------------
  subroutine oc_adjnull(adj,add, x_,yy_, maxmem)
    logical,            intent(in) :: adj,add
    character(len=128), intent(in) :: x_, yy_
    integer,            optional   :: maxmem
    type(fileinfo)                 :: file

    if( .not. add) then
       if (adj) then

          call oc_allocatefile(file,x_,maxmem); call sseek(x_,0,0)
          if (file%esize /=8) then
             call oc_assign(x_,    0. ,maxmem)
          else
             call oc_assign(x_,(0.,0.),maxmem)
          end if
          call oc_deallocatefile(file)

       else

          call oc_allocatefile(file,yy_,maxmem); call sseek(yy_,0,0)
          if (file%esize /=8) then
             call oc_assign(yy_,    0. ,maxmem)
          else
             call oc_assign(yy_,(0.,0.),maxmem)
          end if
          call oc_deallocatefile(file)

       end if
    end if
  end subroutine oc_adjnull
  !----------------------------------------------------------------  
end module oc_adjnull_mod
