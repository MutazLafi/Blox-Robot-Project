
class GPS_Control {
  public:

    void GPSLog() {
      while (GPS_Connection.available()) {
        GPS.encode(GPS_Connection.read());
      }

      if (GPS.location.isUpdated()) {
        DataFile.print("https://www.google.com/maps/place/");   // lat then lng
        DataFile.print(GPS.location.lat(), 6);
        DataFile.print(",");
        DataFile.println(GPS.location.lng(), 6);
        DataFile.println();

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

  private:
    int MotorPin1 = 6;
    int MotorPin2 = 7;
    int MotorPin3 = 8;
    int MotorPin4 = 9;

  public:
    void Begin() {
      pinMode(MotorPin1, OUTPUT);
      pinMode(MotorPin2, OUTPUT);
      pinMode(MotorPin3, OUTPUT);
      pinMode(MotorPin4, OUTPUT);
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
          break;


      }
    }
};



class RobotSounds {

  public:
    const int SoundSensorPin = 4;
    const int Buzzer = 5;

    void Begin() {
      pinMode(SoundSensorPin, INPUT);

      pinMode(Buzzer, OUTPUT);
    }

    void ErrorSound() {
      digitalWrite(Buzzer, HIGH);
      delay(2000);
      digitalWrite(Buzzer, LOW);
      delay(1000);
    }


    int SoundSensor() {
      boolean SoundReading = map(digitalRead(SoundSensorPin), HIGH, LOW, false, true);

      return SoundReading;
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
        return 400;
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

    void StartBluetoothAndSensorsMode() {

      boolean Sound = Sounds.SoundSensor();
      if (Sound == true) {
        Motors.MoveRandom();
      }

      while (Bluetooth.available() > 0) {
        c = Bluetooth.read();
        delay(10);
        Command += c;
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
