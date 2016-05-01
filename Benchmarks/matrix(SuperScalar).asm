addi r4, r0, 0
addi r1, r0, 10
addi r2, r0, 100
addi r3, r0, 300
begina: sub r31, r4, r1
bgtz r31, endloopa
addi r5, r0, 0
beginb: sub r30, r5, r1
bgtz r30, endloopb
mul r30, r1, r4
add r10, r30, r5
add r11, r2, r10
sw r4, 0(r11)
add r12, r3, r10
sw r5, 0(r12)
addi r5, r5, 1
j beginb
endloopb: addi r4, r4, 1
j begina
endloopa: add r0, r0, r0
add r0, r0, r0
addi r4, r0, 1
beginc: sub r31, r4, r1
addi r31, r31, 1
bgtz r31, endloopc
addi r5, r0, 1
begind: sub r30, r5, r1
addi r30, r30, 1
bgtz r30, endloopd
addi r10, r4, -1 
addi r11, r4, 1 
mul r30, r1, r10
mul r31, r1, r11
add r17, r5, r30
add r18, r5, r31
add r30, r17, r2
add r31, r18, r2
lw r21, 0(r30)
lw r22, 0(r31)
mul r30, r1, r4
addi r6, r5, 1
add r6, r30, r6
add r0, r0, r0
add r6, r6, r2
add r0, r0, r0
lw r23, 0(r6)
lw r24, -2(r6)
add r21, r21, r22
add r23, r23, r24
add r0, r0, r0
add r21, r21, r23
addi r23, r0, 4
div r21, r21, r23
add r30, r30, r5
add r0, r0, r0
add r8, r30, r3
add r0, r0, r0
sw r21, 0(r8)
addi r5, r5, 1
j begind
endloopd: addi r4, r4, 1
j beginc
endloopc: add r0, r0, r0