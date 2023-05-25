include "ToyInstrFormats.td"

def imm12 : Operand<i32>, ImmLeaf<i32, [{return isInt<12>(Imm);}]>;
def imm20 : Operand<i32>, ImmLeaf<i32, [{return isInt<20>(Imm);}]>;

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

def ToyHi    : SDNode<"ToyISD::Hi", SDTIntUnaryOp>;
def ToyLo    : SDNode<"ToyISD::Lo", SDTIntUnaryOp>;

def LUI : InstU<(outs GPR:$rd), (ins imm20:$imm),
          "lui $rd, $imm", [], IIAlu>;

def : Pat<(ToyHi tglobaladdr:$in), (LUI tglobaladdr:$in)>;
def : Pat<(add GPR:$hi, (ToyLo tglobaladdr:$lo)),
          (ADDI GPR:$hi, tglobaladdr:$lo)>;

def AddrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;

def STORE : ToyInst<(outs), (ins GPR:$ra, mem:$addr), "sw \t$ra, $addr", [], IIAlu>;

def LOADFI : ToyInst<(outs GPR:$ra), (ins mem:$addr), "lw \t$ra, $addr", [], IIAlu>;
def LOAD : ToyInst<(outs GPR:$ra), (ins GPR:$x), "lw \t$ra, $x", [], IIAlu>;

def : Pat<(i32 imm12:$in),
          (ADDI ZERO, imm12:$in)>;

def : Pat<(store GPR:$ra, AddrFI:$addr),
          (STORE GPR:$ra, AddrFI:$addr)>;

// NOTE: 使用 Pat 等价与下面的 LOAD 定义
// def LOAD : ToyInst<(outs GPR:$ra), (ins mem:$addr), "lw \t$ra, $addr",
//          [(set GPR:$ra, (load AddrFI:$addr))],
//          IIAlu>;
def : Pat<(load AddrFI:$addr),
          (LOADFI  AddrFI:$addr)>;

def : Pat<(load GPR:$x),
          (LOAD GPR:$x)>;
