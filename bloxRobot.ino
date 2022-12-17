#include <SPI.h>

#include "TinyGPS++.h"

#include <NewPing.h>

#include <SoftwareSerial.h>

#include <Servo.h>

#include <SD.h>


const int ServoPin = 2;

const int Servo_Right_Angle = 0;
const int Servo_Center_Angle = 90;
const int Servo_Left_Angle = 180;

const int ChipSelect = 53;


#define Trig_Pin  14
#define Echo_Pin  13

#define BluetoothRXPin 10
#define BluetoothTXPin 11

#define GPS_RXPin 12
#define GPS_TXPin 13

#define Buzzer 5
#define ModePin A0

int GPS_State = 0;
int GPS_Count = 0;
boolean SD_State = false;
int MaxDistance = 350;

#define DEBUG    // undefined or comment it to hide Serial Data

SoftwareSerial Bluetooth (BluetoothRXPin, BluetoothTXPin);

SoftwareSerial GPS_Connection (GPS_RXPin, GPS_TXPin);

NewPing sonar (Trig_Pin, Echo_Pin, MaxDistance);

Servo servo;

TinyGPSPlus GPS;


#include "classes.h"

RobotMotors Motors;

RobotSounds Sounds;

ExplorerMode Explorer;

BluetoothControl BluetoothMode;

GPS_Control GPS_Logging;

void setup() {    // Setup

#ifdef DEBUG
  Serial.begin(9600);
#endif

  pinMode(Buzzer, OUTPUT);

  if (!SD.begin(ChipSelect)) {
    BuzzerSound();
    Serial.println("Error opening SD card.");
    delay(2000);
    ErrorSound();


  } else {
    Serial.println("SD Card opened. ");
  }

  DataFile = SD.open("ROBOT.TXT", FILE_WRITE);


  if (DataFile) {
    Serial.println("file opened.");
  } else {
    Serial.println("Error opening the File.");
    ErrorSound();
    while (true);
  }
  BuzzerSound();
  DataFile.println("Robot Blox By Mutaz Lafi Data File. ");

  Serial.println("Robot Starting....");

  DataFile.println("Robot Starting....");
  delay(1000);
  BuzzerSound();
  BluetoothMode.Begin(9600);
  servo.attach(ServoPin);
  pinMode(Buzzer, OUTPUT);
  GPS_Connection.begin(9600);



  Serial.println("Bluetooth,Servo and Sound Started.");
  DataFile.println("Bluetooth,Servo and Sound Started.");

  servo.write(Servo_Center_Angle);

  DataFile.println("Servo is on 90 Angle.");
  Serial.println("Servo is on 90 Angle.");
  delay(500);
  Serial.println("Getting Some infromations from the Ultrasonic sensor....");
  DataFile.println("Getting Some infromations from the Ultrasonic sensor....");

  Explorer.Begin();
  Serial.println("Done.");
  DataFile.println("Done.");

  Serial.println("AL Sensors And Modules Started in The Robot.");
  DataFile.println("AL Sensors And Modules Started in The Robot.");

GPSCheck:             // goto label

  GPS_Logging.GPSLog();

  if (GPS_State == 0) {
    Serial.println("GPS DATA NOT DETECTED");
    Serial.println(GPS_State);
    GPS_Count++;
    delay(100);
    Serial.print(GPS_Count);
    if (GPS_Count >= 500) {
      Serial.print("GPS DATA WAS UNABLE TO BE DETECTED");
      DataFile.println("GPS DATA WAS UNABLE TO BE DETECTED");
      DataFile.close();
      while (true);
    }
    goto GPSCheck;
  }



  BuzzerSound();

  DataFile.close();
}


void loop() {         //Loop
  if (SD_State == false) {
    File DataFile = SD.open("ROBOT.TXT");
    SD_State = true;
  }
  int ModePinData = map(analogRead(ModePin), 1023, 0, 1000, 0);

  DataFile.print("The Mode Pin is On: ");
  DataFile.println(ModePinData);
  DataFile.print("In Mode: ");
  if (ModePinData < 500) {
    DataFile.println("Explorer Mode");
    Explorer.StartExplorer();
  } else {
    DataFile.println("Bluetooth And Sensors Mode");
    Sounds.StartSoundSensor();
    BluetoothMode.StartBluetoothMode();
  }
  DataFile.close();
}

void BuzzerSound() {
  digitalWrite(Buzzer, HIGH);
  delay(2000);
  digitalWrite(Buzzer, LOW);
  delay(1000);
}

void ErrorSound() {
  digitalWrite(Buzzer, HIGH);
  delay(2000);
  digitalWrite(Buzzer, LOW);
  delay(500);
  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);
  delay(500);
  digitalWrite(Buzzer, HIGH);
  delay(5000);
}
