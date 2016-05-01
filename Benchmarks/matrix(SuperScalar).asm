ADDI r4, r0, 0
ADDI r1, r0, 10
ADDI r2, r0, 100
ADDI r3, r0, 300
begina: SUB r31, r4, r1
BGEZ r31, endloopa
ADDI r5, r0, 0
beginb: SUB r30, r5, r1
BGEZ r30, endloopb
MUL r30, r1, r4
ADD r10, r30, r5
ADD r11, r2, r10
SW r4, 0(r11)
ADD r12, r3, r10
SW r5, 0(r12)
ADDI r5, r5, 1
J beginb
endloopb: ADDI r4, r4, 1
J begina
endloopa: ADD r0, r0, r0
ADD r0, r0, r0
ADDI r4, r0, 1
beginc: SUB r31, r4, r1
ADDI r31, r31, 1
BGEZ r31, endloopc
ADDI r5, r0, 1
begind: SUB r30, r5, r1
ADDI r30, r30, 1
BGEZ r30, endloopd
ADDI r10, r4, -1 
ADDI r11, r4, 1 
MUL r30, r1, r10
MUL r31, r1, r11
ADD r17, r5, r30
ADD r18, r5, r31
ADD r30, r17, r2
ADD r31, r18, r2
LW r21, 0(r30)
LW r22, 0(r31)
MUL r30, r1, r4
ADDI r6, r5, 1
ADD r6, r30, r6
ADD r0, r0, r0
ADD r6, r6, r2
ADD r0, r0, r0
LW r23, 0(r6)
LW r24, -2(r6)
ADD r21, r21, r22
ADD r23, r23, r24
ADD r0, r0, r0
ADD r21, r21, r23
ADDI r23, r0, 4
DIV r21, r21, r23
ADD r30, r30, r5
ADD r0, r0, r0
ADD r8, r30, r3
ADD r0, r0, r0
SW r21, 0(r8)
ADDI r5, r5, 1
J begind
endloopd: ADDI r4, r4, 1
J beginc
endloopc: ADD r0, r0, r0