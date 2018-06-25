typedef struct {
    char magicNumber[4];      // "IWAD" or "PWAD"
    int32_t numDirectories;   // Num directories 
    int32_t directoryOffset;  // Offset to directies
} header;

typedef struct { 
    int32_t offset;           // Offset to lump 
    int32_t size;             // Size of the lump
    char name[8];             // Name of the lump
} directoryEntry;