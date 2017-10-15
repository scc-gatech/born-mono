package edu.stanford.sep.seplib.corelibs.sep;
/**
 *  Call seplib routine redin()
 *
 * @author  Stewart A. Levin
 */
public class Redin {
    public static native boolean redin();

    static {
       System.loadLibrary("sep_JNI");
    }
}

