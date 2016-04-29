loop: lw r6, 0(r18)
lw r7, 1(r18)
add r6, r6, r2
add r7, r7, r2
sw r20, 0(r6)
sw r21, 0(r7)
addi r18, r18, -2
bgez r18, r0, loop