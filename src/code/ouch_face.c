#define ST_MUCHPAIN 20

void ST_updateFaceWidget(void) {
  // being attacked
  if (plyr->damagecount && plyr->attacker && plyr->attacker != plyr->mo) {
      if (plyr->health - st_oldhealth > ST_MUCHPAIN) {
        // Show Ouch face
      	...
      }   
  } 
}        