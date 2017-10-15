/*$

=head1 NAME

sgainpar -gain seismic data

=head1 SYNOPSIS


C<int sgainpar (tag,adata,ant,ahbytes,anx,adit,atpow,ait0,apclip,aphalf,aclip,agpow,adbias,adt,anp,againip)>



=head1 DESCRIPTION

 Subroutine to compute gain factors for seismic data all arguments 
 are addresses if any argument has the value zero, it reverts to its default.

=head1 PARAMETERS

=over 4

=item tag    -char*    

      tag of dataset, may be lseeked to appropiate place, default 0

=item data   -float*   

       save copy of data here, useful for pipes
       if 0, then data not saved, default 0

=item nt     -int*     

      raslength of data vector, default 1024

=item nx     -int*     

      number of vectors to examine, default all data or 256, 
      whichever is less

=item np     -int*     

      number of planes of data, (n3)  default 1

=item dit    -int*     

      integer separation between samples,
      default nt*nx / 262144, because only 262144 values 
      are sorted to find percentiles (for np=1)

=item tpow   -float*   

      multiply samples by ((it*dit+it0)*dt)**tpow, default 0

=item it0    -float*   

      time of first sample in units of samples, default 0

=item dbias  -float*   

      subtract dbias from the data

=item pclip  -float*   

       percentile for clipping seismic data, default 99

=item phalf  -float*   

      percentile for determining gpow, default 85

=item gainip -int*     

      plane where clip and gpow are estimated
      default 1 (the first plane is used)
      when -1, data of all planes are considered

=item d  -float*   

      sample interval of data

=item clip -float*   

      clip value

=item gpow    -float*   

      raise each data sample to this power,
      gpow = log (.5) / log (half / clip),
      if (half / clip < .001) gpow = 1. (detects synthetics)


=back

=head1 SEE ALSO

L<Grey>

=head1 LIBRARY

B<sepaux>

=cut

>*/
/*
 *
 * 			         
 * Revised      5-16-85    stew    check return codes from malloc
 *  		8-21-85    li      fix bug in estimating gpow when gainip=-1
 *  		11-21-85   stew    comment out brk(startit) - crashes CONVEX!
 *		1-7-86	   li      reduce computations of *tpow-table (when 
 *				   tpow=0.) and  clip (or half) when pclip=100.
 *		9-27-86	   stew    restored brk(startit) now that Convex fixed bug
 *              11-3-92   martin   use sreed, sseek, and malloc/free
 */

