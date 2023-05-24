include "ToyInstrFormats.td"

def imm12 : Operand<i32>, ImmLeaf<i32, [{return isInt<12>(Imm);}]>;

def mem : Operand<iPTR> {
  let MIOperandInfo = (ops GPR, imm12);
  let PrintMethod = "printMemOperand";
}

def ADDI : InstI<(outs GPR:$rd), (ins GPR:$rs1, imm12:$imm),
          "addi \t$rd, $rs1, $imm",
          [(set GPR:$rd, (add GPR:$rs1, imm12:$imm))],
          IIAlu>;

def ADD : InstR<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
          "add \t$rd, $rs1, $rs2",
          [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))],
          IIAlu>;

def MUL : InstR<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
          "mul \t$rd, $rs1, $rs2",
          [(set GPR:$rd, (mul GPR:$rs1, GPR:$rs2))],
          IIAlu>;

def DIV : InstR<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
          "div \t$rd, $rs1, $rs2",
          [(set GPR:$rd, (sdiv GPR:$rs1, GPR:$rs2))],
          IIAlu>;

def REM : InstR<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
          "rem \t$rd, $rs1, $rs2",
          [(set GPR:$rd, (srem GPR:$rs1, GPR:$rs2))],
          IIAlu>;

def ANDI : InstI<(outs GPR:$rd), (ins GPR:$rs1, imm12:$imm),
          "andi \t$rd, $rs1, $imm",
          [(set GPR:$rd, (and GPR:$rs1, imm12:$imm))],
          IIAlu>;

def ORI : InstI<(outs GPR:$rd), (ins GPR:$rs1, imm12:$imm),
          "ori \t$rd, $rs1, $imm",
          [(set GPR:$rd, (or GPR:$rs1, imm12:$imm))],
          IIAlu>;

def AND : InstR<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
          "and \t$rd, $rs1, $rs2",
          [(set GPR:$rd, (and GPR:$rs1, GPR:$rs2))],
          IIAlu>;

def OR : InstR<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
          "or \t$rd, $rs1, $rs2",
          [(set GPR:$rd, (or GPR:$rs1, GPR:$rs2))],
          IIAlu>;

def AddrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;

def STORE : ToyInst<(outs), (ins GPR:$ra, mem:$addr), "sw \t$ra, $addr", [], IIAlu>;

def LOAD : ToyInst<(outs GPR:$ra), (ins mem:$addr), "lw \t$ra, $addr", [], IIAlu>;

def : Pat<(i32 imm12:$in),
          (ADDI ZERO, imm12:$in)>;

def : Pat<(store GPR:$ra, AddrFI:$addr),
          (STORE GPR:$ra, AddrFI:$addr)>;

// NOTE: 使用 Pat 等价与下面的 LOAD 定义
// def LOAD : ToyInst<(outs GPR:$ra), (ins mem:$addr), "lw \t$ra, $addr",
//          [(set GPR:$ra, (load AddrFI:$addr))],
//          IIAlu>;
def : Pat<(load AddrFI:$addr),
          (LOAD  AddrFI:$addr)>;
