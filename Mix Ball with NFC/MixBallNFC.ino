#include<Wire.h>
#include<SPI.h>
#include<Adafruit_PN532.h>
#include <AccelStepper.h>

// Setup NFC
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

const unsigned int MAX_MESSAGE_LENGTH = 12;
long bola;
long boladetect;
int number;

//const int DELAY_BETWEEN_CARDS = 500;
//long timeLastCardRead = 0;
//boolean readerDisabled = false;
int irqCurr;
int irqPrev;

int IRSensor = 9; // connect IR sensor module to Arduino pin D9
int sensorStatus;

const int BLOWER_PIN = 7;
int ledState;

// Setup pin connection and Step motor
const int dirPin = 5;
const int stepPin = 6;
#define motorInterfaceType 1
const int stepsPerRevolution = 200;
const int stepsPerRevolution1 = 210;

const int miliperStep = 500;

// Define a stepper and the pins it will use
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

long int t1;
long int t2;
long int timetaken = 0;

void(* resetFunc) (void) = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.println("System Begin!");

  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(1000);

  // LED Pin
  pinMode(8, OUTPUT);           // LED NFC Read
  pinMode(13, OUTPUT);          // LED NFC Ready
  digitalWrite(13, LOW);
  digitalWrite(8, LOW);

  pinMode(BLOWER_PIN, OUTPUT);
  digitalWrite(BLOWER_PIN, HIGH);

  // Pin Stepper
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(IRSensor, INPUT); // IR Sensor pin INPUT
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.println("Board tidak ditemukan");
    digitalWrite(13, LOW);
    while (1); // halt
  }
  //Turn On LED when scanner board detected
  digitalWrite(13, HIGH);
  Serial.print("Chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
  Serial.println("NFC Ready");

  int sensorStatus = digitalRead(IRSensor);
  //  Serial.print("Sensor "); Serial.println(sensorStatus);
  (sensorStatus == 0) ? Serial.println("Ball Exist") : Serial.println("Ball Empty");
  // configure board to read RFID tags
  nfc.SAMConfig();

}
void loop(void) {
  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else
    {
      //Add null character to string
      message[message_pos] = '\0';

      Serial.print("Receiving Data ");
      Serial.println(message);

      //Print the message (or do other things)
      number = atoi(message);


      if (number == 0) {
        bola = 1677721600;
      } else if (number == 1) {
        bola = 16777216;
      } else if (number == 2) {
        bola = 33554432;
      } else if (number == 3) {
        bola = 50331648;
      } else if (number == 4) {
        bola = 67108864;
      } else if (number == 5) {
        bola = 83886080;
      } else if (number == 6) {
        bola = 100663296;
      } else if (number == 7) {
        bola = 117440512;
      } else if (number == 8) {
        bola = 134217728;
      } else if (number == 9) {
        bola = 150994944;
      } else if (number == 11) {
        bola = 11;
      } else if (number == 12) {
        bola = 12;
      }
      Serial.print("Typed Ball number ");
      Serial.println(number);

      //Reset for the next message
      message_pos = 0;

      if (bola != 0 || bola != 11 || bola != 12) {
        readiness(bola);

      } else if (bola == 11) {
        callStepper(bola);
        Serial.print("RESET");
      }  else if (bola == 12) {
        runMesin();
      }
    }
  }
}

void readiness(int bola) {
  sensorStatus = digitalRead(IRSensor);
  (sensorStatus == 0) ? Serial.println("Ball Exist") : Serial.println("Ball Empty");

  while (sensorStatus == 0)
  {
    callStepper(bola);
    delay(1000);
    sensorStatus = digitalRead(IRSensor);
    (sensorStatus == 0) ? Serial.println("Ball Exist") : Serial.println("Ball Empty");
    if (sensorStatus == 1) {
      digitalWrite(BLOWER_PIN, LOW);
      t1 = millis();
      runMesin();
    }
  }

  if (sensorStatus == 1) {
    callStepper(bola);
    digitalWrite(BLOWER_PIN, LOW);
    t1 = millis();
    runMesin();
  }
}

