#include <SoftwareSerial.h>
#define FS_PIN1 A0 // the FSR and 10K pulldown are connected to A0
#define FS_PIN2 A1 
#define FS_PIN3 A2 

#define RX 2
#define TX 3

String AP = "RunShaw Tech";       // AP NAME
String PASS = "9916259955"; // AP PASSWORD

String API = "H0JQGAKMR0NGI8PY";
  // Write API KEY YOUR API WRITE KEY
String HOST = "api.thingspeak.com";
String PORT = "80";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

SoftwareSerial esp8266(RX,TX); 

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() {
  
  String point1 = getP1Data();
  String point2 = getP2Data();
  String point3 = getP3Data();
  String point4 = getP4Data();
  String point5 = getP5Data();
  String point6 = getP6Data();
  
  String getData = "GET /update?api_key="+ API +"&field1="+point1+"&field3="+point2+"&field2="+point3+"&field4="+point4+"&field5="+point5+"&field6="+point6;
  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");
  delay(1000);
}

String getP1Data(){
   int Point1 = analogRead(FS_PIN1);
   Serial.print("Force sensor reading = \t");
   Serial.println(Point1); // print the raw analog reading
   return String(Point1);
}
  
String getP2Data(){
   int Point2 = analogRead(FS_PIN2);
   Serial.print("Force sensor reading = \t");
   Serial.println(Point2); // print the raw analog reading
   return String(Point2);
}
String getP3Data(){
   int Point3 = analogRead(FS_PIN3);
   Serial.print("Force sensor reading = \t");
   Serial.println(Point3); // print the raw analog reading
   return String(Point3);
}
String getP4Data(){
   int Point1 = analogRead(FS_PIN1);
   int Point2 = analogRead(FS_PIN2);
   int P4 =(Point1+Point2)/2;
   Serial.print("Force sensor reading = \t");
   Serial.println(P4); // print the raw analog reading
   return String(P4);
}
String getP5Data(){
   int Point1 = analogRead(FS_PIN1);
   int Point3 = analogRead(FS_PIN3);
   int P5 =(Point1+Point3)/2;
   Serial.print("Force sensor reading = \t");
   Serial.println(P5); // print the raw analog reading
   return String(P5);
}
String getP6Data(){
   int Point2 = analogRead(FS_PIN2);
   int Point3 = analogRead(FS_PIN3);
   int P6 =(Point2+Point3)/2;
   Serial.print("Force sensor reading = \t");
   Serial.println(P6); // print the raw analog reading
   return String(P6);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("NFU");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
