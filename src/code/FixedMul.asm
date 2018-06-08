#ifdef __WATCOMC__            
#pragma aux FixedMul =     \    
    "imul ebx",            \    
    "shrd eax,edx,16"      \    
    parm    [eax] [ebx]    \    
    value   [eax]          \    
    modify exact [eax edx]      
#endif                        