void I_UpdateSoundParams(int handle, int vol, int sep, int pitch) {
  SFX_SetOrigin(handle, pitch, sep, vol);
}

int I_StartSound (void *data, int vol, int sep, int pitch, int priority) {
	...
	return SFX_PlayPatch(data, sep, pitch, vol, 0, 100);
}