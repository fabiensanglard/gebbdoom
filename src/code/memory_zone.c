//-----------
//MEMORY ZONE
//-----------
// tags < 100 are not overwritten until freed
#define PU_STATIC       1  // static entire execution time
#define PU_SOUND        2  // NEVER USED
#define PU_MUSIC        3  // static while playing
#define PU_DAVE         4  // Dave's static NEVER USED
#define PU_LEVEL       50  // static until level exited
#define PU_LEVSPEC     51  // a special thinker in a level
// tags >= 100 are purgable whenever needed
#define PU_PURGELEVEL 100
#define PU_CACHE      101