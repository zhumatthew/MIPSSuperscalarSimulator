addi $2, $0, 1000
addi $3, $0, 20
addi $4, $0, 0
init: sub $7, $4, $2
bgtz $7, endinit
add $5, $3, $4
sw $4, 0($5)
addi $4, $4, 1 
j init
endinit: addi $4, $0, 0
add $0, $0, $0
move: sub $8, $4, $2
bgtz $8, endmove
add $5, $3, $4 
lw $6, 0($5) 
addi $6, $6, 1 
sw $6, 0($5)
addi $4, $4, 1
j move
endmove: add $0, $0, $0

