addi $2, $0, 4
addi $7, $0, 20
addi $8, $0, 100
addi $9, $0, 120
begina: sub $31, $3, $2
add $0, $0, $0
bgtz $31, endloopa
addi $4, $0, 0
beginb: sub $31, $4, $2
add $0, $0, $0
bgtz $31, endloopb
mult $29, $3, $2
add $10, $29, $4
add $0, $0, $0
add $11, $7, $10
add $12, $8, $10
sw $3, 0($11)
sw $4, 0($12)
addi $4, $4, 1
j beginb
endloopb: addi $3, $3, 1
j begina
endloopa: add $0, $0, $0
add $4, $4, $0
addi $3, $0, 0
add $0, $0, $0
loop1: sub $31, $3, $2
bgtz $31, endloop1
addi $4, $0, 0
addi $1, $1, 0
loop2: sub $31, $4, $2
add $0, $0, $0
bgtz $31, endloop2
addi $6, $0, 0
addi $5, $0, 0
loop3: sub $31, $5, $2
add $0, $0, $0
bgtz $31, endloop3
mult $22, $3, $2
mult $23, $5, $2
add $13, $22, $5
add $16, $23, $4
add $13, $13, $7
add $16, $16, $8
lw $15, 0($13)
lw $18, 0($16)
mult $30, $15, $18
addi $5, $5, 1
add $6, $6, $30
j loop3
endloop3: mult $25, $3, $2
add $0, $0, $0
add $20, $25, $4
add $0, $0, $0
addi $4, $4, 1
add $21, $20, $9
add $0, $0, $0
sw $6, 0($21)
j loop2
endloop2: addi $3, $3, 1
j loop1
endloop1: add $0, $0, $0