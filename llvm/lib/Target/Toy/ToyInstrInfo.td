include "ToyInstrFormats.td"

def imm12 : Operand<i32>, ImmLeaf<i32, [{return isInt<12>(Imm);}]>;
def imm20 : Operand<i32>, ImmLeaf<i32, [{return isInt<20>(Imm);}]>;
def brtarget : Operand<OtherVT>;

def mem : Operand<iPTR> {
  let MIOperandInfo = (ops GPR, imm12);
  let PrintMethod = "printMemOperand";
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
def MUL : ArithLogicR<"mul", mul>;
def DIV : ArithLogicR<"div", sdiv>;
def REM : ArithLogicR<"rem", srem>;
def AND : ArithLogicR<"and", and>;
def OR : ArithLogicR<"or", or>;
def XOR : ArithLogicR<"xor", xor>;

def LUI : InstU<(outs GPR:$rd), (ins imm20:$imm),
          "lui $rd, $imm", [], IIAlu>;

def AddrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;

def STOREFI : ToyInst<(outs), (ins GPR:$ra, mem:$addr), "sw \t$ra, $addr", [], IIAlu>;
def STORE : ToyInst<(outs), (ins GPR:$ra, GPR:$rb), "sw \t$ra, 0($rb)", [], IIAlu>;

def LOADFI : ToyInst<(outs GPR:$ra), (ins mem:$addr), "lw \t$ra, $addr", [], IIAlu>;
def LOAD : ToyInst<(outs GPR:$ra), (ins GPR:$x), "lw \t$ra, 0($x)", [], IIAlu>;

def : Pat<(i32 imm12:$in),
          (ADDI ZERO, imm12:$in)>;

def : Pat<(store GPR:$ra, AddrFI:$addr),
          (STOREFI GPR:$ra, AddrFI:$addr)>;
def : Pat<(store GPR:$x, GPR:$y),
          (STORE GPR:$x, GPR:$y)>;

// NOTE: 使用 Pat 等价与下面的 LOAD 定义
// def LOAD : ToyInst<(outs GPR:$ra), (ins mem:$addr), "lw \t$ra, $addr",
//          [(set GPR:$ra, (load AddrFI:$addr))],
//          IIAlu>;
def : Pat<(load AddrFI:$addr),
          (LOADFI AddrFI:$addr)>;
def : Pat<(load GPR:$x),
          (LOAD GPR:$x)>;

def ToyRET : SDNode<"ToyISD::Ret", SDTNone, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def JALR : InstI<(outs GPR:$rd),(ins GPR:$rs1, imm12:$imm12),
           "jalr $rd, ${imm12}(${rs1})", [],
           IIAlu>;

def J : InstJ<(outs),(ins brtarget:$imm12),
           "j ${imm12}", [],
           IIAlu> {
  let isTerminator = 1;
}

// def RET : InstI<(outs),(ins), "jalr zero, 0(ra)", [(ToyRET)], IIAlu>;
let isReturn=1, isTerminator=1, hasDelaySlot=1, isBarrier=1, hasCtrlDep=1 in
def RET : Pseudo<(outs),(ins), [(ToyRET)]>, PseudoInstExpansion<(JALR ZERO, RA, 0)>;

def SDT_Call      : SDTypeProfile<0, 1, [SDTCisVT<0, iPTR>]>;

def ToyCall : SDNode<"ToyISD::Call", SDT_Call, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

// def CALL : InstI<(outs),(ins GPR:$rs1), "jalr ra, 0($rs1)", [(ToyCall GPR:$rs1)], IIAlu>;
def CALL : Pseudo<(outs),(ins GPR:$rs1), [(ToyCall GPR:$rs1)]>, PseudoInstExpansion<(JALR RA, GPR:$rs1, 0)> {
    let isCall = 1;
}

def SLT : InstR<(outs GPR:$rd),(ins GPR:$rs1, GPR:$rs2),
           "slt \t$rd, $rs1, $rs2", [],
           IIAlu>;

def : Pat<(setgt GPR:$lhs, GPR:$rhs), (SLT GPR:$rhs, GPR:$lhs)>;
def : Pat<(setlt GPR:$lhs, GPR:$rhs), (SLT GPR:$lhs, GPR:$rhs)>;
def : Pat<(seteq GPR:$lhs, GPR:$rhs), (SLT (XOR GPR:$lhs, GPR:$rhs), 1)>;
def : Pat<(setne GPR:$lhs, GPR:$rhs), (SLT ZERO, (XOR GPR:$lhs, GPR:$rhs))>;
// def : Pat<(setge GPR:$rs1, GPR:$rs2), (XORI (SLT GPR:$rs1, GPR:$rs2), 1)>;
// def : Pat<(setle GPR:$rs1, GPR:$rs2), (XORI (SLT GPR:$rs2, GPR:$rs1), 1)>;

def BLT : InstB<(outs),(ins GPR:$rs1, GPR:$rs2, brtarget:$imm),
           "blt \t$rs1, $rs2, $imm", [],
           IIAlu>;

def BEQ : InstB<(outs),(ins GPR:$rs1, GPR:$rs2, brtarget:$imm),
           "beq \t$rs1, $rs2, $imm", [],
           IIAlu>;

def BNE : InstB<(outs),(ins GPR:$rs1, GPR:$rs2, brtarget:$imm),
           "bne \t$rs1, $rs2, $imm", [],
           IIAlu>;

def : Pat<(brcond (i32 (setlt GPR:$lhs, GPR:$rhs)), bb:$dst), (BLT GPR:$lhs, GPR:$rhs, brtarget:$dst)>;

def : Pat<(brcond (i32 (setgt GPR:$lhs, GPR:$rhs)), bb:$dst), (BLT GPR:$rhs, GPR:$lhs, brtarget:$dst)>;

def : Pat<(brcond (i32 (seteq GPR:$lhs, GPR:$rhs)), bb:$dst), (BEQ GPR:$rhs, GPR:$lhs, brtarget:$dst)>;

def : Pat<(brcond (i32 (setne GPR:$lhs, GPR:$rhs)), bb:$dst), (BNE GPR:$rhs, GPR:$lhs, brtarget:$dst)>;

def : Pat<(br bb:$imm), (J brtarget:$imm)>;

