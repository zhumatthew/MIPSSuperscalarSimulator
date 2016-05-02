addi $1, $0, 10
addi $3, $0, 10
addi $2, $0, 16
sw $3, 0($2)
addi $3, $3, 20
sw $3, 0($1)
beq $1, $3, labela
mult $2, $1, $1
addi $1, $1, 20
addi $5, $0, 10
labela: addi $5, $5, 10
beq $1, $3, labelb
addi $6, $6, 300
addi $10, $10, 300
labelb: add $6, $2, $3
lw $7, -20($1)
addi $8, $0, 11
mult $4, $2, $3
add $9, $7, $1
div $10, $4, $3
sub $2, $6, $10
div $1, $9, $2