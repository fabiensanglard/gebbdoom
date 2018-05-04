typedef struct memblock_s {
  int                size;         // header and fragments
  void               **user;       // NULL if a free block
  int                tag;          // purgelevel
  int                id;           // should be ZONEID
  struct memblock_s  *next, *prev; // Double linked list
} memblock_t;

typedef struct {
    int              size;         // total bytes malloced,
    memblock_t       blocklist;    // start/end linked list
    memblock_t       *rover;
} memzone_t;

memzone_t	*mainzone;