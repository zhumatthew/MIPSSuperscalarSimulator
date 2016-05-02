addi $4, $0, 0
addi $1, $0, 10
addi $2, $0, 100
addi $3, $0, 300
begina: sub $31, $4, $1
bgtz $31, endloopa
addi $5, $0, 0
beginb: sub $30, $5, $1
bgtz $30, endloopb
mul $30, $1, $4
add $10, $30, $5
add $11, $2, $10
sw $4, 0($11)
add $12, $3, $10
sw $5, 0($12)
addi $5, $5, 1
j beginb
endloopb: addi $4, $4, 1
j begina
endloopa: add $0, $0, $0
add $0, $0, $0
addi $4, $0, 1
beginc: sub $31, $4, $1
addi $31, $31, 1
bgtz $31, endloopc
addi $5, $0, 1
begind: sub $30, $5, $1
addi $30, $30, 1
bgtz $30, endloopd
addi $10, $4, -1 
addi $11, $4, 1 
mul $30, $1, $10
mul $31, $1, $11
add $17, $5, $30
add $18, $5, $31
add $30, $17, $2
add $31, $18, $2
lw $21, 0($30)
lw $22, 0($31)
mul $30, $1, $4
addi $6, $5, 1
add $6, $30, $6
add $0, $0, $0
add $6, $6, $2
add $0, $0, $0
lw $23, 0($6)
lw $24, -2($6)
add $21, $21, $22
add $23, $23, $24
add $0, $0, $0
add $21, $21, $23
addi $23, $0, 4
div $21, $21, $23
add $30, $30, $5
add $0, $0, $0
add $8, $30, $3
add $0, $0, $0
sw $21, 0($8)
addi $5, $5, 1
j begind
endloopd: addi $4, $4, 1
j beginc
endloopc: add $0, $0, $0