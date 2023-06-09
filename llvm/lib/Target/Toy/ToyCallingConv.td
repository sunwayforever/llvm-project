def CSR : CalleeSavedRegs<(add RA, S0, S1, FS0,FS1)>;
def ToyCC : CallingConv<[
    CCIfType<[i32], CCAssignToReg<[A0, A1, A2, A3, A4, A5, A6, A7]>>,
    CCIfType<[f32], CCAssignToReg<[FA0, FA1, FA2, FA3, FA4, FA5, FA6, FA7]>>,
    CCIfType<[f64], CCAssignToReg<[FA0, FA1, FA2, FA3, FA4, FA5, FA6, FA7]>>,
    CCAssignToStack<4, 4>,
]>;
