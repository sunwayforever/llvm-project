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

def LUI : InstU<(outs GPR:$rd), (ins imm20:$imm),
          "lui $rd, $imm", [], IIAlu>;

def AddrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;

def STOREFI : ToyInst<(outs), (ins GPR:$ra, mem:$addr), "sw \t$ra, $addr", [], IIAlu>;
def STORE : ToyInst<(outs), (ins GPR:$ra, GPR:$rb), "sw \t$ra, 0($rb)", [], IIAlu>;

def LOADFI : ToyInst<(outs GPR:$ra), (ins mem:$addr), "lw \t$ra, $addr", [], IIAlu>;
def LOAD : ToyInst<(outs GPR:$ra), (ins GPR:$x), "lw \t$ra, $x", [], IIAlu>;

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

def ToyRET : SDNode<"ToyISD::Ret", SDTNone,
                             [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def JALR : InstI<(outs GPR:$rd),(ins GPR:$rs1, imm12:$imm12),
           "jalr $rd, ${imm12}(${rs1})", [],
           IIAlu>;

// def RET : InstI<(outs),(ins), "jalr zero, 0(ra)", [(ToyRET)], IIAlu>;
def RET : Pseudo<(outs),(ins), [(ToyRET)]>, PseudoInstExpansion<(JALR ZERO, RA, 0)>;
