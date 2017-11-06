// include libraries
#include "SSD1306.h"
#include <Servo.h>
//#include "DCMotor.h"
 
// setup servo
#define SERVORIGHT   50
#define SERVOCENTRE 100
#define SERVOLEFT   150
#define SERVOPIN     16
#define TRIGPIN 13    // pin no. 13 is D7 ESP8266
#define ECHOPIN 15    // pin no. 15  is D8 ESP8266

int Speed = 1023;
int TSpeed = 700;
Servo servo;
SSD1306  display(0x3c, D5, D6); //sda-D5, sck -d6

// setup Ultrasonic sensor
//#define TRIGPIN 4
//#define ECHOPIN 0


void stoped()
{
    analogWrite(5, 0);
    analogWrite(4, 0);
    Serial.println("Stop");
}
 
void forward()
{
    analogWrite(5, Speed);
    analogWrite(4, Speed);
    digitalWrite(0, 1);
    digitalWrite(2, 1);
    Serial.println("forward");
}
 
void back()
{
    analogWrite(5, Speed);
    analogWrite(4, Speed);
    digitalWrite(0, 0);
    digitalWrite(2, 0);
    Serial.println("Back");
}
 
void left()
{
    analogWrite(5, TSpeed);
    analogWrite(4, TSpeed);
    digitalWrite(0, 0);
    digitalWrite(2, 1);
    Serial.println("Left");
}
 
void right()
{
    analogWrite(5, TSpeed);
    analogWrite(4, TSpeed);
    digitalWrite(0, 1);
    digitalWrite(2, 0);
    Serial.println("right");
}

int stopCount = 0;

int ping()
{
    // pause for 50ms between scans
    delay(50);
 
    // send ping
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
 
    // read echo
    long duration = pulseIn(ECHOPIN, HIGH);
 
    // convert distance to cm
    unsigned int centimetres = int(duration / 2 / 29.1);
 
    return centimetres;
}
 
char scan()
{
    // ping times in microseconds
    unsigned int left_scan, centre_scan, right_scan;
    char choice;
 
    // scan left
    servo.write(SERVOLEFT);
    delay(300);
    left_scan = ping();
 
    // scan right
    servo.write(SERVORIGHT);
    delay(600);
    right_scan = ping();
 
    // scan straight ahead
    servo.write(SERVOCENTRE);
    delay(300);
    centre_scan = ping();
 
    if (left_scan>right_scan && left_scan>centre_scan)
    {
        choice = 'l';
    }
    else if (right_scan>left_scan && right_scan>centre_scan)
    {
        choice = 'r';
    }
    else {
      choice = 'c';
    }
 
    return choice;
}
 
void setup()
{
    Serial.begin(115200);
    Serial.println("MakrFun Obstacle Bot 2WD v1.0");
    // set the servo data pin
    servo.attach(SERVOPIN);

    pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(5, 0);
  digitalWrite(4, 0);
  digitalWrite(0, 1);
  digitalWrite(2, 1);
    // set the trig pin to output (send sound waves)
    pinMode(TRIGPIN, OUTPUT);
 
    // set the echo pin to input (receive sound waves)
    pinMode(ECHOPIN, INPUT);

display.init();
display.flipScreenVertically();
display.setFont(ArialMT_Plain_16);
display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void loop()
{
    // get distance from obstacle straight ahead
    unsigned int distance = ping();
    Serial.print("Distance: "); Serial.println(distance);
    display.clear();
   display.drawString(10, 20, "Distance: "+String(distance));
   display.display();
    if (distance < 50 && distance > 0)
    {
        if (distance < 10)
        {
            // turn around
            Serial.println("Turn around..."); 
            display.drawString(10, 40, "Turn around...") ;         
            back();
            delay(300);
            left();
            delay(500);
        }
        else
        {
            // stop both motors
            Serial.println("Motor stop...");
            display.drawString(10, 40, "Motor stop...") ; 
            stoped();
            
            // scan for obstacles
            char turn_direction = scan();
 
            // turn left/right or ignore and go straight
            if (turn_direction == 'l')
            {
              Serial.println("Turn left...");
              display.drawString(10, 40, "Turn left...") ; 
                left();
                delay(200);
            }
            else if (turn_direction == 'r')
            {
              Serial.println("Turn right...");
              display.drawString(10, 40, "Turn right...") ; 
                right();
                delay(200);
            }
            else if (turn_direction == 'c')
            {
              stopCount++;
              if(stopCount > 3){
                stopCount = 0;
                Serial.println("Turn back...");
                display.drawString(10, 40, "Turn back...") ; 
                right();
                delay(400);
              }
              
            }
        }
    }
    else
    {
        // no obstacle, keep going forward
        Serial.println("No obstacle, keep going forward...");
        display.drawString(5, 40, "keep going forward...") ; 
        forward();
    }
}

