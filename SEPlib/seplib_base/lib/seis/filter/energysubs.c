/*$

=head1 NAME

 energy - calculate energy
 
=head1 SYNOPSIS

C<ierr= energy (intag,outtag,data,data2,n1,n2,n3,lwind,j1,n1new,norm,ave)>

=head1 INPUT PARAMETERS

=over 4

=item intag- char*       

      Input tag

=item outtag - char*       

      Output tag

=item data - real(n1,n2) 

      Input offset-time gather

=item data2 - real(n1new,n2) 

      Output velocity-time gather

=item n1- integer     

      Input length1

=item n2 - integer     

      Length axis2

=item n3 - integer     

      Length axis3

=item lwind - integer     

      Length of energy window

=item j1- integer     

      Spacing betweew centers of energy windows

=item n1new - integer     

      Output fast axis length

=item norm - integer     

      Flag for normalizing energy (0,1)

=item ave - integer     

      Flag for normalizing window length

=back

=head1 DESCRIPTION

  Subroutine to calculate energy in running windows along fast data axis

=head1 SEE ALSO

B<Energy>

=head1 LIBRARY

B<sepfilter>


=cut

>*/

/*

  Author: Paul Fowler
  Date: March 1986

  July 1986 updated argument lists for vector calls to match new library
            conventions -Paul Fowler
  July 1986 added normalization by window length -Paul Fowler
	8-99 Bob converted to C
*/

#include<sepfilter.h>
#include<seplib.h>

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int energy(char *intag,char *outtag,float *data, float *data2,int n1, int n2,
 int n3,int lwind,int j1, int n1new,int norm,int ave)
_XFUNCPROTOEND
#else
int energy(intag,outtag,data,data2,n1,n2,n3,lwind,j1,n1new,norm,ave)
char *intag, *outtag;
float *data, *data2;
int n1,n2,n3,lwind, j1,n1new,norm, ave;
#endif
{

      int start;/*	!start of energy window */
      int i0;
      float big	;/*!largest energy value calculated */
			int nbyte1,nbyte2;
			int i3,i1,count,i2;
      nbyte1 = n1*n2*4;
      nbyte2 = n1new*n2*4;

			for(i3=0; i3 < n3; i3++){

         count = sreed( intag, data, nbyte1 );/*	!read in a gather*/
         if( count != nbyte1 ) seperr("Energy: unexpected end of data\n");

				
			for(i1=0; i1 < n2*n1; i1++){
				 data2[i1]=0;
				 data[i1]=data[i1]*data[i1];
			}

      big = 0.0;
			for(i2=0;i2< n2; i2++){
				for(i1=0;i1< n1new; i1++){
           start = (i1-1)*j1 ;
           data2[i1+i2*n1new]=0.;
					for(i0=start;i0< lwind+start; i0++)data2[i1+i2*n1new]+=data[i0+i2*n1];
					data2[i1+i2*n1new]=data[i1+i2*n1new]/lwind;
					big=MAX(big,data2[i1+i2*n1new]);
				}
			}
			if(ave==1){ /*normalize by length of window*/
				for(i1=0;i1< n2; i1++)
					for(i0=0;i0< n1new; i0++)
						data2[i0+i1*n1new]=data2[i0+i1*n1new]/lwind;
				big=big/lwind;
			}
			if(norm==1 && big!=0.0){/*normalize to max=1.0*/
				for(i1=0;i1<n2;i1++){
					for(i0=0;i0<n1new;i0++){
						data2[i0+i1*n1new]=data2[i0+i1*n1new]/big;
					}
				}
			}
			if(nbyte2!=srite(outtag,data2,nbyte2))
				seperr("trouble writting to tag %s \n",outtag);


	}
	return(0);
}
