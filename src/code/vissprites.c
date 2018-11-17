typedef struct vissprite_s {
  struct vissprite_s*  prev; // Doubly linked list.
  struct vissprite_s*  next;
  int    x1;
  int    x2;
  fixed_t  scale;
  int      patch;
  lighttable_t*  colormap;
} vissprite_t;

#define MAXVISSPRITES   128
vissprite_t   vissprites[MAXVISSPRITES];
vissprite_t*  vissprite_p;
vissprite_t   vsprsortedhead;