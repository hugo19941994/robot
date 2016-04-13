/* Arduino Due code to read frame from a LEPTON LWIR core.
    Code will read data from the LEPTON over SPI and transfer this data over UART serial at 250000 Baud rate.

    WIRING
    Using...
    1. a FLIR Lepton Breakout Board from GroupGets (https://store.groupgets.com/#!/p/52239830)
    2. Female to Male jumper wires (https://www.sparkfun.com/products/9385)
    3. an Arduino Due
    - connect CS to pin 10 on the Arduino Due
    - Connect MOSI to GND on the Arduino
    - Connect MISO to pin 1 (MISO) on the Arduino Due SPI header (refer to https://www.arduino.cc/en/Reference/SPI for pin numbers on the SPI header)
    - Connect CLK to pin 3 (SCK) on the Arduino Due SPI header
    - Connect GND to GND on the Arduino
    - Connect VIN to 3.3V on the Arduino
    - Connect SDA to pin 2
    - Connect SCL to pin 3

    Connect the micro USB cable to the PROGRAMMING ATMEGA port on the Due and in the IDE select Arduino Due(Programming Port).

    Makesure the correct Baud rate is selected in the serial monitor.

*/

#include <SPI.h>
#include <stdio.h>
#include <string.h>

#define PACKET_SIZE (164)
#define PACKETS_PER_FRAME 60

uint8_t frame_buffer[PACKET_SIZE * PACKETS_PER_FRAME] = {0};
uint8_t *p;
uint16_t *frame16;

int ledPin = 13;

boolean stringComplete = false;

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);         //I2C
  pinMode(3, OUTPUT);         //I2C
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);

  openSPI();

  //Serial.println("Setup Complete");
}

void loop() {
  p = frame_buffer;

  while (true) {
    serialEvent();
    if (stringComplete == true){
      stringComplete = false;
      readFrame();          //Reads a frame from the Lepton over SPI
      sendLeptonFrame();    //Transfers a frame over UART serial
    }
  }
  closeSPI();
}

void openSPI(void) {
  SPI.begin(10);
  SPI.setClockDivider(10, 8);   //Due 84MHz / 8 ~= 10MHz SPI clk
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
}

void closeSPI(void) {
  SPI.end(10);
}

void readFrame(void) {
  int packetID = 0;
  int resets = 0;

  for (int j = 0 ; j < PACKETS_PER_FRAME; j++) {
    //read 164 byte packet directly into frame buffer

    for (int k = 0; k < (PACKET_SIZE); k++) {
      if (k < (PACKET_SIZE - 1)) { //first 59 packets
        *(p + j * PACKET_SIZE + k) = SPI.transfer(10, 0x00, SPI_CONTINUE); // use SPI_CONTINUE to keep CS low for entire 164 byte packet
      }
      else {                     //last packet
        *(p + j * PACKET_SIZE + k) = SPI.transfer(10, 0x00);
      }
    }

    packetID = *(p + j * PACKET_SIZE + 1);

    if (packetID != j) {  //if discard packet reset j, make j=-1 so 0 on next loop
      j = -1;
      resets += 1;
      delayMicroseconds(1000);
      //750 resets is an arbitrary limit, since there should never be 750 "null" packets between two valid transmissions at the current poll rate
      //Polling faster may exceed this count, and the down period between frames may then be flagged as a loss of sync
      if (resets == 750) {
        Serial.print("Sync lost at packet ");
        Serial.println(packetID);
        closeSPI();
        delay(750);
        openSPI();
      }
    }
  }
}

//swaps the endianess of the frame data. Lepton data is big-endian
void endianSwap(void) {
  // frame consists of 60 packets, each 82*2bytes long
  for (int row = 0; row < (PACKETS_PER_FRAME); row++) {
    for (int col = 0; col < (PACKET_SIZE / 2); col++) {
      int temp1 = *(frame16 + row * (PACKET_SIZE / 2) + col) << 8;
      int temp2 = *(frame16 + row * (PACKET_SIZE / 2) + col) >> 8;
      *(frame16 + row * (PACKET_SIZE / 2) + col) = temp1 | temp2;
    }
  }
}

//Sends a lepton frame (16 bit endian swapped) over UART serial (without packet ID and CRC).
void sendLeptonFrame(void) {
  //Serial.print(" S ");
  frame16 = (uint16_t *)&p + 2; //convert array of 8-bit pixel values to 16-bit pixel values
  endianSwap();

  // frame consists of 60 packets, each 80*2bytes long
  for (int row = 0; row < (PACKETS_PER_FRAME); row++) {
    for (int col = 2; col < (PACKET_SIZE / 2); col++) {
      Serial.print(*(frame16 + row * (PACKET_SIZE / 2) + col));
      Serial.print(" ");
    }
    //Serial.println("");
  }
  //Serial.print(" F ");
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    //st += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == 'A') {
      stringComplete = true;
    }
  }
}
