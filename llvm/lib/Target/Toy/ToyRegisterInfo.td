let Namespace = "Toy" in {
class ToyReg<string n> : Register<n>;
}

let Namespace = "Toy" in {
    def ZERO : ToyReg<"zero">,  DwarfRegNum<[0]>;
    def RA :   ToyReg<"ra">,    DwarfRegNum<[1]>;
    def SP :   ToyReg<"sp">,    DwarfRegNum<[2]>;
    def T0 :   ToyReg<"t0">,    DwarfRegNum<[5]>;
    def T1 :   ToyReg<"t1">,    DwarfRegNum<[6]>;
    def T2 :   ToyReg<"t2">,    DwarfRegNum<[7]>;
    def S0 :   ToyReg<"s0">,    DwarfRegNum<[8]>;
    def S1 :   ToyReg<"s1">,    DwarfRegNum<[9]>;
    def A0 :   ToyReg<"a0">,    DwarfRegNum<[10]>;
    def A1 :   ToyReg<"a1">,    DwarfRegNum<[11]>;
    def A2 :   ToyReg<"a2">,    DwarfRegNum<[12]>;
    def A3 :   ToyReg<"a3">,    DwarfRegNum<[13]>;
    def A4 :   ToyReg<"a4">,    DwarfRegNum<[14]>;
    def A5 :   ToyReg<"a5">,    DwarfRegNum<[15]>;
    def A6 :   ToyReg<"a6">,    DwarfRegNum<[16]>;
    def A7 :   ToyReg<"a7">,    DwarfRegNum<[17]>;
    def FT0:   ToyReg<"ft0">,    DwarfRegNum<[0]>;
    def FT1:   ToyReg<"ft1">,    DwarfRegNum<[1]>;
    def FT2:   ToyReg<"ft2">,    DwarfRegNum<[2]>;
    def FS0:   ToyReg<"fS0">,    DwarfRegNum<[8]>;
    def FS1:   ToyReg<"fS1">,    DwarfRegNum<[9]>;
    def FA0:   ToyReg<"fa0">,    DwarfRegNum<[10]>;
    def FA1:   ToyReg<"fa1">,    DwarfRegNum<[11]>;
    def FA2:   ToyReg<"fa2">,    DwarfRegNum<[12]>;
    def FA3:   ToyReg<"fa3">,    DwarfRegNum<[13]>;
    def FA4:   ToyReg<"fa4">,    DwarfRegNum<[14]>;
    def FA5:   ToyReg<"fa5">,    DwarfRegNum<[15]>;
    def FA6:   ToyReg<"fa6">,    DwarfRegNum<[16]>;
    def FA7:   ToyReg<"fa7">,    DwarfRegNum<[17]>;
}

def GPR:RegisterClass<"Toy", [i32], 32, (add
    ZERO, RA, SP,
    T0, T1, T2,
    S0, S1,
    A0, A1, A2, A3, A4, A5, A6, A7)>;

def FPR:RegisterClass<"Toy", [f32], 32, (add
    FT0, FT1, FT2,
    FS0, FS1,
    FA0, FA1, FA2, FA3, FA4, FA5, FA6, FA7)>;
