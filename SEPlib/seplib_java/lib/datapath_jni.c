/* native method for Datapath class static method datapath() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Datapath.h"
#include <seplib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Datapath_datapath
  (JNIEnv *env, jclass cls, jbyteArray jdatapath) {
   jboolean isCopy;
   jsize dataLen;
   jbyte *buf;

   dataLen = (*env)->GetArrayLength(env,jdatapath);
   buf = (jbyte *) calloc(dataLen,sizeof(jbyte)+1);
   if(buf == ((jbyte *) NULL)) return 0;

   (*env)->GetByteArrayRegion(env,jdatapath,0,dataLen,buf);
   (void) datapath((char *)buf);
   dataLen = (jsize) strlen((const char *) buf);
   if(dataLen > 0) (*env)->SetByteArrayRegion(env,jdatapath,0,dataLen,buf);
   return ((jint) dataLen);
}
