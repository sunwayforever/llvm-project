def ZERO : Register<"zero">,  DwarfRegNum<[0]> {
    let Namespace="Toy";
}
def RA : Register<"ra">,  DwarfRegNum<[1]> {
    let Namespace="Toy";
}
def SP : Register<"sp">,  DwarfRegNum<[2]> {
    let Namespace="Toy";
}

def GPR:RegisterClass<"Toy", [i32], 32, (add ZERO, RA, SP)> {}
