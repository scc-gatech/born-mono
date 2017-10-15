/* native method for GetNumberAxes class static methods
   sep_{get,put}_data_axis_par() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_GetPutAxis.h"
#include <seplib.h>
#include <sep3d.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <errno.h>

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_GetPutAxis_sep_1get_1data_1axis_1par
  (JNIEnv *env, jclass cls, jstring jtag, jintArray ji_axis, jintArray jn, jfloatArray jo, jfloatArray jd, jbyteArray jlabel) {
   jboolean isCopy;
   const char *tag;
   jint i_axis, n, nlabel;
   jfloat o,d;
   jbyte label[BUFSIZ];
   
   jint rc = -1;

   if((*env)->GetArrayLength(env, ji_axis) < 1) return rc;
   if((*env)->GetArrayLength(env, jn) < 1) return rc;
   if((*env)->GetArrayLength(env, jo) < 1) return rc;
   if((*env)->GetArrayLength(env, jd) < 1) return rc;
   nlabel = (*env)->GetArrayLength(env,jlabel);

   tag = (*env)->GetStringUTFChars(env,jtag,&isCopy);

   if(tag == ((const char *) NULL)) return rc;

   (*env)->GetIntArrayRegion(env,ji_axis,0,1,&i_axis);
   
   rc = (jint) sep_get_data_axis_par(tag, &i_axis, &n, &o, &d, (char *)label);

   if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag,tag);

   if(rc == 0) {
     (*env)->SetIntArrayRegion(env,jn,0,1,&n);
     (*env)->SetFloatArrayRegion(env,jo,0,1,&o);
     (*env)->SetFloatArrayRegion(env,jd,0,1,&d);
     if(nlabel > strlen((const char *)label)) nlabel = strlen((const char *)label)+1;
     (*env)->SetByteArrayRegion(env,jlabel,0,nlabel,label);
   }

   return (rc);
}
/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_GetPutAxis_sep_1put_1data_1axis_1par
  (JNIEnv *env, jclass cls, jstring jtag, jintArray ji_axis, jintArray jn, jfloatArray jo, jfloatArray jd, jbyteArray jlabel) {
   jboolean isCopy;
   const char *tag;
   jint i_axis, n, nlabel;
   jfloat o,d;
   jbyte *label;
   
   int arg1, arg2;
   float arg3, arg4;
   jint rc = -1;

   if((*env)->GetArrayLength(env, ji_axis) < 1) return rc;
   if((*env)->GetArrayLength(env, jn) < 1) return rc;
   if((*env)->GetArrayLength(env, jo) < 1) return rc;
   if((*env)->GetArrayLength(env, jd) < 1) return rc;
   nlabel = (*env)->GetArrayLength(env,jlabel);
   label = (jbyte *) calloc(nlabel+1,sizeof(label[0]));
   if(label == ((jbyte *) NULL)) return rc;

   tag = (*env)->GetStringUTFChars(env,jtag,&isCopy);

   if(tag == ((const char *) NULL)) return rc;

   (*env)->GetIntArrayRegion(env,ji_axis,0,1,&i_axis);
   (*env)->GetIntArrayRegion(env,jn,0,1,&n);
   (*env)->GetFloatArrayRegion(env,jo,0,1,&o);
   (*env)->GetFloatArrayRegion(env,jd,0,1,&d);
   (*env)->GetByteArrayRegion(env,jlabel,0,nlabel,label);
   arg1 = (int) i_axis; arg2 = (int) n; arg3 = (float) o; arg4 = (float) d;
   rc = (jint) sep_get_data_axis_par(tag, &arg1, &arg2, &arg3, &arg4, (char *)label);

   if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag,tag);

   return (rc);
}
