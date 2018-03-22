int R_PointOnSide(fixed_t x,fixed_t	y, node_t* node){
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	left;
    fixed_t	right;
	
    if (!node->dx){
	  if (x <= node->x)
	    return node->dy > 0;
	  return node->dy < 0;
    }

    if (!node->dy) {
  	  if (y <= node->y)
	    return node->dx < 0;
	  return node->dx > 0;
    }
	
    dx = (x - node->x);
    dy = (y - node->y);
	
    if ( (node->dy ^ node->dx ^ dx ^ dy)&0x80000000 ) {
	  if  ( (node->dy ^ dx) & 0x80000000 ) {
	    // (left is negative)
	    return 1;
	  }
	  return 0;
    }

    left = FixedMul ( node->dy>>FRACBITS , dx );
    right = FixedMul ( dy , node->dx>>FRACBITS );
	
    if (right < left) { // front side
	  return 0;
    }
    
    return 1; // back side			
}