cli          ; disable interrupts.
lgdt [gdtr]  ; set Global Descriptor Table address.
mov eax, cr0 
or al, 1     ; Prepare Protected Mode.
mov cr0, eax
 
; Flush of the pipeline via a far jump instruction. 
JMP 08h:PModeMain
  
PModeMain:
; load DS, ES, FS, GS, SS, ESP.