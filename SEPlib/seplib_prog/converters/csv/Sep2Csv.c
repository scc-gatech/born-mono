/*
 *  Sep2Csv < infile.H outpref="{datapath}/gather"
 *                     supergather=n csvpar="csv.par" verbose=n
 */

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

#define MY_SEP_DOC \
sep_add_doc_line("NAME");\
sep_add_doc_line("    Sep2Csv - export SEP gathers to comma-separated value (CSV) files");\
sep_add_doc_line("");\
sep_add_doc_line("SYNOPSIS");\
sep_add_doc_line("    Sep2Csv < in.H outpref=\"{datapath}/gather\"");\
sep_add_doc_line("                   supergather=n csvpar=\"csv.par\" verbose=n");\
sep_add_doc_line("");\
sep_add_doc_line("INPUT PARAMETERS");\
sep_add_doc_line("    outpref - string");\
sep_add_doc_line("                  Directory to contain output CSV files");\
sep_add_doc_line("");\
sep_add_doc_line("    supergather - boolean");\
sep_add_doc_line("                  Set to y or 1 for additional file");\
sep_add_doc_line("                  with all gathers combined into one.");\
sep_add_doc_line("");\
sep_add_doc_line("    csvpar - string");\
sep_add_doc_line("                  parfile created to read back csv file(s)");\
sep_add_doc_line("                  into Csv2Sep converter");\
sep_add_doc_line("");\
sep_add_doc_line("    verbose - boolean");\
sep_add_doc_line("                  Set to y or 1 for progress details.");\
sep_add_doc_line("");\
sep_add_doc_line("DESCRIPTION");\
sep_add_doc_line("    The Comma-Separated Value text file format (RFC 4180) is widely accepted");\
sep_add_doc_line("    by many software packages as a way to deal with matrix-like");\
sep_add_doc_line("    spreadsheet data. This utility copies each 2D gather in the");\
sep_add_doc_line("    input SEP dataset to a separate CSV file in a designated directory.");\
sep_add_doc_line("    If the input data have trace headers, they, too, are copied to the");\
sep_add_doc_line("    CSV file. The CSV files are numbered by their (multi-)index");\
sep_add_doc_line("    in the input, i.e. {outpref}_{iN}_..._{i3}.csv for N-dimensional input. ");\
sep_add_doc_line("");\
sep_add_doc_line("    The format of an output CSV file for an input with trace headers is:");\
sep_add_doc_line("");\
sep_add_doc_line("    HDR_NAME_1,val_1,val_2,val_3,...,val_N2");\
sep_add_doc_line("    HDR_NAME_2,val_1,val_2,val_3,...,val_N2");\
sep_add_doc_line("    ...                                    ");\
sep_add_doc_line("    <blank>,trace1[1],trace2[1],trace3[1],...,traceN2[1]");\
sep_add_doc_line("    <blank>,trace1[2],trace2[2],trace3[2],...,traceN2[2]");\
sep_add_doc_line("    ...                                    ");\
sep_add_doc_line("    <blank>,trace1[N1],trace2[N1],trace3[N1],...,traceN2[N1]");\
sep_add_doc_line("");\
sep_add_doc_line("    The format of an output CSV file for an input without trace headers is:");\
sep_add_doc_line("");\
sep_add_doc_line("    trace1[1],trace2[1],trace3[1],...,traceN2[1]");\
sep_add_doc_line("    trace1[2],trace2[2],trace3[2],...,traceN2[2]");\
sep_add_doc_line("    ...                                    ");\
sep_add_doc_line("    trace1[N1],trace2[N1],trace3[N1],...,traceN2[N1]");\
sep_add_doc_line("");\
sep_add_doc_line("SEE ALSO");\
sep_add_doc_line("    Csv2Sep");\
sep_add_doc_line("");\
sep_add_doc_line("CATEGORY");\
sep_add_doc_line("    converters/csv");\
sep_add_doc_line(""); 

#define HEAD "/dev/null"
#define OUT "/dev/null"

#include <sep.main> 

#include <sep3d.h> 

#include "../lib/corelibs/include/sep_main_internal.h"

static volatile int wrapup;
static void gotowrapup(int signum) {
  wrapup = 1;
} 

