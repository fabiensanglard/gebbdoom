lighttable_t*	ds_colormap;
void PseudoCodeDrawRoutine {
  int lightMapId = X % 16;	  // Select lightmap.
  byte ds_source = [...]      // Texel from texture.
  ds_colormap = palette + lightMapId * 256;
  // Draw using lightmap indirection.
  *dest++ = ds_colormap[ds_source[spot]];
}
