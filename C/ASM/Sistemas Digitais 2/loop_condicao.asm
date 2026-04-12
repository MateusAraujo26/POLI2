main:
        addi t0, x0, 0        # fot = 0
        addi t1, x0, 100      # n = 100

loop:
        addi t2, x0, 1        # t2 = 1
        bge  t2, t1, fim      # if (n <= 1) break

        add  t0, t0, t1       # fot = fot + n
        addi t1, t1, -1       # n = n - 1
        jal  x0, loop         # goto loop

fim:
        addi a0, x0, 0        # return 0
        jalr x0, 0(ra)        # return