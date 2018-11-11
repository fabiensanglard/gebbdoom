// Now what is a visplane, anyway?
typedef struct {
  fixed_t    height;
  int        picnum;
  int        lightlevel;
  int        minx;
  int        maxx;
  // 4 padding bytes
  byte       top[SCREENWIDTH];
  byte       bottom[SCREENWIDTH];
} visplane_t;

// Here comes the obnoxious "visplane".
#define MAXVISPLANES    128
visplane_t   visplanes[MAXVISPLANES];
visplane_t*  lastvisplane;