int MAIN(void) {

  int n1=1, n2=1; 
  int ndims;
  int dimlist[7] = {1, 1, 1, 1, 1, 1, 1};
  int idims[7] = {0, 0, 0, 0, 0, 0, 0};
  float o[7] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  float d[7] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
  float **intraces;
  int **inhdrs;
  int verbose = 0;
  char outpref[BUFSIZ];
  char orient[BUFSIZ];
  char csvpar[BUFSIZ];
  char tempbuf[BUFSIZ]; 
  char labelbuf[BUFSIZ]; 
  char supergather[BUFSIZ]; 
  char hdrname[256];
  char hdrtype[32];
  int nbytes, nreed, nrite;
  const int synch = 1;
  int dosuper;
  int nbytesHdr;
  size_t ikey, nkeys; 
  int nvals = 1;
  int c;
  int rc;
  int j,k;
  char **keylist;
#define INTKEY 1
#define FLOATKEY 2
  int  *keytype;
  char *cptr;
  char **ccptr;
  off_t *rowptrs;
  char ctmp;
  size_t slen;
  unsigned long irec, jrec, nrec;
  char *sortdir;
  union {
    int i;
    float f;
  } convbuf; 
  int imgood;
  int itemp;
  int istat;
  int drn_key;
  int drn;
  int ioff;
  int sep77;
  int nhdrbytes;
  int nhdrblocks;
  char *tag_header[1];
  char outname[BUFSIZ];
#define CRLF "\015\012"
  FILE *csvparfp;
  FILE *outfp;
  FILE *sgfp;

  init_3d();

  (void) setlocale(LC_ALL,"C");

  if(0 == getch("verb verbose","1",&verbose)) verbose = 0;
  if(0 == getch("super supergather sg","1",&dosuper)) dosuper = 0;

  if(0 == getch("orient","s",orient)) strcpy(orient,"incolumns");
  if(0 == strncmp(orient,"inc",3)) strcpy(orient,"incolumns");
  if(0 == strncmp(orient,"inr",3)) strcpy(orient,"inrows");
  /* if(verbose) (void) fprintf(stderr,"orient=\"%s\"\n",orient); */
  switch(orient[2]) {
    case 'c':
    case 'r':
         break;
    default:
         seperr("Unable to decode \"orient\" option\n");
         break;
  }

  rowptrs = calloc(n2,sizeof(off_t));
  if(rowptrs == ((off_t *) NULL)) {
      perror("failed calloc of rowptrs, supergather will not be generated\n");
      dosuper = 0;
  }


  /* set prefix for output file(s) */
  outpref[0] = '.';
  (void) datapath(outpref); strcat(outpref,"/gather");
  (void) getch("outprefix outpref","s",outpref);
  if(verbose) (void) fprintf(stderr,"outpref=\"%s\"\n",outpref);
  strcpy(supergather,outpref);
  strcat(supergather,".csv");

  strcpy(csvpar,"csv.par");
  (void) getch("csvpar","s",csvpar);
  if(verbose) fprintf(stderr,"csvpar=\"%s\"\n",csvpar);
  csvparfp = fopen(csvpar,"w+");
  if(csvparfp == ((FILE *) NULL)) {
     perror("csvparfp open failure: ");
     fprintf(stderr,"Error opening csvpar file \"%s\".  Exiting.\n",csvpar);
     seperr("Error opening csvpar file.\n");
  }

  if(0 == fetch("n1","d",&n1)) { seperr("Missing n1!\n"); }
  if(0 == fetch("n2","d",&n2)) { n2 = 1; }
  dimlist[0] = n1;
  dimlist[1] = n2;
  if(0 >= fetch("n3","d",dimlist+2)) ndims = 2;
  else if(0 >= fetch("n4","d",dimlist+3)) ndims = 3;
  else if(0 >= fetch("n5","d",dimlist+4)) ndims = 4;
  else if(0 >= fetch("n6","d",dimlist+5)) ndims = 5;
  else if(0 >= fetch("n7","d",dimlist+6)) ndims = 6;
  else ndims = 7;

  while(ndims > 2) {
    if(dimlist[ndims-1] > 1) break;
    ndims--;
  }

  fprintf(csvparfp,"outpref=\"%s\"%sorient=\"%s\"%sndims=%d%s",
                   outpref,CRLF, orient, CRLF, ndims, CRLF);
  fprintf(csvparfp,"supergather=%d%s",dosuper, CRLF);
  if(0 < fetch("title","s",tempbuf)) 
        (void) fprintf(csvparfp,"title=\"%s\"%s",tempbuf,CRLF);
  for(j=0; j<ndims; ++j) {
     (void) sprintf(tempbuf,"o%d",j+1);
     (void) fetch(tempbuf,"f",&(o[j]));
     (void) fprintf(csvparfp,"%s=%f%s",tempbuf,o[j],CRLF);
     tempbuf[0]='d';
     (void) fetch(tempbuf,"f",&(d[j]));
     (void) fprintf(csvparfp,"%s=%f%s",tempbuf,d[j],CRLF);
     (void) fprintf(csvparfp,"dimlist[%d]=%d%s",j,dimlist[j],CRLF);
     (void) sprintf(tempbuf,"label%d",j+1);
     if(0 < fetch(tempbuf,"s",labelbuf))
        (void) fprintf(csvparfp,"%s=\"%s\"%s",tempbuf,labelbuf,CRLF);
  }

  /* count number of gathers */
  nrec = 1;
  for(j=2; j<ndims; ++j) {
     nrec *= dimlist[j];
  }

  istat=sep_get_number_keys("in",&itemp);
  nkeys = itemp;
  sep77 = (istat != 0);
  if(sep77) nkeys = 0;

       
  if(verbose) fprintf(stderr,"number of gathers is %lu\n",(unsigned long) nrec);
  if(verbose) fprintf(stderr,"number of keys is %lu\n",(unsigned long) nkeys);
  /* capture each key location in the list */
  /* and turn the commas into null terminators */
  keylist = (char **) calloc(sizeof(char *),nkeys);
  keytype = (int *) calloc(sizeof(int),nkeys);
  if(keylist == ((char **)NULL) || keytype == ((int *)NULL) ) {
     seperr("Unable to allocate memory!\n");
  }

  /* retrieve key names */
  for(ikey = 0; ikey < nkeys; ++ikey) {
     hdrname[0] = '\0';
     itemp = (int) ikey;
     (void) sep_get_key_name("in", &itemp, hdrname);
     keylist[ikey] = (char *) calloc(sizeof(char), 1+strlen(hdrname));
     (void) strcpy(keylist[ikey], hdrname);
     hdrtype[0] = '\0';
     (void) sep_get_key_type("in", &itemp, hdrtype);
     ikey = itemp;
     if(0 == strcmp(hdrtype,"scalar_int")) keytype[ikey] = INTKEY;
     if(0 == strcmp(hdrtype,"scalar_float")) keytype[ikey] = FLOATKEY;
     if(0 == keytype[ikey]) {
        seperr("key \"%s\" not scalar int or float!\n",keylist[ikey]);
     }
  }

  if(0 != sep_get_header_format_tag("in",tag_header)) {
     if(!sep77) seperr("Error finding header tag\n");
  }
     
  if(0 != sep_get_header_bytes("in",&nhdrbytes)) {
     if(!sep77)seperr("Error finding header length\n");
     else nhdrbytes = 0;
  }

  if(sep77) nhdrblocks = 0;
  else nhdrblocks = ssize_block(*tag_header,nhdrbytes);
  
  if(verbose) (void) fprintf(stderr,"number of header bytes is %d\n",nhdrbytes);
  if(verbose) (void) fprintf(stderr,"number of headers is %d\n",nhdrblocks);

  if(!sep77) free(*tag_header); 
  

  /* handle data_record_number as needed */
  if(synch) {
     istat = sep_get_key_index("in", "data_record_number",&drn_key);
     if( istat != 0 ) {
        drn_key = -1;
     }
  } 

  if(verbose) (void) fprintf(stderr,"Finished history processing.\n");

  intraces = (float **)calloc(n2,sizeof(float *));
  if(intraces == ((float **) NULL)) {
      seperr("Unable to allocate memory!\n");
  }
  for(j=0; j<n2; ++j) {
      intraces[j] = calloc(n1,sizeof(float));
      if(intraces[j] == ((float *) NULL)) seperr("Unable to allocate memory!\n");
  }
  nbytes = (int) (n1*sizeof(float));

  if(!sep77) {
     sep_get_header_bytes("in",&nbytesHdr);
     inhdrs = (int **) calloc(n2,sizeof(int *));
     if(inhdrs == ((int **) NULL)) {
        seperr("Unable to allocate memory!\n");
     }
     for(j=0; j<n2; ++j) {
        inhdrs[j] = (int *) calloc(nbytesHdr,sizeof(char));
        if(inhdrs[j] == ((int *) NULL))
            seperr("Unable to allocate memory!\n");
     }
  } else {
      nbytesHdr = 0;
  }

  wrapup = 0;
  (void) signal(SIGINT,gotowrapup); 

  for(irec=0; irec<nrec; ) {
       
    irec++;
    if(verbose && irec == nrec) (void) fprintf(stderr,"read last gather, no. %lu\n", irec);
    
    for(k=0; k<n2; ++k) {
       jrec = k+irec*n2;
       if(!sep77) {
           itemp = jrec;
           istat = sep_get_val_headers("in",&itemp,&nvals,inhdrs[k]);
           if(0 != istat) seperr("header values retrieval for record %d  failed!\n", itemp);
       }
     
       if(synch && (drn_key > -1)) {
          drn = inhdrs[k][drn_key-1];
       }
       if(synch) {
          if(drn_key < 0) drn = jrec;
          if(!sep77) {
             ioff = sseek_block("in",drn-1,n1*4,SEEK_SET);
             if(ioff == -1 && drn != jrec) {
                seperr("Trace seek problem, ioff=%d.  Exiting...",ioff);
             }
          }
          nreed = sreed("in",intraces[k],n1*4);
          if(nreed != n1*4) {
              seperr("Trace read problem, nreed=%d.  Exiting...",nreed);
          }
       }
    }

    /* create CSV file */
    (void) strcpy(outname,outpref);
    for(j=ndims; j>2;) {
	--j;
        (void) sprintf(tempbuf,"_%09d",idims[j]);
	(void) strcat(outname,tempbuf);
    }
    (void) strcat(outname,".csv");
    outfp = fopen(outname,"w+");
    if(outfp == ((FILE *) NULL)) {
       perror("output csv file create failure: ");
       seperr("Unable to create file \"%s\"\n",outname);
    }

    clearerr(outfp);
    if(0 == strncmp(orient,"inc",3)) { /* traces go down columns */
	 if(!sep77) {
	     /* write out headers */
	     for(j=0; j < nkeys; ++j) {
		(void) fprintf(outfp,"%s",keylist[j]);
		if(keytype[j] == INTKEY)
		   for(k=0; k<n2; ++k) {
		       (void) fprintf(outfp,",%-d",inhdrs[k][j]);
		   }
		if(keytype[j] == FLOATKEY)
		   for(k=0; k<n2; ++k) {
		       (void) fprintf(outfp,",%-g",((float *)(inhdrs[k]))[j]);
		   }
                (void) fprintf(outfp,"%s",CRLF);
	     }
	     /* write out traces */
	     for(j=0; j<n1; ++j) {
                putc(' ',outfp);
		for(k=0; k<n2; ++k) {
		   fprintf(outfp,",%-g",intraces[k][j]);
		}
		fprintf(outfp,"%s",CRLF);
	     }

         } else { /* no headers */

	     /* write out traces */
	     for(j=0; j<n1; ++j) {
		for(k=0; k<n2;) {
		   fprintf(outfp,"%-g",intraces[k][j]);
                   ++k;
		   if(k<n2) putc(',',outfp);
		}
		fprintf(outfp,"%s",CRLF);
	     }
	 }
    } else { /* traces go across rows */
	 if(!sep77) {
             /* write out header names line */
	     for(j=0; j<nkeys;) {
		 fprintf(outfp,"%s",keylist[j]);
	         putc(',',outfp);
	         ++j;
	     }
	     for(j=0; j<n1;) {
		 putc(' ',outfp);
		 ++j;
		 if(j < n1) putc(',',outfp);
	     }
	     fprintf(outfp,CRLF);

	     /* write out headers and traces */
	     for(k=0; k<n2; ++k) {
		 for(j=0; j<nkeys;) {
		    if(keytype[j] == INTKEY) fprintf(outfp,"%-d",inhdrs[k][j]);
		    if(keytype[j] == FLOATKEY) fprintf(outfp,"%-g",((float *)(inhdrs[k]))[j]);
		    j++;
		    putc(',',outfp);
	         }
		 for(j=0; j<n1;) {
                    sprintf(tempbuf,"%-g",intraces[k][j]);
	            fprintf(outfp,"%s",tempbuf);
                    if(dosuper) rowptrs[k] += strlen(tempbuf);
                    j++;
                    if(j<n1) {
                       putc(',',outfp);
                       if(dosuper) rowptrs[k]++;
                    }
		 }
	         fprintf(outfp,CRLF);
                 rowptrs[k] += 2;
	     }
         } else {
	     /* write out traces */
	     for(k=0; k<n2; ++k) {
		 for(j=0; j<n1;) {
                    sprintf(tempbuf,"%-g",intraces[k][j]);
	            fprintf(outfp,"%s",tempbuf);
                    rowptrs[k] += strlen(tempbuf);
                    j++;
                    if(j<n1) {
                       putc(',',outfp);
                       rowptrs[k]++;
                    }
		 }
	         fprintf(outfp,CRLF);
                 rowptrs[k] += 2;
	     }
         }
       if(dosuper) {
         for(k=1; k<n2; ++k) rowptrs[k] += rowptrs[k-1];
       }
    }

    if(feof(outfp)) {
       perror("unexpected EOF: ");
       seperr("EOF on file \"%s\"\n",outname);
    }
    if(ferror(outfp)) {
       perror("I/O error: ");
       seperr("I/O error on file \"%s\"\n",outname);
    }

    rc = fclose(outfp);
    if(rc != 0) {
       perror("I/O error: ");
       seperr("I/O error closing file \"%s\"\n",outname);
    }

    if(wrapup) {
	nrec = irec;
        (void) fprintf(stderr,"User interrupt - processing only first %lu records.\n", 
			nrec);
    }

    idims[2]++;
    for(j=2; j<(ndims-1); ++j) {
       if(idims[j] == dimlist[j]) {idims[j] = 0; idims[j+1]++;}
    }

  } /* end of main R/W loop */

  if(verbose) (void) fprintf(stderr,"wrote %lu CSV records to output\n",nrec);
  (void) fprintf(csvparfp,"nrec=%lu%s",nrec,CRLF);
  (void) fclose(csvparfp);

  /* free allocated memory */

  for(j=0; j<n2; ++j) {
      if(!sep77) free(inhdrs[j]);
      free(intraces[j]);
  }
  free(intraces);
  if(!sep77) {
      for( j=0; j<nkeys; ++j) {
	  free(keylist[j]);
      }
      free(inhdrs);
      free(keytype);
      free(keylist);
  }

  if(verbose) (void) fprintf(stderr,"freed buffers\n");


  sep_3d_close();

  if(verbose) (void) fprintf(stderr,"closed sep3d\n");

  if(nrec == 0) return (EXIT_FAILURE);

  if(dosuper && ndims > 2) {
     sgfp = fopen(supergather,"w+");
     if(sgfp == ((FILE *) NULL)) {
        perror("supergather file open\n");
        (void) fprintf(stderr,"problem opening supergather file \"%s\" ... exiting\n",supergather);
        goto nosupergather ;
     }

     (void) memset(idims,'\0',sizeof(idims));
     clearerr(sgfp);

     if(0 == strncmp(orient,"inc",3)) { /* in columns */

        for(irec = 0; irec < nrec; ++irec) {
           (void) strcpy(outname,outpref);
           for(j=ndims; j>2; ) {
             --j;
             (void) sprintf(tempbuf,"_%09d",idims[j]);
             (void) strcat(outname,tempbuf);
           }
           (void) strcat(outname,tempbuf);
           outfp = fopen(outname,"r");
           if(outfp == ((FILE *) NULL)) {
              perror("csv input file open failure\n");
              (void) fprintf(stderr,"Error reopening file \"%s\" for input\n",outname);
              break;
           }
           if(sep77) {
              while((c = getc(outfp)) != EOF) putc(c,sgfp);
           } else {
              for(j=0; j<nkeys; ++j) {
                 while(EOF != (rc = getc(outfp))){
                    if(rc == '\012') break;
                 }                
                 if(rc == EOF) {
                    (void) fprintf(stderr,"corrupt csv file?\n"
                           "CRLF not found at end of header line!\n");
                    seperr("corrupt csv file?\n");
                 }
              }
              for(j=0; j<n1; ++j) {
                 /* skip initial ' ,' */
                 (void) getc(outfp); (void) getc(outfp);
                 while(EOF != (rc = getc(outfp))) {
                    putc(rc,sgfp);
                    if(rc == '\012') break;
                 }
                 if(rc == EOF) {
                    (void) fprintf(stderr,"corrupt csv file?\n"
                           "CRLF not found at end of trace data line!\n");
                    seperr("corrupt csv file?\n");
                 }
              }
           }
           rc = fclose(outfp);
           if(rc != 0) {
              perror("I/O error closing input file\n");
           }
           idims[2]++;
           for(j=2; j<(ndims-1); ++j) {
              if(idims[j] == dimlist[j]) { idims[j] = 0; idims[j+1]++; }
           }
        }
     } else { /* in rows */ /* NOT EXPOSED TO PUBLIC */


        if(rowptrs == ((off_t *) NULL)) goto nosupergather;

        for(irec = 0; irec < nrec; ++irec) {
           (void) strcpy(outname,outpref);
           for(j=ndims; j>2; ) {
             --j;
             (void) sprintf(tempbuf,"_%09d",idims[j]);
             (void) strcat(outname,tempbuf);
           }
           (void) strcat(outname,tempbuf);
           outfp = fopen(outname,"r");
           if(outfp == ((FILE *) NULL)) {
              perror("csv input file open failure\n");
              (void) fprintf(stderr,"Error reopening file \"%s\" for input\n",outname);
              break;
           }

           if(!sep77) {
              /* skip initial header line */
              while((c = getc(outfp)) != EOF) { if(c == '\015') /*LF*/ break; }
           }
           for(k=0; k<n2; ++k) {
              if(!sep77) {
                 /* skip the trace header entries */
                 for(j=0; j<nkeys; j++) {
                    while((c = getc(outfp)) != EOF) {if(c == ',') break; }
                 }
              }
              /* copy rest of line to its slot in the supergather */
              rc = fseeko(sgfp,rowptrs[k],SEEK_SET);
              if(rc != 0) {
                  perror("seek error on output supergather\n");
                  seperr("Error seeking in supergather output\n");
              }
              while((c = getc(outfp)) != EOF) {
                 if(c != '\012' && c != '\015') {
                    putc(c,sgfp);
                    rowptrs[k]++;
                 }
                 if(c == '\015') break;
              }
           }

           rc = fclose(outfp);
           if(rc != 0) {
              perror("I/O error closing input file\n");
           }

           idims[2]++;
           for(j=2; j<(ndims-1); ++j) {
              if(idims[j] == dimlist[j]) { idims[j] = 0; idims[j+1]++; }
           }
        }

        /* append CRLF line terminators */
        for(k=0; k<n2; ++k) {
           rc = fseeko(sgfp,rowptrs[k],SEEK_SET);
           if(rc != 0) {
               perror("seek error on output supergather\n");
               seperr("Error seeking in supergather output\n");
           }
           (void) fprintf(sgfp,"%s",CRLF);
        }
     }

     if(feof(sgfp)) {
        perror("supergather EOF: ");
        seperr("Unexpected EOF creating CSV supergather \"%s\"\n",supergather);
     }
     if(ferror(sgfp)) {
        perror("supergather I/O error: ");
        seperr("I/O error creating CSV supergather \"%s\"\n",supergather);
     }
     rc = fclose(sgfp);
     if(rc != 0) {
         perror("Problem closing supergather output file: ");
     }
     if(verbose) fprintf(stderr,"created supergather \"%s\"\n",supergather);
  }

  nosupergather:

  if(rowptrs != ((off_t *) NULL)) free(rowptrs);

  return (EXIT_SUCCESS);
}
