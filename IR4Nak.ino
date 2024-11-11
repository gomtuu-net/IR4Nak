// IR control a Nakamichi tapedeck with Arduino, TSOP4838 and Sony CamCorder remote
// URL: https://github.com/Arduino-IRremote/Arduino-IRremote/wiki/Receiving-with-the-IRremote-library
// - drive LED's, later drive FET/transistors
// - test boards: Uno(ok), Nano(??), ESPxx(??)
// - use expander (i2c mcp23008...)
// - add MQTT status/control?

#include <IRremote.h>	// IRremote library v3.3.0

int RECV_PIN = 10;		// IR sensor data-pin
int pulseDly = 230;		// Pulse length in ms
#define pPlay 2		// DIN-pin: 3
#define pRew 3		// DIN-pin: 5
#define pFwd 4		// DIN-pin: 4
#define pStop 5		// DIN-pin: 1
#define pPauze 6	// DIN-pin: 2
#define pRec 7		// DIN-pin: 6 (DIN-pin: 8 -> +12V)
decode_results results;

void setup() {
  Serial.begin(9600);
  Serial.println("IR control for Nakamichi tapedeck with Arduino and TSOP4838...");
  pinMode(pPlay, OUTPUT);
  pinMode(pRew, OUTPUT);
  pinMode(pFwd, OUTPUT);
  pinMode(pStop, OUTPUT);
  pinMode(pPauze, OUTPUT);
  pinMode(pRec, OUTPUT);
//  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);  // Start the receiver, enable feedback LED, take LED feedback pin from the internal boards definition
  IrReceiver.begin(RECV_PIN);
  IrReceiver.enableIRIn();                          // Start the receiver
}

void sendPulse(int pulsePin, int pulseDly) {
  digitalWrite(pulsePin, HIGH); delay(pulseDly); digitalWrite(pulsePin, LOW);
  Serial.println(pulsePin);
}

void loop() {
  if (IrReceiver.decode())	// have we received an IR signal?
  {
    IrReceiver.printIRResultShort(&Serial);		// usefull for IR debugging
    translateIR();
    IrReceiver.resume();		// continue receiving
  }
}

void translateIR() {				// takes action based on IR code received
  switch (IrReceiver.decodedIRData.decodedRawData)
  {
    case 0x39A: Serial.println("Play...");
    	sendPulse(pPlay,pulseDly);
      break;
    case 0x39B: Serial.println("Rewind...");
    	sendPulse(pRew,pulseDly);
      break;
    case 0x39C: Serial.println("Forward...");
    	sendPulse(pFwd,pulseDly);
      break;
    case 0x398: Serial.println("Stop...");
    	sendPulse(pStop,pulseDly);
      break;
    case 0x399: Serial.println("Pauze...");
    	sendPulse(pPauze,pulseDly);
      break;
    case 0x3A3: Serial.println("Slow..."); break;
    case 0x3DA: Serial.println("Display..."); break;
    case 0x5C99: Serial.println("Start/stop...");
//     	sendPulse(pStartStop,pulseDly);
      digitalWrite(pStartStop, HIGH); delay(250); digitalWrite(pStartStop, LOW);
      break;
    case 0x6C9A: Serial.println("T..."); break;
    case 0x6C9B: Serial.println("W..."); break;

    default:
      Serial.println("Other button...");
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
  }
  delay(500);   // Do not get immediate repeat
}

