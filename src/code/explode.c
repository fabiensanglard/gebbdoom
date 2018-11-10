void P_KillMobj (mobj_t *source, mobj_t *target) {
  [...]
  if (target->health < -target->info->spawnhealth
      && target->info->xdeathstate) {
    P_SetMobjState (target, target->info->xdeathstate);
  } else {
	P_SetMobjState (target, target->info->deathstate);
  }
  [...]
}