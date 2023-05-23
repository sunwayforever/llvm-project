def ALU : FuncUnit;

def IIAlu : InstrItinClass;

def Cpu0GenericItineraries : ProcessorItineraries<[ALU], [], [
  InstrItinData<IIAlu , [InstrStage<1,  [ALU]>]>,
]>;
