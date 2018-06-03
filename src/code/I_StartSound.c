int I_StartSound (void *data, int vol, int sep, int pitch, int priority) {
	...
	return SFX_PlayPatch(data, sep, pitch, vol, 0, 100);
}