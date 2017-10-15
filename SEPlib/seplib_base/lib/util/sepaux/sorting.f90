module sorting_mod 
use sep_mod
use sep_func_mod
!!$<
!!$
!!$=head1 NAME
!!$
!!$ quick_sort - quick sort
!!$
!!$=head1 SYNOPSIS
!!$
!!$C<call quick_sort(rarray,key,n1,n2)>
!!$
!!$C<call quick_sort(iarray,key,n1,n2)>
!!$
!!$C<call quick_sort(rarray,key,n1)>
!!$
!!$=head1 INPUT PARAMETERS
!!$
!!$=over 4
!!$
!!$=item array - C<real(:,:)>
!!$
!!$       input array
!!$
!!$=item key   - integer
!!$
!!$      key to sort off of
!!$
!!$=item n1 - integer
!!$
!!$      size of data, first axis
!!$
!!$=item n2 - integer
!!$
!!$      size of data, second axis
!!$
!!$=back
!!$
!!$=head1 DESCRIPTION
!!$
!!$ In place quick sort routine for a variety of different data configurations
!!$
!!$
!!$=head1 LIBRARY
!!$
!!$B<sepauxf90>
!!$
!!$=cut
!!>
  implicit none
  interface quick_sort
  module procedure quick_sort_2d_i,quick_sort_2d_r,quick_sortr
  end interface
  contains
  subroutine quick_sort_2d_i(array,key,n1,ibeg,iend)
    integer, dimension(:,:) :: array
    integer :: n1,n2
    integer :: hold(n1),key,hold2(n1)
    integer, parameter :: nn=15,nstack=100
    integer :: k,i,j,jstack,l,r,ibeg,iend
    integer,dimension(nstack) :: istack
    call Qsorti(array,key,n1,ibeg,iend)
!    jstack=0
!    l=1
!    r=n2
!    do  
!      if (r-l < nn) then
!        do j=l+1,r 
!          hold=array(:,j)
!          do i=j-1,l,-1 
!            if (array(key,i) <= hold(key)) then
!              exit
!            end if
!            array(:,i+1)=array(:,i)
!          end do 
!          array(:,i+1)=hold
!        end do 
!        if (jstack .eq. 0) then
!          return
!        end if
!        r=istack(jstack)
!        l=istack(jstack-1)
!        jstack=jstack-2
!      else
!        k=(l+r)/2
!        hold=array(:,k)
!        array(:,k)=array(:,l+1)
!        array(:,l+1)=hold
!        if (array(key,l)>array(key,r)) then
!          hold=array(:,l)
!          array(:,l)=array(:,r)
!          array(:,r)=hold
!        end if
!        if (array(key,l+1)>array(key,r)) then
!          hold=array(:,l+1)
!          array(:,l+1)=array(:,r)
!          array(:,r)=hold
!        end if
!        if (array(key,l+1)<array(key,l)) then
!          hold=array(:,l+1)
!          array(:,l+1)=array(:,l)
!          array(:,l)=hold
!        end if
!        i=l+1
!        j=r
!        hold=array(:,l+1)
!        do 
!          do 
!            i=                                i+1
!            if (array(key,i) >= hold(key)) then
!              exit
!            end if
!          end do 
!          do 
!            j=                                j-1
!            if (array(key,j) <= hold(key)) then
!              exit
!            end if
!          end do 
!          if (j<i) then
!            exit
!          end if
!          hold2=array(:,i)
!          array(:,i)=array(:,j)
!          array(:,j)=hold2
!        end do 
!        array(:,l+1)=array(:,j)
!        array(:,j) = hold
!        jstack=                jstack+2
!        if (jstack > nstack) then
!          call erexit("need to increase size of stack in quick sort")
!        end if
!        if (r-i+1 >= j-1) then
!          istack(jstack)=r
!          istack(jstack-1)=i
!          r=j-1
!        else
!          istack(jstack)=j-1
!          istack(jstack-1)=l
!          l=i
!        end if
!      end if
!    end do
  end subroutine 
  subroutine quick_sort_2d_r(array,key,n1,n2)
    real, dimension(:,:) :: array
    integer :: n1,n2,key
    real :: hold(n1),hold2(n1)
    integer, parameter :: nn=15,nstack=100
    integer :: k,i,j,jstack,l,r
    integer,dimension(nstack) :: istack
    jstack=0
    l=1
    r=n2
    do  
      if (r-l < nn) then
        do j=l+1,r 
          hold=array(:,j)
          do i=j-1,l,-1 
            if (array(key,i) <= hold(key)) then
              exit
            end if
            array(:,i+1)=array(:,i)
          end do 
          array(:,i+1)=hold
        end do 
        if (jstack .eq. 0) then
          return
        end if
        r=istack(jstack)
        l=istack(jstack-1)
        jstack=jstack-2
      else
        k=(l+r)/2
        hold=array(:,k)
        array(:,k)=array(:,l+1)
        array(:,l+1)=hold
        if (array(key,l)>array(key,r)) then
          hold=array(:,l)
          array(:,l)=array(:,r)
          array(:,r)=hold
        end if
        if (array(key,l+1)>array(key,r)) then
          hold=array(:,l+1)
          array(:,l+1)=array(:,r)
          array(:,r)=hold
        end if
        if (array(key,l+1)<array(key,l)) then
          hold=array(:,l+1)
          array(:,l+1)=array(:,l)
          array(:,l)=hold
        end if
        i=l+1
        j=r
        hold=array(:,l+1)
        do 
          do 
            i=                                i+1
            if (array(key,i) >= hold(key)) then
              exit
            end if
          end do 
          do 
            j=                                j-1
            if (array(key,j) <= hold(key)) then
              exit
            end if
          end do 
          if (j<i) then
            exit
          end if
          hold2=array(:,i)
          array(:,i)=array(:,j)
          array(:,j)=hold2
        end do 
        array(:,l+1)=array(:,j)
        array(:,j) = hold
        jstack=                jstack+2
        if (jstack > nstack) then
          call erexit("need to increase size of stack in quick sort")
        end if
        if (r-i+1 >= j-1) then
          istack(jstack)=r
          istack(jstack-1)=i
          r=j-1
        else
          istack(jstack)=j-1
          istack(jstack-1)=l
          l=i
        end if
      end if
    end do
  end subroutine 
  subroutine quick_sortr(array,n1)
    real, dimension(:) :: array
    integer :: n1
    integer, parameter :: nn=15,nstack=100
    integer ::  k,i,j,jstack,l,r
    integer,dimension(nstack) :: istack
    real :: hold,hold2
    jstack=0
    l=1
    r=n1
    do  
