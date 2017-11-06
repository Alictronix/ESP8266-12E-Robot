// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>


// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "wifi-name";
const char* password = "wifi-pass";


// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);
int Speed = 800;
int TSpeed = 1023;

// Function
int stop(String message);
int forward(String message);
int right(String message);
int left(String message);
int backward(String message);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);

  

  // Functions          
  rest.function("stop", stop);
  rest.function("forward", forward);
  rest.function("left", left);
  rest.function("right", right);
  rest.function("backward", backward);
      
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("robot");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

 Serial.println("pinmode set output");
  digitalWrite(5, 0);
  digitalWrite(4, 0);
  
  digitalWrite(0, 1);
  digitalWrite(2, 1);

  Serial.println("digital write set ");
}

void loop() {
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
 /*   analogWrite(5, 0);
    analogWrite(4, 0);
    digitalWrite(0, 1);
    digitalWrite(2, 1);
   */ return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}

int stop(String command) {
  
   analogWrite(5, 0);
   analogWrite(4, 0);
   Serial.println("Stop");
  
}

int forward(String command) {
  
  analogWrite(5, Speed);
    analogWrite(4, Speed);
    digitalWrite(0, 1);
    digitalWrite(2, 1);
    Serial.println("forward");
  
}

int left(String command) {
  
 analogWrite(5, TSpeed);
    analogWrite(4, TSpeed);
    digitalWrite(0, 0);
    digitalWrite(2, 1);
    Serial.println("Left");  
}

int right(String command) {
  
 analogWrite(5, TSpeed);
    analogWrite(4, TSpeed);
    digitalWrite(0, 1);
    digitalWrite(2, 0);
    Serial.println("right"); 
}

int backward(String command) {
  
  analogWrite(5, Speed);
    analogWrite(4, Speed);
    digitalWrite(0, 0);
    digitalWrite(2, 0);
    Serial.println("Back");
}
