#ifndef RICKMOVIE_PLANE_H
#define RICKMOVIE_PLANE_H
/*
plane object definition

A plane is a component of an image
*/

/* constants */
#define NPLANE		2500

/* Plane object */
typedef struct {
	Map	hmap;		/* five map axes */
	Map	vmap;
	Map	zmap;
	Map	map4;
	Map	map5;
	int	frame3;	/* three frames of a plane */
	int	frame4;	/* three frames of a plane */
	int	frame5;	/* three frames of a plane */
	int	h0;		/* panel origin */
	int	v0;		/* panel origin */
	int	nh;		/* panel dimensions */
	int	nv;		/* panel dimensions */
	int	*margins;
	RenderAttr attr;	/* render attributes */
	} *Plane;

/* typed returns */
extern Plane PlaneInit (void);
extern Plane PlaneFind (int dir3,int frame3,int dir4,int frame4,int dir5,int frame5);
extern void PlaneReset (void);
extern void PlaneSet (Map hmap,Map vmap,Map zmap,Map map4,Map map5,int h0,int v0,int nh,int nv,Render render,int *margins,RenderAttr attr);
extern void PlaneSetFrame ( Plane plane, int frame3);
extern void PlaneListInfo (void);
extern int PlaneDir (Plane plane);
extern int PlaneFrame (Plane plane);
extern void PlaneRect (int *h0,int *v0,int *nh,int *nv);
extern int PlaneSkew (Plane plane);
extern int PlaneType (Plane plane);
extern int PlaneV0 (Plane plane);
extern int PlaneH0 (Plane plane);
extern int PlaneNV (Plane plane);
extern int PlaneNH (Plane plane);
extern Map PlaneHmap (Plane plane);
extern Map PlaneVmap (Plane plane);
extern Map PlaneZmap (Plane plane);
#endif
