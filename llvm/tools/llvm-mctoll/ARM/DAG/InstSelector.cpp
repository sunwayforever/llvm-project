//===- InstSelector.cpp - Binary raiser utility llvm-mctoll ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementaion of InstSelector class for use
// by llvm-mctoll.
//
//===----------------------------------------------------------------------===//

#include "InstSelector.h"
#include "ARM.h"
#include "ARMSubtarget.h"
#include "SelectionCommon.h"

using namespace llvm;
using namespace llvm::mctoll;

/// Replace all uses of F with T, then remove F from the DAG.
void InstSelector::replaceNode(SDNode *F, SDNode *T) {
  if (MachineSDNode::classof(F)) {
    NodePropertyInfo *NP = DAGInfo->NPMap[F];
    DAGInfo->NPMap.erase(F);
    DAGInfo->NPMap[T] = NP;
  }

  CurDAG->ReplaceAllUsesWith(F, T);
  CurDAG->RemoveDeadNode(F);
}

/// Checks the SDNode is a function argument or not.
bool InstSelector::isArgumentNode(SDNode *Node) {
  if (!FrameIndexSDNode::classof(Node))
    return false;

  return FuncInfo->isArgumentIndex(
      dyn_cast<FrameIndexSDNode>(Node)->getIndex());
}

/// Checks the SDNode is a function return or not.
bool InstSelector::isReturnNode(SDNode *Node) {
  if (!FrameIndexSDNode::classof(Node))
    return false;

  return FuncInfo->isReturnIndex(dyn_cast<FrameIndexSDNode>(Node)->getIndex());
}

/// Record the new defined Node, it uses to map the register number to Node.
/// In DAG emitter, emitter get a value of use base on this defined Node.
void InstSelector::recordDefinition(SDNode *OldNode, SDNode *NewNode) {
  assert(NewNode != nullptr &&
         "The new SDNode ptr is null when record define!");

  if (OldNode == nullptr) {
    outs() << "Warning: RecordDefine error, the SDNode ptr is null!\n";
    return;
  }

  if (RegisterSDNode::classof(OldNode)) {
    Register OpReg = static_cast<RegisterSDNode *>(OldNode)->getReg();
    FuncInfo->setValueByRegister(OpReg, SDValue(NewNode, 0));
    FuncInfo->NodeRegMap[NewNode] = OpReg;
  }

  if (isReturnNode(OldNode)) {
    FuncInfo->setRetValue(SDValue(NewNode, 0));
    FuncInfo->setValueByRegister(ARM::R0, SDValue(NewNode, 0));
    FuncInfo->NodeRegMap[NewNode] = ARM::R0;
  }
}

/// Gets the Metadata of given SDNode.
SDValue InstSelector::getMDOperand(SDNode *N) {
  for (auto &Sdv : N->ops()) {
    if (MDNodeSDNode::classof(Sdv.getNode())) {
      return Sdv.get();
    }
  }
  assert(false && "Should not run at here!");
  return SDValue();
}

