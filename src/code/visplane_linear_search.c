visplane_t* R_FindPlane(fixed_t height, int picnum, int lightlevel) {
    visplane_t*	check;
    ...
    for (check=visplanes; check<lastvisplane; check++) {
        if (height == check->height && 
            picnum == check->picnum && 
            lightlevel == check->lightlevel)
                break;
     }
    ...
}