
Forward 433MHz 1527 encoded codes over LoRa.
---


This code listens for 1527 encoded codes from things like door sensors and smoke detectors and forwards it over LoRa to a receiver which beeps depending on the device code received.

[This is part of a home security project.](https://daniel-mcdonough.com/posts/2024-02-09-433-forwarder/)


If you're using 433MHz door/window/smoke detector sensors and want to receive them over a long distance, you can forward them via LoRa

### Materials:

- Boards: 
  - Controllers: Adafruit Feather RF96 M0
    - This can be swapped for an ESP board with an RF96 chip but the pin numbers may differ. This ARM chip doesn't have as large of a library compared to the ESP boards.
    - Antenna: 8.2cm length of wire     
  - 433MHz: RXB6 Receiver
    - Antenna: 17.32cm length of wire 
    - [Other receivers have terrible sensitivity or are packet-based. Do not use them.](https://stef-aap.github.io/RFLink-ESP/433%20MHz%20Transceivers.html)
- Sensors:
  - Generic 433MHz sensor supporting 1527
  - Generic reed switch(optional)
- Cases(3D printed):
   - Found on Thingiverse.
- Buzzer
  - Any 3.3v buzzer large enough to hear across the house.


![RBX6](/img/rxb6.webp)
Use this RBX6 board.

### Wiring

The default wiring is as shown. The optional reed switches are internal pullup but external resistors can be used instead. The 443MHz antenna length is ~17.32cm. The 912MHz antenna length is ~8.2cm.

![Forwarder board](/img/forwarder.webp)

![Receiver board](/img/receiver.webp)

### Flashing 

I used the Arduino IDE with the RCSwitch, SPI, and RadioHead libraries. They can be found in the Library Manager.

The Adafruit Feather M0 needs a board repository added: ` https://adafruit.github.io/arduino-board-index/package_adafruit_index.json` This option can be found in the preferences menu under Additional Boards Manager URLs.

Flashing is standard fare. No special settings needed. You will get a warning that the RC-Switch library may not be compatible if using the M0 board. Ignore it.


### Troubleshooting

- Check the serial output from the forwarder in the serial monitor to make sure it's receiving, processing, and forwarding the codes.

- Make sure the antennas are fairly straight and in the same orientation.

- Activate the sensor from a closer distance(~2 feet).

- Distance the boards from one another.

- Make sure your sensors use 1527 encoding.

### Potential Additions

- Transmitting the codes on 433MHz once received for a transparent repeater system

- Attaching a WiFi board or using another microcontroller to submit codes to MQTT for HomeAssistant integration

- Using LoRaWAN