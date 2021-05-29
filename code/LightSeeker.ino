#include <TaskScheduler.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// dichiarazione di variabili

const char* ssid = "Vodafone-84068020";
const char* password = "ic3dmcfkydtccbi";
const char* mqtt_server = "test.mosquitto.org";

int EnginePin1 = D1;
int EnginePin2 = D2;
int EnginePin3 = D3;
int EnginePin4 = D4;
int PhotoPin1 = D5;
int PhotoPin2 = D6;
int velocita = 600;
int turnTimer = 450;
boolean turning = false;
int crescitaf1;
int crescitaf2;
int f1storico;
int f2storico;
int f1;
int f2;
Scheduler runner;
WiFiClient espClient;
PubSubClient client(espClient);

// interfaccia funzioni per task
void choosedirect();
 void updateGrow(); 
void publisheffect();
void stabilizza();


// TASK
Task updateCrescita(100 * TASK_MILLISECOND, TASK_FOREVER,updateGrow);

Task chooseDirection(600 * TASK_MILLISECOND, TASK_FOREVER, choosedirect);

Task publishEffect(3000 * TASK_MILLISECOND, TASK_FOREVER, publisheffect);

Task stabilizza(turnTimer * TASK_MILLISECOND, TASK_FOREVER, stabilizza);



void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  /*while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }*/
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("ruck/machine");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  //
}

void publisheffect(){
  String s = String(crescitaf1);
  char fs[10];
  s.toCharArray(fs,10);
  client.publish("ruck/machine",fs);
  startRun();
  
}

void updateGrow(){
  readFoto1();
  readFoto2();
  crescitaf1 = crescitaf1 + (f1 - f1storico);
  crescitaf2 =  crescitaf2 + (f2 - f2storico);
}

void stabilizza(){
  if(turning && millis() - turnTime > turnTimer){
    analogWrite(EnginePin1,velocita);
    analogWrite(EnginePin2,0);
    analogWrite(EnginePin3,0);
    analogWrite(EnginePin4,velocita);
    turning = false;
  }
}

void choosedirect(){
  if(crescitaf1 < -20 && crescitaf2 < -20){
    stop();
    tar = false;
  }else if(crescitaf1 > crescitaf2 + 10){
    turnRight();
  }else if(crescitaf2 > crescitaf1 + 10){
    turnLeft();
  }
}

void startRun(){
  analogWrite(EnginePin1,velocita);
  analogWrite(EnginePin2,0);
  analogWrite(EnginePin3,0);
  analogWrite(EnginePin4,velocita);
}



void stop(){
  analogWrite(EnginePin1,0);
  analogWrite(EnginePin2,0);
  analogWrite(EnginePin3,0);
  analogWrite(EnginePin4,0);
}



void turnRight(){
  analogWrite(EnginePin1,0);
  analogWrite(EnginePin2,velocita);
  turnTime = millis();
  turning = true;
}



void turnLeft(){
  analogWrite(EnginePin3,velocita);
  analogWrite(EnginePin4,0);
  turnTime = millis();
  turning = true;
}



void readFoto1(){
  f1storico = f1;
  pinMode(PhotoPin1,INPUT);
  //pinMode(D2,INPUT);
  pinMode(PhotoPin2,OUTPUT);
  digitalWrite(PhotoPin2,HIGH);
  f1 = analogRead(A0) - 40;
}



void readFoto2(){
  f2storico = f2;
  pinMode(PhotoPin2,INPUT);
  //pinMode(D2,INPUT);
  pinMode(PhotoPin1,OUTPUT);
  digitalWrite(PhotoPin1,HIGH);
  f2 = analogRead(A0) + 40;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 // setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  runner.init();
  runner.addTask(chooseDirection);
  chooseDirection.enable();
  runner.addTask(updateCrescita);
  updateCrescita.enable();
  runner.addTask(publishEffect);
  publishEffect.enable();
  readFoto1();
  readFoto2();
  pinMode(EnginePin1,OUTPUT);
  pinMode(EnginePin2,OUTPUT);
  pinMode(EnginePin3,OUTPUT);
  pinMode(EnginePin4,OUTPUT);
  analogWrite(EnginePin1,velocita);
  analogWrite(EnginePin2,0);
  analogWrite(EnginePin3,0);
  analogWrite(EnginePin4,velocita);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if(!tar){
    taratura();
    tar = true;
  }*/
  runner.execute();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 }
