ADDI r2, r0, 4
ADDI r7, r0, 20
ADDI r8, r0, 100
ADDI r9, r0, 120
begina: SUB r31, r3, r2
ADD r0, r0, r0
BGEZ r31, endloopa
ADDI r4, r0, 0
beginb: SUB r31, r4, r2
ADD r0, r0, r0
BGEZ r31, endloopb
MUL r29, r3, r2
ADD r10, r29, r4
ADD r0, r0, r0
ADD r11, r7, r10
ADD r12, r8, r10
SW r3, 0(r11)
SW r4, 0(r12)
ADDI r4, r4, 1
J beginb
endloopb: ADDI r3, r3, 1
J begina
endloopa: ADD r0, r0, r0
ADD r4, r4, r0
ADDI r3, r0, 0
ADD r0, r0, r0
loop1: SUB r31, r3, r2
BGEZ r31, endloop1
ADDI r4, r0, 0
ADDI r1, r1, 0
loop2: SUB r31, r4, r2
ADD r0, r0, r0
BGEZ r31, endloop2
ADDI r6, r0, 0
ADDI r5, r0, 0
loop3: SUB r31, r5, r2
ADD r0, r0, r0
BGEZ r31, endloop3
MUL r22, r3, r2
MUL r23, r5, r2
ADD r13, r22, r5
ADD r16, r23, r4
ADD r13, r13, r7
ADD r16, r16, r8
LW r15, 0(r13)
LW r18, 0(r16)
MUL r30, r15, r18
ADDI r5, r5, 1
ADD r6, r6, r30
J loop3
endloop3: MUL r25, r3, r2
ADD r0, r0, r0
ADD r20, r25, r4
ADD r0, r0, r0
ADDI r4, r4, 1
ADD r21, r20, r9
ADD r0, r0, r0
SW r6, 0(r21)
J loop2
endloop2: ADDI r3, r3, 1
J loop1
endloop1: ADD r0, r0, r0