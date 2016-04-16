ADDI r2, r0, 1000
ADDI r3, r0, 20
ADDI r4, r0, 0
init: SUB r7, r4, r2
BGEZ r7, endinit
ADD r5, r3, r4
SW r4, 0(r5)
ADDI r4, r4, 1 
J init
endinit: ADDI r4, r0, 0
ADD r0, r0, r0
move: SUB r8, r4, r2
BGEZ r8, endmove
ADD r5, r3, r4 
LW r6, 0(r5) 
ADDI r6, r6, 1 
SW r6, 0(r5)
ADDI r4, r4, 1
J move
endmove: ADD r0, r0, r0

