
class RobotMotors {

  private:
    int MotorPin1 = 6;
    int MotorPin2 = 7;
    int MotorPin3 = 8;
    int MotorPin4 = 10;

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
    const int SpeakerPin = 22;
    const int Buzzer = 5;

    void Begin() {
      pinMode(SoundSensorPin, INPUT);
      pinMode(SpeakerPin, OUTPUT);
      pinMode(Buzzer, OUTPUT);
    }

    void ErrorSound() {
      digitalWrite(Buzzer, HIGH);
      delay(2000);
      digitalWrite(Buzzer, LOW);
      delay(1000);
    }

    void MovingSoundON() {
      digitalWrite(SpeakerPin, HIGH);
    }

    void MovingSoundOFF() {
      digitalWrite(SpeakerPin, LOW);
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

      if (distance < 15) {
        Motors.Move('S');
        delay(500);
        Motors.Move('B');
        delay(700);
        Motors.Move('S');
        delay(200);
        distanceR = LookRight();
        distanceL = LookLeft();
        delay(200);

        if (distanceR < distanceL) {
          Motors.Move('R');
          delay(600);
          Motors.Move('S');
        } else {
          Motors.Move('L');
          delay(600);
          Motors.Move('S');
        }

      } else {
        Motors.Move('F');
      }
    }
    int ReadPing() {
      int cm = sonar.ping_cm();
      if (cm = 0)
        cm = 230;
      return cm;
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
