if (target->health < -target->info->spawnhealth {
        && target->info->xdeathstate)
    P_SetMobjState (target, target->info->xdeathstate);
} else {
	P_SetMobjState (target, target->info->deathstate);
}