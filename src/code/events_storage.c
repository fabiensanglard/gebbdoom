#define MAXEVENTS       64

event_t         events[MAXEVENTS];
int             eventhead, eventtail; // Circular buffer

void D_PostEvent (event_t *ev) {
	events[eventhead] = *ev;
	eventhead = (++eventhead)&(MAXEVENTS-1);
}