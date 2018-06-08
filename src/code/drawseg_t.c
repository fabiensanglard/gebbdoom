typedef struct drawseg_s {
  seg_t   *curline;
  int     x1, x2;
  fixed_t scale1, scale2, scalestep;
  int     silhouette;   // 0=none, 1=bottom, 2=top, 3=both
  fixed_t bsilheight;   // don't clip sprites above this
  fixed_t tsilheight;   // don't clip sprites below this
  // pointers to lists for sprite clipping
  short *sprtopclip;  // adjusted so [x1] is first value
  short *sprbottomclip; // adjusted so [x1] is first value
  short *maskedtexturecol; // adjusted so [x1] is first value
} drawseg_t;

#define  MAXDRAWSEGS    256
drawseg_t  drawsegs[MAXDRAWSEGS];