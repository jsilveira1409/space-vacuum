// ======================================================================
// \title  Roomba.cpp
// \author jsilveira
// \brief  cpp file for Roomba component implementation class
// ======================================================================

#include "Components/Roomba/Roomba.hpp"
#include "FpConfig.hpp"

namespace Components {

    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    Roomba::Roomba(const char* const compName) : RoombaComponentBase(compName) {}

    Roomba ::~Roomba() {}

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    void Roomba::drvConnected_handler(FwIndexType portNum) {
        // TODO
    }

    void Roomba::drvDataIn_handler(FwIndexType portNum, Fw::Buffer& recvBuffer, const Drv::RecvStatus& recvStatus) {
        // TODO
    }

    void Roomba::run_handler(FwIndexType portNum, U32 context) {
        // TODO
    }

    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    void Roomba::WakeUp_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Control_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::CONTROL;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Safe_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Full_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::FULL;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Power_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Spot_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::SPOT;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Clean_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Max_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Dock_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Play_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Drive_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U16 speed, U16 radius) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Start_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::START;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

}  // namespace Components
