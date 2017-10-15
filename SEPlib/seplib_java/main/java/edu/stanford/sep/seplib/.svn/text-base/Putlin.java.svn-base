package edu.stanford.sep.seplib.corelibs.sep;
/**
 * Call seplib putlin() routine.
 *
 * @author  Stewart A. Levin 
 */
public class Putlin {
    /**
     * @param textline  String containing the text to add to the history file
     */
    public static native int putlin(String textline);

    static {
       System.loadLibrary("sep_JNI");
    }
}

