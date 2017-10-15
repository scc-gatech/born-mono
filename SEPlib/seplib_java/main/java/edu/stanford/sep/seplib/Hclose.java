package edu.stanford.sep.seplib.corelibs.sep;
/**
 *  Call seplib routine hclose()
 *
 * @author  Stewart A. Levin
 */
public class Hclose {
    public static native void hclose();

    static {
       System.loadLibrary("sep_JNI");
    }
}

