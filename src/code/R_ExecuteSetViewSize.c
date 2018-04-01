void R_ExecuteSetViewSize (void) {
    viewwidth  =  numblocks * 32;
    viewheight = (numblocks * 168/10)&~7;
}