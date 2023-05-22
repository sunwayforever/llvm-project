def ToyInstrInfo : InstrInfo;

def Toy : Target {
  let InstructionSet = ToyInstrInfo;
}
