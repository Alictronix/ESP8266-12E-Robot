/*
 * This sketch lets you control an NodeMCU motorshield with attached engines
 * from a (smart phone) browser by using android app.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 

const char* ssid = "NodeMCU";
//const char* password = "abcd1234";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
int Speed = 1023;
int TSpeed = 700;


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
 
void backward()
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

String response(String r){
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nCar is now ";
  s += r;
  s += "</html>\n";
  return s;
  }


void setup() {
    
  Serial.begin(115200);
  delay(10);

  Serial.println();
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.println("Configuring access point...");
  /* You can add the password parameter if you want the AP to be closed. */
  WiFi.softAP(ssid);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("server started");
  
  
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

 Serial.println("pinmode set to output");
 
  digitalWrite(5, 0);
  digitalWrite(4, 0);
  
  digitalWrite(0, 1);
  digitalWrite(2, 1);

  Serial.println("digital write set ");
  
 }

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    analogWrite(5, 0);
    analogWrite(4, 0);
    digitalWrite(0, 1);
    digitalWrite(2, 1);
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  //1 stop
  //2 Forward; 3 reverse; 4 Right; 5 left
  // Match the request
 
  
  int direction;
  
  if (req.indexOf("/key/1") != -1)
    direction = 1;
  else if (req.indexOf("/key/2") != -1)
    direction = 2;
  else if (req.indexOf("/key/3") != -1)
    direction = 3;
  else if (req.indexOf("/key/4") != -1)
    direction = 4;
  else if (req.indexOf("/key/5") != -1) 
    direction = 5;
  else {
    Serial.println("invalid request");
    client.stop();
    return;}
String html;
switch (direction)
        {
            case 1:
               stoped();
               html= response("stopping");
               break;
            case 2:
                forward();
                html=  response("going forward");
                break;
            case 3:
                backward();
                html= response("going back");
                break;
            case 4:
                right();
                html= response("turning right");
                break;
            case 5:
                left();
                html=  response("turning left");
                break;
           
        }
 
        // move for 300ms, gives chip time to update wifi also
  delay(300);
   
  client.flush();
 // Send the response to the client
  client.print(html);
  delay(1);
  Serial.println("Client disonnected");
  delay(200);
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}




