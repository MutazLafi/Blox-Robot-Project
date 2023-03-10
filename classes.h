
File DataFile = SD.open("ROBOT.TXT");

class GPS_Control {

  public:

    void GPSLog() {
      while (GPS_Connection.available()) {
        GPS.encode(GPS_Connection.read());
      }

      if (GPS.location.isUpdated()) {
        DataFile.println("GPS Location Link: ");
        DataFile.print("https://www.google.com/maps/place/");   // lat then lng
        DataFile.print(GPS.location.lat(), 6);
        DataFile.print(",");
        DataFile.println(GPS.location.lng(), 6);
        DataFile.println();
        Serial.println("GPS Location Link:  ");
        Serial.print("https://www.google.com/maps/place/");   // lat then lng
        Serial.print(GPS.location.lat(), 6);
        Serial.print(",");
        Serial.println(GPS.location.lng(), 6);
        Serial.println();
        GPS_State = 1;

      } else {
        GPS_State = 0;
      }
    }
};


class RobotMotors {
    GPS_Control GPS_Data;

  private:
    const int MotorPin1 = 6;
    const int MotorPin2 = 7;
    const int MotorPin3 = 8;
    const int MotorPin4 = 9;
    const int ENA = 3;
    const int ENB = 5;

  public:
    void Begin() {
      pinMode(MotorPin1, OUTPUT);
      pinMode(MotorPin2, OUTPUT);
      pinMode(MotorPin3, OUTPUT);
      pinMode(MotorPin4, OUTPUT);
      pinMode(ENA, OUTPUT);
      pinMode(ENB, OUTPUT);
    }
    void setSpeed(int Speed) {
      analogWrite(ENA, Speed);
      analogWrite(ENB, Speed);
    }

    void MoveRandom() {
      int P = random(5);
      switch (P) {

        case 1:
          Move('F');
          delay(1000);
          Move('S');
          break;

        case 2:
          Move('B');
          delay(1000);
          Move('S');
          break;

        case 3:
          Move('L');
          delay(1000);
          Move('S');
          break;

        case 4:
          Move('R');
          delay(1000);
          Move('S');
          break;
      }
    }

    void Move(char path) {


      switch (path) {
        case 'F':

          digitalWrite(MotorPin1, HIGH);
          digitalWrite(MotorPin2, LOW);
          digitalWrite(MotorPin3, HIGH);
          digitalWrite(MotorPin4, LOW);


          break;



        case 'B':

          digitalWrite(MotorPin1, LOW);
          digitalWrite(MotorPin2, HIGH);
          digitalWrite(MotorPin3, LOW);
          digitalWrite(MotorPin4, HIGH);

          break;




        case 'L':

          digitalWrite(MotorPin1, HIGH);
          digitalWrite(MotorPin2, LOW);
          digitalWrite(MotorPin3, LOW);
          digitalWrite(MotorPin4, LOW);
          break;




        case 'R':

          digitalWrite(MotorPin1, LOW);
          digitalWrite(MotorPin2, LOW);
          digitalWrite(MotorPin3, HIGH);
          digitalWrite(MotorPin4, LOW);
          break;


        case 'S':

          digitalWrite(MotorPin1, LOW);
          digitalWrite(MotorPin2, LOW);
          digitalWrite(MotorPin3, LOW);
          digitalWrite(MotorPin4, LOW);

GPS_Logging_Check:

          GPS_Data.GPSLog();

          if (GPS_State == 0) {
            Serial.println("GPS DATA NOT DETECTED");
            Serial.println(GPS_State);
            GPS_Count++;
            delay(100);

            if (GPS_Count == 300) {
              Serial.print("GPS DATA WAS UNABLE TO BE DETECTED");
              DataFile.println("GPS DATA WAS UNABLE TO BE DETECTED");
              DataFile.close();

            }
            goto GPS_Logging_Check;
          }

          break;
      }
    }
};



class RobotSounds {
    RobotMotors Motors;

  public:
    const int SoundSensorPin = 4;


    void Begin() {
      pinMode(SoundSensorPin, INPUT);
      pinMode(Buzzer, OUTPUT);
    }



    boolean GetSoundSensorData() {
      boolean SoundReading = map(digitalRead(SoundSensorPin), HIGH, LOW, false, true);

      return SoundReading;
    }

    void StartSoundSensor() {
      boolean Sound = GetSoundSensorData();
      if (Sound == true) {
        Motors.MoveRandom();
      }
    }
};


class ExplorerMode {
    RobotMotors Motors;

  public:

    int distance = 0;

    void Begin() {
      distance = ReadPing();
      delay(100);
      distance = ReadPing();
      delay(100);
      distance = ReadPing();
      delay(100);
      distance = ReadPing();
      delay(100);
    }

    void StartExplorer() {

      distance = ReadPing();

      int distanceR = 0;
      int distanceL = 0;

      delay(70);

      if (distance < 30) {
        Motors.Move('S');
        delay(500);
        Motors.Move('B');
        delay(700);
        Motors.Move('S');
        delay(200);
        distanceR = LookRight();
        delay(300);
        distanceL = LookLeft();
        delay(300);

        if (distanceR < distanceL) {
          Motors.Move('L');
          delay(600);
          Motors.Move('S');
        } else {
          Motors.Move('R');
          delay(600);
          Motors.Move('S');
        }

      } else {
        Motors.Move('F');
      }
    }
    int ReadPing() {
      int cm = sonar.ping_cm();
      if (cm = 0) {
        return 350;
      } else {
        return cm;
      }
    }

    int LookLeft() {
      servo.write(Servo_Left_Angle);
      delay(500);
      int distance = ReadPing();
      delay(200);
      servo.write(Servo_Center_Angle);
      delay(100);
      return distance;
    }


    int LookRight() {
      servo.write(Servo_Right_Angle);
      delay(500);
      int distance = ReadPing();
      delay(200);
      servo.write(Servo_Center_Angle);
      delay(100);
      return distance;
    }
};

class BluetoothControl {
    GPS_Control GPS_Log;
    RobotSounds Sounds;
    RobotMotors Motors;
  public:

    String Command = "";
    char c = ' ';
    void Begin(int BaudRate) {
      Bluetooth.begin(BaudRate);
    }

    void StartBluetoothMode() {


      while (Bluetooth.available() > 0) {
        c = Bluetooth.read();
        delay(10);
        Command = Command + c;
      }

      switch (c) {
        case 'f':
          Motors.Move('F');
          break;

        case 'b':
          Motors.Move('B');
          break;

        case 'L':
          Motors.Move('L');
          break;

        case 'R':
          Motors.Move('R');
          break;

        case 'S':
          Motors.Move('S');

GPS_Check:

          GPS_Log.GPSLog();

          if (GPS_State == 0) {

            goto GPS_Check;
          }

          break;
      }


      if (Command == "forward") {
        Motors.Move('F');
        delay(1000);
        Motors.Move('S');
      }


      if (Command == "back") {
        Motors.Move('B');
        delay(1000);
        Motors.Move('S');
      }


      if (Command == "right") {
        Motors.Move('R');
        delay(1000);
        Motors.Move('S');
      }


      if (Command == "left") {
        Motors.Move('L');
        delay(1000);
        Motors.Move('S');
      }

      if (Command == "stop") {
        Motors.Move('F');
        delay(1000);
        Motors.Move('S');
      }
      Command = "";
      c = ' ';

    }
};
