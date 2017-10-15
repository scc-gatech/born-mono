/*
 *  Csv2Sep csvpar="csv.par" verbose=n > outfile.H
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
sep_add_doc_line("    Csv2Sep - import SEP gathers from comma-separated value (CSV) files");\
sep_add_doc_line("");\
sep_add_doc_line("SYNOPSIS");\
sep_add_doc_line("    Csv2Sep > out.H csvpar=\"csv.par\" verbose=n");\
sep_add_doc_line("");\
sep_add_doc_line("INPUT PARAMETERS");\
sep_add_doc_line("");\
sep_add_doc_line("    csvpar - string");\
sep_add_doc_line("                  parfile created by Sep2Csv converter");\
sep_add_doc_line("");\
sep_add_doc_line("    verbose - boolean");\
sep_add_doc_line("                  Set to y or 1 for progress details.");\
sep_add_doc_line("");\
sep_add_doc_line("DESCRIPTION");\
sep_add_doc_line("");\
sep_add_doc_line("    The Comma-Separated Value text file format (RFC 4180) is widely accepted");\
sep_add_doc_line("    by many software packages as a way to deal with matrix-like");\
sep_add_doc_line("    spreadsheet data. This utility reads each 2D gather in the");\
sep_add_doc_line("    input CSV dataset into an SEP dataset.");\
sep_add_doc_line("    If the input has trace headers, they, too, are copied to the");\
sep_add_doc_line("    SEP file.");\
sep_add_doc_line("");\
sep_add_doc_line("SEE ALSO");\
sep_add_doc_line("    Sep2Csv");\
sep_add_doc_line("");\
sep_add_doc_line("CATEGORY");\
sep_add_doc_line("    converters/csv");\
sep_add_doc_line(""); 

#define NOHEADER 1
#define IN "/dev/null"

#include <sep.main> 

#include <sep3d.h> 

#include "../lib/corelibs/include/sep_main_internal.h"

/* allow for graceful exit after interrupt */
static volatile int wrapup;
static void gotowrapup(int signum) {
  wrapup = 1;
} 

/* 3 pass logic:
   a) scan CSV's to find longest text line for memory allocation
   b) scan CSV's to compile list of trace headers, if any
   c) read and convert trace headers and trace data to SEP3D output
*/

