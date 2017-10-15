C<
Cscale_it
C
CUsage:
Cscale_it.x  <in.H >out.H  scale=
C
CInput Parameters:
C scale - real scale value
C
C
CDescription:
C Intro program taht simply scles the dataset
C
C
C>
C%end of self-documentation
C-------------------------------------------------
C
CAuthor: Robert Clapp, ESMB 463, 7230253
C
CDate Created:Mon Jul  7 16:22:44 PDT 1997
C
CPurpose:
C
C
      integer i1 , i2 , n1 , n2,n3
      real data ( 4096)
      real scale
      integer  hetch,getch 
      
      call initpar()
      call doc(source)
      call noieeeinterupt()
      idum=hetch('Gobbledegook','s','Lets get started.')

      if(hetch('n1','d',n1) .eq. 0) then
        call erexit("n1 must be supplied")
      end if
 
      if(n1 .gt. 4096) then
        call erexit("n1 can not be greater than 4096")
      end if

      if(hetch('n2','d',n2) .eq. 0) then
        n2=1
      end if

      if(hetch('n3','d',n3) .eq. 0) then
        n3=1
      end if

      n2=n2*n3

      if(getch('scale','r',scale).eq.0)then
         call erexit('need scale:scale')
      endif

      call putch('From par: scale = scale','r',scale)
      call hclose()

      do 20 i2=1,n2
        call sreed("in",data,n1*4)
      
        do 15 i1=1,n1
          data(i1)=data(i1)*scale
15      end do

        call srite("out", data , n1 *4)

20    end do
      end
