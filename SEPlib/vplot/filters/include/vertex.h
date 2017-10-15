#ifndef STRUCT_VERTEX_H_
#define STRUCT_VERTEX_H_
struct vertex
{
	int x;
	int y;
	struct vertex *next;		/* pointer to next vertex */
	struct vertex *last;		/* pointer to last vertex */
	struct vertex *soft;		/* pointer to some other vertex */
};
#endif
