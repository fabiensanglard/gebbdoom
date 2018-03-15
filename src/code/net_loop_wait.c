void TryRunTics (void) {
  int		lowtic;
  ...
  // a gametic cannot be run until nettics[] > gametic
  while (lowtic < gametic) {
        NetUpdate ();   
        lowtic = MAXINT;
        
        for (i=0 ; i<doomcom->numnodes ; i++)
          if (nodeingame[i] && nettics[i] < lowtic)
            lowtic = nettics[i];
              
        // don't stay in here forever 
        if (I_GetTime ()/ticdup - entertic >= 20)
        { // give the menu a chance to work
          M_Ticker ();
          return;
        } 
      }
  ...
}      