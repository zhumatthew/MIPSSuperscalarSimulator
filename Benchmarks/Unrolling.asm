Loop: LW r6, 0(r18)
LW r7, 1(r18)
ADD r6, r6, r2
ADD r7, r7, r2
SW r20, 0(r6)
SW r21, 0(r7)
ADDI r18, r18, -2
BGEZ r18, r0, Loop