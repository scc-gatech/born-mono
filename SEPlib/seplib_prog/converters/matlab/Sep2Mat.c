/*
 *  Sep2Mat infile1.H [infile2.H ... ] outdir=[here|datapath|directory_name]
 *                                     verbose=n synch=n
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <seplib.h>
#include <sepmath.h>
#include <sep3d.h> 

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

static int VERBOSE = 0;
#define noDEBUG 1

static void swab4(void *item) {
   char ctmp;
   char *citem = (char *) item;
   ctmp = citem[0]; citem[0] = citem[3]; citem[3] = ctmp;
   ctmp = citem[1]; citem[1] = citem[2]; citem[2] = ctmp;
}
 
static void swab8(void *item) {
   char ctmp;
   int ii;
   int *item32 = (int *) item;
   ii = item32[0];
   item32[0] = item32[1];
   item32[1] = ii;
   swab4(item32+0);
   swab4(item32+1);
}

/* Writes an ndim array plus SEPLIB n, d and f info to a MATLAB file */
/* Variable names are {prefix}_n, {prefix}_d, {prefix}_f, {prefix}_data */


int Sep2MatGenFileHeader(FILE *outfile, int ndim, int *n) 
/* returns EXIT_FAILURE if data are too big for the format */
{
    char MatFileHdr[128];
    int i;
    double n64;

    n64 = 8; /* sizeof(IEEE double) */
    for(i=0; i<ndim; ++i) n64 *= n[i];
    if(n64 > (4294967296.0-64.0)) return (EXIT_FAILURE);
    
    memset(MatFileHdr,'\0',sizeof(MatFileHdr));
    strcpy(MatFileHdr,"MATLAB 5.0 MAT-file, XDR format, Created by Sep2Mat\n");
    MatFileHdr[124] ='\001';
    MatFileHdr[125] ='\000';
    MatFileHdr[126] = 'M';
    MatFileHdr[127] = 'I';
    fwrite(MatFileHdr,sizeof(char),sizeof(MatFileHdr),outfile);

    return (EXIT_SUCCESS);
}