void runMesin() {

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {

    t2 = millis();
    timetaken = t2 - t1;
    Serial.print("Time Elapsed: "); Serial.print(timetaken / 1000); Serial.println(" seconds");

    if (uidLength == 4) {
      uint32_t cardid = uid[0];
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];
      cardid <<= 8;
      cardid |= uid[3];

      digitalWrite(8, HIGH);
      //      Serial.println(cardid);

      if ( cardid == 1677721600) {
        //        Serial.println("Bola 0");
        boladetect = 0;
      }
      if ( cardid == 16777216) {
        //        Serial.println("Bola 1");
        boladetect = 1;
      }
      if ( cardid == 33554432) {
        //        Serial.println("Bola 2");
        boladetect = 2;
      }
      if ( cardid == 50331648) {
        //        Serial.println("Bola 3");
        boladetect = 3;
      }
      if ( cardid == 67108864) {
        //        Serial.println("Bola 4");
        boladetect = 4;
      }
      if ( cardid == 83886080) {
        //        Serial.println("Bola 5");
        boladetect = 5;
      }
      if ( cardid == 100663296) {
        //        Serial.println("Bola 6");
        boladetect = 6;
      }
      if ( cardid == 117440512) {
        //        Serial.println("Bola 7");
        boladetect = 7;
      }
      if ( cardid == 134217728) {
        //        Serial.println("Bola 8");
        boladetect = 8;
      }
      if ( cardid == 150994944) {
        //        Serial.println("Bola 9");
        boladetect = 9;
      }

      // Blower off if cardid overtime
      if (timetaken >= 120000) {
        if (cardid != 1) {
          Serial.print("Detected Ball: ");
          Serial.print(boladetect);
          Serial.print(" Waiting for ID ");
          Serial.println(number);
          callStepper(boladetect);
          delay(1000);
          sensorStatus = digitalRead(IRSensor);
          //          Serial.println(sensorStatus);
          while (sensorStatus == 1) {
            sensorStatus = digitalRead(IRSensor);
            (sensorStatus == 0) ? Serial.println("Ball Exist") : Serial.println("Ball Empty");
            if (sensorStatus == 0) {
              OffBlower();
            }
            runMesin();
          }
          if (sensorStatus == 0) {
            OffBlower();
          }
        }
      }

      sensorStatus = digitalRead(IRSensor);
      (sensorStatus == 0) ? Serial.println("Ball Exist") : Serial.println("Ball Empty");
      if (sensorStatus == 0) {
        OffBlower();
      }

      // Blower off if cardid detected
      if (cardid == bola) {
        Serial.print("Detected Ball: ");
        Serial.print(boladetect);
        Serial.print(" Waiting for ID ");
        Serial.println(number);
        callStepper(boladetect);
        delay(1000);
        sensorStatus = digitalRead(IRSensor);
        //        Serial.println(sensorStatus);
        while (sensorStatus == 1) {
          sensorStatus = digitalRead(IRSensor);
          (sensorStatus == 0) ? Serial.println("Ball Exist") : Serial.println("Ball Empty");
          if (sensorStatus == 0) {
            OffBlower();
          }
          runMesin();
        }

        if (sensorStatus == 0) {
          OffBlower();
        }

      } else {
        delay(1000);
        digitalWrite(8, LOW);
        Serial.print("Detected Ball: ");
        Serial.print(boladetect);
        Serial.print(" Waiting for ID ");
        Serial.println(number);
        runMesin();
      }
    }
  }
}

void OffBlower() {
  Serial.println("2");
  delay(500);
  Serial.println("1");
  delay(500);
  if (ledState == LOW) {
    ledState = HIGH;
    digitalWrite(BLOWER_PIN, ledState);
  } else {
    ledState = LOW;
    digitalWrite(BLOWER_PIN, ledState);
  }
  Serial.println("Done");
  delay(1000);
  resetFunc();
}

void callStepper(int bola) {
  stepperRun(bola);
  //  runStepper(bola);
}

void stepperRun(int s) {
  int delayint = 250;
  Serial.println("Buka Gate");
  // Set motor direction clockwise
  digitalWrite(dirPin, LOW);
  // Spin motor
  for (int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(miliperStep);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(miliperStep);
  }

  delay(delayint); // Wait a second

  // Set motor direction counterclockwise
  Serial.println("Tutup Gate");
  digitalWrite(dirPin, HIGH);

  // Spin motor
  for (int x = 0; x < stepsPerRevolution1; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(miliperStep);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(miliperStep);
  }
  //  delay(1000);

  if (s == 11 || s == 12) {
    delay(1000);
    resetFunc();
  }

}

void runStepper(int bola) {
  //  stepper.setCurrentPosition(0);
  while (stepper.currentPosition() != -50)
  {
    stepper.setSpeed(-800);
    stepper.runSpeed();
  }
  delay(500);
  //  stepper.setCurrentPosition(-2);
  while (stepper.currentPosition() != 0)
  {
    stepper.setSpeed(800);
    stepper.runSpeed();
  }

  if (bola == 11 || bola == 12) {
    delay(1000);
    resetFunc();
  }
}
