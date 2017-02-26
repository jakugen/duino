/*
File: RestServer.ino
This example for the Arduino Uno WiFi shows how to access the digital and analog pins
of the board through REST calls. It demonstrates how you can create your own API using
REST style.

Possible commands created in this shetch:

	"/arduino/digital/13"     -> digitalRead(13)
	"/arduino/digital/13/1"   -> digitalWrite(13, HIGH)
	"/arduino/analog/2/123"   -> analogWrite(2, 123)
	"/arduino/analog/2"       -> analogRead(2)
	"/arduino/mode/13/input"  -> pinMode(13, INPUT)
	"/arduino/mode/13/output" -> pinMode(13, OUTPUT)

This example code is part of the public domain

Note: works only with Arduino Uno WiFi Developer Edition.

http://www.arduino.org/learning/tutorials/boards-tutorials/restserver-and-restclient
*/

#include <Wire.h>
#include <UnoWiFiDevEd.h>


// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;

void setup() {
    pinMode(enA, OUTPUT);
    Wifi.begin();
    Wifi.println("REST Server is up");
}

void loop() {

    while(Wifi.available()){
      process(Wifi);
    }
  delay(50);

}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');
  
  // is "digital" command?
  if (command == "digital") {
    digitalCommand(client);
  }

  // is "analog" command?
  if (command == "analog") {
    //go_backwards(1);
    client.println("HTTP/1.1 200 OK\n");
    client.print(F("Mode - analog"));
    client.print(EOL);    //char terminator
  }
}

void digitalCommand(WifiData client) {
  int pin, value;

  // Read pin number
  pin = client.parseInt();

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
  }
  else {
    value = -1;
  }

  // pin 1 is movement and value 1 forward and value 2 backwards
  if(pin == 1){
    if(value == 1){
      client.println("HTTP/1.1 200 OK\n");
      client.print(F("Pin "));
      client.print(pin);
      client.print(F(" value "));
      client.print(value);
      client.println(F(" ===MOVE FORWARD=== "));
      client.print(EOL);    //char terminator

      go_forward(1);
    }
    else if(value == 2){
      client.println("HTTP/1.1 200 OK\n");
      client.print(F("Pin "));
      client.print(pin);
      client.print(F(" value "));
      client.print(value);
      client.println(F(" ===MOVE BACKWARDS === "));
      client.print(EOL);    //char terminator

      go_backwards(1);
    }
    else if(value == 3){
      client.println("HTTP/1.1 200 OK\n");
      client.print(F("Pin "));
      client.print(pin);
      client.print(F(" value "));
      client.print(value);
      client.println(F(" ===TURN LEFT=== "));
      client.print(EOL);    //char terminator

      turn_left();
    }
    else if(value == 4){
      client.println("HTTP/1.1 200 OK\n");
      client.print(F("Pin "));
      client.print(pin);
      client.print(F(" value "));
      client.print(value);
      client.println(F(" ===TURN RIGHT === "));
      client.print(EOL);    //char terminator

      turn_right();
    }
    else {
      client.println("HTTP/1.1 200 OK\n");
      client.print(F("Pin "));
      client.print(pin);
      client.print(F(" value "));
      client.print(value);
      client.println(F(" ===TURN ? === "));
      client.print(EOL);    //char terminator

      analogWrite(enA, value);
      delay(120);
      analogWrite(enA, 0);
    }
  }
  else if(pin == 2){
    client.println("HTTP/1.1 200 OK\n");
      client.print(F("Pin D"));
      client.print(pin);
      client.print(F(" set to "));
      client.println(value);
      client.print(EOL);    //char terminator
  }
}

void go_forward(int seconds)
{
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 200);
  delay(seconds * 1000);
  stop();
}
void go_backwards(int seconds)
{
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 200);
  delay(seconds * 1000);
  stop();
}
void stop()
{
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void turn_left(){
  analogWrite(enA, 10);
  delay(1000);
  analogWrite(enA, 0);
}

void turn_right(){
  analogWrite(enA, 240);
  delay(1000);
  analogWrite(enA, 0);
}
