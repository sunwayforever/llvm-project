include "ToyInstrFormats.td"

def imm12 : Operand<i32>, ImmLeaf<i32, [{return isInt<12>(Imm);}]>;
def imm20 : Operand<i32>, ImmLeaf<i32, [{return isInt<20>(Imm);}]>;
def imm32 : Operand<i32>, ImmLeaf<i32, [{return isInt<32>(Imm);}]>;

def brtarget : Operand<OtherVT>;
def calltarget  : Operand<iPTR>;

def mem : Operand<iPTR> {
  let MIOperandInfo = (ops GPR, imm12);
  let PrintMethod = "printMemOperand";
}

def mem_stack : Operand<iPTR> {
  let MIOperandInfo = (ops GPR, imm12);
  let PrintMethod = "printMemStackOperand";
}

class ArithLogicI <string inst, SDNode node>:
      InstI<(outs GPR:$rd), (ins GPR:$rs1, imm12:$imm),
      !strconcat(inst, "\t$rd, $rs1, $imm"),
      [(set GPR:$rd, (node GPR:$rs1, imm12:$imm))],
      IIAlu>;

def ADDI : ArithLogicI<"addi", add>;
def ANDI : ArithLogicI<"andi", and>;
def ORI : ArithLogicI<"ori", or>;
def XORI : ArithLogicI<"xori", xor>;

class ArithLogicR <string inst, SDNode node>:
      InstI<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
      !strconcat(inst, "\t$rd, $rs1, $rs2"),
      [(set GPR:$rd, (node GPR:$rs1, GPR:$rs2))],
      IIAlu>;

def ADD : ArithLogicR<"add", add>;
def SUB : ArithLogicR<"sub", sub>;
def MUL : ArithLogicR<"mul", mul>;
def DIV : ArithLogicR<"div", sdiv>;
def REM : ArithLogicR<"rem", srem>;
def AND : ArithLogicR<"and", and>;
def OR : ArithLogicR<"or", or>;
def XOR : ArithLogicR<"xor", xor>;

def SLL : ArithLogicR<"sll", shl>;
def SRA : ArithLogicR<"sra", sra>;
def SRL : ArithLogicR<"srl", srl>;

def LUI : InstU<(outs GPR:$rd), (ins imm20:$imm),
          "lui $rd, $imm", [], IIAlu>;

def AddrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;

def : Pat<(i32 imm12:$in),
          (ADDI ZERO, imm12:$in)>;

// load/store for data types
multiclass STORE<PatFrag op, string asm, DAGOperand ty> {
  def FI : InstS<(outs), (ins ty:$ra, mem:$addr), !strconcat(asm, "\t$ra, $addr"), [(op ty:$ra, AddrFI:$addr)], IIAlu>;
  def R : InstS<(outs), (ins ty:$ra, GPR:$rb), !strconcat(asm, "\t$ra, 0($rb)"), [(op ty:$ra, GPR:$rb)], IIAlu>;
}

defm STOREW:STORE<store, "sw", GPR>;
defm STOREH:STORE<truncstorei16, "sh", GPR>;
defm STOREB:STORE<truncstorei8, "sb", GPR>;

defm STORF:STORE<store, "fsw", FPR>;
defm STORD:STORE<store, "fsd", FPR64>;

multiclass LOAD<PatFrag op, string asm, DAGOperand ty> {
  def FI : InstI<(outs ty:$ra), (ins mem:$addr), !strconcat(asm, "\t$ra, $addr"), [(set ty:$ra, (op AddrFI:$addr))], IIAlu>;
  def R : InstI<(outs ty:$ra), (ins GPR:$rb), !strconcat(asm, "\t$ra, 0($rb)"), [(set ty:$ra, (op GPR:$rb))], IIAlu>;
}

defm LOADW:LOAD<load, "lw", GPR>;
defm LOADH:LOAD<extloadi16, "lh", GPR>;
defm LOADSH:LOAD<sextloadi16, "lh", GPR>;
defm LOADZH:LOAD<zextloadi16, "lh", GPR>;
defm LOADB:LOAD<extloadi8, "lb", GPR>;
defm LOADSB:LOAD<sextloadi8, "lb", GPR>;
defm LOADZB:LOAD<zextloadi8, "lb", GPR>;

defm LOADF:LOAD<load, "flw", FPR>;
defm LOADD:LOAD<load, "fld", FPR64>;

def ToyRET : SDNode<"ToyISD::Ret", SDTNone, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def JALR : InstI<(outs GPR:$rd),(ins GPR:$rs1, imm12:$imm12),
           "jalr\t$rd, ${imm12}(${rs1})", [],
           IIAlu>;

def J : InstJ<(outs),(ins brtarget:$imm12),
           "j\t${imm12}", [],
           IIAlu> {
  // NOTE: isTerminator is required to generator label of the fallthrough bb
  let isTerminator = 1;
}

