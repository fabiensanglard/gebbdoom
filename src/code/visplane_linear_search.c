for (check=visplanes; check<lastvisplane; check++) {
    if (height == check->height
        && picnum == check->picnum
        && lightlevel == check->lightlevel){
            break;
    }
}