
#include <stdio.h>
#include <qwidget.h>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <vector>
#include <qpixmap.h>
#include <qpainter.h>
#include "bar_pt.h"
#ifndef CLIP_BAR_H
#define CLIP_BAR_H 1


class clip_bar: public QWidget
{
	Q_OBJECT
  signals:
    void emitit(std::vector<QString> com);
  public:
    clip_bar(  int id, QWidget *parent = 0, const char *name = 0 );
    
    void draw_bar();
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

    void scale();
    void set_histo(QString  h);
    void paintEvent( QPaintEvent *e );
    void resizeEvent( QResizeEvent * );
    void set_pts(QString pts) ;
    void to_bar_pts(QString com);
    void to_pct(int ix, int iy, float *px, float *py);
    void to_pt(float px, float py, int *ix, int *iy);
    void to_pct_small(int ix, int iy, float *px, float *py);
    void to_pt_small(float px, float py, int *ix, int *iy);
    void add_pt(int ix, int iy);
    void add_pt_pct(float px, float py);
    void set_pcts(float *pct);
    void clear_picks();
    void del_pt(int ix, int iy);
    void del_pt_pct(float px, float py);
    int find_pt(int ix, int iy, int dist,int ib, int ie);
    void set_symmetric(bool dd){ symmetric=dd; fprintf(stderr,"Chaning symetric\n");}

    QPolygon *boxit(float bx,float by, float ex, float ey);
    signals:
      void actionDetected(std::vector <QString>);
    public:
    int ipt_down;
    QString idat;
    std::vector<bar_pt> pts;
    std::vector<float> histo;
	  int W,H;
		QPixmap	pm;
		bool symmetric;
    QPixmap	pmScaled,pmScaledCopy;
};
#endif
