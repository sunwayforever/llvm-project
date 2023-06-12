include "ToyInstrFormats.td"

def imm12 : Operand<i32>, ImmLeaf<i32, [{return isInt<12>(Imm);}]>;
def imm20 : Operand<i32>, ImmLeaf<i32, [{return isInt<20>(Imm);}]>;
def imm32 : Operand<i32>, ImmLeaf<i32, [{return isInt<32>(Imm);}]>;

def brtarget : Operand<OtherVT>;
def calltarget  : Operand<iPTR>;

def mem_stack : Operand<iPTR> {
  let MIOperandInfo = (ops GPR, imm12);
  let PrintMethod = "printMemStackOperand";
}

class ArithLogicI <bits<7> opcode, bits<3> funct3, string inst, SDNode node>:
      InstI<opcode, funct3, (outs GPR:$rd), (ins GPR:$rs1, imm12:$imm),
      !strconcat(inst, "\t$rd, $rs1, $imm"),
      [(set GPR:$rd, (node GPR:$rs1, imm12:$imm))],
      IIAlu>;

def ADDI : ArithLogicI<0b0010011, 0b000, "addi", add>;
def ANDI : ArithLogicI<0b0010011, 0b111, "andi", and>;
def ORI : ArithLogicI<0b0010011, 0b110, "ori", or>;
def XORI : ArithLogicI<0b0010011, 0b100, "xori", xor>;

