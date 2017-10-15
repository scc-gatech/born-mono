/* native method for Auxpar class static methods auxpar() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Auxpar.h"
#include <seplib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#define BODYCODE(Typeval, typeval, mytypeval) \
  j ## typeval *valBuf; \
  jsize valBufLen; \
  jboolean isCopyName, isCopyFormat, isCopyTag, isCopyVals; \
  const char *name, *format, *tag; \
  jint rc; \
  jint i; \
  jclass exceptCls; \
  mytypeval myVals[BUFSIZ]; /* try to protect against memory overflow */ \
  name = (*env)->GetStringUTFChars(env,jname,&isCopyName); \
  if(name == ((char *) NULL)) return(0); /* an exception was thrown */ \
  format = (*env)->GetStringUTFChars(env,jformat,&isCopyFormat); \
  if(format == ((char *) NULL)) return(0); /* an exception was thrown */ \
  tag = (*env)->GetStringUTFChars(env,jtag,&isCopyTag); \
  if(tag == ((char *) NULL)) return(0); /* an exception was thrown */ \
  rc = auxpar((char *)name, (char *)format, myVals, (char *) tag); \
  if(rc < 1) return(rc); \
  if( ((char *) format)[0] == 's' ) rc = strlen((char *) myVals); \
  if(rc > sizeof(myVals)/sizeof(myVals[0])) { /* throw an overflow exception */\
     exceptCls = (*env)->FindClass(env,"java/lang/ArrayIndexOutOfBoundsException"); \
     if(exceptCls != ((jclass) NULL)) { \
         (*env)->ThrowNew(env,exceptCls,"auxpar array overrun\n"); \
     } \
     return(rc); \
  } \
  /* Copy auxpar result to output argument */ \
  valBufLen = (*env)->GetArrayLength(env, jvals); \
  if(rc > valBufLen) rc = valBufLen; /* shorten to prevent overflow */ \
  valBuf = (*env)->Get ## Typeval ## ArrayElements(env, jvals, &isCopyVals); \
  if(valBuf == ((j ## typeval *) NULL)) return(0); /* an exception was thrown */ \
  for(i=0; i<rc; ++i) valBuf[i] = myVals[i]; \
  if(isCopyVals == JNI_TRUE) (*env)->Release ## Typeval ## ArrayElements(env, jvals, valBuf, 0); \
  if(isCopyTag == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag,tag); \
  if(isCopyFormat == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jformat,format); \
  if(isCopyName == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jname,name); \
  return(rc)

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Auxpar_auxpar__Ljava_lang_String_2Ljava_lang_String_2_3ILjava_lang_String_2
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jintArray jvals, jstring jtag) {

   BODYCODE(Int,int,int);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Auxpar_auxpar__Ljava_lang_String_2Ljava_lang_String_2_3FLjava_lang_String_2
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jfloatArray jvals, jstring jtag){

   BODYCODE(Float,float,float);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Auxpar_auxpar__Ljava_lang_String_2Ljava_lang_String_2_3DLjava_lang_String_2
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jdoubleArray jvals, jstring jtag){

   BODYCODE(Double,double,double);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Auxpar_auxpar__Ljava_lang_String_2Ljava_lang_String_2_3BLjava_lang_String_2
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jbyteArray jvals, jstring jtag){

  BODYCODE(Byte,byte,char);
}
