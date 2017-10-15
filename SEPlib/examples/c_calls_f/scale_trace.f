
      subroutine scale_trace(n,data,scale)
      integer i,n
      real data ( n)
      real scale
        do 15 i=1,n
          data(i)=data(i)*scale
15      end do

      return
      end
