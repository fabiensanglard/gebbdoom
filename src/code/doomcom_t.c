#define BACKUPTICS 12

typedef struct {
  unsigned checksum;       // high bit=retransmit request
  byte     retransmitfrom; // only valid if NCMD_RETRANSMIT
  byte     starttic;
  byte     player, numtics;// player is player id.                
  ticcmd_t cmds[BACKUPTICS];
} doomdata_t;

typedef struct {  
  long    id;         // MUST be = DOOMCOM_ID (0x12345678l)
  short   intnum;     // DOOM interrupt to execute commands 
 
  // communication between DOOM and the driver 
  short   command;    // CMD_SEND or CMD_GET 
  short   remotenode; // dest for send 
  short   datalength; // bytes in doomdata to be sent 
 
  // info common to all nodes 
  short   numnodes;   // console is always node 0 
  short   ticdup;     // 1 = no dup, 2-5 =dup for slow nets 
  short   extratics;  // 1 = send a backup tic in packets
  short   deathmatch; // 1 = deathmatch 
  short   savegame;   // -1 = new game, 0-5 = load savegame 
  short   episode;    // 1-3 
  short   map;        // 1-9 
  short   skill;      // 1-5 
 
  // info specific to this node 
  short   consoleplayer; 
  short   numplayers; 
 
  doomdata_t   data; // packet data to be sent 
} doomcom_t; 