void Sep2MatInitData(FILE *outfile, const char *prefix,
             int ndim, int *n, float *d, float *f)
{
    size_t prefLen;
    uint32_t matrixDataType = 14; /* miMATRIX */
    uint32_t int8DataType = 1; /* miINT8 */
    uint32_t int32DataType = 5; /* miINT32 */
    uint32_t uint32DataType = 6; /* miUINT32 */
    uint32_t ieee64DataType = 9; /* miDOUBLE */
    uint32_t ieee32DataType = 7; /* miSINGLE */
    uint32_t arrayIeee64Flags = 6; /* mxDOUBLE_CLASS */
    uint32_t arrayIeee32Flags = 7; /* mxSINGLE_CLASS */
    uint32_t arrayIntFlags = 12; /* mxINT32_CLASS */
    uint32_t arrayNumBytes;
    uint32_t undefZero = 0;
    uint32_t padNumBytes;
    uint32_t numDataElements = 1;
    int64_t n64;
    int32_t itmp;
    double   ftmp;

    int i;
    union { int i; char c[4]; } echeck;

    if(ndim < 1) {
       fprintf(stderr,"ndim=%d is less than 1!\n",ndim);
       return;
    }

    echeck.i = 1;
#ifndef DOSWAPS
#define DOSWAPS echeck.c[0] != '\0'
#endif
    if( DOSWAPS ) {
        swab4(&matrixDataType);
        swab4(&int8DataType);
        swab4(&int32DataType);
        swab4(&uint32DataType);
        swab4(&ieee64DataType);
        swab4(&arrayIeee64Flags);
        swab4(&ieee32DataType);
        swab4(&arrayIeee32Flags);
        swab4(&arrayIntFlags);
    }

    prefLen = strlen(prefix);
#ifdef DEBUG
fprintf(stdout,"ndim %d prefLen %u\n", ndim, (int) prefLen);
fprintf(stdout,"sizeof(uint32_t) %d\n", (int) sizeof(uint32_t));
#endif
    
    n64 = 1;
    for(i=0; i<ndim; ++i) n64 *= n[i];
    numDataElements = (int32_t) n64;

 /* write out n[ndim] array */

    fwrite(&matrixDataType,sizeof(matrixDataType),1,outfile);

    /* write out number of bytes to follow */
    arrayNumBytes = (uint32_t) (
                    4*8 +
                    2*sizeof(uint32DataType) +
                    2*sizeof(int32DataType) +
                    prefLen+2 + (((prefLen+2)%8 != 0) ? 8-((prefLen+2)%8) : 0) + 
                    (ndim+ndim%2)*sizeof(int32DataType) 
                    );
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);

    /* Array Flags */
    fwrite(&uint32DataType, sizeof(uint32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (2*sizeof(uint32DataType));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(&arrayIntFlags, sizeof(arrayIntFlags), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Dimensions Array */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (2 * sizeof(int32DataType));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    itmp = ndim; if( DOSWAPS) swab4(&itmp);
    fwrite(&itmp,sizeof(itmp),1,outfile);
    itmp = 1; if( DOSWAPS) swab4(&itmp);
    fwrite(&itmp,sizeof(itmp),1,outfile);

    /* Array Name */
    fwrite(&int8DataType, sizeof(int8DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (prefLen + 2);
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(prefix,prefLen, 1, outfile);
    fwrite("_n",sizeof(char), 2, outfile);
    padNumBytes = 0;
    if((prefLen+2)%8 != 0) padNumBytes = 8 - ((prefLen+2)%8);
    while(padNumBytes != 0) {
       fwrite(&undefZero, sizeof(char), 1, outfile);
       padNumBytes--;
    }

    /* Real Part */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (ndim*sizeof(int32DataType));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    for(i=0; i<ndim; ++i) {
       itmp = n[i];
       if( DOSWAPS ) swab4(&itmp);
       fwrite(&itmp, sizeof(itmp), 1, outfile);
    }
    if(ndim%2 != 0) fwrite(&undefZero, sizeof(undefZero), 1, outfile);

 /* write out d[ndim] array */

    fwrite(&matrixDataType,sizeof(matrixDataType),1,outfile);

    /* write out number of bytes to follow */
    arrayNumBytes = (uint32_t) (
                    4*8 +
                    2*sizeof(uint32DataType) +
                    2*sizeof(int32DataType) +
                    prefLen+2 + (((prefLen+2)%8 != 0) ? (8-((prefLen+2)%8)) : 0) + 
                    (ndim)*sizeof(double)
                    );
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);

    /* Array Flags */
    fwrite(&uint32DataType, sizeof(uint32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) 2*sizeof(uint32DataType);
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(&arrayIeee64Flags, sizeof(arrayIeee64Flags), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Dimensions Array */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (2 * sizeof(int32DataType));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    itmp = ndim; if( DOSWAPS) swab4(&itmp);
    fwrite(&itmp,sizeof(itmp),1,outfile);
    itmp = 1; if( DOSWAPS) swab4(&itmp);
    fwrite(&itmp,sizeof(itmp),1,outfile);

    /* Array Name */
    fwrite(&int8DataType, sizeof(int8DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (prefLen + 2);
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(prefix,prefLen, 1, outfile);
    fwrite("_d",sizeof(char), 2, outfile);
    padNumBytes = 0;
    if((prefLen+2)%8 != 0) padNumBytes = 8 - ((prefLen+2)%8);
    while(padNumBytes != 0) {
       fwrite(&undefZero, sizeof(char), 1, outfile);
       padNumBytes--;
    }

    /* Real Part */
    fwrite(&ieee64DataType, sizeof(ieee64DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (ndim*sizeof(double));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    for(i=0; i<ndim; ++i) {
       ftmp = d[i]; if( DOSWAPS ) swab8(&ftmp);
       fwrite(&ftmp, sizeof(ftmp), 1, outfile);
    }

 /* write out f[ndim] array */

    fwrite(&matrixDataType,sizeof(matrixDataType),1,outfile);

    /* write out number of bytes to follow */
    arrayNumBytes = (uint32_t) (
                    4*8 +
                    2*sizeof(uint32DataType) +
                    2*sizeof(int32DataType) +
                    prefLen+2 + (((prefLen+2)%8 != 0) ? (8-((prefLen+2)%8)) : 0) + 
                    (ndim)*sizeof(double)
                    );
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);

    /* Array Flags */
    fwrite(&uint32DataType, sizeof(uint32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) 2*sizeof(uint32DataType);
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(&arrayIeee64Flags, sizeof(arrayIeee64Flags), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Dimensions Array */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (2 * sizeof(int32DataType));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    itmp = ndim; if( DOSWAPS ) swab4(&itmp);
    fwrite(&itmp,sizeof(itmp),1,outfile);
    itmp = 1; if( DOSWAPS) swab4(&itmp);
    fwrite(&itmp,sizeof(itmp),1,outfile);

    /* Array Name */
    fwrite(&int8DataType, sizeof(int8DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (prefLen + 2);
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(prefix,prefLen, 1, outfile);
    fwrite("_o",sizeof(char), 2, outfile);
    padNumBytes = 0;
    if((prefLen+2)%8 != 0) padNumBytes = 8 - ((prefLen+2)%8);
    while(padNumBytes != 0) {
       fwrite(&undefZero, sizeof(char), 1, outfile);
       padNumBytes--;
    }

    /* Real Part */
    fwrite(&ieee64DataType, sizeof(ieee64DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (ndim*sizeof(double));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    for(i=0; i<ndim; ++i) {
       ftmp = f[i]; if( DOSWAPS ) swab8(&ftmp);
       fwrite(&ftmp, sizeof(ftmp), 1, outfile);
    }

 /* write out data array */

    fwrite(&matrixDataType,sizeof(matrixDataType),1,outfile);

    /* write out number of bytes to follow */
    arrayNumBytes = (uint32_t) (
                    4*8 +
                    2*sizeof(uint32DataType) +
                    (ndim+ndim%2)*sizeof(int32DataType) +
                    prefLen+5 + (((prefLen+5)%8 != 0) ? (8-((prefLen+5)%8)) : 0) + 
                    (numDataElements)*sizeof(double)
                    );
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);

    /* Array Flags */
    fwrite(&uint32DataType, sizeof(uint32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) 2*sizeof(uint32DataType);
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(&arrayIeee64Flags, sizeof(arrayIeee64Flags), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Dimensions Array */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (((ndim>1) ? ndim : 2) * sizeof(int32DataType));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    for(i=0; i<ndim; ++i) {
       itmp = n[i]; if( DOSWAPS ) swab4(&itmp);
       fwrite(&itmp,sizeof(itmp),1,outfile);
    }
    if(ndim == 1) {
       itmp = 1; if( DOSWAPS ) swab4(&itmp);
       fwrite(&itmp,sizeof(itmp),1,outfile);
    } else {
       if(ndim%2 != 0) fwrite(&undefZero, sizeof(undefZero), 1, outfile);
    }

    /* Array Name */
    fwrite(&int8DataType, sizeof(int8DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (prefLen + 5);
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(prefix,prefLen, 1, outfile);
    fwrite("_data",sizeof(char), 5, outfile);
    padNumBytes = 0;
    if((prefLen+5)%8 != 0) padNumBytes = 8 - ((prefLen+5)%8);
    while(padNumBytes != 0) {
       fwrite(&undefZero, sizeof(char), 1, outfile);
       padNumBytes--;
    }

    /* Real Part */
    fwrite(&ieee64DataType, sizeof(ieee64DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (numDataElements*sizeof(double));
#ifdef DEBUG
fprintf(stdout,"numbytes %u\n", arrayNumBytes);
#endif
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
}

void Sep2MatWriteData(FILE *outfile, int nsamps, float *data)
{
    double ftmp;
    int i;
    union { int i; char c[4]; } echeck;

    echeck.i = 1;
#ifndef DOSWAPS
#define DOSWAPS echeck.c[0] != '\0'
#endif

    for(i=0; i<nsamps; ++i) {
      ftmp = data[i];
      if( DOSWAPS ) swab8(&ftmp);
      fwrite(&ftmp, sizeof(ftmp), 1, outfile);
    }
}

void Sep2MatFinalizeData(FILE *outfile, int ndim, int *n)
{
    int nElements = 1;
    int i;
    uint32_t zeroPad = 0;

    for(i=0; i<ndim; ++i) nElements *= n[i];
}

void Sep2MatInitHeaders(FILE *outfile, const char *prefix, int ninthdrs, int nflthdrs, char *hdrnames[], const int ndim, int *n)
{
    uint32_t matrixDataType = 14; /* miMATRIX */
    uint32_t int8DataType = 1; /* miINT8 */
    uint32_t int32DataType = 5; /* miINT32 */
    uint32_t uint32DataType = 6; /* miINT32 */
    uint32_t ieee64DataType = 9; /* miDOUBLE */
    uint32_t ieee32DataType = 7; /* miSINGLE */
    uint32_t arrayIeee64Flags = 6; /* mxDOUBLE_CLASS */
    uint32_t arrayIeee32Flags = 7; /* mxSINGLE_CLASS */
    uint32_t arrayStructFlags = 2; /* mxSTRUCT_CLASS */
    uint32_t arrayIntFlags = 12; /* mxINT32_CLASS */
    uint32_t undefZero = 0;
    uint32_t arrayNumBytes;
    uint32_t padNumBytes;
    uint32_t numDataElements = 1;
    size_t prefLen;
    int nhdrs;
    int32_t itmp;
    float   ftmp;

    int i;
    union { int i; char c[4]; } echeck;

    if(ndim < 1) {
       fprintf(stderr,"ndim=%d is less than 1!\n",ndim);
       return;
    }

    echeck.i = 1;
#ifndef DOSWAPS
#define DOSWAPS echeck.c[0] != '\0'
#endif
    if( DOSWAPS ) {
        swab4(&matrixDataType);
        swab4(&int8DataType);
        swab4(&int32DataType);
        swab4(&uint32DataType);
        swab4(&ieee64DataType);
        swab4(&arrayIeee64Flags);
        swab4(&ieee32DataType);
        swab4(&arrayIeee32Flags);
        swab4(&arrayIntFlags);
        swab4(&arrayStructFlags);
    }

    prefLen = strlen(prefix);
    
    for(i=0; i<ndim; ++i) numDataElements *= n[i];

    nhdrs = ninthdrs + nflthdrs;

/* write out STRUCT header */
    fwrite(&matrixDataType, sizeof(matrixDataType), 1, outfile);
    itmp = ndim; if(itmp < 2) itmp = 2;
    arrayNumBytes = (uint32_t) (
       10*4 +
       prefLen+4 + (((prefLen+4)%8 != 0) ? 8 - ((prefLen+4)%8) : 0 ) +
       2*4 +
       2*4 +
       nhdrs*32 + 
       ninthdrs*(  8*4 +
               (itmp+itmp%2)*4 +
                4*4 +
               (numDataElements+numDataElements%2)*4
              ) +
       nflthdrs*(  8*4 +
               (itmp+itmp%2)*4 +
                4*4 +
               (numDataElements)*8
              )
       );
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);

    /* Array Flags */
    fwrite(&uint32DataType, sizeof(uint32DataType),1,outfile);
    arrayNumBytes = (uint32_t) (2*sizeof(uint32_t));
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(&arrayStructFlags, sizeof(arrayStructFlags), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Dimensions (1x1) */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (2*sizeof(int));
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    itmp = 1;
    if( DOSWAPS ) swab4(&itmp);
    fwrite(&itmp, sizeof(itmp), 1, outfile);
    fwrite(&itmp, sizeof(itmp), 1, outfile); /* not a typo */

    /* Structure name */
    fwrite(&int8DataType, sizeof(int8DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (prefLen+4);
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    fwrite(prefix, sizeof(char), prefLen, outfile);
    fwrite("_hdr", sizeof(char), 4, outfile);
    padNumBytes = 0;
    if((prefLen+4)%8 != 0) padNumBytes = 8 - ((prefLen+4)%8);
    while(padNumBytes != 0) {
	fwrite(&undefZero, sizeof(char), 1, outfile);
	padNumBytes--;
    }

    /* Field name length */
    putc('\000', outfile);
    putc('\004', outfile);
    putc('\000', outfile);
    putc('\005', outfile);
    /*putc(int32DataType, outfile);*/
    itmp = 32;
    if( DOSWAPS ) swab4(&itmp);
    fwrite(&itmp, sizeof(itmp), 1, outfile);

    /* List of field names */
    fwrite(&int8DataType, sizeof(int8DataType), 1, outfile);
    itmp = 32*nhdrs;
    if( DOSWAPS ) swab4(&itmp);
    fwrite(&itmp, sizeof(itmp), 1, outfile);
    for(i=0; i<nhdrs; ++i) {
	arrayNumBytes = strlen(hdrnames[i]);
	if(arrayNumBytes > 31) arrayNumBytes = 31;
	fwrite(hdrnames[i], sizeof(char), arrayNumBytes, outfile);
	padNumBytes = 32 - arrayNumBytes;
        while(padNumBytes != 0) {
           fwrite(&undefZero, sizeof(char), 1, outfile);
	   padNumBytes--;
	}
    }
}

void Sep2MatInitIntHeader(FILE *outfile, int ndim, int *n)
{
    uint32_t matrixDataType = 14; /* miMATRIX */
    uint32_t int8DataType = 1; /* miINT8 */
    uint32_t int32DataType = 5; /* miINT32 */
    uint32_t uint32DataType = 6; /* miUINT32 */
    uint32_t arrayIntFlags = 12; /* mxINT32_CLASS */
    uint32_t undefZero = 0;
    uint32_t arrayNumBytes;
    uint32_t padNumBytes;
    uint32_t numHdrElements = 1;
    int32_t itmp;
    int i;
    union { int i; char c[4]; } echeck;

    for(i=0; i<ndim; ++i) numHdrElements *= n[i];

    echeck.i = 1;
    if( DOSWAPS ) {
        swab4(&matrixDataType);
        swab4(&int8DataType);
        swab4(&int32DataType);
        swab4(&uint32DataType);
        swab4(&arrayIntFlags);
    }
    fwrite(&matrixDataType, sizeof(matrixDataType),1, outfile);
    itmp = ndim; if(itmp < 2) itmp = 2;
    arrayNumBytes = (uint32_t) (
	    4*4 +
	    2*4 + (itmp+itmp%2)*sizeof(int) +
            2*4 +
            2*4 +
	    (numHdrElements + numHdrElements%2) * sizeof(int)
	);
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);

    /* Array Flags */
    fwrite(&uint32DataType, sizeof(uint32DataType), 1, outfile);
    itmp = 8;
    if( DOSWAPS ) swab4(&itmp);
    fwrite(&itmp, sizeof(itmp), 1, outfile);
    fwrite(&arrayIntFlags, sizeof(arrayIntFlags), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Dimensions */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (((ndim > 1) ? ndim : 2) * sizeof(int));
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    for(i=0; i<ndim; ++i) {
       itmp = n[i]; if( DOSWAPS ) swab4(&itmp);
       fwrite(&itmp, sizeof(itmp), 1, outfile);
    }
    if(ndim == 1) {
       itmp = 1; if( DOSWAPS ) swab4(&itmp);
       fwrite(&itmp, sizeof(itmp), 1, outfile);
    } else {
       if(ndim %2 != 0) fwrite(&undefZero, sizeof(undefZero), 1, outfile);
    }

    /* Array name (leave blank) */
    fwrite(&int8DataType, sizeof(int8DataType), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Real Part */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (numHdrElements * sizeof(int));
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
}

void Sep2MatInitFltHeader(FILE *outfile, int ndim, int *n)
{
    uint32_t matrixDataType = 14; /* miMATRIX */
    uint32_t int8DataType = 1; /* miINT8 */
    uint32_t int32DataType = 5; /* miINT32 */
    uint32_t uint32DataType = 6; /* miUINT32 */
    uint32_t ieee64DataType = 9; /* miDOUBLE */
    uint32_t arrayIeee64Flags = 6; /* mxDOUBLE_CLASS */
    uint32_t undefZero = 0;
    uint32_t arrayNumBytes;
    uint32_t padNumBytes;
    uint32_t numHdrElements = 1;
    int32_t itmp;
    int i;
    union { int i; char c[4]; } echeck;

    for(i=0; i<ndim; ++i) numHdrElements *= n[i];

    echeck.i = 1;
    if( DOSWAPS ) {
        swab4(&matrixDataType);
        swab4(&int8DataType);
        swab4(&int32DataType);
        swab4(&uint32DataType);
        swab4(&ieee64DataType);
        swab4(&arrayIeee64Flags);
    }
    fwrite(&matrixDataType, sizeof(matrixDataType),1, outfile);
    itmp = ndim; if(itmp < 2) itmp = 2;
    arrayNumBytes = (uint32_t) (
	    4*4 +
	    2*4 + (itmp+itmp%2)*sizeof(int) +
            2*4 +
            2*4 +
	    (numHdrElements) * sizeof(double)
	);
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);

    /* Array Flags */
    fwrite(&uint32DataType, sizeof(uint32DataType), 1, outfile);
    itmp = 8;
    if( DOSWAPS ) swab4(&itmp);
    fwrite(&itmp, sizeof(itmp), 1, outfile);
    fwrite(&arrayIeee64Flags, sizeof(arrayIeee64Flags), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Dimensions */
    fwrite(&int32DataType, sizeof(int32DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (((ndim > 1) ? ndim : 2) * sizeof(int));
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
    for(i=0; i<ndim; ++i) {
       itmp = n[i]; if( DOSWAPS ) swab4(&itmp);
       fwrite(&itmp, sizeof(itmp), 1, outfile);
    }
    if(ndim == 1) {
       itmp = 1; if( DOSWAPS ) swab4(&itmp);
       fwrite(&itmp, sizeof(itmp), 1, outfile);
    } else {
       if(ndim %2 != 0) fwrite(&undefZero, sizeof(undefZero), 1, outfile);
    }

    /* Array name (leave blank) */
    fwrite(&int8DataType, sizeof(int8DataType), 1, outfile);
    fwrite(&undefZero, sizeof(undefZero), 1, outfile);

    /* Real Part */
    fwrite(&ieee64DataType, sizeof(ieee64DataType), 1, outfile);
    arrayNumBytes = (uint32_t) (numHdrElements * sizeof(double));
    if( DOSWAPS ) swab4(&arrayNumBytes);
    fwrite(&arrayNumBytes, sizeof(arrayNumBytes), 1, outfile);
}

void Sep2MatWriteIntHeader(FILE *outfile, int nsamps, void *data)
{
    int itmp;
    int i;
    union { int i; char c[4]; } echeck;

    echeck.i = 1;
#ifndef DOSWAPS
#define DOSWAPS echeck.c[0] != '\0'
#endif

    for(i=0; i<nsamps; ++i) {
      itmp = ((int *)data)[i];
      if( DOSWAPS ) swab4(&itmp);
      fwrite(&itmp, sizeof(itmp), 1, outfile);
    }
}

void Sep2MatWriteFltHeader(FILE *outfile, int nsamps, void *data)
{
    double ftmp;
    int i;
    union { int i; char c[4]; } echeck;

    echeck.i = 1;
#ifndef DOSWAPS
#define DOSWAPS echeck.c[0] != '\0'
#endif

    for(i=0; i<nsamps; ++i) {
      ftmp = (double) (((float *)data)[i]);
      if( DOSWAPS ) swab8(&ftmp);
      fwrite(&ftmp, sizeof(ftmp), 1, outfile);
    }
}

void Sep2MatFinalizeIntHeader(FILE *outfile, int ndim, int *n)
{
    int nElements = 1;
    int i;
    uint32_t zeroPad = 0;

    for(i=0; i<ndim; ++i) nElements *= n[i];

    if(nElements%2 != 0) fwrite(&zeroPad,sizeof(zeroPad),1,outfile);
}

void Sep2MatFinalizeFltHeader(FILE *outfile, int ndim, int *n)
{
    int nElements = 1;
    int i;

    for(i=0; i<ndim; ++i) nElements *= n[i];

}

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>

static const char *MY_SEP_DOC = \
"NAME\n"
"    Sep2Mat - Convert SEP file to MATLAB .mat format\n"
"\n"
"SYNOPSIS\n"
"    Sep2mat infile1.H [infile2.H ...] outdir=[here|datapath|directory]\n"
"            verbose=n synch=n\n"
"\n"
"INPUT PARAMETERS\n"
"\n"
"    outdir - where to put MATLAB output\n"
"                  here - (default) current directory\n"
"                  datapath - user's DATAPATH directory\n"
"                  directory - explicit directory name\n"
"\n"
"    verbose - \"y\" for extra printout\n"
"\n"
"    synch   - \"y\" to reorder traces to match headers\n"
"\n"
"DESCRIPTION\n"
"    Converts infile.H to infile.mat in the output directory.\n"
"    Creates n[], d[], o[] and data[] arrays.\n"
"    If trace headers are present, creates a hdr structure.\n"
"    The MATLAB file is retrieved via the load('infile.mat') command.\n"
"\n"
"    Note: If data_record_number[] is present in the hdr structure,\n"
"          either use it to index the trace that belongs to that header,\n"
"          or specify synch=y to align traces with headers\n"
"\n"
"CATEGORY\n"
"    util/converters\n"
"\n";

#include <unistd.h>
#include <signal.h>

extern int sepxargc;
extern char **sepxargv;
/*ARGUSED*/
int main(int argc, char **argv, char **envp) {

  int n[7]; 
  float d[7];
  float o[7];
  float f;
  int i,j,k,m;
  int noff;
  int synch = 0;
  char inbufname[BUFSIZ];
  char outbufname[BUFSIZ];
  char datadir[BUFSIZ];
  char prefix[BUFSIZ];
  char hdrname[BUFSIZ];
  char hdrtype[BUFSIZ];
  char *ctmp;
  char **hdrnames;
  char **hdrtypes;
  FILE *outfp;
  int ndim;
  int saven1;
  int rc;
  int n_hdr_keys, nvalid_keys;
  int ninthdrs, nflthdrs;
  int ninputs = 0; 
  int idata, mdata, ndata;
  int useit;
  int havedrn;
  int same_record_number;
  int itmp,jtmp,ktmp;
  
  initpar(argc, argv);
  getch_add_string("in=/dev/null");
  getch_add_string("out=/dev/null");
  getch_add_string("head=/dev/null");
  getch_add_string("noheader=y");
  (void)signal(SIGALRM,SIG_IGN);/*set default to ignore "late" getpar signals*/

  getch("verb verbose","1",&VERBOSE);
  getch("synch","1",&synch);
  for(j=1; j<argc; ++j) {
     if(NULL == strchr(argv[j],'=')) ++ninputs;
  }
  if(ninputs == 0) {
     fprintf(stderr,"%s", MY_SEP_DOC);
     fprintf(stderr,"\n**** No input files?! Exiting. ****\n\n");
     return(EXIT_FAILURE);
  }

  init_3d();

  (void) setlocale(LC_ALL,"C");
  strcpy(datadir,".");
  (void) getch("outdir","s",datadir);
  if(0 == strcmp(datadir,"here")) {
     strcpy(datadir,".");
  } else {
     if(0 == strcmp(datadir,"datapath")) {
      (void) datapath(datadir);
     }
  }
  if(VERBOSE) fprintf(stderr,"datadir=\"%s\"\n",datadir);
  strcat(datadir,"/");
  
  hclose();
  if(VERBOSE) fprintf(stderr,"Finished history processing.\n");

  for(i=1; i<argc; ++i) {
     if(NULL == strchr(argv[i],'=')) {
	 strcpy(inbufname,"/dev/null");
	 auxpar("in","s",inbufname,argv[i]);
	 for(j=0; j<7; ++j) {
	    n[j] = 1; d[j] = 1.0f; o[j] = 0.0f;
	 }
         same_record_number = 1;
	 if(0 == auxpar("n1","d",n+0,argv[i])) {
	    fprintf(stderr,"Unable to retrieve n1 from %s ... skipping\n",
		argv[i]);
	    continue;
	 }
         saven1 = n[0];
	 auxpar("n2","d",n+1,argv[i]);
	 auxpar("n3","d",n+2,argv[i]);
	 auxpar("n4","d",n+3,argv[i]);
	 auxpar("n5","d",n+4,argv[i]);
	 auxpar("n6","d",n+5,argv[i]);
	 auxpar("n7","d",n+6,argv[i]);
	 auxpar("d1","f",d+0,argv[i]);
	 auxpar("d2","f",d+1,argv[i]);
	 auxpar("d3","f",d+2,argv[i]);
	 auxpar("d4","f",d+3,argv[i]);
	 auxpar("n5","f",d+4,argv[i]);
	 auxpar("d6","f",d+5,argv[i]);
	 auxpar("d7","f",d+6,argv[i]);
	 auxpar("o1","f",o+0,argv[i]);
	 auxpar("o2","f",o+1,argv[i]);
	 auxpar("o3","f",o+2,argv[i]);
	 auxpar("o4","f",o+3,argv[i]);
	 auxpar("o5","f",o+4,argv[i]);
	 auxpar("o6","f",o+5,argv[i]);
	 auxpar("o7","f",o+6,argv[i]);

         auxpar("same_record_number","1",&same_record_number,argv[i]);

         if(VERBOSE) {
           fprintf(stderr,"Input: %s\n", argv[i]);
           fprintf(stderr,"n: %d %d %d %d %d %d %d\n",n[0],n[1],n[2],n[3],n[4],n[5],n[6]);
           fprintf(stderr,"d: %f %f %f %f %f %f %f\n",d[0],d[1],d[2],d[3],d[4],d[5],d[6]);
           fprintf(stderr,"o: %f %f %f %f %f %f %f\n",o[0],o[1],o[2],o[3],o[4],o[5],o[6]);
         }
	 for(j=6; j>1; --j) {
	    if(n[j] != 1) break;
	 }
         ndim = j+1;

	 strcpy(outbufname,datadir);
	 ctmp = strrchr(inbufname,'/');
	 if(ctmp == ((char *) NULL)) {
	    strcat(outbufname,inbufname);
	    strcpy(prefix,inbufname);
	 } else {
	    strcat(outbufname,ctmp+1);
	    strcpy(prefix,ctmp+1);
	 }
	 ctmp = strrchr(outbufname,'.');
	 if(ctmp != ((char *) NULL)) {
	    *ctmp = '\0';
	 }
	 strcat(outbufname,".mat");
	 ctmp = strrchr(prefix,'.');
	 if(ctmp != ((char *) NULL)) {
	    *ctmp = '\0';
	 }
	 outfp = fopen(outbufname,"wb");
	 if(outfp == ((FILE *) NULL)) {
	    fprintf(stderr,"Unable to open output \"%s\" ... skipping\n",
		outbufname);
	    continue;
	 }

         if(EXIT_FAILURE == Sep2MatGenFileHeader(outfp,ndim,n)) {
            fprintf(stderr,"%s: data are too large (> 2^32?) for MATLAB file format ... skipping\n",argv[i]);
            fclose(outfp);
            continue;
         }

         if(ferror(outfp) || feof(outfp)) {
            perror(outbufname);
            fprintf(stderr,"Trouble writing output MATLAB file header\n");
            fclose(outfp);
            continue;
         }
 
         /* include headers if present */
	 nvalid_keys = 0;
         ninthdrs = 0;
         nflthdrs = 0;
         havedrn = 0;
         rc = sep_get_number_keys(argv[i], &n_hdr_keys);
         if(rc == 0 && n_hdr_keys > 0) {
             hdrnames = (char **) malloc(n_hdr_keys * sizeof(char *));
             hdrtypes = (char **) malloc(n_hdr_keys * sizeof(char *));
	     for(j=0; j < n_hdr_keys; ++j) {
		 itmp = j+1;
                 sep_get_key_type(argv[i], &itmp, hdrtype);
                 if(0==strcmp(hdrtype,"scalar_int")) {
                     useit = 1;
		     sep_get_key_name(argv[i],&itmp,hdrname);
                     if(0 == strcmp(hdrname,"data_record_number")) havedrn = 1;
                     if(same_record_number && havedrn) useit = 0;
                     if(useit) {
		        hdrtypes[nvalid_keys] = (char *) malloc(1+strlen("scalar_int"));
		        strcpy(hdrtypes[nvalid_keys],"scalar_int");
		        hdrnames[nvalid_keys] = (char *) calloc(32, sizeof(char));
		        strncpy(hdrnames[nvalid_keys], hdrname, 31);
		        nvalid_keys++;
                        ninthdrs++;
                     }
		 }
                 if(0==strcmp(hdrtype,"scalar_float")) {
		     hdrtypes[nvalid_keys] = (char *) malloc(1+strlen("scalar_float"));
		     strcpy(hdrtypes[nvalid_keys],"scalar_float");
		     sep_get_key_name(argv[i],&itmp,hdrname);
		     hdrnames[nvalid_keys] = (char *) malloc(32);
		     memset(hdrnames[nvalid_keys],'\0',32);
		     strncpy(hdrnames[nvalid_keys], hdrname, 31);
		     nvalid_keys++;
                     nflthdrs++;
		 }
             }
         }
         if(VERBOSE) {
            fprintf(stderr,"ninthdrs %d nflthdrs %d nvalidhdrs %d\n",ninthdrs,nflthdrs,nvalid_keys);
         }

	 if(nvalid_keys > 0) {
             
             jtmp = (ndim < 2) ? ndim : (ndim-1);
             if(jtmp != ndim) {
                noff = 1;
             } else {
                noff = 0;
                n[0] = 1;
             }
	     mdata = n[noff];
	     for(j = 1; j < jtmp; ++j) mdata *= n[j+noff];
             Sep2MatInitHeaders(outfp, prefix, ninthdrs, nflthdrs, hdrnames, jtmp, n+noff);
             if(ferror(outfp) || feof(outfp)) {
                perror(outbufname);
                fprintf(stderr,"1. Trouble writing trace headers\n");
                fclose(outfp);
                continue;
             }

	     for(j=0; j<nvalid_keys; ++j) {
		 if(0 == strcmp(hdrtypes[j],"scalar_int")) {
  		     Sep2MatInitIntHeader(outfp, jtmp, n+noff);
		     for(k=0; k<mdata; ++k) {
		         itmp = 1;
		         ktmp = k+1;
		         sep_get_val_by_name(argv[i],&ktmp,hdrnames[j],&itmp,&m);
                         Sep2MatWriteIntHeader(outfp, 1, &m);
		     }
                     Sep2MatFinalizeIntHeader(outfp, jtmp, n+noff);
		 }
		 if(0 == strcmp(hdrtypes[j],"scalar_float")) {
  		     Sep2MatInitFltHeader(outfp, jtmp, n+noff);
		     for(k=0; k<mdata; ++k) {
		         itmp = 1;
		         ktmp = k+1;
		         sep_get_val_by_name(argv[i],&ktmp,hdrnames[j],&itmp,&f);
                         Sep2MatWriteFltHeader(outfp, 1, &f);
		     }
                     Sep2MatFinalizeFltHeader(outfp, jtmp, n+noff);
		 }
		 free(hdrtypes[j]); free(hdrnames[j]);
	     }
             free(hdrtypes); free(hdrnames);
             if(ferror(outfp) || feof(outfp)) {
                perror(outbufname);
                fprintf(stderr,"2. Trouble writing trace headers\n");
                fclose(outfp);
                continue;
             }
	 }

         /* write out trace data */
         n[0] = saven1;
	 ndata = n[0];
	 for(j = 1; j < ndim; ++j) ndata *= n[j];
         idata = 0;
         Sep2MatInitData(outfp, prefix, ndim, n, d, o);
         if(ferror(outfp) || feof(outfp)) {
            perror(outbufname);
            fprintf(stderr,"1. Trouble writing trace data\n");
            fclose(outfp);
            continue;
         }
         if(!synch || !havedrn || same_record_number) {
	    while(4 == (rc = sreed(argv[i],&f,4))) {
                 idata++;
		 Sep2MatWriteData(outfp,1,&f);
            }
         } else {
            for(k=0; k<mdata; ++k) {
               itmp = 1; ktmp = k+1;
               sep_get_val_by_name(argv[i],&ktmp,"data_record_number",&itmp,&m);
               rc = sseek_block(argv[i],m-1,saven1*4,0);
               for(j=0; j<saven1; ++j) {
                  rc = sreed(argv[i],&f,4);
                  if(rc != 4) break;
                  idata++;
                  Sep2MatWriteData(outfp,1,&f);
               }
               if(rc != 4) break;
            }
         }
         if(ferror(outfp) || feof(outfp)) {
            perror(outbufname);
            fprintf(stderr,"2. Trouble writing trace data\n");
            fclose(outfp);
            continue;
         }
         Sep2MatFinalizeData(outfp, ndim, n);
         if(ferror(outfp) || feof(outfp)) {
            perror(outbufname);
            fprintf(stderr,"3. Trouble writing trace data\n");
            fclose(outfp);
            continue;
         }
         if(idata != ndata)
             sepwarn(2,"%s: Expected %d got only %d samples.\n",argv[i],ndata,idata);

	 fclose(outfp);
     }
  }

  /*sep_3d_close();*/

  if(VERBOSE) fprintf(stderr,"closed sep3d\n");
  return (EXIT_SUCCESS);
}
