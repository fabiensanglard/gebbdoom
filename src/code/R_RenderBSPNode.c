void R_RenderBSPNode (int bspnum)
{
    node_t*    bsp;
    int        side;

    // Found a subsector?
    if (bspnum & NF_SUBSECTOR)
    {
      if (bspnum == -1)            
        R_Subsector (0);
      else
        R_Subsector (bspnum&(~NF_SUBSECTOR));
      return;
    }
        
    bsp = &nodes[bspnum];
    
    // Decide which side the view point is on.
    side = R_PointOnSide (viewx, viewy, bsp);

    // Recursively divide front space.
    R_RenderBSPNode (bsp->children[side]); 

    // Possibly divide back space.
    if (R_CheckBBox (bsp->bbox[side^1]))    
        R_RenderBSPNode (bsp->children[side^1]);
}