typedef struct {
  fixed_t    x,y,dx,dy;       // partition line
  fixed_t    bbox[2][4];      // child bounding box
  unsigned short children[2]; // NF_SUBSECTOR = subsector
} node_t;