!        write(0,*) "in do",r,l
      if (r-l < nn) then
        do j=l+1,r 
          hold=array(j)
          do i=j-1,l,-1 
            if (array(i) <= hold) then
              exit
            end if
            array(i+1)=array(i)
          end do 
          array(i+1)=hold
        end do 
        if (jstack .eq. 0) then
          return
        end if
        r=istack(jstack)
        l=istack(jstack-1)
        jstack=jstack-2
      else
        k=(l+r)/2
        hold=array(k)
        array(k)=array(l+1)
        array(l+1)=hold
        if (array(l)>array(r)) then
          hold=array(l)
          array(l)=array(r)
          array(r)=hold
        end if
        if (array(l+1)>array(r)) then
          hold=array(l+1)
          array(l+1)=array(r)
          array(r)=hold
        end if
        if (array(l+1)<array(l)) then
          hold=array(l+1)
          array(l+1)=array(l)
          array(l)=hold
        end if
        i=l+1
        j=r
        hold=array(l+1)
        do 
          do 
            i=                                i+1
            if (array(i) >= hold) then
              exit
            end if
          end do 
          do 
            j=                                j-1
            if (array(j) <= hold) then
              exit
            end if
          end do 
          if (j<i) then
            exit
          end if
          hold2=array(i)
          array(i)=array(j)
          array(j)=hold2
        end do 
        array(l+1)=array(j)
        array(j) = hold
        jstack=                jstack+2
        if (jstack > nstack) then
          call erexit("need to increase size of stack in quick sort")
        end if
        if (r-i+1 >= j-1) then
          istack(jstack)=r
          istack(jstack-1)=i
          r=j-1
        else
          istack(jstack)=j-1
          istack(jstack-1)=l
          l=i
        end if
      end if
    end do
  end subroutine 

recursive subroutine Qsorti(A,key,n1,ibeg,n2)
  integer, intent(in out), dimension(:,:) :: A
  integer :: iq,n1,n2,key,ibeg

  if(n2/=ibeg) then
     call Partitioni(A,key,n1, ibeg,n2,iq)
     call Qsorti(    A,key,n1, ibeg,iq-1)
     call Qsorti(    A,key,n1,   iq,n2 )
  endif
end subroutine Qsorti





subroutine Partitioni(A,ikey, n1,ibeg,iend,marker)
  integer, intent(in out), dimension(:,:) :: A
  integer, intent(in)   :: ibeg,iend
  integer, intent(out) :: marker
  integer :: i, j,n1
  integer :: x(n1),temp(n1),ikey! pivot point

  x = A(1:n1,ibeg)
  i= ibeg-1
  j= iend+1
  do
     j = j-1
     do
        if (A(ikey,j) <= x(ikey)) exit
        j = j-1
     end do
     i = i+1
     do
        if (A(ikey,i) >= x(ikey)) exit
        i = i+1
     end do
     if (i < j) then
        ! exchange A(i) and A(j)
        temp = A(1:n1,i)
        A(:n1,i) = A(1:n1,j)
        A(:n1,j) = temp
     elseif (i == j) then
        marker = i+1
        return
     else
        marker = i
        return
     endif
  end do
end subroutine Partitioni

end module 
