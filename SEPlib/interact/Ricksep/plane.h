#ifndef PLANE_RICK
#define PLANE_RICK 4
#include<render.h>
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
	int pickable;
	RenderAttr attr;	/* render attributes */
	} *Plane;

/* API */
int PlaneListInfo (void);

#endif
