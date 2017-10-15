#<
#scale_it
#
#Usage:
#scale_it.x  <in.H >out.H  scale=
#
#Input Parameters:
# scale - real scale value
#
#
#Description:
# Intro program taht simply scles the dataset
#
#
#>
#%end of self-documentation
#-------------------------------------------------
#
#Author: Robert Clapp, ESMB 463, 7230253
#
#Date Created:Mon Jul  7 16:22:44 PDT 1997
#
#Purpose: 
#
#
integer n1,n2 ,i1,esize,ndim,logic,mem
real o,d
character*128  temp_ch,label
integer sep_get_number_data_axes,tempi
integer sep_get_data_axis_par
 
#call to get information from the history file
from history: integer esize=4

#error checking mechanism
if(esize !=4 && esize !=8)  call erexit("Unacceptable esize value")

#Get the number of data axes
if(0 != sep_get_number_data_axes("in",ndim)) 
	call erexit("Trouble obtaining number of data axes")

n2=1; mem=1;

from history: integer n1

#read in cube dimensions, calculating what size subcube we
#  can reasonably read in at one time 

do i1=2,ndim{
	if(0 != sep_get_data_axis_par("in",i1,tempi,o,d,label)) 
 		call erexit("trouble obtaining dimension the dataset ")
 	if(mem * tempi < 2000000 &&  logic==0) {
   	mem=mem*tempi
		n1=n1*tempi;
 	}else{
    logic=1 
		n2=n2*tempi;
	}
}
#fake a real dataset if data is complex 
n1=n1*esize/4




#allocate storage array 
allocate: real data(n1)
subroutine scale_it(n1,n2,data)
integer i1,i2,n1,n2
real data(n1)
real scale

from par: real scale

	#close the output history file
	call hclose()

	#loop over calculated subcubes
	do i2=1,n2{
		call sreed("in",data,n1*4)
  	#scale the data 
		do i1=1,n1
		  data(i1)=data(i1)*scale
  	#write out data  to stdout ("out")
  	call srite("out", data , n1 *4)
	}

	return;end
