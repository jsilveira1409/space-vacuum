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
/*         U8* data = recvBuffer.getData();
        U32 data_size = recvBuffer.getSize();

        std::cout << "[Roomba] Received " << (unsigned)data_size << " bytes" << std::endl;
        for (U32 i = 0; i < data_size; ++i)
        {
            std::cout << (signed)data[i] << " ";
        }
        std::cout << std::endl;
        if (data[0] == RoombaSCI::SENSORS) {
            parseSensors(data);
        }*/
        this->deallocate_out(0, recvBuffer);
    }

    void Roomba::run_handler(FwIndexType portNum, U32 context) {
        
        /* Fw::Buffer buff = this->allocate_out(0, 2);
        U16 data = static_cast<U16>(RoombaSCI::SENSORS << 8 | 0);
        buff.getSerializeRepr().serialize(data);
        this->drvDataOut_out(0, buff);
        std::cout << "[Roomba] Requested Sensor Data" << std::endl; */
    }

    void Roomba ::dispenserDrvConnected_handler(FwIndexType portNum)
    {
        // TODO
    }

    void Roomba ::dispenserDrvDataIn_handler(FwIndexType portNum,Fw::Buffer& recvBuffer,const Drv::RecvStatus& recvStatus)
    {
        // TODO
    }


    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    void Roomba::WakeUp_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::WAKEUP;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
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
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::SAFE;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
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
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::POWER;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
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
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::CLEAN;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Max_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::MAX;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Dock_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::DOCK;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Play_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        // TODO
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba::Drive_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, I16 speed, I16 radius) {
        Fw::Buffer buff = this->allocate_out(0, 5);
        U8 data = RoombaSCI::DRIVE;

        buff.getSerializeRepr().serialize(data);
        buff.getSerializeRepr().serialize(speed);
        buff.getSerializeRepr().serialize(radius);

        U8* buff_data = buff.getData();
        U32 data_size = buff.getSize();

        std::cout << "[Roomba] Drive " << (unsigned)data_size << " bytes" << std::endl;
        for (U32 i = 0; i < data_size; ++i)
        {
            std::cout << (signed)buff_data[i] << " ";
        }
        std::cout << std::endl;

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }



    void Roomba::Start_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data = RoombaSCI::START;

        buff.getSerializeRepr().serialize(data);

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba ::Forward_cmdHandler(FwOpcodeType opCode,U32 cmdSeq)
    {
        Fw::Buffer buff = this->allocate_out(0, 5);
        U8 data = RoombaSCI::DRIVE;

        I16 speed = 200;
        I16 radius = 32768;

        buff.getSerializeRepr().serialize(data);
        buff.getSerializeRepr().serialize(speed);
        buff.getSerializeRepr().serialize(radius);

        U8* buff_data = buff.getData();
        U32 data_size = buff.getSize();

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba ::Backward_cmdHandler(FwOpcodeType opCode,U32 cmdSeq)
    {
        Fw::Buffer buff = this->allocate_out(0, 5);
        U8 data = RoombaSCI::DRIVE;

        I16 speed = -200;
        I16 radius = 0;

        buff.getSerializeRepr().serialize(data);
        buff.getSerializeRepr().serialize(speed);
        buff.getSerializeRepr().serialize(radius);

        U8* buff_data = buff.getData();
        U32 data_size = buff.getSize();

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba ::Right_cmdHandler(FwOpcodeType opCode,U32 cmdSeq)
    {
        Fw::Buffer buff = this->allocate_out(0, 5);
        U8 data = RoombaSCI::DRIVE;

        I16 speed = 100;
        I16 radius = -1;

        buff.getSerializeRepr().serialize(data);
        buff.getSerializeRepr().serialize(speed);
        buff.getSerializeRepr().serialize(radius);

        U8* buff_data = buff.getData();
        U32 data_size = buff.getSize();

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba ::Left_cmdHandler(FwOpcodeType opCode,U32 cmdSeq)
    {
        Fw::Buffer buff = this->allocate_out(0, 5);
        U8 data = RoombaSCI::DRIVE;

        I16 speed = 100;
        I16 radius = 1;

        buff.getSerializeRepr().serialize(data);
        buff.getSerializeRepr().serialize(speed);
        buff.getSerializeRepr().serialize(radius);

        U8* buff_data = buff.getData();
        U32 data_size = buff.getSize();

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba ::Stop_cmdHandler(FwOpcodeType opCode,U32 cmdSeq)
    {
        Fw::Buffer buff = this->allocate_out(0, 5);
        U8 data = RoombaSCI::DRIVE;

        I16 speed = 0;
        I16 radius = 0;

        buff.getSerializeRepr().serialize(data);
        buff.getSerializeRepr().serialize(speed);
        buff.getSerializeRepr().serialize(radius);

        U8* buff_data = buff.getData();
        U32 data_size = buff.getSize();

        this->drvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }

    void Roomba ::Dispense_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, bool state)
    {
        Fw::Buffer buff = this->allocate_out(0, 1);
        U8 data;
        if (state)
        {
            data = DispenserCmd::OPEN;
        }
        else
        {
            data = DispenserCmd::CLOSE;
        }
        
        buff.getSerializeRepr().serialize(data);

        this->dispenserDrvDataOut_out(0, buff);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
    }


    
    
    // Helpers

    void Roomba::parseSensors(const U8* data) {

        this->tlmWrite_BumpWheeldrops(data[1]);
        this->tlmWrite_Wall(data[2]);
        this->tlmWrite_CliffLeft(data[3]);
        this->tlmWrite_CliftFrontLeft(data[4]);
        this->tlmWrite_CliffFrontRight(data[5]);
        this->tlmWrite_CliffRight(data[6]);
        this->tlmWrite_VirtualWall(data[7]);
        this->tlmWrite_MotorOvercurrents(data[8]);
        this->tlmWrite_DirtLeft(data[9]);
        this->tlmWrite_DirtRight(data[10]);
        this->tlmWrite_RemoteOpcode(data[11]);
        this->tlmWrite_Buttons(data[12]);
        this->tlmWrite_Distance(static_cast<I16>(data[13] << 8 | data[14]));
        this->tlmWrite_Angle(static_cast<I16>(data[15] << 8 | data[16]));
        this->tlmWrite_ChargingState(data[17]);
        this->tlmWrite_Voltage(static_cast<U16>(data[18] << 8 | data[19]));
        this->tlmWrite_Current(static_cast<I16>(data[20] << 8 | data[21]));
        this->tlmWrite_Temperature(static_cast<I8>(data[22]));
        this->tlmWrite_Charge(static_cast<U16>(data[23] << 8 | data[24]));
        this->tlmWrite_Capacity(static_cast<U16>(data[25] << 8 | data[26]));
    }

}  // namespace Components
