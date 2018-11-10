fixed_t P_AimLineAttack (mobj_t *t1, angle_t angle, fixed_t distance) {
    P_PathTraverse ( t1->x, t1->y, x2, y2,
        PT_ADDLINES|PT_ADDTHINGS, PTR_AimTraverse );
}