def RET : InstI<(outs),(ins), "ret", [(ToyRET)], IIAlu> {
    let isReturn = 1;
    let isTerminator=1;
}

def SDT_Call      : SDTypeProfile<0, 1, [SDTCisVT<0, iPTR>]>;

def ToyCall : SDNode<"ToyISD::Call", SDT_Call, [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;

def CALL : InstI<(outs),(ins calltarget:$rs1), "call\t$rs1", [], IIAlu> {
    // NOTE: isCall is required to save RA, check `determineCalleeSaves` for details
    let isCall = 1;
}

def : Pat<(ToyCall tglobaladdr:$rs1),(CALL calltarget:$rs1)>;
def : Pat<(ToyCall texternalsym:$rs1),(CALL calltarget:$rs1)>;

def LEA : InstI<(outs GPR:$rd),(ins calltarget:$rs1), "la\t$rd, $rs1", [], IIAlu>;

def SLT : InstR<(outs GPR:$rd),(ins GPR:$rs1, GPR:$rs2),
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

def LI : InstI<(outs GPR:$rd),(ins imm32:$rs1), "li\t$rd, $rs1", [], IIAlu>;

def : Pat<(i32 imm32:$in),
          (LI imm32:$in)>;

def LEA_STACK : InstI<(outs GPR:$rd),(ins mem_stack:$mem), "addi \t$rd, $mem", [(set GPR:$rd, AddrFI:$mem)], IIAlu>;

class ArithLogicF <string inst, SDNode node>:
      InstR<(outs FPR:$rd), (ins FPR:$rs1, FPR:$rs2),
      !strconcat(inst, "\t$rd, $rs1, $rs2"),
      [(set FPR:$rd, (node FPR:$rs1, FPR:$rs2))],
      IIAlu>;

def FADDS : ArithLogicF<"fadd.s", fadd>;
def FSUBS : ArithLogicF<"fsub.s", fsub>;
def FMULS : ArithLogicF<"fmul.s", fmul>;
def FDIVS : ArithLogicF<"fdiv.s", fdiv>;

def FMADDS : InstR<(outs FPR:$rd), (ins FPR:$rs1, FPR:$rs2, FPR: $rs3),
      "fmadd.s\t$rd, $rs1, $rs2, $rs3",
      [(set FPR:$rd, (fma FPR:$rs1, FPR:$rs2, FPR:$rs3))],
      IIAlu>;

def FMAXNUM : InstR<(outs FPR:$rd), (ins FPR:$rs1, FPR:$rs2),
      "fmax.s\t$rd, $rs1, $rs2",
      [(set FPR:$rd, (fmaxnum FPR:$rs1, FPR:$rs2))],
      IIAlu>;

class FSETCC<string asm, PatFrag node>:InstR<(outs GPR:$rd),(ins FPR:$rs1, FPR:$rs2),
                                     !strconcat(asm, "\t$rd, $rs1, $rs2"),
                                     [(set GPR:$rd, (node FPR:$rs1, FPR:$rs2))], IIAlu>;
def FEQS:FSETCC<"feq.s", seteq>;
def FLTS:FSETCC<"flt.s", setlt>;
def FLES:FSETCC<"fle.s", setle>;

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

class UnaryF <string inst, SDNode node>:
      InstR<(outs FPR:$rd), (ins FPR:$rs1),
      !strconcat(inst, "\t$rd, $rs1"),
      [(set FPR:$rd, (node FPR:$rs1))],
      IIAlu>;

def FABSS:UnaryF<"fabs.s", fabs>;

def FCVTSD : InstR<(outs FPR:$rd), (ins FPR64:$rs1),
      "fcvt.s.d\t$rd, $rs1",
      [(set FPR:$rd, (fpround FPR64:$rs1))],
      IIAlu>;

def FCVTDS : InstR<(outs FPR64:$rd), (ins FPR:$rs1),
      "fcvt.d.s\t$rd, $rs1",
      [(set FPR64:$rd, (fpextend FPR:$rs1))],
      IIAlu>;

class ArithLogicF64 <string inst, SDNode node>:
      InstR<(outs FPR64:$rd), (ins FPR64:$rs1, FPR64:$rs2),
      !strconcat(inst, "\t$rd, $rs1, $rs2"),
      [(set FPR64:$rd, (node FPR64:$rs1, FPR64:$rs2))],
      IIAlu>;

def FADDD : ArithLogicF64<"fadd.d", fadd>;
def FSUBD : ArithLogicF64<"fsub.d", fsub>;
def FMULD : ArithLogicF64<"fmul.d", fmul>;
def FDIVD : ArithLogicF64<"fdiv.d", fdiv>;
