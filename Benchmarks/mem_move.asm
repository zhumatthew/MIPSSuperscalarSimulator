addi r2, r0, 1000
addi r3, r0, 20
addi r4, r0, 0
init: sub r7, r4, r2
bgez r7, endinit
add r5, r3, r4
sw r4, 0(r5)
addi r4, r4, 1 
j init
endinit: addi r4, r0, 0
add r0, r0, r0
move: sub r8, r4, r2
bgez r8, endmove
add r5, r3, r4 
lw r6, 0(r5) 
addi r6, r6, 1 
sw r6, 0(r5)
addi r4, r4, 1
j move
endmove: add r0, r0, r0

