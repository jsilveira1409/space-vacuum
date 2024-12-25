#include <WiFi101.h>

enum CMD{
  WAKEUP    = 0,
  CONTROL   = 130,
  SAFE      = 131,
  FULL      = 132,
  POWER     = 133,
  SPOT      = 134,
  CLEAN     = 135,
  MAX       = 136,
  DRIVE     = 137,
  MOTORS    = 138,
  LEDS      = 139,
  SONG      = 140,
  PLAY      = 141,
  SENSORS   = 142,
  DOCK      = 143
  
};

const int wakeup_pin = 6;  
const uint8_t sevenNations[] = {
    140, 0, 16, // Song number 0, 16 notes
    64, 37, 64, 12, 67, 18, 64, 18, 59, 18, 57, 50, 56, 50, // Melody part 1
    64, 37, 64, 12, 67, 18, 64, 18, 59, 18, 57, 20, 59, 20, 57, 20, 56, 38 // Melody part 2
};
const char* ssid = "Jojo";       
const char* password = "legrandinquisiteur";
const uint16_t serverPort = 50054; 
WiFiServer server(serverPort);     
WiFiClient client;              

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    while (!Serial) {}

    Serial.println("Connecting to WiFi...");
    int status = WiFi.begin(ssid, password);
    while (status != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
        status = WiFi.status();
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    server.begin();
    Serial.print("Server started on port: ");
    Serial.println(serverPort);
}

int swapEndian(int value) {
    return ((value >> 24) & 0xFF) |       // Move byte 3 to byte 0
           ((value >> 8) & 0xFF00) |      // Move byte 2 to byte 1
           ((value << 8) & 0xFF0000) |    // Move byte 1 to byte 2
           ((value << 24) & 0xFF000000);  // Move byte 0 to byte 3
}

void loop() {
    if (!client || !client.connected()) {
        client = server.available(); 
        if (client) {
            Serial.println("New client connected!");
        }
    }

    if (client && client.connected() && client.available() >= sizeof(uint8_t)) {
        uint8_t command = 0;
        client.read(&command, sizeof(uint8_t)); 

        Serial.print("Received command: ");
        Serial.println(command);

        uint8_t response = processCommand(command);

        Serial.print("Response: ");
        Serial.println(response);
    }

    if (client && !client.connected()) {
        Serial.println("Client disconnected.");
        client.stop();
    }    
}


int processCommand(int command) {
    switch (command) {
        case CMD::WAKEUP:
            wakeup();
            return CMD::WAKEUP; 
            
        case CMD::CONTROL:
        case CMD::SAFE:
        case CMD::FULL:
        case CMD::POWER:
        case CMD::SPOT:
        case CMD::CLEAN:
        case CMD::MAX:
        case CMD::DOCK:            
            Serial1.write(command);
            return command;
        
        case CMD::DRIVE:
            // TODO read data for speed and radius 
            //drive();
            return CMD::DRIVE; 
            
        case CMD::MOTORS:
            return 200; 
            
        case CMD::LEDS:
            return 200; 
            
        case CMD::SONG:
            return 200; 
            
        case CMD::PLAY:
            play();
            return CMD::PLAY; 
            
        case CMD::SENSORS:
            sensors();
            return CMD::SENSORS;             
            
        default:
            return -1; // Unknown command
    }
}


void sensors(){
  uint8_t sensor_pack = 0;
  uint8_t index = 1;
  uint8_t data[27];            
  
  //client.read(&sensor_pack, sizeof(uint8_t)); 
  
  Serial1.write(CMD::SENSORS);
  Serial1.write(sensor_pack);
  
  data[0] = CMD::SENSORS;
  
  while (Serial1.available() > 0){
    data[index] = Serial1.read();
    index = index + 1;
    delay(10);
  }
  client.write(data, sizeof(data));
  Serial.println("Sending data to client");
}

void wakeup() {
    digitalWrite(wakeup_pin, LOW);
    delay(100);
    digitalWrite(wakeup_pin, HIGH);
}

void play() {
    for (uint8_t i = 0; i < sizeof(sevenNations); i++) {
        Serial1.write(sevenNations[i]);
    }
    delay(100);
    uint8_t playCommand[] = {141, 0}; // Play song number 0
    Serial1.write(playCommand, sizeof(playCommand));
}

void drive(int16_t speed, int16_t radius) {
    Serial1.write(CMD::DRIVE); 
    Serial1.write((speed >> 8) & 0xFF); // Speed high byte
    Serial1.write(speed & 0xFF);        // Speed low byte
    Serial1.write((radius >> 8) & 0xFF); // Radius high byte
    Serial1.write(radius & 0xFF);        // Radius low byte
}

int getCliffLeftSensor() {
    sendSensorRequest(9); // Packet code 9: Cliff left sensor
    return readSensorResponse();
}

int getCliffFrontLeftSensor() {
    sendSensorRequest(10); // Packet code 10: Cliff front-left sensor
    return readSensorResponse();
}

int getCliffFrontRightSensor() {
    sendSensorRequest(11); // Packet code 11: Cliff front-right sensor
    return readSensorResponse();
}

int getCliffRightSensor() {
    sendSensorRequest(12); // Packet code 12: Cliff right sensor
    return readSensorResponse();
}

int getWallSensor() {
    sendSensorRequest(8); // Wall sensor packet code
    return readSensorResponse();
}

void sendSensorRequest(uint8_t packetCode) {
    uint8_t command[] = {142, packetCode};
    Serial1.write(command, sizeof(command));
    delay(50);
}

int readSensorResponse() {
    if (Serial1.available()) {
        return Serial1.read();
    }
    return -1;
}

int read16BitSensorResponse() {
    if (Serial1.available() >= 2) {
        int highByte = Serial1.read();
        int lowByte = Serial1.read();
        return (highByte << 8) | lowByte;
    }
    return -1;
}
