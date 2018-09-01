
//Speed of the Motors
// Left  motor = A
// Right motor = B
#define ASpeed 500 
#define BSpeed 500

#define DIRA 0  //D3
#define DIRB 2  //D4
#define PWMA  5 //D1
#define PWMB  4 //D2



//SENSOR PINS
int S1 = 14;  //D5
int S2 = 12;  //D6
int S3 = 13;  //D7
//int S4 = 15;  //D8

int Sensor1 = 0;  // right
int Sensor2 = 0;  // centre
int Sensor3 = 0;  // left
//int Sensor4 = 0;

void Forward(){
    analogWrite(PWMA, ASpeed);
    analogWrite(PWMB, BSpeed);
    digitalWrite(DIRA, 1);
    digitalWrite(DIRB, 1);
}

void Left(){
    analogWrite(PWMA, ASpeed);
    analogWrite(PWMB, BSpeed);
    digitalWrite(DIRA, 1);
    digitalWrite(DIRB, 0);
}
void Right(){
    analogWrite(PWMA, ASpeed);
    analogWrite(PWMB, BSpeed);
    digitalWrite(DIRA, 0);
    digitalWrite(DIRB, 1);
}
void Stop(){
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}
void setup() {

  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  digitalWrite(PWMA, 0);
  digitalWrite(PWMB, 0);
  digitalWrite(DIRA, 1);
  digitalWrite(DIRB, 1);
  
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
 // pinMode(S4, INPUT);

  Serial.begin(115200);

}

void loop(){

  //Use analogWrite to run motor at adjusted speed
  analogWrite(PWMA, ASpeed);
  analogWrite(PWMB, BSpeed);
  digitalWrite(DIRA, 1);
  digitalWrite(DIRB, 1);

  //Read the Sensor if HIGH (BLACK Line) or LOW (WHITE Line)
  Sensor1 = digitalRead(S1);
  Sensor2 = digitalRead(S2);
  Sensor3 = digitalRead(S3);
  //Sensor4 = digitalRead(S4);
  
  
  //Set conditions for FORWARD, LEFT and RIGHT
if(Sensor1==HIGH && Sensor2==LOW && Sensor3==LOW)          {  Left();    } 
    else if(Sensor1==HIGH && Sensor2==HIGH && Sensor3==LOW){  Left();     } 
    else if(Sensor3==HIGH && Sensor1==LOW && Sensor2==LOW){ Right();       }
    else if(Sensor3==HIGH && Sensor2==HIGH && Sensor1==LOW){  Right();        }
    else if(Sensor2==HIGH && Sensor1==LOW && Sensor3==LOW){ Forward();    }       
    else if(Sensor1==LOW && Sensor2==LOW && Sensor3==LOW) { Forward();    } 
    else  Stop(); 


}
