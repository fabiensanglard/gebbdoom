void P_Ticker (void) {
	for (int i=0 ; i<MAXPLAYERS ; i++)
		if (playeringame[i])
			P_PlayerThink (&players[i]);
			
	P_RunThinkers ();
	P_UpdateSpecials ();
	P_RespawnSpecials ();
}