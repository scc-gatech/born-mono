package edu.stanford.sep.seplib.corelibs.sep;
/**
 * Call seplib sep_{get,put}_data_axis_par() routine.
 *
 * @author  Stewart A. Levin 
 */
public class GetPutAxis {
    /**
     * @param tag  String containing the tag of the seplib dataset.
     * @param i_axis Axis number of interest. Axes numbered from 1.
     * @param n Number of samples in axis of interest.
     * @param o Origin on axis of interest.
     * @param d Spacing on axis of interest.
     * @param label Label on axis of interest.
     *
     * @return   0 if successful, 1 if tag is not Sep history, -1 otherwise
     */
    public static native int sep_get_data_axis_par(String tag, int[] i_axis, int[] n, float[] o, float[] d, byte[] label);
    /**
     * @param tag  String containing the tag of the seplib dataset.
     * @param i_axis Axis number of interest. Axes numbered from 1.
     * @param n Number of samples in axis of interest.
     * @param o Origin on axis of interest.
     * @param d Spacing on axis of interest.
     * @param label Label on axis of interest.
     *
     * @return   0 if successful, 1 if tag is not Sep history, -1 otherwise
     */
    public static native int sep_put_data_axis_par(String tag, int[] i_axis, int[] n, float[] o, float[] d, byte[] label);

    /**
     * @param tag  String containing the tag of the seplib dataset.
     * @param i_axis Axis number of interest. Axes numbered from 1.
     * @param n Number of samples in axis of interest.
     * @param o Origin on axis of interest.
     * @param d Spacing on axis of interest.
     * @param label Label on axis of interest.
     *
     * @return   0 if successful, 1 if tag is not Sep history, -1 otherwise
     */
    public static int sep_get_data_axis_par(String tag, int[] i_axis, int[] n, float[] o, float[] d, StringBuffer label) {
       byte[] tmpbuf = new byte[256];
       int rc;
       rc = sep_get_data_axis_par(tag,i_axis,n,o,d,tmpbuf);
       if(rc == 0) {
          int i;
          for(i=0; i< tmpbuf.length; ++i) {
              if(tmpbuf[i] == 0) break;
          }
          byte tmpbuf2[] = new byte[i];
          System.arraycopy(tmpbuf,0,tmpbuf2,0,i);
          String tmpstr = new String(tmpbuf2);
          label.setLength(0);
          label.insert(0,tmpstr);
       }
       return rc;
    }

    /**
     * @param tag  String containing the tag of the seplib dataset.
     * @param i_axis Axis number of interest. Axes numbered from 1.
     * @param n Number of samples in axis of interest.
     * @param o Origin on axis of interest.
     * @param d Spacing on axis of interest.
     * @param label Label on axis of interest.
     *
     * @return   0 if successful, 1 if tag is not Sep history, -1 otherwise
     */
    public static int sep_put_data_axis_par(String tag, int[] i_axis, int[] n, float[] o, float[] d, String label) {
       return sep_put_data_axis_par(tag,i_axis,n,o,d,label.getBytes());
    }

    static {
       System.loadLibrary("sep_JNI");
    }
}
