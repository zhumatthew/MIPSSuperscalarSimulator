addi $1, $0, 10
addi $3, $0, 10
addi $2, $0, 16
sw $3, 0($2)
addi $3, $3, 20
sw $3, 0($1)
beq $1, $3, labela
mult $2, $1, $1
addi $5, $0, 10
labela: addi $1, $1, 20
beq $1, $3, labelb
add $2, $1, $2
add $3, $1, $3
labelb: add $6, $2, $3
