module Components {
    @ Component for F Prime FSW framework.
    active component Roomba {

        @ Wakeup Roomba
        async command WakeUp opcode 0
        
        @ Enable User Control of Roomba
        async command Control opcode 1

        @ Safe Mode
        async command Safe opcode 2

        @ Enable unrestricted control of Roomba
        async command Full opcode 3

        @ Put Roomba to Sleep
        async command Power opcode 4

        @ Spot Cleaning Cycle
        async command Spot opcode 5

        @ Normal Cleaning Cycle
        async command Clean opcode 6
        
        @ Maximum Cleaning Cycle
        async command Max opcode 7
        
        @ Force Roomba to Dock Seek
        async command Dock opcode 8

        @ Play Song on Roomba
        async command Play opcode 9

        @ Drive Roomba
        async command Drive(
            speed  : I16,
            radius : I16
        ) opcode 10

        @ Enable User Start of Roomba
        async command Start opcode 11

        @ Roomba Drive Forward
        async command Forward opcode 12
        
        @ Roomba Drive Backward
        async command Backward opcode 13

        @ Roomba Right Turn
        async command  Right opcode 14

        @ Roomba Left Turn
        async command Left opcode 15

        @ Roomba Stop
        async command Stop opcode 16

        @ Open/Close Lid, dispense food
        async command Dispense(
            $state:bool
            ) opcode 17


        
        
        @ Roomba's Battery Voltage in microvolts
        telemetry BumpWheeldrops: U8
        
        @ Roomba's Wall Sensor
        telemetry Wall: U8
        
        @ Roomba's Cliff Left Sensor
        telemetry CliffLeft: U8

        @ Roomba's Cliff Front Left Sensor
        telemetry CliftFrontLeft: U8

        @ Roomba's Cliff Front Right Sensor
        telemetry CliffFrontRight: U8

        @ Roomba's Cliff Lift Sensor
        telemetry CliffRight: U8

        @ Roomba's Virtual Wall Sensor
        telemetry VirtualWall: U8

        @ Roomba's Motor Overcurrents
        telemetry MotorOvercurrents: U8

        @ Roomba's Left Dirt Detector
        telemetry DirtLeft: U8

        @ Roomba's Right Dirt Detector
        telemetry DirtRight: U8

        @ Roomba's Remote OpCodes
        telemetry RemoteOpcode: U8

        @ Roomba's Buttons States
        telemetry Buttons: U8

        @ Roomba's Distance Travelled in millimeters
        telemetry Distance: I16

        @ Roomba's Angle
        telemetry Angle: I16

        @ Roomba's Charging State
        telemetry ChargingState: U8

        @ Roomba's Battery Voltage in microvolts
        telemetry Voltage: U16

        @ Roomba's Battery Current in microampers
        telemetry Current: I16

        @ Roomba's Battery Temperature in degrees Celsius
        telemetry Temperature: I8

        @ Roomba's Battery Charge in milliampers-hour
        telemetry Charge: U16

        @ Roomba's Battery Capacity in milliampers-hour
        telemetry Capacity: U16


        @ Rate Group Entry Port
        sync input port run: Svc.Sched

        
        @ Ready signal when driver is connected
        sync input port drvConnected: Drv.ByteStreamReady

        @ Data received from driver
        sync input port drvDataIn: Drv.ByteStreamRecv

        @ Data going to the underlying driver
        output port drvDataOut: Drv.ByteStreamSend

        @ Ready signal when driver is connected
        sync input port dispenserDrvConnected: Drv.ByteStreamReady

        @ Data received from driver
        sync input port dispenserDrvDataIn: Drv.ByteStreamRecv

        @ Data going to the underlying driver
        output port dispenserDrvDataOut: Drv.ByteStreamSend

        output port allocate: Fw.BufferGet

        output port deallocate: Fw.BufferSend
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}