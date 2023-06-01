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
}

def GPR:RegisterClass<"Toy", [i32], 32, (add
    ZERO, RA, SP,
    T0, T1, T2,
    S0, S1,
    A0, A1, A2)>;
