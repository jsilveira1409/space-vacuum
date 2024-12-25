// ======================================================================
// \title  Roomba.hpp
// \author jsilveira
// \brief  hpp file for Roomba component implementation class
// ======================================================================

#ifndef Components_Roomba_HPP
#define Components_Roomba_HPP

#include "Components/Roomba/RoombaComponentAc.hpp"

namespace Components {

  class Roomba : public RoombaComponentBase {
  public:
    enum RoombaSCI {
      WAKEUP = 0,
      START = 128,
      CONTROL = 130,
      SAFE = 131,
      FULL = 132,
      POWER = 133,
      SPOT = 134,
      CLEAN = 135,
      MAX = 136,
      DRIVE = 137,
      MOTORS = 138,
      LEDS = 139,
      SONG = 140,
      PLAY = 141,
      SENSORS = 142,
      DOCK = 143
    };
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct Roomba object
    Roomba(const char* const compName  //!< The component name
    );

    //! Destroy Roomba object
    ~Roomba();

  PRIVATE:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for drvConnected
    //!
    //! Ready signal when driver is connected
    void drvConnected_handler(FwIndexType portNum  //!< The port number
    ) override;

    //! Handler implementation for drvDataIn
    //!
    //! Data received from driver
    void drvDataIn_handler(FwIndexType portNum,  //!< The port number
      Fw::Buffer& recvBuffer,
      const Drv::RecvStatus& recvStatus) override;

    //! Handler implementation for run
    //!
    //! Rate Group Entry Port
    void run_handler(FwIndexType portNum,  //!< The port number
      U32 context           //!< The call order
    ) override;

  PRIVATE:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command WakeUp
    //!
    //! Wakeup Roomba
    void WakeUp_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Control
    //!
    //! Enable User Control of Roomba
    void Control_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Safe
    //!
    //! Safe Mode
    void Safe_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Full
    //!
    //! Enable unrestricted control of Roomba
    void Full_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Power
    //!
    //! Put Roomba to Sleep
    void Power_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Spot
    //!
    //! Spot Cleaning Cycle
    void Spot_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Clean
    //!
    //! Normal Cleaning Cycle
    void Clean_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Max
    //!
    //! Maximum Cleaning Cycle
    void Max_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Dock
    //!
    //! Force Roomba to Dock Seek
    void Dock_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Play
    //!
    //! Play Song on Roomba
    void Play_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;

    //! Handler implementation for command Drive
    //!
    //! Drive Roomba
    void Drive_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq,           //!< The command sequence number
      U16 speed,
      U16 radius) override;

    //! Handler implementation for command Start
    //!
    //! Enable User Start of Roomba
    void Start_cmdHandler(FwOpcodeType opCode,  //!< The opcode
      U32 cmdSeq            //!< The command sequence number
    ) override;
  };

}  // namespace Components

#endif
