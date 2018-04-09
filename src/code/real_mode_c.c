void MM_Startup (void) {
  int i;
  unsigned long length;
  void far      *start;
  unsigned      segstart,seglength,endfree;

  // get all available near conventional memory segments
  length=coreleft();
  start = (void far *)(nearheap = malloc(length));

  length -= 16-(FP_OFF(start)&15);
  length -= SAVENEARHEAP;
  seglength = length / 16;      // now in paragraphs
  segstart = FP_SEG(start)+(FP_OFF(start)+15)/16;
  MML_UseSpace (segstart,seglength);
  mminfo.nearheap = length;

  // get all available far conventional memory segments
  length=farcoreleft();
  start = farheap = farmalloc(length);
  length -= 16-(FP_OFF(start)&15);
  length -= SAVEFARHEAP;
  seglength = length / 16;      // now in paragraphs
  segstart = FP_SEG(start)+(FP_OFF(start)+15)/16;
  MML_UseSpace (segstart,seglength);
  mminfo.farheap = length;
  mminfo.mainmem = mminfo.nearheap + mminfo.farheap;
}