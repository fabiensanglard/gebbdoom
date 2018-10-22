PROC   R_DrawColumn_
  [...]. ; edi = destscreen + y*80 + x/4
  [...]  ; set VGA mapmask register
  [...]  ; ebp= texture delta 7:25 fix point
  mov  esi,[_dc_source] ; esi = texture source
  mov  ebx,[_dc_iscale]
  shl  ebx,9
  mov  eax,OFFSET patch1+2 ; patch scaling code
  mov  [eax],ebx
  mov  eax,OFFSET patch2+2 ; patch scaling code
  mov  [eax],ebx

  mov  ecx,ebp          ; begin calculating 1st pixel
  add  ebp,ebx          ; advance frac pointer
  shr  ecx,25           ; finish calculation for 1st pixel
  mov  edx,ebp          ; begin calculating 2nd pixel
  add  ebp,ebx          ; advance frac pointer
  shr  edx,25           ; finish calculation for 2nd pixel
  mov  eax,[_dc_colormap]
  mov  ebx,eax
  mov  al,[esi+ecx]     ; get first pixel
  mov  bl,[esi+edx]     ; get second pixel
  mov  al,[eax]         ; color translate 1st pixel
  mov  bl,[ebx]         ; color translate 2nd pixel
doubleloop:
  mov  ecx,ebp          ; begin calculating 3rd pixel
patch1:
  add  ebp,12345678h    ; advance frac pointer
  mov  [edi],al         ; write first pixel
  shr  ecx,25           ; finish calculation for 3rd pixel
  mov  edx,ebp          ; begin calculating  for 4th pixel
patch2:
  add  ebp,12345678h    ; advance frac pointer
  mov  [edi+PLANEWIDTH],bl ; write second pixel
  shr  edx,25           ; finish calculation for 4th pixel
  mov  al,[esi+ecx]     ; get third pixel
  add  edi,PLANEWIDTH*2 ; advance to 3rd pixel dest
  mov  bl,[esi+edx]     ; get fourth pixel
  dec  [loopcount]      ; done with loop?
  mov  al,[eax]         ; color translate 3rd pixel
  mov  bl,[ebx]         ; color translate 4th pixel
  jnz  doubleloop
ENDP