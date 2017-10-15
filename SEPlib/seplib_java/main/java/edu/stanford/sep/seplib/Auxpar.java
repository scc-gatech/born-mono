package edu.stanford.sep.seplib.corelibs.sep;
/**
 * Call seplib auxpar routine.
 *
 * To a large degree, the "format" argument is redundant but kept
 * to limit change.
 */
public class Auxpar {
    /**
     * Retrieve ints for named parameter.
     *
     * @param name   The parameter name to search for.
     * @param format The format such as "i" in which to interpret the
     *	             parameter.
     * @param val    Array to hold the values retrieved, if any. The number
     *               actually retrieved is returned and will never exceed the
     *               length of this array.
     * @param tag    The tag, e.g. "in", pointing to which auxiliary input
     *               should be searched to find the parameter.
     *
     * @return       The number of values returned.
     */
    public static native int auxpar(String name, String format, int[] val, String tag);
    /**
     * Retrieve floats for named parameter.
     *
     * @param name   The parameter name to search for.
     * @param format The format such as "f" in which to interpret the
     *	             parameter.
     * @param val    Array to hold the values retrieved, if any. The number
     *               actually retrieved is returned and will never exceed the
     *               length of this array.
     * @param tag    The tag, e.g. "in", pointing to which auxiliary input
     *               should be searched to find the parameter.
     *
     * @return       The number of values returned.
     */
    public static native int auxpar(String name, String format, float[] val, String tag);
    /**
     * Retrieve doubles for named parameter.
     *
     * @param name   The parameter name to search for.
     * @param format The format such as "g" in which to interpret the
     *	             parameter.
     * @param val    Array to hold the values retrieved, if any. The number
     *               actually retrieved is returned and will never exceed the
     *               length of this array.
     * @param tag    The tag, e.g. "in", pointing to which auxiliary input
     *               should be searched to find the parameter.
     *
     * @return       The number of values returned.
     */
    public static native int auxpar(String name, String format, double[] val, String tag);
    /**
     * Retrieve bytes for named parameter.
     *
     * @param name   The parameter name to search for.
     * @param format The format such as "s" in which to interpret the
     *	             parameter.
     * @param val    Array to hold the values retrieved, if any. The number
     *               actually retrieved is returned and will never exceed the
     *               length of this array.
     * @param tag    The tag, e.g. "in", pointing to which auxiliary input
     *               should be searched to find the parameter.
     *
     * @return       The number of values returned.
     *
     * Note: this is private because auxpar doesn't know about bytes.
     */
    private static native int auxpar(String name, String format, byte[] val, String tag);
    
    /**
     * Retrieve a String for named parameter.
     *
     * @param name   The parameter name to search for.
     * @param format The format such as "s" in which to interpret the
     *	             parameter.
     * @param val    StringBuffer to hold the value retrieved, if any.
     * @param tag    The tag, e.g. "in", pointing to which auxiliary input
     *               should be searched to find the parameter.
     *
     * @return       0 or 1: the number of strings returned.
     */
    public int auxpar(String name, String format, StringBuffer val, String tag) {
       int rc = 0;
       byte byteBuf[] = new byte[2048];
       rc = auxpar(name, format, byteBuf, tag);
       if(rc > 0) {
          byte tmpBuf[] = new byte[rc];
          System.arraycopy(byteBuf,0,tmpBuf,0,rc);
          String tmpStr = new String(byteBuf);
          val.setLength(0);
          val.insert(0,tmpStr);
       }
       if(rc > 1) rc = 1;
       return rc;
    }

    static {
       System.loadLibrary("sep_JNI");
    }
}

