package edu.stanford.sep.seplib.corelibs.sep;
/**
 * Call seplib sep_get_number_data_axes() routine.
 *
 * @author  Stewart A. Levin 
 */
public class GetNumberAxes {
    /**
     * @param tag  String containing the tag of the seplib dataset.
     * @param n_axis int[1] array for storing the output number of axes
     *
     * @return   0 if successful, 1 if tag is Sep77 history, -1 otherwise
     */
    public static native int sep_get_number_data_axes(String tag, int[] n_axis);

    /**
     * @param tag  String containing the tag of the seplib dataset.
     * @param n_axis int[1] array for storing the output number of axes
     *
     * @return   0 if successful, 1 if tag is Sep77 history, -1 otherwise
     */
    public static int getnumberaxes(String tag, int[] n_axis) {
       return sep_get_number_data_axes(tag, n_axis);
    }

    static {
       System.loadLibrary("sep_JNI");
    }
}
