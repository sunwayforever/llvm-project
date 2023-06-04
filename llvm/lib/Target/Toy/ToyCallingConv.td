def CSR : CalleeSavedRegs<(add RA, S0, S1)>;
def ToyCC : CallingConv<[
    CCIfType<[i32], CCAssignToReg<[A0, A1, A2, A3, A4, A5, A6, A7]>>,
    CCAssignToStack<4, 4>,
]>;