/// Instruction opcode selection.
void InstSelector::selectCode(SDNode *N) {
  SDLoc Dl(N);

  switch (N->getMachineOpcode()) {
  default:
    break;
  /* ADC */
  case ARM::ADCrr:
  case ARM::ADCri:
  case ARM::ADCrsr:
  case ARM::ADCrsi:
  case ARM::tADC:
  case ARM::t2ADCrr:
  case ARM::t2ADCri:
  case ARM::t2ADCrs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      // ADCS <Rdn>,<Rm>
      // ADC<c> <Rdn>,<Rm>
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG
              ->getNode(ISD::ADDC, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      // ADC{S}<c> <Rd>,<Rn>,#<const>
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node = CurDAG
                 ->getNode(ISD::ADDC, Dl, getDefaultEVT(), Rn, Op2,
                           getMDOperand(N))
                 .getNode();
    }

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* ADD */
  case ARM::ADDri:
  case ARM::ADDrr:
  case ARM::ADDrsi:
  case ARM::ADDrsr:
  case ARM::tADDspi:
  case ARM::tADDrSP:
  case ARM::tADDi3:
  case ARM::tADDrSPi:
  case ARM::tADDi8:
  case ARM::tADDhirr:
  case ARM::tADDrr:
  case ARM::tADDspr:
  case ARM::t2ADDrs:
  case ARM::t2ADDri:
  case ARM::t2ADDrr:
  case ARM::t2ADDri12: {
    // TODO:
    // 1. Check out MI is two-address or three-address
    // 2. Do with the displacement operation.(not yet implement.)
    // Judge the MI address module, then check out whether has the imm.
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    // <opcode>   {<cond>}{s}<Rd>，<Rn>{，<OP2>}
    SDNode *Node = nullptr;
    if (FrameIndexSDNode::classof(N->getOperand(1).getNode())) {
      Node = CurDAG
                 ->getNode(EXT_ARMISD::LOAD, Dl, getDefaultEVT(), Rn,
                           getMDOperand(N))
                 .getNode();
    } else {
      if (isTwoAddressMode(Rd.getNode())) {
        if (RegisterSDNode::classof(N->getOperand(1).getNode()))
          Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

        SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
        Node = CurDAG
                   ->getNode(ISD::ADD, Dl, getDefaultEVT(), Rd, Rn,
                             getMDOperand(N))
                   .getNode();
      } else {
        SDValue Op2 = N->getOperand(2);
        if (RegisterSDNode::classof(Op2.getNode()))
          Op2 = FuncInfo->getValFromRegMap(Op2);

        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
        Node = CurDAG
                   ->getNode(ISD::ADD, Dl, getDefaultEVT(), Rn, Op2,
                             getMDOperand(N))
                   .getNode();
      }
    }

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* SUB */
  case ARM::SUBri:
  case ARM::SUBrr:
  case ARM::SUBrsi:
  case ARM::SUBrsr:
  case ARM::tSUBi3:
  case ARM::tSUBi8:
  case ARM::tSUBrr:
  case ARM::tSUBspi:
  case ARM::t2SUBri:
  case ARM::t2SUBri12:
  case ARM::t2SUBrr:
  case ARM::t2SUBrs:
  case ARM::t2SUBS_PC_LR: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG
              ->getNode(ISD::SUB, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);

      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node =
          CurDAG
              ->getNode(ISD::SUB, Dl, getDefaultEVT(), Rn, Op2, getMDOperand(N))
              .getNode();
    }
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* MOV */
  case ARM::MOVi16:
  case ARM::t2MOVi16:
  case ARM::MOVi32imm:
  case ARM::tMOVr:
  case ARM::MOVr:
  case ARM::t2MOVi:
  case ARM::t2MOVr:
  case ARM::MOVCCr:
  case ARM::t2MOVCCr:
  case ARM::t2MOVi32imm:
  case ARM::MOVTi16:
  case ARM::MOVi: {
    // Dispalcement operation need do.
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    if (RegisterSDNode::classof(Rn.getNode()))
      Rn = FuncInfo->getValFromRegMap(Rn);

    SDNode *Node = CurDAG
                       ->getNode(ARMISD::CMOV, Dl, getDefaultEVT(), Rn,
                                 CurDAG->getConstant(0, Dl, getDefaultEVT()))
                       .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* STR */
  case ARM::STRi12:
  case ARM::STRrs:
  case ARM::STRD:
  case ARM::STRD_POST:
  case ARM::STRD_PRE:
  case ARM::t2STREXD:
  case ARM::STREXB:
  case ARM::STREXD:
  case ARM::STREXH:
  case ARM::STREX:
  case ARM::STR_PRE_IMM:
  case ARM::STR_PRE_REG:
  case ARM::STR_POST_IMM:
  case ARM::STR_POST_REG: {
    SDValue Val = N->getOperand(0);
    SDValue Ptr = N->getOperand(1); // This is a pointer.

    if (RegisterSDNode::classof(Val.getNode()))
      Val = FuncInfo->getValFromRegMap(Val);

    if (RegisterSDNode::classof(Ptr.getNode()))
      Ptr = FuncInfo->getValFromRegMap(Ptr);

    SDNode *Node = CurDAG
                       ->getNode(EXT_ARMISD::STORE, Dl, getDefaultEVT(), Val,
                                 Ptr, getMDOperand(N))
                       .getNode();
    replaceNode(N, Node);
  } break;
  case ARM::STRH:
  case ARM::STRH_PRE:
  case ARM::STRH_POST: {
    EVT InstTy = EVT::getEVT(Type::getInt16Ty(*CurDAG->getContext()));
    SDValue Val = N->getOperand(0);
    SDValue Op1 = N->getOperand(1);
    SDNode *Node = nullptr;

    if (RegisterSDNode::classof(Val.getNode()))
      Val = FuncInfo->getValFromRegMap(Val);

    if (RegisterSDNode::classof(Op1.getNode()))
      Op1 = FuncInfo->getValFromRegMap(Op1);

    if (N->getNumOperands() < 5)
      Node = CurDAG
                 ->getNode(EXT_ARMISD::STORE, Dl, InstTy, Val, Op1,
                           getMDOperand(N))
                 .getNode();
    else {
      SDValue Op2 = N->getOperand(2);
      Op2 = FuncInfo->getValFromRegMap(Op2);
      Node = CurDAG
                 ->getNode(EXT_ARMISD::STORE, Dl, InstTy, Val, Op1, Op2,
                           getMDOperand(N))
                 .getNode();
    }

    replaceNode(N, Node);
  } break;
  case ARM::STRBi12:
  case ARM::STRBrs:
  case ARM::STRB_PRE_IMM:
  case ARM::STRB_PRE_REG:
  case ARM::STRB_POST_IMM:
  case ARM::STRB_POST_REG: {
    EVT InstTy = EVT::getEVT(Type::getInt8Ty(*CurDAG->getContext()));
    SDValue Val = N->getOperand(0);
    SDValue Op1 = N->getOperand(1);
    SDNode *Node = nullptr;

    if (RegisterSDNode::classof(Val.getNode()))
      Val = FuncInfo->getValFromRegMap(Val);

    if (RegisterSDNode::classof(Op1.getNode()))
      Op1 = FuncInfo->getValFromRegMap(Op1);

    if (N->getNumOperands() < 5)
      Node = CurDAG
                 ->getNode(EXT_ARMISD::STORE, Dl, InstTy, Val, Op1,
                           getMDOperand(N))
                 .getNode();
    else {
      SDValue Op2 = N->getOperand(2);
      Op2 = FuncInfo->getValFromRegMap(Op2);
      Node = CurDAG
                 ->getNode(EXT_ARMISD::STORE, Dl, InstTy, Val, Op1, Op2,
                           getMDOperand(N))
                 .getNode();
    }

    replaceNode(N, Node);
  } break;
  /* LDR */
  case ARM::LDRi12:
  case ARM::LDRrs:
  case ARM::t2LDR_PRE:
  case ARM::t2LDR_POST:
  case ARM::tLDR_postidx:
  case ARM::LDR_PRE_IMM:
  case ARM::LDR_PRE_REG:
  case ARM::LDR_POST_IMM:
  case ARM::LDR_POST_REG: {
    EVT InstTy = EVT::getEVT(Type::getInt32Ty(*CurDAG->getContext()));
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (RegisterSDNode::classof(Rn.getNode()))
      Rn = FuncInfo->getValFromRegMap(Rn);

    Node = CurDAG->getNode(EXT_ARMISD::LOAD, Dl, InstTy, Rn, getMDOperand(N))
               .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  case ARM::LDRH:
  case ARM::LDRSH:
  case ARM::t2LDRSH_PRE:
  case ARM::t2LDRSH_POST:
  case ARM::t2LDRH_PRE:
  case ARM::t2LDRH_POST:
  case ARM::LDRSH_PRE:
  case ARM::LDRSH_POST: {
    EVT InstTy = EVT::getEVT(Type::getInt16Ty(*CurDAG->getContext()));
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;

    if (RegisterSDNode::classof(Rn.getNode()))
      Rn = FuncInfo->getValFromRegMap(Rn);
    Node = CurDAG->getNode(EXT_ARMISD::LOAD, Dl, InstTy, Rn, getMDOperand(N))
               .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  case ARM::LDRBi12:
  case ARM::LDRBrs:
  case ARM::t2LDRSB_PRE:
  case ARM::t2LDRSB_POST:
  case ARM::t2LDRB_PRE:
  case ARM::t2LDRB_POST:
  case ARM::LDRSB_PRE:
  case ARM::LDRSB_POST:
  case ARM::LDRB_PRE_IMM:
  case ARM::LDRB_POST_IMM:
  case ARM::LDRB_PRE_REG:
  case ARM::LDRB_POST_REG: {
    EVT InstTy = EVT::getEVT(Type::getInt8Ty(*CurDAG->getContext()));
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;

    if (RegisterSDNode::classof(Rn.getNode()))
      Rn = FuncInfo->getValFromRegMap(Rn);
    Node = CurDAG->getNode(EXT_ARMISD::LOAD, Dl, InstTy, Rn, getMDOperand(N))
               .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* Branch */
  case ARM::Bcc:
  case ARM::tBcc:
  case ARM::t2Bcc: {
    SDValue Iftrue = N->getOperand(0);
    SDValue Cond = N->getOperand(1);
    SDNode *Node = nullptr;

    if (DAGInfo->NPMap[N]->HasCPSR)
      Node = CurDAG
                 ->getNode(ISD::BRCOND, Dl, getDefaultEVT(), Iftrue, Cond,
                           getMDOperand(N))
                 .getNode();
    else
      Node =
          CurDAG->getNode(ISD::BR, Dl, getDefaultEVT(), Iftrue, getMDOperand(N))
              .getNode();

    const MachineBasicBlock *LMBB = DAGInfo->NPMap[N]->MI->getParent();
    if (LMBB->succ_size() == 0) {
      FuncInfo->setValueByRegister(ARM::R0, SDValue(Node, 0));
      FuncInfo->NodeRegMap[Node] = ARM::R0;
    }
    replaceNode(N, Node);
  } break;
  case ARM::B:
  case ARM::tB:
  case ARM::t2B: {
    SDValue BrBlock = N->getOperand(0);
    SDNode *Node =
        CurDAG->getNode(ISD::BR, Dl, getDefaultEVT(), BrBlock, getMDOperand(N))
            .getNode();

    replaceNode(N, Node);
  } break;
  case ARM::BL:
  case ARM::BL_pred:
  case ARM::tBL: {
    SDValue Func = N->getOperand(0);
    SDNode *Node = nullptr;
    if (RegisterSDNode::classof(Func.getNode())) {
      Func = FuncInfo->getValFromRegMap(Func);
      Node =
          CurDAG
              ->getNode(ISD::BRIND, Dl, getDefaultEVT(), Func, getMDOperand(N))
              .getNode();
    } else {
      Node = CurDAG
                 ->getNode(EXT_ARMISD::BRD, Dl, getDefaultEVT(), Func,
                           getMDOperand(N))
                 .getNode();
    }

    FuncInfo->setValueByRegister(ARM::R0, SDValue(Node, 0));
    FuncInfo->NodeRegMap[Node] = ARM::R0;
    replaceNode(N, Node);
  } break;
  case ARM::BLX:
  case ARM::BLXi:
  case ARM::BLX_pred:
  case ARM::tBLXi:
  case ARM::tBLXr: {
    SDValue Func = N->getOperand(0);
    SDNode *Node = nullptr;
    if (RegisterSDNode::classof(Func.getNode())) {
      Func = FuncInfo->getValFromRegMap(Func);
      Node =
          CurDAG
              ->getNode(ISD::BRIND, Dl, getDefaultEVT(), Func, getMDOperand(N))
              .getNode();
    } else {
      Node = CurDAG
                 ->getNode(EXT_ARMISD::BRD, Dl, getDefaultEVT(), Func,
                           getMDOperand(N))
                 .getNode();
    }

    FuncInfo->setValueByRegister(ARM::R0, SDValue(Node, 0));
    FuncInfo->NodeRegMap[Node] = ARM::R0;
    replaceNode(N, Node);
  } break;
  case ARM::BR_JTr: {
    SDNode *Node = nullptr;
    SDValue Rd = N->getOperand(0);
    Node = CurDAG->getNode(ISD::BR_JT, Dl, getDefaultEVT(), Rd, getMDOperand(N))
               .getNode();
    replaceNode(N, Node);
  } break;
  case ARM::BX:
  case ARM::BX_CALL:
  case ARM::BX_pred:
  case ARM::tBX:
  case ARM::tBX_CALL: {
    SDValue CallReg = N->getOperand(0);
    if (RegisterSDNode::classof(CallReg.getNode()))
      CallReg = FuncInfo->getValFromRegMap(CallReg);

    SDNode *Node =
        CurDAG
            ->getNode(ISD::BRIND, Dl, getDefaultEVT(), CallReg, getMDOperand(N))
            .getNode();
    replaceNode(N, Node);
  } break;
  case ARM::BX_RET:
  case ARM::tBX_RET:
    // assert(0 && "Branch instructions are removed in previous stage. should
    // not get here!");
    break;
  case ARM::tCMPhir:
  case ARM::CMPrr:
  case ARM::t2CMPri:
  case ARM::CMPri:
  case ARM::tCMPi8:
  case ARM::t2CMPrr:
  case ARM::tCMPr: {
    SDValue Cmpl = N->getOperand(0);
    SDValue Cmph = N->getOperand(1);
    if (RegisterSDNode::classof(Cmph.getNode()))
      Cmph = FuncInfo->getValFromRegMap(N->getOperand(1));
    Cmpl = FuncInfo->getValFromRegMap(Cmpl);

    // Create condition SDValuleR
    // TODO: It should be verified why this type node can not be added Metadata
    // Operand.
    SDNode *Node = CurDAG
                       ->getNode(ISD::SETCC, Dl, getDefaultEVT(), Cmpl, Cmph
                                 /* , getMDOperand(N) */)
                       .getNode();

    replaceNode(N, Node);
  } break;
  /* AND */
  case ARM::ANDri:
  case ARM::ANDrr:
  case ARM::ANDrsi:
  case ARM::ANDrsr:
  case ARM::tAND:
  case ARM::t2ANDri:
  case ARM::t2ANDrr:
  case ARM::t2ANDrs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;

    if (isTwoAddressMode(Rd.getNode())) {
      // AND<c> <Rdn>,<Rm>
      // ANDS <Rdn>,<Rm>
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG
              ->getNode(ISD::AND, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      // AND{S}<c> <Rd>,<Rn>,#<const>
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);

      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node =
          CurDAG
              ->getNode(ISD::AND, Dl, getDefaultEVT(), Rn, Op2, getMDOperand(N))
              .getNode();
    }

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
    // TODO:
    // AND{S}<c>.W <Rd>,<Rn>,<Rm>{,<shift>}
    // AND{S}<c> <Rd>,<Rn>,<Rm>{,<shift>}
    // AND{S}<c> <Rd>,<Rn>,<Rm>,<type> <Rs>
  } break;
  /* ASR */
  case ARM::ASRr:
  case ARM::ASRi:
  case ARM::tASRrr:
  case ARM::tASRri:
  case ARM::t2ASRrr:
  case ARM::t2ASRri: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG
              ->getNode(ISD::SRA, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);

      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node =
          CurDAG
              ->getNode(ISD::SRA, Dl, getDefaultEVT(), Rn, Op2, getMDOperand(N))
              .getNode();
    }

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* CMN */
  case ARM::CMNri:
  case ARM::CMNzrr:
  case ARM::tCMNz:
  case ARM::t2CMNri:
  case ARM::t2CMNzrr:
  case ARM::t2CMNzrs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;

    if (RegisterSDNode::classof(N->getOperand(1).getNode()))
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
    Rd = FuncInfo->getValFromRegMap(Rd);
    Node =
        CurDAG
            ->getNode(ARMISD::CMN, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
            .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* EOR */
  case ARM::EORri:
  case ARM::EORrr:
  case ARM::EORrsi:
  case ARM::EORrsr:
  case ARM::tEOR:
  case ARM::t2EORrr:
  case ARM::t2EORrs:
  case ARM::t2EORri: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      // EORS <Rdn>,<Rm>
      // EOR<c> <Rdn>,<Rm>
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG
              ->getNode(ISD::XOR, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      // EOR{S}<c> <Rd>,<Rn>,#<const>
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node =
          CurDAG
              ->getNode(ISD::XOR, Dl, getDefaultEVT(), Rn, Op2, getMDOperand(N))
              .getNode();
    }
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
    // TODO:
    // EOR{S}<c>.W <Rd>,<Rn>,<Rm>{,<shift>}
    // EOR{S}<c> <Rd>,<Rn>,<Rm>{,<shift>}
    // EOR{S}<c> <Rd>,<Rn>,<Rm>,<type> <Rs>
  } break;
  /* MSR */
  case ARM::MSR:
  case ARM::MSRi:
  case ARM::MSRbanked:
  case ARM::t2MSR_M:
  case ARM::t2MSR_AR:
  case ARM::t2MSRbanked: {
    // Update the CPSR.
    SDValue Cond = N->getOperand(1);
    SDNode *Node = nullptr;
    if (RegisterSDNode::classof(N->getOperand(1).getNode()))
      Cond = FuncInfo->getValFromRegMap(N->getOperand(1));

    Node = CurDAG
               ->getNode(EXT_ARMISD::MSR, Dl, getDefaultEVT(), Cond,
                         getMDOperand(N))
               .getNode();

    replaceNode(N, Node);
  } break;
  /* MUL */
  case ARM::MUL:
  case ARM::tMUL:
  case ARM::t2MUL: {
    /* MULS <Rd>, <Rn>, <Rm> */
    /* MUL<c> <Rd>, <Rn>, <Rm> */
    /* MUL{S}<c> <Rd>, <Rn>, <Rm> */
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    SDValue Op2 = N->getOperand(2);
    Op2 = FuncInfo->getValFromRegMap(Op2);
    Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
    Node =
        CurDAG->getNode(ISD::MUL, Dl, getDefaultEVT(), Rn, Op2, getMDOperand(N))
            .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* MVN */
  case ARM::MVNi:
  case ARM::MVNr:
  case ARM::MVNsi:
  case ARM::MVNsr:
  case ARM::tMVN:
  case ARM::t2MVNi:
  case ARM::t2MVNr:
  case ARM::t2MVNs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (RegisterSDNode::classof(N->getOperand(1).getNode()))
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

    Node = CurDAG
               ->getNode(ISD::XOR, Dl, getDefaultEVT(), Rn,
                         CurDAG->getConstant(-1, Dl, getDefaultEVT()))
               .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* LSL */
  case ARM::LSLi:
  case ARM::LSLr:
  case ARM::tLSLri:
  case ARM::tLSLrr:
  case ARM::t2LSLri:
  case ARM::t2LSLrr: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG
              ->getNode(ISD::SHL, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);

      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node =
          CurDAG
              ->getNode(ISD::SHL, Dl, getDefaultEVT(), Rn, Op2, getMDOperand(N))
              .getNode();
    }
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* LSR */
  case ARM::LSRi:
  case ARM::LSRr:
  case ARM::tLSRri:
  case ARM::tLSRrr:
  case ARM::t2LSRri:
  case ARM::t2LSRrr: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG
              ->getNode(ISD::SRL, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node =
          CurDAG
              ->getNode(ISD::SRL, Dl, getDefaultEVT(), Rn, Op2, getMDOperand(N))
              .getNode();
    }
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* ORR */
  case ARM::ORRri:
  case ARM::ORRrr:
  case ARM::ORRrsi:
  case ARM::ORRrsr:
  case ARM::tORR:
  case ARM::t2ORRri:
  case ARM::t2ORRrr:
  case ARM::t2ORRrs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    // <opcode>   {<cond>}{s}<Rd>，<Rn>{，<OP2>}
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG->getNode(ISD::OR, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);

      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node =
          CurDAG
              ->getNode(ISD::OR, Dl, getDefaultEVT(), Rn, Op2, getMDOperand(N))
              .getNode();
    }
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* ROR */
  case ARM::RORi:
  case ARM::RORr:
  case ARM::tROR:
  case ARM::t2RORri:
  case ARM::t2RORrr: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node =
          CurDAG
              ->getNode(ISD::ROTR, Dl, getDefaultEVT(), Rd, Rn, getMDOperand(N))
              .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node = CurDAG
                 ->getNode(ISD::ROTR, Dl, getDefaultEVT(), Rn, Op2,
                           getMDOperand(N))
                 .getNode();
    }
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* RRX */
  case ARM::RRX: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
    SDNode *Node = nullptr;
    Node =
        CurDAG->getNode(ARMISD::RRX, Dl, getDefaultEVT(), Rn, getMDOperand(N))
            .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* RSB */
  case ARM::RSBri:
  case ARM::RSBrr:
  case ARM::RSBrsi:
  case ARM::RSBrsr:
  case ARM::tRSB:
  case ARM::t2RSBri:
  case ARM::t2RSBrr:
  case ARM::t2RSBrs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node = CurDAG
                 ->getNode(EXT_ARMISD::RSB, Dl, getDefaultEVT(), Rd, Rn,
                           getMDOperand(N))
                 .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node = CurDAG
                 ->getNode(EXT_ARMISD::RSB, Dl, getDefaultEVT(), Op2, Rn,
                           getMDOperand(N))
                 .getNode();
    }
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* RSC */
  case ARM::RSCri:
  case ARM::RSCrr:
  case ARM::RSCrsi:
  case ARM::RSCrsr: {
    // RSC{S}<c> <Rd>,<Rn>, #0
    // RSC{S}<c>.W <Rd>,<Rn>,#<const>
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node = CurDAG
                 ->getNode(EXT_ARMISD::RSC, Dl, getDefaultEVT(), Rd, Rn,
                           getMDOperand(N))
                 .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node = CurDAG
                 ->getNode(EXT_ARMISD::RSC, Dl, getDefaultEVT(), Rn, Op2,
                           getMDOperand(N))
                 .getNode();
    }
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* CLZ */
  case ARM::CLZ:
  case ARM::t2CLZ: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
    SDNode *Node = nullptr;
    Node = CurDAG->getNode(ISD::CTLZ, Dl, getDefaultEVT(), Rn, getMDOperand(N))
               .getNode();
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* SBC */
  case ARM::SBCrr:
  case ARM::SBCri:
  case ARM::tSBC: {
    SDValue Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
    SDValue Operand2 = FuncInfo->getValFromRegMap(N->getOperand(2));
    SDNode *Node = CurDAG
                       ->getNode(EXT_ARMISD::SBC, Dl, getDefaultEVT(), Rn,
                                 Operand2, getMDOperand(N))
                       .getNode();

    recordDefinition(Rn.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* TEQ */
  case ARM::TEQri:
  case ARM::TEQrr:
  case ARM::TEQrsi:
  case ARM::TEQrsr:
  case ARM::t2TEQri:
  case ARM::t2TEQrr:
  case ARM::t2TEQrs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;

    if (RegisterSDNode::classof(N->getOperand(1).getNode()))
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

    Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
    Node = CurDAG
               ->getNode(EXT_ARMISD::TEQ, Dl, getDefaultEVT(), Rd, Rn,
                         getMDOperand(N))
               .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* TST */
  case ARM::TSTrsi:
  case ARM::TSTrr:
  case ARM::TSTri:
  case ARM::TSTrsr:
  case ARM::tTST:
  case ARM::t2TSTri:
  case ARM::t2TSTrr:
  case ARM::t2TSTrs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;

    if (RegisterSDNode::classof(N->getOperand(1).getNode()))
      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

    Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
    Node = CurDAG
               ->getNode(EXT_ARMISD::TST, Dl, getDefaultEVT(), Rd, Rn,
                         getMDOperand(N))
               .getNode();

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* BIC */
  case ARM::BICri:
  case ARM::BICrr:
  case ARM::BICrsi:
  case ARM::BICrsr:
  case ARM::tBIC:
  case ARM::t2BICri:
  case ARM::t2BICrr:
  case ARM::t2BICrs: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = N->getOperand(1);
    SDNode *Node = nullptr;
    if (isTwoAddressMode(Rd.getNode())) {
      if (RegisterSDNode::classof(N->getOperand(1).getNode()))
        Rn = FuncInfo->getValFromRegMap(N->getOperand(1));

      SDValue Rd = FuncInfo->getValFromRegMap(N->getOperand(0));
      Node = CurDAG
                 ->getNode(EXT_ARMISD::BIC, Dl, getDefaultEVT(), Rd, Rn,
                           getMDOperand(N))
                 .getNode();
    } else {
      SDValue Op2 = N->getOperand(2);
      if (RegisterSDNode::classof(Op2.getNode()))
        Op2 = FuncInfo->getValFromRegMap(Op2);

      Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
      Node = CurDAG
                 ->getNode(EXT_ARMISD::BIC, Dl, getDefaultEVT(), Rn, Op2,
                           getMDOperand(N))
                 .getNode();
    }

    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* MLA */
  case ARM::MLA:
  case ARM::t2MLA: {
    SDValue Rd = N->getOperand(0);
    SDValue Rn = FuncInfo->getValFromRegMap(N->getOperand(1));
    SDValue Rm = FuncInfo->getValFromRegMap(N->getOperand(2));
    SDValue Ra = FuncInfo->getValFromRegMap(N->getOperand(3));
    SDNode *Node = nullptr;
    Node = CurDAG
               ->getNode(EXT_ARMISD::MLA, Dl, getDefaultEVT(), Rn, Rm, Ra,
                         getMDOperand(N))
               .getNode();
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  /* UXTB */
  case ARM::UXTB: {
    SDValue Rd = N->getOperand(0);
    SDValue Rm = N->getOperand(1);
    SDValue Rotation = N->getOperand(2);
    SDNode *Node = nullptr;

    if (RegisterSDNode::classof(N->getOperand(1).getNode()))
      Rm = FuncInfo->getValFromRegMap(N->getOperand(1));
    Node = CurDAG
               ->getNode(EXT_ARMISD::UXTB, Dl, getDefaultEVT(), Rd, Rm,
                         Rotation, getMDOperand(N))
               .getNode();
    recordDefinition(Rd.getNode(), Node);
    replaceNode(N, Node);
  } break;
  case ARM::MCR:
  case ARM::MCRR:
  case ARM::t2MCR:
  case ARM::t2MCRR:
  case ARM::VMSR:
  case ARM::VMSR_FPEXC:
  case ARM::VMSR_FPSID:
  case ARM::VMSR_FPINST:
  case ARM::VMSR_FPINST2: {
    dbgs() << "WARNING: ARM::MCR Not yet implemented!\n";
  } break;
  case ARM::MRS:
  case ARM::MRSsys:
  case ARM::t2MRS_AR:
  case ARM::t2MRSsys_AR: {
    SDValue Rn = N->getOperand(0);
    if (RegisterSDNode::classof(Rn.getNode()))
      Rn = FuncInfo->getValFromRegMap(Rn);

    SDNode *Node =
        CurDAG
            ->getNode(EXT_ARMISD::MRS, Dl, getDefaultEVT(), Rn, getMDOperand(N))
            .getNode();
    replaceNode(N, Node);
  } break;
  /* ABS */
  case ARM::ABS:
  case ARM::t2ABS: {
    outs() << "WARNING: ARM::ABS Not yet implemented!\n";
  } break;
  case ARM::tLDRpci:
  case ARM::LDRcp: {
    outs() << "WARNING: ARM::LDR Not yet implemented!\n";
  } break;
  case ARM::t2SBFX:
  case ARM::SBFX:
  case ARM::t2UBFX:
  case ARM::UBFX: {
    outs() << "WARNING: ARM::UBFX Not yet implemented!\n";
  } break;
  case ARM::t2UMAAL:
  case ARM::UMAAL: {
    outs() << "WARNING: ARM::UMAAL Not yet implemented!\n";
  } break;
  case ARM::t2UMLAL:
  case ARM::UMLAL:
  case ARM::UMLALv5: {
    outs() << "WARNING: ARM::UMLAL Not yet implemented!\n";
  } break;
  case ARM::t2SMLAL:
  case ARM::SMLAL:
  case ARM::SMLALv5: {
    outs() << "WARNING: ARM::SMLAL Not yet implemented!\n";
  } break;
  case ARM::t2SMMLS:
  case ARM::SMMLS: {
    outs() << "WARNING: ARM::SMMLS Not yet implemented!\n";
  } break;
  case ARM::VZIPd8:
  case ARM::VZIPd16:
  case ARM::VZIPq8:
  case ARM::VZIPq16:
  case ARM::VZIPq32: {
    outs() << "WARNING: ARM::VZIP Not yet implemented!\n";
  } break;
  case ARM::VUZPd8:
  case ARM::VUZPd16:
  case ARM::VUZPq8:
  case ARM::VUZPq16:
  case ARM::VUZPq32: {
    outs() << "WARNING: ARM::VUZP Not yet implemented!\n";
  } break;
  case ARM::VTRNd8:
  case ARM::VTRNd16:
  case ARM::VTRNd32:
  case ARM::VTRNq8:
  case ARM::VTRNq16:
  case ARM::VTRNq32: {
    outs() << "WARNING: ARM::VTRN Not yet implemented!\n";
  } break;
    // TODO: Need to add other pattern matching here.
  }
}

void InstSelector::select(SDNode *N) {
  if (!N->isMachineOpcode()) {
    N->setNodeId(-1);
    return; // Already selected.
  }

  selectCode(N);
}
