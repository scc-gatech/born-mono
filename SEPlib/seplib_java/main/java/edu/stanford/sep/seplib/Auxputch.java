package edu.stanford.sep.seplib.corelibs.sep;
/**
 * Call seplib auxputch routine.
 *
 * To a large degree, the "format" argument is redundant but kept
 * to limit change. Similarly, the "val" argument uses an array (length 1)
 * for the same reason, though I allow for simple numbers, too.
 */
public class Auxputch {
    /**
     * Write an int to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "i" in which to interpret the
     *               parameter.
     * @param val    Array holding the value to write.
     * @param tag    The tag, e.g. "out", pointing to which auxiliary output
     *               to write to.
     *
     * @return       0 on success.
     */
    public static native int auxputch(String name, String format, int[] val, String tag);
    /**
     * Write a float to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "f" in which to interpret the
     *               parameter.
     * @param val    Array holding the value to write.
     * @param tag    The tag, e.g. "out", pointing to which auxiliary output
     *               to write to.
     *
     * @return       0 on success.
     */
    public static native int auxputch(String name, String format, float[] val, String tag);
    /**
     * Write a double to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "g" in which to interpret the
     *               parameter.
     * @param val    Array holding the value to write.
     * @param tag    The tag, e.g. "out", pointing to which auxiliary output
     *               to write to.
     *
     * @return       0 on success.
     */
    public static native int auxputch(String name, String format, double[] val, String tag);
    /**
     * Write bytes to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "s" in which to interpret the
     *               parameter.
     * @param val    Array holding the value to write.
     * @param tag    The tag, e.g. "out", pointing to which auxiliary output
     *               to write to.
     *
     * @return       0 on success.
     *
     * Note: This is private because seplib doesn't understand writing an
     *       individual byte.
     */
    private static native int auxputch(String name, String format, byte[] val, String tag);
    /**
     * Write an int to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "i" in which to interpret the
     *               parameter.
     * @param val    The value to write.
     * @param tag    The tag, e.g. "out", pointing to which auxiliary output
     *               to write to.
     *
     * @return       0 on success.
     */
    public static int auxputch(String name, String format, int val, String tag) {
       int[] itemp = new int[1];
       itemp[0] = val;
       return auxputch(name,format,itemp,tag);
    }
    /**
     * Write a float to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "f" in which to interpret the
     *               parameter.
     * @param val    The value to write.
     * @param tag    The tag, e.g. "out", pointing to which auxiliary output
     *               to write to.
     *
     * @return       0 on success.
     */
    public static int auxputch(String name, String format, float val, String tag) {
       float[] ftemp = new float[1];
       ftemp[0] = val;
       return auxputch(name,format,ftemp,tag);
    }
    /**
     * Write a double to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "g" in which to interpret the
     *               parameter.
     * @param val    The value to write.
     * @param tag    The tag, e.g. "out", pointing to which auxiliary output
     *               to write to.
     *
     * @return       0 on success.
     */
    public static int auxputch(String name, String format, double val, String tag) {
       double[] dtemp = new double[1];
       dtemp[0] = val;
       return auxputch(name,format,dtemp,tag);
    }
    /**
     * Write a string to named parameter.
     *
     * @param name   The parameter name to write.
     * @param format The format such as "g" in which to interpret the
     *               parameter.
     * @param val    The value to write.
     * @param tag    The tag, e.g. "out", pointing to which auxiliary output
     *               to write to.
     *
     * @return       0 on success.
     */
    public static int auxputch(String name, String format, String val, String tag) {
       return auxputch(name,format,val.getBytes(),tag);
    }

    static {
       System.loadLibrary("sep_JNI");
    }
}
