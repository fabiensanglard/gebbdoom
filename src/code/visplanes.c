// Now what is a visplane, anyway?
typedef struct {
  fixed_t		height;
  int			picnum;
  int			lightlevel;
  int			minx;
  int			maxx;

  byte		top[SCREENWIDTH];
  byte		bottom[SCREENWIDTH];
} visplane_t;

#define MAXVISPLANES	128
visplane_t		visplanes[MAXVISPLANES];