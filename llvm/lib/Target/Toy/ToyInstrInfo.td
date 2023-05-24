include "ToyInstrFormats.td"

def imm12 : Operand<i32>, ImmLeaf<i32, [{return isInt<12>(Imm);}]>;

def mem : Operand<iPTR> {
  let MIOperandInfo = (ops GPR, imm12);
  let PrintMethod = "printMemOperand";  
}

def ADDI : InstI<0b0010011, 0b000, (outs GPR:$rd), (ins GPR:$rs1, imm12:$imm),
          "addi \t$rd, $rs1, $imm",
          [(set GPR: $rd, (add GPR: $rs1, imm12:$imm))],
          IIAlu>;

def AddrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;

def STORE : ToyInst<(outs), (ins GPR:$ra, mem:$addr), "sw \t$ra, $addr", [],IIAlu>;

def : Pat<(i32 imm12:$in),
          (ADDI ZERO, imm12:$in)>;

def : Pat<(store GPR:$ra, AddrFI:$addr),
          (STORE GPR:$ra, AddrFI:$addr)>;       
