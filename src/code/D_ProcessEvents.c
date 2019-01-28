void D_ProcessEvents (void) {

  event_t     *ev;  
  int         head = eventhead;
  int         tail = eventtail;

  for ( ; tail != head ; tail = (++tail)&(MAXEVENTS-1) ) {
    ev = &events[tail];
    if (M_Responder (ev))
      continue;     // menu ate the event
    G_Responder (ev);
  }
  eventhead = eventtail;
}

boolean G_Responder (event_t *ev) {
  ...
  
  if (HU_Responder (ev)) 
      return true;    // chat ate the event 

  if (ST_Responder (ev)) 
      return true;    // status window ate it 

  if (AM_Responder (ev)) 
      return true;    // automap ate it 

  if (F_Responder (ev)) 
      return true;    // finale ate the event   

  // 3D renderer consumes event here
  ...      
}