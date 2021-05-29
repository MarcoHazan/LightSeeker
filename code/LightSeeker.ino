#include <TaskScheduler.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Vodafone-84068020";
const char* password = "ic3dmcfkydtccbi";
const char* mqtt_server = "test.mosquitto.org";

void readFoto();
void choosedirect();
 void updateGrow(); 
void publisheffect();
 
WiFiClient espClient;
PubSubClient client(espClient);

int counter;
int crescitaf1;
int crescitaf2;
int f1storico;
int f2storico;
int f1;
int f2;
boolean tar;
Scheduler runner;

int tarvalues[8];

//Task letturafoto(450*TASK_MILLISECOND, TASK_FOREVER, readFoto);

Task updateCrescita(100 * TASK_MILLISECOND, TASK_FOREVER,updateGrow);

Task chooseDirection(600 * TASK_MILLISECOND, TASK_FOREVER, choosedirect);

Task publishEffect(3000 * TASK_MILLISECOND, TASK_FOREVER, publisheffect);

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

void callback(char* topic, byte* payload, unsigned int length) {
  stop();
}

void stop(){
  analogWrite(D0,0);
  analogWrite(D1,0);
  analogWrite(D2,0);
  analogWrite(D3,0);
}

void publisheffect(){
  String s = String(crescitaf1);
  char fs[10];
  s.toCharArray(fs,10);
  client.publish("ruck/machine",fs);
  startRun();
  
}

void updateGrow(){
  readFoto();
  crescitaf1 = crescitaf1 + (f1 - f1storico);
  crescitaf2 =  crescitaf2 + (f2 - f2storico);
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 // setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  runner.init();
  //runner.addTask(letturafoto);
  //letturafoto.enable();
  runner.addTask(chooseDirection);
  chooseDirection.enable();
  runner.addTask(updateCrescita);
  updateCrescita.enable();
  runner.addTask(publishEffect);
  publishEffect.enable();
  counter = 0;
  tar = false;
  crescitaf1 = 0;
  crescitaf2 = 0;
  readFoto1();
  readFoto2();
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  analogWrite(D0,700);
  analogWrite(D1,0);
  analogWrite(D2,0);
  analogWrite(D3,700);
}

void turnRight(){
  analogWrite(D0,0);
  analogWrite(D1,600);
  delay(450);
  analogWrite(D0,600);
  analogWrite(D1,0);
}

void turnLeft(){
  analogWrite(D2,600);
  analogWrite(D3,0);
  delay(450);
  analogWrite(D2,0);
  analogWrite(D3,600);
}


void gira(int ngiri){
  if(ngiri > 4){
    int act = ngiri - 4;
    analogWrite(D0,0);
    analogWrite(D1,600);
    analogWrite(D2,0);
    analogWrite(D3,600);
    for(int i=0;i< act;i++){
      delay(450);
    }
  }else{
    analogWrite(D0,600);
    analogWrite(D1,0);
    analogWrite(D2,600);
    analogWrite(D3,0);
    for(int i=0;i<ngiri;i++){
      delay(450);
    }
    startRun();
  }
}

void readFoto1(){
  f1storico = f1;
  pinMode(D5,INPUT);
  //pinMode(D2,INPUT);
  pinMode(D6,OUTPUT);
  digitalWrite(D6,HIGH);
  f1 = analogRead(A0) - 40;
}

void readFoto(){
  readFoto1();
  readFoto2();
}


void readFoto2(){
  f2storico = f2;
  pinMode(D6,INPUT);
  //pinMode(D2,INPUT);
  pinMode(D5,OUTPUT);
  digitalWrite(D5,HIGH);
  f2 = analogRead(A0) + 40;
}

int max(int array[],int len){
  int max = 0;
  for(int i=1;i<len;i++){
    if(array[i] > array[i-1]){
      max = i;
    }
  }
  return max;
}

void startRun(){
  analogWrite(D0,600);
  analogWrite(D1,0);
  analogWrite(D2,0);
  analogWrite(D3,600);
}

void taratura(){
  /*for(int i=0;i<8;i++){
    analogWrite(D0,700);
    analogWrite(D1,0);
    analogWrite(D3,0);
    analogWrite(D4,0);
    readFoto1();
    tarvalues[i] = f1;
    delay(450);
    stop();
    delay(500);
  }*/
  stop();
  /*delay(2000);
  int index = max(tarvalues,8);
  gira(index);
  startRun();
  Serial.print("[ ");
  for(int i=0;i<8;i++){
    Serial.print(tarvalues[i]);
    Serial.print(",");
  }
  Serial.println( " ]");*/
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
