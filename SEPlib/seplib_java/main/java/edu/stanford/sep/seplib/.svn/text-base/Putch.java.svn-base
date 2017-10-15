package edu.stanford.sep.seplib.corelibs.sep;
/**
 * Call seplib putch routine.
 *
 * To a large degree, the "format" argument is redundant but kept
 * to limit change. Similarly, the "val" argument uses an array (length 1)
 * for the same reason, though I allow for simple numbers, too.
 */
public class Putch {
    /**
     * Write an int to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "i" in which to interpret the
     *               parameter.
     * @param val    Array holding the value to write.
     *
     * @return       0 on success.
     */
    public static native int putch(String name, String format, int[] val);
    /**
     * Write a float to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "f" in which to interpret the
     *               parameter.
     * @param val    Array holding the value to write.
     *
     * @return       0 on success.
     */
    public static native int putch(String name, String format, float[] val);
    /**
     * Write a double to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "g" in which to interpret the
     *               parameter.
     * @param val    Array holding the value to write.
     *
     * @return       0 on success.
     */
    public static native int putch(String name, String format, double[] val);
    /**
     * Write bytes to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "s" in which to interpret the
     *               parameter.
     * @param val    Array holding the value to write.
     *
     * @return       0 on success.
     *
     * Note: This is private because seplib doesn't understand writing an
     *       individual byte.
     */
    private static native int putch(String name, String format, byte[] val);
    /**
     * Write an int to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "i" in which to interpret the
     *               parameter.
     * @param val    The value to write.
     *
     * @return       0 on success.
     */
    public static int putch(String name, String format, int val) {
       int[] itemp = new int[1];
       itemp[0] = val;
       return putch(name,format,itemp);
    }
    /**
     * Write a float to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "f" in which to interpret the
     *               parameter.
     * @param val    The value to write.
     *
     * @return       0 on success.
     */
    public static int putch(String name, String format, float val) {
       float[] ftemp = new float[1];
       ftemp[0] = val;
       return putch(name,format,ftemp);
    }
    /**
     * Write a double to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "g" in which to interpret the
     *               parameter.
     * @param val    The value to write.
     *
     * @return       0 on success.
     */
    public static int putch(String name, String format, double val) {
       double[] dtemp = new double[1];
       dtemp[0] = val;
       return putch(name,format,dtemp);
    }
    /**
     * Write a string to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "g" in which to interpret the
     *               parameter.
     * @param val    The value to write.
     *
     * @return       0 on success.
     */
    public static int putch(String name, String format, String val) {
       return putch(name,format,val.getBytes());
    }

    static {
       System.loadLibrary("sep_JNI");
    }
}

