// This is the exact code from Tindie, modified only slightly for my own project:
// https://www.tindie.com/products/deftaudio/teensy-32-midi-breakout-board-3in-3out-usb/

/* Create a "class compliant " USB to 3 MIDI IN and 3 MIDI OUT interface.
   MIDI receive (6N138 optocoupler) input circuit and series resistor
   outputs need to be connected to Serial1, Serial2 and Serial3.
   You must select MIDIx4 from the "Tools > USB Type" menu
   This example code is in the public domain.
*/


#include <MIDI.h>

// Create the Serial MIDI ports
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);

// A variable to know how long the LED has been turned on
elapsedMillis ledOnMillis;

void setup() {

// set the Recieve(RX) pin number here:
// On my Teensy 3.2, the 
  Serial2.setRX(26);

// Teensy Hardware Serial information can be found here:
// https://www.pjrc.com/teensy/td_uart.html

  Serial.begin(9600);
  //pinMode(13, OUTPUT); // LED pin
  MIDI1.begin(MIDI_CHANNEL_OMNI);
  MIDI2.begin(MIDI_CHANNEL_OMNI);
  MIDI3.begin(MIDI_CHANNEL_OMNI);
  MIDI1.turnThruOff();
  MIDI2.turnThruOff();
  MIDI3.turnThruOff();

    digitalWriteFast(13, HIGH); // LED on
    delay(500);
    digitalWriteFast(13, LOW);
    Serial.println("TRAGIC!");
   
}


void loop() {
  bool activity = false;

  if (MIDI1.read()) {
    // get a MIDI IN1 (Serial) message
    byte type = MIDI1.getType();
    byte channel = MIDI1.getChannel();
    byte data1 = MIDI1.getData1();
    byte data2 = MIDI1.getData2();
    Serial.println(data1);

    // forward the message to USB MIDI virtual cable #0
    if (type != midi::SystemExclusive) {
      // Normal messages, simply give the data to the usbMIDI.send()
      //MIDI.send(type, data1, data2, channel, 0);
      MIDI1.send(type, data1, data2, channel);
      Serial.println("001");
      MIDI2.send(type, data1, data2, channel);
      MIDI3.send(type, data1, data2, channel);      
      
    } else {
      // SysEx messages are special.  The message length is given in data1 & data2
      unsigned int SysExLength = data1 + data2 * 256;
      //MIDI.sendSysEx(SysExLength, MIDI1.getSysExArray(), true, 0);
      MIDI1.sendSysEx(SysExLength, MIDI1.getSysExArray(), true);
      MIDI2.sendSysEx(SysExLength, MIDI1.getSysExArray(), true);
      MIDI3.sendSysEx(SysExLength, MIDI1.getSysExArray(), true);
    }
    activity = true;
  }

  if (MIDI2.read()) {
    // get a MIDI IN2 (Serial) message
    byte type = MIDI2.getType();
    byte channel = MIDI2.getChannel();
    byte data1 = MIDI2.getData1();
    byte data2 = MIDI2.getData2();
Serial.println("002");
    // forward the message to USB MIDI virtual cable #1
    if (type != midi::SystemExclusive) {
      // Normal messages, simply give the data to the usbMIDI.send()
      //MIDI.send(type, data1, data2, channel, 1);
      MIDI1.send(type, data1, data2, channel);
      MIDI2.send(type, data1, data2, channel);
      MIDI3.send(type, data1, data2, channel);  
      
    } else {
      // SysEx messages are special.  The message length is given in data1 & data2
      unsigned int SysExLength = data1 + data2 * 256;
      //MIDI.sendSysEx(SysExLength, MIDI2.getSysExArray(), true, 1);
      MIDI1.sendSysEx(SysExLength, MIDI2.getSysExArray(), true);
      MIDI2.sendSysEx(SysExLength, MIDI2.getSysExArray(), true);
      MIDI3.sendSysEx(SysExLength, MIDI2.getSysExArray(), true);
      
    }
    activity = true;
  }

  if (MIDI3.read()) {
    // get a MIDI IN1 (Serial) message
    byte type = MIDI3.getType();
    byte channel = MIDI3.getChannel();
    byte data1 = MIDI3.getData1();
    byte data2 = MIDI3.getData2();
    Serial.println("003");

    // forward the message to USB MIDI virtual cable #0
    if (type != midi::SystemExclusive) {
      // Normal messages, simply give the data to the usbMIDI.send()
      //MIDI.send(type, data1, data2, channel, 2);
      MIDI1.send(type, data1, data2, channel);
      MIDI2.send(type, data1, data2, channel);
      MIDI3.send(type, data1, data2, channel);  
      
    } else {
      // SysEx messages are special.  The message length is given in data1 & data2
      unsigned int SysExLength = data1 + data2 * 256;
      //MIDI.sendSysEx(SysExLength, MIDI3.getSysExArray(), true, 2);
      MIDI1.sendSysEx(SysExLength, MIDI3.getSysExArray(), true);
      MIDI2.sendSysEx(SysExLength, MIDI3.getSysExArray(), true);
      MIDI3.sendSysEx(SysExLength, MIDI3.getSysExArray(), true);
    }
    activity = true;
  }

//  if (MIDI.read()) {
//    // get the USB MIDI message, defined by these 5 numbers (except SysEX)
//    byte type = MIDI.getType();
//    byte channel = MIDI.getChannel();
//    byte data1 = MIDI.getData1();
//    byte data2 = MIDI.getData2();
//    byte cable = MIDI.getCable();
//
//    // forward this message to 1 of the 3 Serial MIDI OUT ports
//    if (type != MIDI.SystemExclusive) {
//      // Normal messages, first we must convert usbMIDI's type (an ordinary
//      // byte) to the MIDI library's special MidiType.
//      midi::MidiType mtype = (midi::MidiType)type;
//
//      // Then simply give the data to the MIDI library send()
//      switch (cable) {
//        case 0:
//          MIDI1.send(mtype, data1, data2, channel);
//          break;
//        case 1:
//          MIDI2.send(mtype, data1, data2, channel);
//          break;
//        case 2:
//          MIDI3.send(mtype, data1, data2, channel);
//          break;
//      }
//
//    } else {
//      // SysEx messages are special.  The message length is given in data1 & data2
//      unsigned int SysExLength = data1 + data2 * 256;
//      switch (cable) {
//        case 0:
//          MIDI1.sendSysEx(SysExLength, MIDI.getSysExArray(), true);
//          break;
//        case 1:
//          MIDI2.sendSysEx(SysExLength, MIDI.getSysExArray(), true);
//          break;
//        case 2:
//          MIDI3.sendSysEx(SysExLength, MIDI.getSysExArray(), true);
//          break;
//      }
//    }
//    activity = true;
//  }
////
//  // blink the LED when any activity has happened
//  if (activity) {
//    digitalWriteFast(13, HIGH); // LED on
//    ledOnMillis = 0;
//  }
//  if (ledOnMillis > 15) {
//    digitalWriteFast(13, LOW);  // LED off
//  }

}
