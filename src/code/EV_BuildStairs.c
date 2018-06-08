void T_MoveFloor(floormove_t *floor);

int EV_BuildStairs(line_t *line,stair_e type) {
  floormove_t	*floor;
  [...]
  floor = Z_Malloc (sizeof(*floor), PU_LEVSPEC, 0);
  P_AddThinker (&floor->thinker);
  floor->thinker.function = (think_t) T_MoveFloor;
  floor->direction = 1;
  floor->sector = sec;
  floor->speed = speed;
  floor->floordestheight = height;
  [...]
}
