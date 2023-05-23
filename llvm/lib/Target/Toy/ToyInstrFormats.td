class ToyInst<dag outs, dag ins, string asm, list<dag> pattern,
       InstrItinClass itin>: Instruction
{
    let Namespace = "Toy";
    
    field bits<32> Inst;
    let Size = 4;
    bits<7> Opcode = 0;
    let Inst{6-0} = Opcode;
    
    let OutOperandList = outs;
    let InOperandList = ins;

    let AsmString = asm;
    let Pattern = pattern;
    let Itinerary = itin;

    let DecoderNamespace = "Toy";
}


class InstI<bits<7> opcode, bits<3> funct3, dag outs, dag ins, string asm, list<dag> pattern,
      InstrItinClass itin>: ToyInst<outs, ins, asm, pattern, itin>
{
    bits<5> rd;
    bits<5> rs1;
    bits<12> imm;

    let Inst{31-20} = imm;
    let Inst{19-15} = rs1;
    let Inst{14-12} = funct3;
    let Inst{11-7} = rd;

    let Opcode = opcode;
}
