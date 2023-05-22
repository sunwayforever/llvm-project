def ZERO : Register<"zero">,  DwarfRegNum<[0]> {
    let Namespace="Toy";
}

def GPR:RegisterClass<"Toy", [i32], 32, (add ZERO)> {}

