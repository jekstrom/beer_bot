#include <SoftwareSerial.h>  
#define RxD 7
#define TxD 6
SoftwareSerial BlueToothSerial(RxD,TxD);
char flag=1;

//Hbridge Motor Control
const int EN=9;   //Half Bridge 1 Enable
const int EN2=10;
const int MC1=2;  //Motor Control 1
const int MC2=3;  //Motor Control 2
 
void sendBlueToothCommand(char *Command)
{
  BlueToothSerial.print(Command);
  Serial.print(Command); 
  delay(100);
  //while(BlueToothSerial.available())
  //{    
  //   Serial.print(char(BlueToothSerial.read())); 
  //}
}
 
void setupBlueTooth()
{
  Serial.println("Bluetooth Initialization ...");     
  sendBlueToothCommand("AT+RESET\r\n"); 
  sendBlueToothCommand("AT+NAME=LinkSprite\r\n");
  sendBlueToothCommand("AT+ROLE=0\r\n");
  sendBlueToothCommand("AT+CMODE=0\r\n");
  sendBlueToothCommand("AT+PSWD=1234\r\n");
  sendBlueToothCommand("AT+UART=38400,0,0\r\n");
  delay(500);
  Serial.println("Bluetooth Initialized Successfully !\r\n");
}
 
void setup()
{
  Serial.begin(38400);     
  BlueToothSerial.begin(38400); 
  delay(500);
  setupBlueTooth();

  pinMode(EN, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(MC1, OUTPUT);
  pinMode(MC2, OUTPUT);
  brake(); //Initialize with motor stopped
}
 
void loop()
{
    if(BlueToothSerial.available())
    {
      String sent = BlueToothSerial.readString();
      Serial.print(sent); 
      if (sent == "F\r\n") {
        Serial.print("Got F");
        forward(45);
        delay(1000);
        brake();
      }
      else if (sent == "B\r\n") {
        Serial.print("Got B");
        brake();
      }
      else if (sent == "R\r\n") {
        Serial.print("Got R");
        reverse(45);
        delay(1000);
        brake();
      }
      else if (sent == "<\r\n") {
        Serial.print("Got <");
        left(45);
        delay(750);
        brake();
      }
      else if (sent == ">\r\n") {
        Serial.print("Got >");
        right(45);
        delay(750);
        brake();
      } else {
        brake();
      }
    }
}


//Motor goes backward at given rate (from 0-255)
void reverse (int rate)
{
    digitalWrite(EN, LOW);
    digitalWrite(EN2, LOW);
    digitalWrite(MC1, HIGH);
    digitalWrite(MC2, HIGH);
    analogWrite(EN, rate);
    analogWrite(EN2, rate);
}

//Motor goes forward at given rate (from 0-255)
void forward (int rate)
{
    digitalWrite(EN, LOW);
    digitalWrite(EN2, LOW);
    digitalWrite(MC1, LOW);
    digitalWrite(MC2, LOW);
    analogWrite(EN, rate);    
    analogWrite(EN2, rate);
}

void left(int rate)
{
    digitalWrite(EN, LOW);
    digitalWrite(EN2, LOW);
    digitalWrite(MC1, HIGH);
    digitalWrite(MC2, LOW);
    analogWrite(EN, rate);    
    analogWrite(EN2, rate);
}

void right(int rate)
{
    digitalWrite(EN, LOW);
    digitalWrite(EN2, LOW);
    digitalWrite(MC1, LOW);
    digitalWrite(MC2, HIGH);
    analogWrite(EN, rate);    
    analogWrite(EN2, rate);
}

//Stops motor
void brake ()
{
    digitalWrite(EN, LOW);
    digitalWrite(EN2, LOW);
    digitalWrite(MC1, LOW);
    digitalWrite(MC2, LOW);
    analogWrite(EN, 0);    
    analogWrite(EN2, 0);
}


