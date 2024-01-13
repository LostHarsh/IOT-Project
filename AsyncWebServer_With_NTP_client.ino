#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <time.h> 

//  Time zone
#define MY_NTP_SERVER "pool.ntp.org"
#define MY_TZ "NTP-TiemZone"


//  Wifi Name and Password
const char* ssid = "your_ssid";
const char* password = "your_pass";

//  IP at which Node MCU hosts server
IPAddress staticIP(192, 168, xx, xx);
IPAddress gateway(192, 168, xx, xx);
IPAddress subnet(255, 255, xx, xx);


//  Variable declaration
const char* PARAM_INPUT = "buttonID";
const char* PARAM_HOUR = "hour";  
const char* PARAM_MIN = "minute";

int Ihour = 0,Imin = 0;

//const int TimerStartButton = D0;
//const int QuickStartButton = D2;
const int relayPin1 = D1; 
const int relayPin2 = D5; 
const int relayPin11 = D4; 
const int relayPin22 = D6; 
const int BackwardLimit = D7;
const int ForwardLimit = D8;
//const int HomeLimit = D3;

unsigned long t1Duration = 2;  
unsigned long t2Duration = 20; 
unsigned long t3Duration = 2;  
unsigned long t4Duration = 20; 
unsigned long t5Duration = 6;  

unsigned long t1StartTime;
unsigned long t2StartTime;
unsigned long t3StartTime;
unsigned long t4StartTime;
unsigned long t5StartTime;

bool T1TimerStarted = false;
bool T2TimerStarted = false;
bool T3TimerStarted = false;
bool T4TimerStarted = false;
bool T5TimerStarted = false;
bool executed1 = false;
bool executed2 = false;
bool executed3 = false;
bool executed4 = false;
bool executed5 = false;
//bool HomeStatus = false; 
//bool TimerStartFlag = false;
//bool QuickStartFlag = false;

AsyncWebServer server(80);

time_t now; 
tm tm;


void setup() {
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin11, HIGH);
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin22, HIGH);
  Serial.begin(115200);
  
 WiFi.config(staticIP, gateway, gateway, subnet);

  t1StartTime = 0;
  t2StartTime = 0;
  t3StartTime = 0;
  t4StartTime = 0;
  t5StartTime = 0;


  configTime(MY_TZ, MY_NTP_SERVER);

  pinMode (relayPin1,OUTPUT);
  pinMode (relayPin2,OUTPUT);
  pinMode (relayPin11,OUTPUT);
  pinMode (relayPin22,OUTPUT);
  
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

    request->send(200, "text/plain", "Hello from NodeMCU!");
  });

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {

    request->send(200, "text/plain", "Hello from index.html!");
  });


//  Server code to respond to send Time function of client
  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request) {
    String Shour;
    String Smin;

    if (request->hasParam(PARAM_HOUR) && request->hasParam(PARAM_MIN)) {
      Shour = request->getParam(PARAM_HOUR)->value();
      Smin = request->getParam(PARAM_MIN)->value();

      Ihour = Shour.toInt();
      Imin = Smin.toInt();

      Serial.print("hour - ");
      Serial.println(Ihour);
      Serial.print("minute - ");
      Serial.println(Imin);

      request->send(200, "text/plain", "Time received successfully");
    } else {

      request->send(400, "text/plain", "Bad Request");
    }
  });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  
  server.begin();


}

void loop() {
  delay(200);
  time(&now);
  localtime_r(&now, &tm);
 if ((tm.tm_hour == Ihour && tm.tm_min == Imin && tm.tm_sec < 5) && !T1TimerStarted) {
   startT1();
   T1TimerStarted = true;
 }
  fun();

}

void fun() {
  if ((tm.tm_sec - t1StartTime >= t1Duration) && T1TimerStarted && !executed1) {
    startR1();
    startT2();
    T2TimerStarted = true;
    executed1 = true;
  }
  
  if ((tm.tm_sec - t2StartTime >= t2Duration || digitalRead(BackwardLimit) != LOW ) && T1TimerStarted && T2TimerStarted && !executed2) {
    t2StartTime = tm.tm_sec;
    stopR1();
    startT3();
    T3TimerStarted = true;
    executed2 = true;            
  }

  if ((tm.tm_sec - t3StartTime >= t3Duration) && T1TimerStarted && T2TimerStarted && T3TimerStarted && !executed3) {
    t3StartTime = tm.tm_sec;
    startR2();
    startT4();
    T4TimerStarted = true;
    executed3 = true;
  }
  
  if ((tm.tm_sec - t4StartTime >= t4Duration || digitalRead(ForwardLimit) != LOW ) && T1TimerStarted && T2TimerStarted && T3TimerStarted && T4TimerStarted && !executed4) {
    t4StartTime = tm.tm_sec;
    stopR2();
    startT5();
    T5TimerStarted = true;
    executed4 = true;
  }

  if ((tm.tm_sec - t5StartTime >= t5Duration) && T1TimerStarted && T2TimerStarted && T3TimerStarted && T4TimerStarted && T5TimerStarted && !executed5) {
    t5StartTime = tm.tm_sec;
    T2TimerStarted = T3TimerStarted = T4TimerStarted = T5TimerStarted = T1TimerStarted = false;
    executed1 = executed2 = executed3 = executed4 = executed5 = false;
  }
}

void startT1() {
  t1StartTime = tm.tm_sec;
  Serial.println("T1 Start");
}

void startT2() {
  t2StartTime = tm.tm_sec;
  Serial.println("T2 Start");
}

void startR1() {
  Serial.println("R1 Start");
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin11, LOW);
}

void stopR1() {
  Serial.println("R1 Stop");
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin11, HIGH);
}

void startT3() {
  t3StartTime = tm.tm_sec;
  Serial.println("T3 Start");
}

void startR2() {
  Serial.println("R2 Start");
  digitalWrite(relayPin2, LOW);
  digitalWrite(relayPin22, LOW);
}

void stopR2() {
  Serial.println("R2 Stop");
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin22, HIGH);
}

void startT4() {
  t4StartTime = tm.tm_sec;
  Serial.println("T4 Start");
}

void startT5() {
  t5StartTime = tm.tm_sec;
  Serial.println("T5 Start");
}