int MAIN(void) {

  int n1=1, n2=1; 
  int ndims;
  int dimlist[7] = {1, 1, 1, 1, 1, 1, 1};
  int idims[7] = {0, 0, 0, 0, 0, 0, 0};
  float o[7] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
  float d[7] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
  float **outtraces;
  int **outhdrs;
  int verbose = 0;
  int inhdr;
  char outpref[BUFSIZ];
  char orient[BUFSIZ];
  char csvpar[BUFSIZ];
  char *tempbuf = (char *) NULL; 
  char labelbuf[BUFSIZ]; 
  char stdoutname[BUFSIZ]; 
  char hdrname[256];
  char hdrtype[32];
  int nbytes, nreed, nrite;
  const int synch = 1;
  int dosuper;
  int nbytesHdr;
  size_t ikey, nkeys = 0; 
  int nvals = 1;
  int c;
  int rc;
  int i,j,k;
  char **keylist;
#define INTKEY 1
#define FLOATKEY 2
  int  *keytype;
  char *cptr;
  char **ccptr;
  off_t *rowptrs;
  char ctmp;
  size_t slen;
  float ftemp;
  double dtemp;
  unsigned long irec, jrec, nrec;
  char *sortdir;
  union {
    int i;
    float f;
  } convbuf; 
  int imgood;
  int hdrfd;
  int itemp;
  int istat;
  int drn_key;
  int drn;
  int ioff;
  int sep77;
  int nhdrbytes;
  int nhdrblocks;
  int maxlinelen;
  char *tok;
  char *tokarg;
  char *keyname;
  char *tag_header[1];
  char csvname[BUFSIZ];
#define CRLF "\015\012"
  FILE *csvparfp;
  FILE *csvfp;
  FILE *sgfp;

  init_3d();

  (void) setlocale(LC_ALL,"C");

  tempbuf = (char *) malloc(BUFSIZ);
  if(tempbuf == ((char *) NULL)) seperr("Unable to allocate memory!\n");

  /* make sure we have and can read the control file created by Sep2Csv */
  if(0 == getch("csvpar","s",csvpar)) {
     seperr("missing csvpar command line control file argument!\n");
  }
  rc = access(csvpar, R_OK);
  if(rc != 0) {
     perror(csvpar);
     seperr("Unable to access csvpar=\"%s\"\n",csvpar);
  }

  if(0 == getch("verb verbose","1",&verbose)) verbose = 0;
  
  /* retrieve the original Sep2Csv parameters */
  if(1 > auxpar("outpref","s",outpref,"csvpar")) {
     if(1 > getch("outpref","s",outpref))
        seperr("Missing outpref= in \"%s\"\n",csvpar);
  }
  if(1 > auxpar("orient","s",orient,"csvpar")) {
     if(1 > getch("orient","s",orient))
        seperr("Missing orient= in \"%s\"\n",csvpar);
  }
  if(1 > auxpar("ndims","d",&ndims,"csvpar")) {
     if(1 > getch("ndims","d",&ndims))
        seperr("Missing ndims= in \"%s\"\n",csvpar);
  }

  /* retrieve saved origins and increments */
  nrec = 1;
  for(j=0; j<ndims; ++j) {
     (void) sprintf(tempbuf,"dimlist[%d]",j);
     if(1 > auxpar(tempbuf,"d",dimlist+j,"csvpar")) {
        (void) sprintf(tempbuf,"n%d",j+1);
        if(1 > getch(tempbuf,"d",dimlist+j))
        seperr("Missing %s! check \"%s\"\n",tempbuf, csvpar);
     nrec *= dimlist[j];
     }
     (void) sprintf(tempbuf,"o%d",j+1);
     if(1 > auxpar(tempbuf,"f",o+j,"csvpar"))
        (void) getch(tempbuf,"f",o+j);
     tempbuf[0] = 'd';
     if(1 > auxpar(tempbuf,"f",d+j,"csvpar"))
        (void) getch(tempbuf,"f",d+j);
  }

  if(1 > auxpar("nrec","d",&nrec,"csvpar"))
      (void) getch("nrec","d",&nrec);

  memset(idims,'\0',sizeof(idims));

  maxlinelen = 0;
  /* phase 1: scan inputs to determine maximum line length */
  for(irec=0; irec<nrec; irec++) {

    (void) strcpy(csvname,outpref);
    for(j=ndims; j>2; ) {
       --j;
       (void) sprintf(tempbuf,"_%09d",idims[j]);
       (void) strcat(csvname,tempbuf);
    }
    (void) strcat(csvname,".csv");
    csvfp = fopen(csvname,"r");
    if(csvfp != ((FILE *) NULL)) {
       k = 0;
       while(EOF != (rc = getc(csvfp))) {
         k++;
         if(rc == '\012') {
           if(k > maxlinelen) maxlinelen = k;
           k = 0;
         }
       }
       if(k != 0) {
          sepwarn(2,"file \"%s\" missing terminating newline.\n",csvname);
          if(k > maxlinelen) maxlinelen = k;
       }
       fclose(csvfp);
    }

    /* update for next file name */
    idims[2]++;
    for(j=2; j<(ndims-1); ++j) {
       if(idims[j] == dimlist[j]) {idims[j] = 0; idims[j+1]++;}
    }
  }

  /* reallocate tempbuf to hold complete line */
  tempbuf = (char *) realloc(tempbuf, maxlinelen+2);
  if(tempbuf == ((char *) NULL))
     seperr("Unable to allocate enough memory to hold a single csv line!\n");


  n1 = 1; n2=1;

  memset(idims,'\0',sizeof(idims));

  /* phase 2: scan inputs to determine list of headers, if any,
   * and maximum n1, n2
   */
  for(irec=0; irec<nrec; irec++) {

    (void) strcpy(csvname,outpref);
    for(j=ndims; j>2; ) {
       --j;
       (void) sprintf(tempbuf,"_%09d",idims[j]);
       (void) strcat(csvname,tempbuf);
    }
    (void) strcat(csvname,".csv");
    csvfp = fopen(csvname,"r");
    if(csvfp != ((FILE *) NULL)) {

       if(0 == strncmp(orient,"inc",3)) { /* traces go down columns */
          /* see if there are any trace header names in the first column */
          j=0;
          while(((char *)NULL) != fgets(tempbuf,maxlinelen+1,csvfp)) {
             tokarg = &(tempbuf[0]);
             tok = strtok(tokarg,"\t ,");
             if(tok == ((char *) NULL)) /* something is really wrong? */ break;
             if(0 == sscanf(tok,"%lg",&dtemp)) {
                for(ikey=0; ikey<nkeys; ++ikey) {
                    if(0 == strcmp(tok,keylist[ikey])) break;
                }
                if(ikey >= nkeys) { /* new header name */
                   keylist[nkeys] = strdup(tok);
                   keytype[nkeys] = (NULL == strchr(tok+strlen(tok)+1,'.')) ? INTKEY : FLOATKEY;
                   nkeys++;
                   keylist = (char **) realloc(keylist, nkeys+1);
                   keytype = (int *) realloc(keytype, nkeys+1);
                   keylist[nkeys] = (char *) NULL;
                   keytype[nkeys] = 0;
                }
                /* check if we got a complete line */
                for(i=0; i<sizeof(tempbuf); ++i) 
                   if(tempbuf[i] == '\012') break;
                /* if not, consume rest of current line */
                if(i < sizeof(tempbuf)) {
                   while(EOF != (rc = fgetc(csvfp))) {
                      if(rc == '\012') break;
                   }
                }
             } else {
                k = 1; j++;
                /* count number of numeric entries */
                tokarg = (char *) NULL;
                while(strtok(tokarg,"\t ,") != ((char *) NULL)) ++k;
                if(k > n2) n2 = k;
             }
          }
          if(j > n1) n1 = j;
       
       } else { /* traces go across rows - NOT IMPLEMENTED */
          j=0;
          k=1;
       }

       fclose(csvfp);
    }


    /* update for next file name */
    idims[2]++;
    for(j=2; j<(ndims-1); ++j) {
       if(idims[j] == dimlist[j]) {idims[j] = 0; idims[j+1]++;}
    }
  }
       
  /* create and populate a trace header file if needed */

  if(nkeys > 0) {
     rc = findnm(fileno(stdout),stdoutname,BUFSIZ-2);
     if(rc > 0) {
         strcpy(stdoutname+rc,"@@");
         if(((FILE *) NULL) == auxout(stdoutname)) {
            sepwarn(3,"Unable to create output trace header file \"%s\"\n"
                     "Will continue without creating trace headers.\n",
                     stdoutname);
            nkeys = 0;
         }
     } else {
         stdoutname[0] = '\0';
         (void) datapath(stdoutname);
         if(tempbuf[0] == '\0') {
            sepwarn(4,"Trouble trying to create output trace header file.\n"
                    "Will continue without creating trace headers.\n");
            nkeys = 0;
         } else {
           (void) strcat(stdoutname,"/sephdrsXXXXXX");
           if(-1 == (hdrfd = mkstemp(stdoutname))) {
              sepwarn(5,"Trouble trying to create output trace header file.\n"
                      "Will continue without creating trace headers.\n");
              nkeys = 0;
           }
           if(((FILE *) NULL) == auxout(stdoutname)) {
              sepwarn(6,"Unable to create output trace header file \"%s\"\n"
                     "Will continue without creating trace headers.\n",
                     tempbuf);
              nkeys = 0;
           }
        }
     }
  }
  if(nkeys > 0) {
     rc = auxputch("n1","d",&nkeys,stdoutname);
     ftemp = 0.0f;
     rc = auxputch("o1","f",&ftemp,stdoutname);
     ftemp = 1.0f;
     rc = auxputch("d1","f",&ftemp,stdoutname);
     for(j=1; j<ndims; ++j) {
        (void) sprintf(tempbuf,"n%d",j+1);
        rc = auxputch(tempbuf,"d",dimlist+j,stdoutname);
        tempbuf[0] = 'o';
        rc = auxputch(tempbuf,"f",o+j,stdoutname);
        tempbuf[0] = 'd';
        rc = auxputch(tempbuf,"f",d+j,stdoutname);
     }
     for(j=0; j<nkeys; ++j) {
        (void) sprintf(tempbuf,"hdrkey%d",j+1);
        rc = auxputch(tempbuf,"s",keylist[j],stdoutname);
        (void) sprintf(tempbuf,"hdrtype%d",j+1);
        rc = auxputch(tempbuf,"s","scalar_float",stdoutname);
        (void) sprintf(tempbuf,"hdrfmt%d",j+1);
        rc = auxputch(tempbuf,"s","xdr_float",stdoutname);
     }
  }


/*-----------------------------------------------------------------------*/
  sep77 = (nkeys == 0);
       
  outtraces = (float **)calloc(n2,sizeof(float *));
  if(outtraces == ((float **) NULL)) {
      seperr("Unable to allocate memory!\n");
  }
  for(j=0; j<n2; ++j) {
      outtraces[j] = calloc(n1,sizeof(float));
      if(outtraces[j] == ((float *) NULL)) seperr("Unable to allocate memory!\n");
  }
  nbytes = (int) (n1*sizeof(float));

  if(!sep77) {
     nbytesHdr = nkeys*sizeof(float);
     outhdrs = (int **) calloc(n2,sizeof(int *));
     if(outhdrs == ((int **) NULL)) {
        seperr("Unable to allocate memory!\n");
     }
     for(j=0; j<n2; ++j) {
        outhdrs[j] = (int *) calloc(nbytesHdr,sizeof(char));
        if(outhdrs[j] == ((int *) NULL))
            seperr("Unable to allocate memory!\n");
     }
  } else {
      nbytesHdr = 0;
  }

  
  wrapup = 0;
  (void) signal(SIGINT,gotowrapup); 

  (void) memset(idims,'\0',sizeof(idims));

  if(0 == strncmp(orient,"inc",3)) { /* in columns */

        for(irec = 0; irec < nrec; ++irec) {
           (void) strcpy(csvname,outpref);
           for(j=ndims; j>2; ) {
             --j;
             (void) sprintf(tempbuf,"_%09d",idims[j]);
             (void) strcat(csvname,tempbuf);
           }
           (void) strcat(csvname,tempbuf);
           csvfp = fopen(csvname,"r");
           if(csvfp == ((FILE *) NULL)) {
              perror("csv input file open failure\n");
              (void) fprintf(stderr,"Error reopening file \"%s\" for input\n",csvname);
              break;
           }
           if(!sep77) for(j=0; j<n2; ++j) memset(outhdrs[j],'\0',nkeys*sizeof(float));
           for(j=0; j<n2; ++j) memset(outtraces[j],'\0',n1*sizeof(float));
           if(sep77) {
              j=0; k=0;
              while(!feof(csvfp) && !ferror(csvfp)) {
                 if(1 != fscanf(csvfp,"%lg",&dtemp)) break;
                 outtraces[j][k++] = (float) dtemp;
                 while((c = getc(csvfp)) != EOF) {
                    if(c == ',') break; /* skip trailing commas */
                    if(c == '\012') { k=0; j++; break; }
                 }
              }
           } else {
              j = 0;
             
              while(((char *)NULL) != fgets(tempbuf,maxlinelen+1,csvfp)) {
                 tokarg = &(tempbuf[0]);  
                 tok = strtok(tokarg,"\t ,");
                 k = 0;
                 if(0 == sscanf(tok,"%lg",&dtemp)) { /* trace header name */
                    for(ikey = 0; ikey < nkeys; ++ikey) {
                       if(0 == strcmp(tok,keylist[ikey])) break;
                    }
                    if(ikey < nkeys) {
                       tokarg = (char *) NULL;
                       while(((char *) NULL) != (tok = strtok(tokarg,"\t ,"))) {
                          dtemp = 0.0;
                          (void) sscanf(tok,"%lg",&dtemp);
                          outhdrs[ikey][k++] = (float) dtemp;
                       }
                    }
                 } else { 
                    outtraces[j][k++] = (float) dtemp;
                    tokarg = (char *) NULL;
                    while(((char *) NULL) != (tok = strtok(tokarg,"\t ,"))) {
                       dtemp = 0.0;
                       (void) sscanf(tok,"%lg",&dtemp);
                       outtraces[j][k++] = (float) dtemp;
                    }
                    j++;
                 }
              }
           }
           rc = fclose(csvfp);
           if(rc != 0) {
              perror("I/O error closing input file\n");
           }
           if(!sep77) { /* write out trace headers */
              rc = (int) (nkeys*sizeof(float));
              for(j=0; j<n2; ++j) if(rc != srite(stdoutname,outhdrs[j],rc)) {
                  perror("Problem writing data trace headers\n");
                  break;
              }
           }
           rc = (int) (n1*sizeof(float));
           for(j=0; j<n2; ++j) if(rc != srite("out",outtraces[j],rc)) {
               perror("Problem writing data traces\n");
               break;
           }

           idims[2]++;
           for(j=2; j<(ndims-1); ++j) {
              if(idims[j] == dimlist[j]) { idims[j] = 0; idims[j+1]++; }
           }
        }
  /* } else { */ /* in rows */
  }

  sep_3d_close();

  if(verbose) (void) fprintf(stderr,"closed sep3d\n");

  for(j=0; j<n2; ++j) {
    if(!sep77) free(outhdrs[j]);
    free(outtraces[j]);
  }
  free(outtraces);
  if(!sep77) free(outhdrs);

  if(rowptrs != ((off_t *) NULL)) free(rowptrs);

  if(nrec == 0) return (EXIT_FAILURE);


  return (EXIT_SUCCESS);
}
