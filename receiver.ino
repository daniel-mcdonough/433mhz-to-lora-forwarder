#include <RH_RF95.h>

#define RF95_CS      8   // Chip select pin for RF96
#define RF95_RST     4   // Reset pin for RF96
#define RF95_INT     3   // Interrupt pin for RF96
#define BUZZER_PIN   10  // Buzzer pin
#define LED_PIN      13

// Door codes
const long triggerCodes[] = {10000001, 10000002, 00000000, 00000001, 00000002, 00000003}; // Add your standard codes here
const int numTriggerCodes = sizeof(triggerCodes) / sizeof(triggerCodes[0]);

// Smoke detector codes
const long smokeDetectorCodes[] = {20000000, 20000002}; // Replace with actual smoke detector codes
const int numSmokeDetectorCodes = sizeof(smokeDetectorCodes) / sizeof(smokeDetectorCodes[0]);

RH_RF95 rf95(RF95_CS, RF95_INT);

void setup() {
    pinMode(RF95_RST, OUTPUT);
    digitalWrite(RF95_RST, HIGH);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT); 

    if (!rf95.init()) {
        // If init fails, blink LED rapidly
        while (1) {
            digitalWrite(LED_PIN, HIGH);
            delay(100);
            digitalWrite(LED_PIN, LOW);
            delay(100);
        }
    }
    if (!rf95.setFrequency(915.0)) {
        // If frequency set fails, blink LED slowly
        while (1) {
            digitalWrite(LED_PIN, HIGH);
            delay(1000);
            digitalWrite(LED_PIN, LOW);
            delay(1000);
        }
    }
    // If setup is successful, turn on LED for 2 seconds
    digitalWrite(LED_PIN, HIGH);
    delay(2000);
    digitalWrite(LED_PIN, LOW);
}

void loop() {
    if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (rf95.recv(buf, &len)) {
            if (len == sizeof(long)) {
                long receivedCode;
                memcpy(&receivedCode, buf, len);

                if (processReceivedCode(receivedCode)) {
                    // Turn on LED to indicate code processed
                    digitalWrite(LED_PIN, HIGH);
                    delay(500);
                    digitalWrite(LED_PIN, LOW);
                }
            }
        }
    }
}
//Determine which function to call based on door or smoke detector
bool processReceivedCode(long code) {
    for (int i = 0; i < numSmokeDetectorCodes; i++) {
        if (code == smokeDetectorCodes[i]) {
            smokeDetectorAlert();
            return true;
        }
    }

    for (int i = 0; i < numTriggerCodes; i++) {
        if (code == triggerCodes[i]) {
            beep(3);
            return true;
        }
    }
    return false;
}

// Change the timings and count to your liking
void beep(int numOfBeeps) {
    for (int i = 0; i < numOfBeeps; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(300);
        digitalWrite(BUZZER_PIN, LOW);
        if (i < numOfBeeps - 1) {
            delay(300);
        }
    }
}

void smokeDetectorAlert() {
    for (int i = 0; i < 10; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
        
        for (int i = 0; i < 3; i++) {
            digitalWrite(BUZZER_PIN, HIGH);
            delay(300);
            digitalWrite(BUZZER_PIN, LOW);
            delay(100);
        }
    }
}
