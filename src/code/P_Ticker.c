void P_Ticker (void) {
	for (int i=0 ; i<MAXPLAYERS ; i++)
		if (playeringame[i])
			P_PlayerThink (&players[i]); // player actions
			
	P_RunThinkers ();      // Monsters
	P_UpdateSpecials ();   // Animate planes, scroll walls
	P_RespawnSpecials ();  // respawn items in deathmatch
}