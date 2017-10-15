package edu.stanford.sep.seplib.corelibs.sep;
/**
 * Call seplib datapath() routine.
 *
 * @author  Stewart A. Levin 
 */
public class Datapath {
    /**
     * @param datapath  byte[] buffer to hold output.
     *
     * @return strlen of datapath.
     */
    private static native int datapath(byte[] datapath);

    /**
     * Return the current datapath.
     */
    public static String datapath() {
       byte[] tmpbuf = new byte[4096];
       int rc = datapath(tmpbuf);
       return new String(java.util.Arrays.copyOfRange(tmpbuf,0,rc));
    }

    static {
       System.loadLibrary("sep_JNI");
    }
}

