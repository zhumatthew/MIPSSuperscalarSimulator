addi $2, $0, 10
addi $3, $0, 50
addi $4, $0, 100
addi $5, $0, 150
addi $6, $0, 200
addi $7, $0, 250
addi $8, $0, 0
init: sub $20, $8, $2
bgtz $20, doneinit
add $9, $3, $8
sw $8, 0($9)
add $9, $4, $8
sw $8, 0($9)
add $9, $5, $8
sw $8, 0($9)
add $9, $6, $8
sw $8, 0($9)
add $9, $7, $8
sw $8, 0($9)
addi $8, $8, 1
j init
doneinit: add $0, $0, $0
addi $8, $0, 0
loop1: sub $20, $8, $2
bgtz $20, endloop1
add $10, $5, $8
add $11, $3, $8
lw $14, 0($11)
add $12, $4, $8
lw $15, 0($12)
add $13, $14, $15
sw $13, 0($10)
addi $8, $8, 1
j loop1
endloop1: add $0, $0, $0
addi $8, $0, 0
loop2: sub $20, $8, $2
bgtz $20, endloop2
add $10, $6, $8
add $11, $5, $8
lw $14, 0($11)
add $12, $7, $8
lw $15, 0($12)
add $13, $14, $15
sw $13, 0($10)
addi $8, $8, 1
j loop2
endloop2: add $0, $0, $0
addi $8, $0, 0
loop3: sub $20, $8, $2
bgtz $20, endloop3
add $11, $3, $8
lw $14, 0($11)
add $12, $4, $8
lw $15, 0($12)
add $13, $14, $15
sw $13, 0($11)
addi $8, $8, 1
j loop3
endloop3: add $0, $0, $0