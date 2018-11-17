typedef struct {
	thinker_t       thinker;
	floor_e         type;
	boolean         crush;
	sector_t        *sector;
	int             direction;
	int             newspecial;
	short           texture;
	fixed_t         floordestheight;
	fixed_t         speed;
} floormove_t;