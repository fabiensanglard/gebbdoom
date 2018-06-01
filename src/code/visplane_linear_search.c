visplane_t* R_FindPlane(fixed_t height, int picnum, int lightlevel) {
    visplane_t*    check;
    ...
    for (check=visplanes; check<lastvisplane; check++) {
        if (height == check->height && 
            picnum == check->picnum && 
            lightlevel == check->lightlevel)
                break;
     }

    if (check < lastvisplane)
        return check;
        
    if (lastvisplane - visplanes == MAXVISPLANES)
        I_Error ("R_FindPlane: no more visplanes");
                
    lastvisplane++;
    check->height = height;
    check->picnum = picnum;
    check->lightlevel = lightlevel;
    check->minx = SCREENWIDTH;
    check->maxx = -1;
    memset (check->top,0xff,sizeof(check->top));
        
    return check;
}