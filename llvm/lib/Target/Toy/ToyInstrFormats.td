class ToyInst<dag outs, dag ins, string asm, list<dag> pattern,
       InstrItinClass itin>: Instruction
{
    let Namespace = "Toy";

    let OutOperandList = outs;
    let InOperandList = ins;

    let AsmString = asm;
    let Pattern = pattern;
    let Itinerary = itin;

    let DecoderNamespace = "Toy";
}


class InstI<dag outs, dag ins, string asm, list<dag> pattern,
      InstrItinClass itin>: ToyInst<outs, ins, asm, pattern, itin>{}

class InstR<dag outs, dag ins, string asm, list<dag> pattern,
      InstrItinClass itin>: ToyInst<outs, ins, asm, pattern, itin>{}

class InstU<dag outs, dag ins, string asm, list<dag> pattern,
      InstrItinClass itin>: ToyInst<outs, ins, asm, pattern, itin>{}

class Pseudo<dag outs, dag ins, list<dag> pattern>
    : ToyInst<outs, ins, "", pattern, IIAlu> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}
