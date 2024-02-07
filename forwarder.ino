#include <RCSwitch.h>
#include <RH_RF95.h>
#include <SPI.h>


#define RXB6_RX_PIN   5   // RX pin for RXB6 - Adjust this pin as per your wiring
#define RF95_CS      8   // Chip select pin for RF96
#define RF95_RST     4   // Reset pin for RF96
#define RF95_INT     3
#define TRIGGER_PIN1 10  // Trigger pin 1
#define TRIGGER_PIN2 11  // Trigger pin 2

// Instantiate the RF95 (RF96) and RCSwitch objects
RH_RF95 rf95(RF95_CS, RF95_INT);
RCSwitch rcSwitch = RCSwitch();

unsigned long lastReceivedTime = 0;  // Last received time for debounce
long lastReceivedCode = -1;          // Last received code for debounce
const unsigned long debounceTime = 3000;  
bool PIN1_RESET = false;
bool PIN2_RESET = false;
    
void setup() {
    Serial.begin(9600);

    // Initialize RCSwitch for receiving 1527 encoded messages
    rcSwitch.enableReceive(digitalPinToInterrupt(RXB6_RX_PIN)); // Enable receiver on interrupt

    // Manual reset of RF96 to make sure it's initialized
    pinMode(RF95_RST, OUTPUT);
    digitalWrite(RF95_RST, HIGH);
    delay(100);
    digitalWrite(RF95_RST, LOW);
    delay(100);
    digitalWrite(RF95_RST, HIGH);
    delay(100);

    // Initialize RF95 (RF96) module
    if (!rf95.init()) {
        Serial.println("RF96 initialization failed!");
    }

    // Set the frequency for the RF96 module
    if (!rf95.setFrequency(915.0)) {
        Serial.println("Setting RF96 frequency failed!");
    } else {
        Serial.println("RF96 frequency set to 915.0 MHz");
    }

    // Initialize trigger pins
    pinMode(TRIGGER_PIN1, INPUT_PULLUP);
    pinMode(TRIGGER_PIN2, INPUT_PULLUP);
}

void loop() {
    // Check for external triggers
    checkExternalTriggers();

    // Check if RCSwitch has received a message
    if (rcSwitch.available()) {
        long receivedValue = rcSwitch.getReceivedValue();

        // Check if the code is a duplicate and if it's within the debounce time
        if (receivedValue == lastReceivedCode && millis() - lastReceivedTime < debounceTime) {
            rcSwitch.resetAvailable();
            return;
        }

        // Update the last received code and time
        lastReceivedCode = receivedValue;
        lastReceivedTime = millis();

        // Process the received code
        processReceivedCode(receivedValue);
        rcSwitch.resetAvailable();
    }
}

//Checks the two reed switches connected to the board and debounces them
//Reset logic added to stop 

void checkExternalTriggers() {

    if (digitalRead(TRIGGER_PIN1) == HIGH && !PIN1_RESET) {
        //Send code for receiver to process
        processReceivedCode(10000001);
        //Debounce
        delay(300); 
        PIN1_RESET = true;
    }
    if (digitalRead(TRIGGER_PIN1) == LOW && PIN1_RESET) {
        delay(300);
        Serial.println("1 closed");
        PIN1_RESET = false;
    }  
    if (digitalRead(TRIGGER_PIN2) == HIGH && !PIN2_RESET) {
        //Send code for receiver to process
        processReceivedCode(10000002);
        delay(300); 
        PIN2_RESET = true;
    }
    if (digitalRead(TRIGGER_PIN2) == LOW && PIN2_RESET) {
        delay(300);
        Serial.println("2 closed");
        PIN2_RESET = false;
    }

}

void processReceivedCode(long receivedValue) {
    if (receivedValue > 0) {
        Serial.print("Received: ");
        Serial.println(receivedValue, DEC);

        // Prepare a buffer to send the message
        uint8_t buf[sizeof(receivedValue)];
        memcpy(buf, &receivedValue, sizeof(receivedValue));

        // Send the message via RF96
        if (rf95.send(buf, sizeof(buf))) {
            rf95.waitPacketSent();
            Serial.println("Message forwarded via RF96");
            } else {
                Serial.println("Error sending message via RF96");
            }
            } else {
               Serial.println("Received unknown code");
            }
}