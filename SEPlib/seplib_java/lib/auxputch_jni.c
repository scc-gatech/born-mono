/* native method for Auxputch class static methods putch() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Auxputch.h"
#include <seplib.h>

#define BODYCODE(Typeval, typeval, mytypeval) \
  j ## typeval *valBuf; \
  jsize valBufLen; \
  jboolean isCopyName, isCopyFormat, isCopyTag; \
  const char *name, *format, *tag; \
  jint rc; \
  int i; \
  j ## typeval *tmpVals; \
  mytypeval *myVals; \
  name = (*env)->GetStringUTFChars(env,jname,&isCopyName); \
  if(name == ((char *) NULL)) return(-1); /* an exception was thrown */ \
  format = (*env)->GetStringUTFChars(env,jformat,&isCopyFormat); \
  if(format == ((char *) NULL)) return(-1); /* an exception was thrown */ \
  tag = (*env)->GetStringUTFChars(env,jtag,&isCopyTag); \
  if(tag == ((char *) NULL)) return(-1); /* an exception was thrown */ \
  valBufLen = (*env)->GetArrayLength(env, jvals); \
  tmpVals = (j ## typeval *) calloc(valBufLen,sizeof(j ## typeval)); \
  if(tmpVals != (j ## typeval *) NULL) { \
  (*env)->Get ## Typeval ## ArrayRegion(env,jvals,0,valBufLen,tmpVals); \
  myVals = (mytypeval *) calloc(valBufLen+1,sizeof(mytypeval)); \
  if(myVals != (mytypeval *) NULL) { \
  for(i=0; i<valBufLen; i++) { \
  myVals[i] = (mytypeval) tmpVals[i]; } \
  rc = auxputch((char *)name, (char *)format, myVals, (char *)tag); \
  free(myVals); \
  } else { rc = -1; } \
  free(tmpVals); \
  } else { rc = -1; } \
  if(isCopyTag == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag,tag); \
  if(isCopyFormat == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jformat,format); \
  if(isCopyName == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jname,name); \
  return(rc)

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Auxputch_putch__Ljava_lang_String_2Ljava_lang_String_2_3ILjava_lang_String_2
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jintArray jvals, jstring jtag) {

   BODYCODE(Int,int,int);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Auxputch_putch__Ljava_lang_String_2Ljava_lang_String_2_3FLjava_lang_String_2
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jfloatArray jvals, jstring jtag){

   BODYCODE(Float,float,float);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Auxputch_putch__Ljava_lang_String_2Ljava_lang_String_2_3DLjava_lang_String_2
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jdoubleArray jvals, jstring jtag){

   BODYCODE(Double,double,double);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Auxputch_putch__Ljava_lang_String_2Ljava_lang_String_2_3BLjava_lang_String_2
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jbyteArray jvals, jstring jtag){

  BODYCODE(Byte,byte,char);
}
