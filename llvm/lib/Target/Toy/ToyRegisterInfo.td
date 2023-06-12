class ToyReg<bits<5> Enc, string n> : Register<n> {
      let HWEncoding{4-0} = Enc;
      let Namespace = "Toy";
}

def ZERO : ToyReg<0, "zero">,  DwarfRegNum<[0]>;
def RA :   ToyReg<1, "ra">,    DwarfRegNum<[1]>;
def SP :   ToyReg<2, "sp">,    DwarfRegNum<[2]>;
def T0 :   ToyReg<5, "t0">,    DwarfRegNum<[5]>;
def T1 :   ToyReg<6, "t1">,    DwarfRegNum<[6]>;
def T2 :   ToyReg<7, "t2">,    DwarfRegNum<[7]>;
def S0 :   ToyReg<8, "s0">,    DwarfRegNum<[8]>;
def S1 :   ToyReg<9, "s1">,    DwarfRegNum<[9]>;
def A0 :   ToyReg<10, "a0">,    DwarfRegNum<[10]>;
def A1 :   ToyReg<11, "a1">,    DwarfRegNum<[11]>;
def A2 :   ToyReg<12, "a2">,    DwarfRegNum<[12]>;
def A3 :   ToyReg<13, "a3">,    DwarfRegNum<[13]>;
def A4 :   ToyReg<14, "a4">,    DwarfRegNum<[14]>;
def A5 :   ToyReg<15, "a5">,    DwarfRegNum<[15]>;
def A6 :   ToyReg<16, "a6">,    DwarfRegNum<[16]>;
def A7 :   ToyReg<17, "a7">,    DwarfRegNum<[17]>;
def FT0:   ToyReg<0, "ft0">,    DwarfRegNum<[0]>;
def FT1:   ToyReg<1, "ft1">,    DwarfRegNum<[1]>;
def FT2:   ToyReg<2, "ft2">,    DwarfRegNum<[2]>;
def FS0:   ToyReg<8, "fS0">,    DwarfRegNum<[8]>;
def FS1:   ToyReg<9, "fS1">,    DwarfRegNum<[9]>;
def FA0:   ToyReg<10, "fa0">,    DwarfRegNum<[10]>;
def FA1:   ToyReg<11, "fa1">,    DwarfRegNum<[11]>;
def FA2:   ToyReg<12, "fa2">,    DwarfRegNum<[12]>;
def FA3:   ToyReg<13, "fa3">,    DwarfRegNum<[13]>;
def FA4:   ToyReg<14, "fa4">,    DwarfRegNum<[14]>;
def FA5:   ToyReg<15, "fa5">,    DwarfRegNum<[15]>;
def FA6:   ToyReg<16, "fa6">,    DwarfRegNum<[16]>;
def FA7:   ToyReg<17, "fa7">,    DwarfRegNum<[17]>;

def GPR:RegisterClass<"Toy", [i32], 32, (add
    ZERO, RA, SP,
    T0, T1, T2,
    S0, S1,
    A0, A1, A2, A3, A4, A5, A6, A7)>;

def FPR:RegisterClass<"Toy", [f32], 32, (add
    FT0, FT1, FT2,
    FS0, FS1,
    FA0, FA1, FA2, FA3, FA4, FA5, FA6, FA7)>;

def FPR64:RegisterClass<"Toy", [f64], 64, (add
    FT0, FT1, FT2,
    FS0, FS1,
    FA0, FA1, FA2, FA3, FA4, FA5, FA6, FA7)>;
