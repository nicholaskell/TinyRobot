
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 
#include <TimerOne.h>
#include <Ultrasonic.h>
#include <Nick_Robot.h>


#define PING_TRIGGER_PIN       13
#define PING_ECHO_PIN          12
#define PING_SERVO_PIN         11

#define MOTOR_A_1_PIN          10
#define MOTOR_A_2_PIN          9
#define MOTOR_B_1_PIN          6
#define MOTOR_B_2_PIN          5

#define ANGLE_COUNT 5

#define MODE_IDLE
#define MODE_DISTANCE

Ultrasonic ping(PING_TRIGGER_PIN, PING_ECHO_PIN);
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display
Servo pingServo;

Nick_Robot robot;

int angles[ANGLE_COUNT] = {20,45,90,135,150};
int distances[ANGLE_COUNT] = {0,0,0,0,0};
int currentAngle;

int speedLeft = 0;
int speedRight = 0;
int angle = 0;
long distance = 0;


void setup(){
  
  pinMode(MOTOR_A_1_PIN, OUTPUT);
  pinMode(MOTOR_A_2_PIN, OUTPUT);
  pinMode(MOTOR_B_1_PIN, OUTPUT);
  pinMode(MOTOR_B_2_PIN, OUTPUT);
  
  analogWrite(MOTOR_A_1_PIN, 0);
  analogWrite(MOTOR_A_2_PIN, 0);
  analogWrite(MOTOR_B_1_PIN, 0);
  analogWrite(MOTOR_B_2_PIN, 0);
  
  robot.run();
  robot.aquire();
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.print("Initilizing...");
  delay(100);
  Serial.begin(9600);
  pingServo.attach(PING_SERVO_PIN);
  pingServo.write(90);
  delay(100);
  
  
//  Timer1.initialize(1000000); 
//  Timer1.attachInterrupt( getDistance ); // attach the service routine here
  
  
  lcd.print("Done...");
  setupSpeedScreen();
}

void loop(){
  
  getDistance(angle);
  
  
 // angle++;
  angle = 2;
  if(angle > ANGLE_COUNT){
    angle = 0;
  }
  
  printScreen();
  delay(1000);
}

void pointPing(int angle){
  pingServo.write(angle);
}

void setupSpeedScreen(){
  lcd.clear();
  lcd.setCursor(7,3);
  lcd.print("SPEEDS");
}

void getDistance(int index){
  pingServo.write(angles[index]);
  distance = ping.timing();
  lcd.setCursor(0,1);
   lcd.print("Angle:    ");
   lcd.setCursor(7,1);
   lcd.print(angles[angle]);
   
   if(distance < 2000){
     
    analogWrite(MOTOR_B_2_PIN, 100);
    analogWrite(MOTOR_A_2_PIN, 100);
   }else{
     analogWrite(MOTOR_B_2_PIN, 200);
    analogWrite(MOTOR_A_2_PIN,  200);
   }
}

void printScreen(){
 printDistance();
 printSpeeds(); 
}

void printDistance(){
   lcd.setCursor(0,2);
   lcd.print("Distance:           ");
   lcd.setCursor(10,2);
   lcd.print(distance);
   
   
}

void printSpeeds(){
  
  lcd.setCursor(0,3);
  lcd.print("    ");
  lcd.setCursor(0,3);
  lcd.print(speedRight);
  
  lcd.setCursor(16,3);
  lcd.print("    ");
  if(speedLeft < 0){
    lcd.setCursor(16,3);
  }else{
    lcd.setCursor(17,3);
  }
   lcd.print(speedLeft);
  lcd.home();
}
