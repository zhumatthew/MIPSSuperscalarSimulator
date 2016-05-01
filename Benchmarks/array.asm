addi r2, r0, 10
addi r3, r0, 50
addi r4, r0, 100
addi r5, r0, 150
addi r6, r0, 200
addi r7, r0, 250
addi r8, r0, 0
init: sub r20, r8, r2
bgtz r20, doneinit
add r9, r3, r8
sw r8, 0(r9)
add r9, r4, r8
sw r8, 0(r9)
add r9, r5, r8
sw r8, 0(r9)
add r9, r6, r8
sw r8, 0(r9)
add r9, r7, r8
sw r8, 0(r9)
addi r8, r8, 1
j init
doneinit: add r0, r0, r0
addi r8, r0, 0
loop1: sub r20, r8, r2
bgtz r20, endloop1
add r10, r5, r8
add r11, r3, r8
lw r14, 0(r11)
add r12, r4, r8
lw r15, 0(r12)
add r13, r14, r15
sw r13, 0(r10)
addi r8, r8, 1
j loop1
endloop1: add r0, r0, r0
addi r8, r0, 0
loop2: sub r20, r8, r2
bgtz r20, endloop2
add r10, r6, r8
add r11, r5, r8
lw r14, 0(r11)
add r12, r7, r8
lw r15, 0(r12)
add r13, r14, r15
sw r13, 0(r10)
addi r8, r8, 1
j loop2
endloop2: add r0, r0, r0
addi r8, r0, 0
loop3: sub r20, r8, r2
bgtz r20, endloop3
add r11, r3, r8
lw r14, 0(r11)
add r12, r4, r8
lw r15, 0(r12)
add r13, r14, r15
sw r13, 0(r11)
addi r8, r8, 1
j loop3
endloop3: add r0, r0, r0