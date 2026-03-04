LOOP:
    lw   x14, 0(x12)
    bne  x14, x11, EXIT
    add  x13, x14, x10
    addi x12, x12, 1
    beq  x0, x0, LOOP
EXIT:
    sw   x13, 0(x15)