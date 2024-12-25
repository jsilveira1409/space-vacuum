#include <WiFi101.h>

#define TIMEOUT_MS 100 // Timeout in milliseconds
#define MAX_BUFFER_SIZE 256

enum CMD {
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

const int wakeup_pin = 2;
const uint8_t sevenNations[] = {
    140, 0, 16, // Song number 0, 16 notes
    64, 37, 64, 12, 67, 18, 64, 18, 59, 18, 57, 50, 56, 50, // Melody part 1
    64, 37, 64, 12, 67, 18, 64, 18, 59, 18, 57, 20, 59, 20, 57, 20, 56, 38 // Melody part 2
};
const char* ssid = "Jojo";                 
const char* password = "legrandinquisiteur";
const char* serverIP = "192.168.1.247";    // Replace with the server IP address
const uint16_t serverPort = 50054;         
uint8_t buffer[MAX_BUFFER_SIZE];
size_t bufferIndex = 0;
unsigned long lastReceivedTime = 0;
WiFiClient client;                        

void setup() {
    pinMode(wakeup_pin, OUTPUT);
    Serial.begin(9600);
    Serial1.begin(19200);
    wakeup();
    delay(100);
    Serial1.write(CMD::CONTROL);
    delay(100);
    Serial1.write(CMD::FULL);
    delay(100);
    Serial1.write(CMD::CLEAN);
    delay(100);
    //play();
    
    /*    
    // Initialize Serial
    Serial.println("Initializing...");

    // Connect to WiFi
    Serial.print("Connecting to WiFi...");
    while (WiFi.begin(ssid, password) != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);  // Retry every second
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Connect to the server
    Serial.print("Connecting to server...");
    unsigned long startAttemptTime = millis();
    const unsigned long timeout = 15000;  // 15 seconds timeout

    while (!client.connect(serverIP, serverPort)) {
        Serial.print(".");
        delay(1000);
        if (millis() - startAttemptTime > timeout) {
            Serial.println("\nFailed to connect to server. Restarting...");
        }
    }

    Serial.println("\nConnected to server!");
    */
}


int swapEndian(int value) {
    return ((value >> 24) & 0xFF) |       // Move byte 3 to byte 0
           ((value >> 8) & 0xFF00) |      // Move byte 2 to byte 1
           ((value << 8) & 0xFF0000) |    // Move byte 1 to byte 2
           ((value << 24) & 0xFF000000);  // Move byte 0 to byte 3
}

void loop() {
    
    if (client.connected()) {
        while (client.available()) {
            // Read byte-by-byte into the buffer
            if (bufferIndex < MAX_BUFFER_SIZE) {
                buffer[bufferIndex++] = client.read();
                lastReceivedTime = millis(); // Update the timestamp of the last received byte
            } else {
                // Handle buffer overflow (discard the message or clear the buffer)
                bufferIndex = 0; 
                break;
            }
        }

        // Process the command if timeout occurs
        if (millis() - lastReceivedTime >= TIMEOUT_MS && bufferIndex > 0) {
            uint8_t response = processCommand(buffer, bufferIndex);

            // Send response back to the server
            client.write(response);
            
            // Reset the buffer after processing
            bufferIndex = 0;
        }
    } else {
        // Attempt to reconnect to the server
        while (!client.connect(serverIP, serverPort)) {
            delay(1000);
        }
    }
    delay(100);
}

int processCommand(uint8_t* buffer, size_t length) {
    if (length == 0) {
        return -1; // No data to process
    }

    // Assume the first byte is the command
    uint8_t command = buffer[0];
    Serial.println(command);

    switch (command) {
        case CMD::WAKEUP:
            Serial.println("Waking Up");
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
            if (length >= 3) { // DRIVE requires additional parameters
                int16_t velocity = (buffer[1] << 8) | buffer[2]; // Combine two bytes for velocity
                int16_t radius = (buffer[3] << 8) | buffer[4];   // Combine two bytes for radius
                //drive(velocity, radius);
                return CMD::DRIVE;
            } else {
                return -1; // Invalid DRIVE command length
            }

        case CMD::MOTORS:
            if (length >= 2) { // MOTORS command with one parameter
                uint8_t motorBits = buffer[1];
                //motors(motorBits);
                return CMD::MOTORS;
            } else {
                return -1; // Invalid MOTORS command length
            }
            
        case CMD::LEDS:
            if (length >= 4) { // LED command with three parameters
                uint8_t ledBits = buffer[1];
                uint8_t powerColor = buffer[2];
                uint8_t powerIntensity = buffer[3];
                //leds(ledBits, powerColor, powerIntensity);
                return CMD::LEDS;
            } else {
                return -1; // Invalid LED command length
            }
            
        case CMD::SONG:
            return 200; 
            
        case CMD::PLAY:
            Serial.println("Playing");
            play();
            return CMD::PLAY; 
            
        case CMD::SENSORS:
            Serial.println("Getting Sensor Data");
            sensors();
            return CMD::SENSORS;             
            
        default:
            return -1; // Unknown command
    }
}


void sensors() {
    const uint8_t sensorPack = 0;       // The sensor packet request
    const uint8_t expectedDataSize = 27; // Expected size of the sensor data (adjust as per your needs)
    uint8_t data[expectedDataSize + 1];  // +1 for the CMD::SENSORS byte
    uint32_t startTime;                 // To track time for timeout
    uint8_t index = 1;                  // Start after the CMD byte

    // Send sensor command
    data[0] = CMD::SENSORS;
    Serial1.write(CMD::SENSORS);
    Serial1.write(sensorPack);

    startTime = millis();

    // Wait for the expected data or timeout
    while (index <= expectedDataSize) {
        if (Serial1.available() > 0) {
            data[index] = Serial1.read();
            index++;
        }

        // Break out if no data arrives within 2 seconds
        if (millis() - startTime > 2000) {
            break;
        }
    }

    // Send the collected data to the server
    client.write(data, index); // Send only the valid data bytes
}
void wakeup() {
    pinMode(wakeup_pin, OUTPUT);

    digitalWrite(wakeup_pin, LOW);
    delay(100); 
    digitalWrite(wakeup_pin, HIGH);
    delay(2000); 

    // Pulse the Device Detect line low three times
    for (int i = 0; i < 3; i++) {
        digitalWrite(wakeup_pin, LOW);  // Set pin LOW
        delay(250);                    // Keep LOW for 100ms (adjustable between 50-500ms)
        digitalWrite(wakeup_pin, HIGH); // Set pin HIGH
        delay(250);                    // Wait 100ms before the next pulse
    }
}




void play() {
    for (uint8_t i = 0; i < sizeof(sevenNations); i++) {
        Serial1.write(sevenNations[i]);
    }
    delay(100);
    uint8_t playCommand[] = {141, 0}; // Play song number 0
    Serial1.write(playCommand, sizeof(playCommand));
}