#include <math.h>
#include <seplib.h>
#include <sepaux.h>
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int sgainpar (char *tag,float *adata,int *ant,int *ahbytes,int *anx,int *adit,float *atpow,float *ait0,float *apclip,float *aphalf,float *aclip,float *agpow,float *adbias,float *adt,int *anp,int *againip)
_XFUNCPROTOEND
#else
int sgainpar (tag,adata,ant,ahbytes,anx,adit,atpow,ait0,apclip,aphalf,aclip,agpow,adbias,adt,anp,againip)
char *tag;
int *ant, *anx, *adit, *ahbytes, *anp, *againip;
float *adata, *atpow, *ait0, *apclip, *aphalf, *aclip, *agpow, *adbias, *adt;
#endif
{
	char * fildes;
        int nt,nx,dit,ix,it,nt1,hbytes,gainip,np,ip;
        float tpow,pclip,phalf,half,*data,*buf,it0,*bp,*tpow_table,dt;
	float *gpownp, *clipnp;
	int ntest, i;

        /* read parameters, set defaults */
        fildes = tag;
        if (*ant==0) nt = 1024;
        else nt = *ant;
	hbytes = *ahbytes;
        if (*anx==0)
                {
                nx = ssize (tag) / (nt*4+hbytes);
                if (nx > 256) nx = 256;
                }
        else nx = *anx;
        if (*adit==0)
                {
                dit = (nt * nx) / 262144;
                if (!dit) dit = 1;
                }
        else dit = *adit;
	*adit = dit;
	dt = *adt;
        tpow = *atpow;
        it0 = *ait0;
        if (*apclip==0.) pclip = 99.;
        else pclip = *apclip;
        if (*aphalf==0.) phalf = 85.;
        else phalf = *aphalf;
	if (*anp == 0) np = 1;
	else np = *anp;
/*	 determine which plane the gain parameters are to be estimated. */
/*	 default to the first plane 	
	 gainip =  i	ith plane 
		  -1	all 3-d data
	 gainip    0	every plane
*/
        if (*againip == 0) gainip = 1;
        else gainip = *againip;
	*againip = gainip;

        /* allocations, initializations */
        nt1 = nt / dit;

	buf = (float *) malloc(nt1*nx*sizeof(float));

        if (adata) data = adata;    /* if not passed allocate space */
        else { data = (float *) malloc(nt*sizeof(float)); }
        tpow_table = (float *) malloc(nt1*sizeof(float));

        if (tpow != 0.0)
                for (it=0; it<(nt/dit); it++)
                      tpow_table[it] = pow ((float)(((it+1)*dit+it0)*dt),tpow);
	
/*	clip and gpow are calculated from plane "ip = gainip" */
   if(gainip > 0)
    	{
        /* read, select, scale, and absolute value */
	sseek_block (fildes,(gainip-1),(nt*nx*4+hbytes*nx),1);
        for (ix=0, bp=buf; ix<nx; ix++)
                {
		sseek (fildes,hbytes,1);
                sreed (fildes,(char *)data,nt*4);
		if ( tpow != 0. )
			{
       		        for (it=0; it<nt1; it++, bp++)
                 	if ((*bp = (data[it*dit]- *adbias) * tpow_table[it]) < 0)
                        *bp *= -1.;
			}
		else 
			{
       		        for (it=0; it<nt1; it++, bp++)
                 	if ((*bp = (data[it*dit]- *adbias) ) < 0)
                        *bp *= -1.;
			}
		if (adata) data += nt;
                }

        /* determine gain factors */
        if (*aclip==0.) 
		{
		ntest = nt1*nx*pclip/100. + .5;
		if (ntest >= nt1*nx) 
			{
			*aclip = buf[0];
			for (i=1;i<nt1*nx;i++)
			if(buf[i]>*aclip) *aclip=buf[i];
			}	
		else *aclip = cent (pclip,buf,nt1*nx);
		}
        if (*agpow==0.)
                {
		ntest = nt1*nx*phalf/100. + .5;
		if (ntest >= nt1*nx) 
			{
			half = buf[0];
			for (i=1;i<nt1*nx;i++)
			if(buf[i]>half) half=buf[i];
			}	
                else half = cent (phalf,buf,nt1*nx);
                if (*aclip==0.|| half == *aclip) *agpow = 1.;
                else if (half/ *aclip < .001) *agpow = 1.;
                else
                        {
                        *agpow = log (.5) / log (half / *aclip);
                        if (*agpow<.1) *agpow = .1;
                        else if (*agpow>10.) *agpow = 10.;
                        }
                }
	}

/* data in all planes are input for estimating clip and gpow */ 
   if( gainip == - 1)	
    {
	clipnp = (float *) malloc(np*sizeof(float));
	gpownp = (float *) malloc(np*sizeof(float));

	for ( ip=0; ip<np; ip++)
		{
        if (adata) data = adata;
        else data = buf+nt1*nx;

        /* read, select, scale, and absolute value */
        for (ix=0, bp=buf; ix<nx; ix++)
                {
		sseek (fildes,hbytes,1);
                sreed (fildes,(char *)data,nt*4);
		if(tpow!=0.)
			{
                	for (it=0; it<nt1; it++, bp++)
                      	if ((*bp = (data[it*dit]- *adbias) * tpow_table[it]) < 0)
                                *bp *= -1.;
			}
		else 
			{
                	for (it=0; it<nt1; it++, bp++)
                      	if ((*bp = (data[it*dit]- *adbias)) < 0)
                                *bp *= -1.;
			}
                if (adata) data += nt;
                }

        /* determine gain factors */
        if (*aclip==0.) 
		{
		ntest = nt1 * nx * pclip / 100. + .5;  
		if ( ntest >= nt1*nx ) 
			{
			clipnp[ip] = buf[0];
			for(i=1;i<nt1*nx;i++)
				if(clipnp[ip]<buf[i]) clipnp[ip] = buf[i];
			}
		else clipnp[ip] = cent (pclip,buf,nt1*nx);
		}

        if (*agpow==0.)
                {
		ntest = nt1 * nx * phalf / 100. + .5;  
		if ( ntest >= nt1*nx ) 
			{
			half = buf[0];
			for(i=1;i<nt1*nx;i++)
				if(half<buf[i]) half = buf[i];
			}
                else half = cent (phalf,buf,nt1*nx);

                if (clipnp[ip]==0. || clipnp[ip] == half) gpownp[ip] = 1.;
                else if (half/clipnp[ip] < .001) gpownp[ip] = 1.;
                else
                        {
                        gpownp[ip] = log (.5) / log (half / clipnp[ip]);
                        if (gpownp[ip]<.1) gpownp[ip] = .1;
                        else if (gpownp[ip]>10.) gpownp[ip] = 10.;
                        }
                }
	}
        if (*aclip==0.) *aclip = cent (pclip,clipnp,np);
        if (*agpow == 0.) *agpow = cent (phalf,gpownp,np);
        free(clipnp);
        free(gpownp);
    }	

	if (!adata) free(data);  /* only free if it was not passed down */
	free(buf);		   /* internal, always free */
	free(tpow_table);          /* internal, always free */

    return(0);
}