class ArithLogicR <bits<7> funct7, bits<3> funct3, string inst, SDNode node>:
      InstR<0b0110011, funct7, funct3, (outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
      !strconcat(inst, "\t$rd, $rs1, $rs2"),
      [(set GPR:$rd, (node GPR:$rs1, GPR:$rs2))],
      IIAlu>;

def ADD : ArithLogicR<0b0000000, 0b000, "add", add>;
def SUB : ArithLogicR<0b0100000, 0b000, "sub", sub>;
def MUL : ArithLogicR<0b0000001, 0b000, "mul", mul>;
def DIV : ArithLogicR<0b0000001, 0b100, "div", sdiv>;
def REM : ArithLogicR<0b0000001, 0b110, "rem", srem>;
def AND : ArithLogicR<0b0000000, 0b111, "and", and>;
def OR : ArithLogicR<0b0000000, 0b110, "or", or>;
def XOR : ArithLogicR<0b0000000, 0b100, "xor", xor>;

def SLL : ArithLogicR<0b0000000, 0b001, "sll", shl>;
def SRA : ArithLogicR<0b0100000, 0b101, "sra", sra>;
def SRL : ArithLogicR<0b0000000, 0b101, "srl", srl>;

def LUI : InstU<(outs GPR:$rd), (ins imm20:$imm),
          "lui $rd, $imm", [], IIAlu>;

def AddrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;

def : Pat<(i32 imm12:$in),
          (ADDI ZERO, imm12:$in)>;

// load/store for data types
multiclass STORE<bits<7> opcode, bits<3> funct3, PatFrag op, string asm, DAGOperand ty> {
  def FI : InstS<opcode, funct3, (outs), (ins ty:$rs2, GPR:$rs1, imm12:$imm), !strconcat(asm, "\t$rs2, $imm($rs1)"), [(op ty:$rs2, (AddrFI GPR:$rs1, imm12:$imm))], IIAlu>;
  def R : InstS<opcode, funct3, (outs), (ins ty:$rs2, GPR:$rs1), !strconcat(asm, "\t$rs2, 0($rs1)"), [(op ty:$rs2, GPR:$rs1)], IIAlu> {
      let imm=0;
  }
}

defm STOREW:STORE<0b0100011, 0b010, store, "sw", GPR>;
defm STOREH:STORE<0b0100011, 0b001, truncstorei16, "sh", GPR>;
defm STOREB:STORE<0b0100011, 0b000, truncstorei8, "sb", GPR>;

defm STORF:STORE<0b0100111, 0b010, store, "fsw", FPR>;
defm STORD:STORE<0b0100111, 0b011, store, "fsd", FPR64>;

multiclass LOAD<bits<7> opcode, bits<3> funct3, PatFrag op, string asm, DAGOperand ty> {
  def FI : InstI<opcode, funct3, (outs ty:$rd), (ins GPR:$rs1, imm12:$imm), !strconcat(asm, "\t$rd, $imm($rs1)"), [(set ty:$rd, (op (AddrFI GPR:$rs1, imm12:$imm)))], IIAlu>;
  def R : InstI<opcode, funct3, (outs ty:$rd), (ins GPR:$rs1), !strconcat(asm, "\t$rd, 0($rs1)"), [(set ty:$rd, (op GPR:$rs1))], IIAlu> {
      let imm=0;
  }
}

defm LOADW:LOAD<0b0000011, 0b010, load, "lw", GPR>;
defm LOADH:LOAD<0b0000011, 0b001, extloadi16, "lh", GPR>;
defm LOADSH:LOAD<0b0000011, 0b001, sextloadi16, "lh", GPR>;
defm LOADZH:LOAD<0b0000011, 0b001, zextloadi16, "lh", GPR>;
defm LOADB:LOAD<0b0000011, 0b000, extloadi8, "lb", GPR>;
defm LOADSB:LOAD<0b0000011, 0b000, sextloadi8, "lb", GPR>;
defm LOADZB:LOAD<0b0000011, 0b000, zextloadi8, "lb", GPR>;

defm LOADF:LOAD<0b0000111, 0b010, load, "flw", FPR>;
defm LOADD:LOAD<0b0000111, 0b011, load, "fld", FPR64>;

def ToyRET : SDNode<"ToyISD::Ret", SDTNone, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def J : InstJ<(outs),(ins brtarget:$imm12),
           "j\t${imm12}", [],
           IIAlu> {
  // NOTE: isTerminator is required to generator label of the fallthrough bb
  let isTerminator = 1;
}

def RET : ToyPseudo<(outs),(ins), "ret", [(ToyRET)]> {
    let isReturn = 1;
    let isTerminator=1;
}

def SDT_Call      : SDTypeProfile<0, 1, [SDTCisVT<0, iPTR>]>;

def ToyCall : SDNode<"ToyISD::Call", SDT_Call, [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;

def CALL : ToyPseudo<(outs),(ins calltarget:$rs1), "call\t$rs1", []> {
    // NOTE: isCall is required to save RA, check `determineCalleeSaves` for details
    let isCall = 1;
}

def : Pat<(ToyCall tglobaladdr:$rs1),(CALL calltarget:$rs1)>;
def : Pat<(ToyCall texternalsym:$rs1),(CALL calltarget:$rs1)>;

def LEA : ToyPseudo<(outs GPR:$rd),(ins calltarget:$rs1), "la\t$rd, $rs1", []>;

def SLT : InstR<0b0110011, 0b0000000, 0b010, (outs GPR:$rd),(ins GPR:$rs1, GPR:$rs2),
           "slt\t$rd, $rs1, $rs2", [],
           IIAlu>;

def : Pat<(setgt GPR:$lhs, GPR:$rhs), (SLT GPR:$rhs, GPR:$lhs)>;
def : Pat<(setlt GPR:$lhs, GPR:$rhs), (SLT GPR:$lhs, GPR:$rhs)>;
def : Pat<(seteq GPR:$lhs, GPR:$rhs), (SLT (XOR GPR:$lhs, GPR:$rhs), 1)>;
def : Pat<(setne GPR:$lhs, GPR:$rhs), (SLT ZERO, (XOR GPR:$lhs, GPR:$rhs))>;
// def : Pat<(setge GPR:$rs1, GPR:$rs2), (XORI (SLT GPR:$rs1, GPR:$rs2), 1)>;
// def : Pat<(setle GPR:$rs1, GPR:$rs2), (XORI (SLT GPR:$rs2, GPR:$rs1), 1)>;

class BRANCH<string asm>:InstB<(outs),(ins GPR:$rs1, GPR:$rs2, brtarget:$imm),
                                     !strconcat(asm, "\t$rs1, $rs2, $imm"),
                                     [], IIAlu>;
def BLT:BRANCH<"blt">;
def BEQ:BRANCH<"beq">;
def BNE:BRANCH<"bne">;
def BGE:BRANCH<"bge">;

def : Pat<(brcond (i32 (setlt GPR:$lhs, GPR:$rhs)), bb:$dst), (BLT GPR:$lhs, GPR:$rhs, brtarget:$dst)>;

def : Pat<(brcond (i32 (setgt GPR:$lhs, GPR:$rhs)), bb:$dst), (BLT GPR:$rhs, GPR:$lhs, brtarget:$dst)>;

def : Pat<(brcond (i32 (seteq GPR:$lhs, GPR:$rhs)), bb:$dst), (BEQ GPR:$lhs, GPR:$rhs, brtarget:$dst)>;

def : Pat<(brcond (i32 (setne GPR:$lhs, GPR:$rhs)), bb:$dst), (BNE GPR:$lhs, GPR:$rhs, brtarget:$dst)>;

def : Pat<(brcond (i32 (setge GPR:$lhs, GPR:$rhs)), bb:$dst), (BGE GPR:$lhs, GPR:$rhs, brtarget:$dst)>;

def : Pat<(brcond (i32 (setle GPR:$lhs, GPR:$rhs)), bb:$dst), (BGE GPR:$rhs, GPR:$lhs, brtarget:$dst)>;

def : Pat<(br bb:$imm), (J brtarget:$imm)>;

def LI : ToyPseudo<(outs GPR:$rd),(ins imm32:$rs1), "li\t$rd, $rs1", []>;

def : Pat<(i32 imm32:$in),
          (LI imm32:$in)>;

def : Pat<(AddrFI GPR:$rs1, imm12:$imm), (ADDI GPR:$rs1, imm12:$imm)>;

class ArithLogicF <bits<7> funct7, bits<3> funct3, string inst, SDNode node>:
      InstR<0b1010011, funct7 , funct3, (outs FPR:$rd), (ins FPR:$rs1, FPR:$rs2),
      !strconcat(inst, "\t$rd, $rs1, $rs2"),
      [(set FPR:$rd, (node FPR:$rs1, FPR:$rs2))],
      IIAlu>;

def FADDS : ArithLogicF<0b0000000, 0b000, "fadd.s", fadd>;
def FSUBS : ArithLogicF<0b0000100, 0b000, "fsub.s", fsub>;
def FMULS : ArithLogicF<0b0001000, 0b000, "fmul.s", fmul>;
def FDIVS : ArithLogicF<0b0001100, 0b000, "fdiv.s", fdiv>;

def FMADDS : InstR4<0b1000011, 0b00, 0b000, (outs FPR:$rd), (ins FPR:$rs1, FPR:$rs2, FPR: $rs3),
      "fmadd.s\t$rd, $rs1, $rs2, $rs3",
      [(set FPR:$rd, (fma FPR:$rs1, FPR:$rs2, FPR:$rs3))],
      IIAlu>;

def FMAXNUM : ArithLogicF<0b0010100, 0b001, "fmax.s", fmaxnum>;

class FSETCC<bits<7> funct7, bits<3> funct3, string asm, PatFrag node>
            :InstR<0b1010011, funct7, funct3, (outs GPR:$rd),(ins FPR:$rs1, FPR:$rs2),
             !strconcat(asm, "\t$rd, $rs1, $rs2"),
             [(set GPR:$rd, (node FPR:$rs1, FPR:$rs2))], IIAlu>;

def FEQS : FSETCC<0b1010000, 0b010, "feq.s", seteq>;
def FLTS : FSETCC<0b1010000, 0b001, "flt.s", setlt>;
def FLES : FSETCC<0b1010000, 0b000, "fle.s", setle>;

def : Pat<(setune FPR:$lhs, FPR:$rhs), (XORI (FEQS FPR:$lhs, FPR:$rhs), 1)>;
def : Pat<(setuge FPR:$lhs, FPR:$rhs), (FLTS FPR:$rhs, FPR:$lhs)>;
def : Pat<(setule FPR:$lhs, FPR:$rhs), (FLES FPR:$lhs, FPR:$rhs)>;
def : Pat<(setult FPR:$lhs, FPR:$rhs), (FLTS FPR:$lhs, FPR:$rhs)>;
def : Pat<(setugt FPR:$lhs, FPR:$rhs), (FLTS FPR:$rhs, FPR:$lhs)>;

def : Pat<(setone FPR:$lhs, FPR:$rhs), (XORI (FEQS FPR:$lhs, FPR:$rhs), 1)>;
def : Pat<(setoge FPR:$lhs, FPR:$rhs), (FLTS FPR:$rhs, FPR:$lhs)>;
def : Pat<(setole FPR:$lhs, FPR:$rhs), (FLES FPR:$lhs, FPR:$rhs)>;
def : Pat<(setolt FPR:$lhs, FPR:$rhs), (FLTS FPR:$lhs, FPR:$rhs)>;
def : Pat<(setogt FPR:$lhs, FPR:$rhs), (FLTS FPR:$rhs, FPR:$lhs)>;

def : Pat<(brcond GPR:$cond, bb:$dst), (BEQ GPR:$cond, (LI 1), brtarget:$dst)>;

def FABSS:ToyPseudo<(outs FPR:$rd), (ins FPR:$rs1),
      "fabs.s $rd, $rs1",
      [(set FPR:$rd, (fabs FPR:$rs1))]>;

def FCVTSD : InstR<0b1010011, 0b0100000, 0b000, (outs FPR:$rd), (ins FPR64:$rs1),
      "fcvt.s.d\t$rd, $rs1",
      [(set FPR:$rd, (fpround FPR64:$rs1))],
      IIAlu> {
      let rs2=0;
}

def FCVTDS : InstR<0b1010011, 0b0100001, 0b000, (outs FPR64:$rd), (ins FPR:$rs1),
      "fcvt.d.s\t$rd, $rs1",
      [(set FPR64:$rd, (fpextend FPR:$rs1))],
      IIAlu> {
     let rs2=0;
}

class ArithLogicF64 <bits<7> funct7, bits<3> funct3, string inst, SDNode node>:
      InstR<0b1010011, funct7, funct3, (outs FPR64:$rd), (ins FPR64:$rs1, FPR64:$rs2),
      !strconcat(inst, "\t$rd, $rs1, $rs2"),
      [(set FPR64:$rd, (node FPR64:$rs1, FPR64:$rs2))],
      IIAlu>;

def FADDD : ArithLogicF64<0b0000001, 0b000, "fadd.d", fadd>;
def FSUBD : ArithLogicF64<0b0000101, 0b000, "fsub.d", fsub>;
def FMULD : ArithLogicF64<0b0001001, 0b000, "fmul.d", fmul>;
def FDIVD : ArithLogicF64<0b0001101, 0b000, "fdiv.d", fdiv>;
