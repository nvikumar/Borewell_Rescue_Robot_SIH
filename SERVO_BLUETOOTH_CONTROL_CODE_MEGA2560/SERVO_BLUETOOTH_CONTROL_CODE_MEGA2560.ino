#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial Bluetooth(10, 11);
char Data;
char SATA;
int count = 0;

void _delay(float seconds) {
  long endTime = millis() + seconds * 1000;
  while (millis() < endTime) loop();
}

Servo myservo_main;
Servo myservo_left_arm;
Servo myservo_right_arm;

int initial_right = 90;
int initial_left = 90;



void both_outside(Servo left_arm, Servo right_arm){
  initial_right = initial_right - 5;
  initial_left = initial_left - 5;
  if( initial_right < 45){
    initial_right = 45;
    initial_left = 45;
    Bluetooth.println("MAXIMUM LIMIT REACHED");
  }
  left_arm.write(initial_left);
  right_arm.write(initial_right);
}

void both_inside(Servo left_arm, Servo right_arm){
  initial_right = initial_right + 5;
  initial_left = initial_left + 5;
  if( initial_right > 135){
    initial_right = 135;
    initial_left = 135;
    Bluetooth.println("MINIMUM LIMIT REACHED");
  }
  left_arm.write(initial_left);
  right_arm.write(initial_right);
}

void setup(){
  myservo_main.attach(6);
  myservo_left_arm.attach(5);
  myservo_right_arm.attach(3);
  myservo_main.write(90);

  Bluetooth.begin(9600);
  delay(2000); 
}

void outside_move(Servo arm){
  int check;
  if(arm.attached() == myservo_right_arm.attached()){
    initial_right = initial_right - 5;
    check = initial_right;
  }
  else if (arm.attached() == myservo_left_arm.attached()){
    initial_left = initial_left - 5;
    check = initial_left;
  }

  if(check < 45){
    check = 45;
    Bluetooth.println("MAXIMUM LIMIT REACHED");
  }
  arm.write(check);
}

void inside_move(Servo arm){
  int check;
  if(arm.attached() == myservo_right_arm.attached()){
    initial_right = initial_right + 5;
    check = initial_right;
  }
  else if (arm.attached() == myservo_left_arm.attached()){
    initial_left = initial_left + 5;
    check = initial_left;
  }

  if(check > 135){
    check = 135;
    Bluetooth.println("MINIMUM LIMIT REACHED");
  }
  arm.write(check);
}

void loop(){
  if(Bluetooth.available()){
    Data = Bluetooth.read();  
    if(Data == 'x'){
      myservo_main.write(45);
      delay(200);
      myservo_main.write(90);
    } 
    else if(Data == 'c'){
      myservo_main.write(135);
      delay(200);
      myservo_main.write(90);
    } 
    else if(Data == 'l'){
      outside_move(myservo_right_arm);
    } 
    else if(Data == 'j'){
      inside_move(myservo_right_arm);
    }  
    else if(Data == 'd'){
      outside_move(myservo_left_arm);
    }  
    else if(Data == 'a'){
      inside_move(myservo_left_arm);
    }
    else if(Data == 'h'){
      if(initial_right != initial_left){
        Bluetooth.println("RESETTING");
        initial_right = initial_left = 90;
        myservo_left_arm.write(initial_left);
        myservo_right_arm.write(initial_right);
      }
      delay(1000);
      both_outside(myservo_left_arm, myservo_right_arm);
    }
    else if(Data == 'r'){
      Bluetooth.println("RESETTING");
      initial_right = initial_left = 90;
      myservo_right_arm.write(initial_right);
      myservo_left_arm.write(initial_left);
    }
    else if(Data == 'f'){
      if(initial_right != initial_left){
        Bluetooth.println("RESETTING");
        initial_right = initial_left = 90;
        myservo_left_arm.write(initial_left);
        myservo_right_arm.write(initial_right);
      }
      delay(1000);
      both_inside(myservo_left_arm, myservo_right_arm);
    }
    else if(Data == 'q'){
      BluetoothSerial.println("DISCONNECTING.....");
      delay(1000);
      BluetoothSerial.println("DISCONNECTED");
      BluetoothSerial.println("BYE:)");
      BluetoothSerial.print("AT+DISC\r\n");
      delay(1000);
      BluetoothSerial.end();
    }      
  }
}