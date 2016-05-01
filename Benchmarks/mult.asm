addi r2, r0, 4
addi r7, r0, 20
addi r8, r0, 100
addi r9, r0, 120
begina: sub r31, r3, r2
add r0, r0, r0
bgtz r31, endloopa
addi r4, r0, 0
beginb: sub r31, r4, r2
add r0, r0, r0
bgtz r31, endloopb
mult r29, r3, r2
add r10, r29, r4
add r0, r0, r0
add r11, r7, r10
add r12, r8, r10
sw r3, 0(r11)
sw r4, 0(r12)
addi r4, r4, 1
j beginb
endloopb: addi r3, r3, 1
j begina
endloopa: add r0, r0, r0
add r4, r4, r0
addi r3, r0, 0
add r0, r0, r0
loop1: sub r31, r3, r2
bgtz r31, endloop1
addi r4, r0, 0
addi r1, r1, 0
loop2: sub r31, r4, r2
add r0, r0, r0
bgtz r31, endloop2
addi r6, r0, 0
addi r5, r0, 0
loop3: sub r31, r5, r2
add r0, r0, r0
bgtz r31, endloop3
mult r22, r3, r2
mult r23, r5, r2
add r13, r22, r5
add r16, r23, r4
add r13, r13, r7
add r16, r16, r8
lw r15, 0(r13)
lw r18, 0(r16)
mult r30, r15, r18
addi r5, r5, 1
add r6, r6, r30
j loop3
endloop3: mult r25, r3, r2
add r0, r0, r0
add r20, r25, r4
add r0, r0, r0
addi r4, r4, 1
add r21, r20, r9
add r0, r0, r0
sw r6, 0(r21)
j loop2
endloop2: addi r3, r3, 1
j loop1
endloop1: add r0, r0, r0