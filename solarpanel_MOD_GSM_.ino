#include <SoftwareSerial.h>
SoftwareSerial SIM900(6,7);
#include <Servo.h>
//defining Servos
Servo servohori;
int servoh = 0;
int servohLimitHigh = 70;
int servohLimitLow = -70;

Servo servoverti; 
int servov = 0; 
int servovLimitHigh = 70;
int servovLimitLow = -70;
//Assigning 4 LDRs
int ldrtopleft= A2; //top left LDR green
int ldrtopright = A1; //top right LDR yellow
int ldrbotleft = A3; // bottom left LDR blue
int ldrbotright =A0; // bottom right LDR orange
 void setup () 
 {
  servohori.attach(10);
  servohori.write(0);
  servoverti.attach(9);
  servoverti.write(0);
  delay(500);
  SIM900.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
 }
void SendMessage()
{
  SIM900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);  // Delay of 1000 milli seconds or 1 second
  SIM900.println("AT+CMGS=\"+254728780003\"\r"); // Replace x with mobile number
  delay(100);
  SIM900.println("The system has noticed no significant movement");// The SMS text you want to send
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
void loop()
{
  servoh = servohori.read();
  servov = servoverti.read();
  //capturing analog values of each LDR
  int topl = analogRead(ldrtopleft);
  int topr = analogRead(ldrtopright);
  int botl = analogRead(ldrbotleft);
  int botr = analogRead(ldrbotright);
  // calculating average
  int avgtop = (topl + topr) / 2; //average of top LDRs
  int avgbot = (botl + botr) / 2; //average of bottom LDRs
  int avgleft = (topl + botl) / 2; //average of left LDRs
  int avgright = (topr + botr) / 2; //average of right LDRs

  if (avgtop < avgbot)
  {
    servoverti.write(servov +1);
    if (servov > servovLimitHigh) 
     { 
      servov = servovLimitHigh;
     }
    delay(10);
  }
  else if (avgbot < avgtop)
  {
    servoverti.write(servov -1);
    if (servov < servovLimitLow)
  {
    servov = servovLimitLow;
  }
    delay(10);
  }
  else 
  {
    servoverti.write(servov);
  }
  
  if (avgleft > avgright)
  {
    servohori.write(servoh +1);
    if (servoh > servohLimitHigh)
    {
    servoh = servohLimitHigh;
    }
    delay(10);
  }
  else if (avgright > avgleft)
  {
    servohori.write(servoh -1);
    if (servoh < servohLimitLow)
     {
     servoh = servohLimitLow;
     }
    delay(10);
  }
  else 
  {
    servohori.write(servoh);
  }
  delay(50);
  if (avgright==avgleft|avgtop==avgbot)// function executes if the system is idle for some time
  {
    delay(5000);
    SendMessage();
  }
}
