ADDI r2, r0, 10
ADDI r3, r0, 50
ADDI r4, r0, 100
ADDI r5, r0, 150
ADDI r6, r0, 200
ADDI r7, r0, 250
ADDI r8, r0, 0
init: SUB r20, r8, r2
BGEZ r20, doneinit
ADD r9, r3, r8
SW r8, 0(r9)
ADD r9, r4, r8
SW r8, 0(r9)
ADD r9, r5, r8
SW r8, 0(r9)
ADD r9, r6, r8
SW r8, 0(r9)
ADD r9, r7, r8
SW r8, 0(r9)
ADDI r8, r8, 1
J init
doneinit: ADD r0, r0, r0
ADDI r8, r0, 0
loop1: SUB r20, r8, r2
BGEZ r20, endloop1
ADD r10, r5, r8
ADD r11, r3, r8
LW r14, 0(r11)
ADD r12, r4, r8
LW r15, 0(r12)
ADD r13, r14, r15
SW r13, 0(r10)
ADDI r8, r8, 1
J loop1
endloop1: ADD r0, r0, r0
ADDI r8, r0, 0
loop2: SUB r20, r8, r2
BGEZ r20, endloop2
ADD r10, r6, r8
ADD r11, r5, r8
LW r14, 0(r11)
ADD r12, r7, r8
LW r15, 0(r12)
ADD r13, r14, r15
SW r13, 0(r10)
ADDI r8, r8, 1
J loop2
endloop2: ADD r0, r0, r0
ADDI r8, r0, 0
loop3: SUB r20, r8, r2
BGEZ r20, endloop3
ADD r11, r3, r8
LW r14, 0(r11)
ADD r12, r4, r8
LW r15, 0(r12)
ADD r13, r14, r15
SW r13, 0(r11)
ADDI r8, r8, 1
J loop3
endloop3: ADD r0, r0, r0