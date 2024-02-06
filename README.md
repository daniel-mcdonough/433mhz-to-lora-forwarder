
Forward 433MHz 1527 encoded codes over LoRa
---


This code listens for 1527 encoded codes from things like door sensors and smoke detectors and forwards it over LoRa to a receiver which beeps depending on the device code received.

This is part of a home security project.


If you're using 433MHz door/window/smoke detector sensors and want to receive them over a long distance, you can forward them via LoRa

### Materials:

Boards: 
- Controllers: Adafruit Feather RF96 M0
  - This can be swapped for an esp board with an RF96 chip but the pins numbers may differ. This ARM chip doesn't have as large of a library compared to the ESP boards.
- 433MHz: RXB6 Receiver
  - Other receivers have terrible sensitivity or are packet-based. Do not use them.
- Sensors:
  - Generic 433MHz sensor supporting 1527
  - Generic reed switch(optional)
- Cases(3D printed):
   - Found on Thingiverse.
- Buzzer
  - Any 3.3v buzzer large enough to hear across the house.


### Flashing 

I used the Arduino IDE with the RCSwitch, SPI, and RadioHead libraries. They can be found in the Library Manager.

The Adafruit Feather M0 needs a board repository added: ` https://adafruit.github.io/arduino-board-index/package_adafruit_index.json` This option can be found in the preferences menu under Additional Boards Manager URLs.

Flashing is standard fare. No special settings needed. You will get a warning that the RadioHead library may not be compatible if using the M0 board. Ignore it.

### Potential Additions

#### Transmitting the codes once received for a transparent repeater system

#### Attaching a WiFi board or using another microcontroller to submit codes to MQTT for HomeAssistant integration