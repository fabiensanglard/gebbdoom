
typedef struct { 
  char            forwardmove;   // *2048 for move 
  char            sidemove;      // *2048 for move 
  short           angleturn;     // <<16 for angle delta 
  short           consistancy;   // checks for net game 
  unsigned char   chatchar; 
  unsigned char   buttons; 
} ticcmd_t; 

