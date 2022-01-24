#include <TaskScheduler.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void choosedirect();
void stabilize();
void publisheffect();
int velocita = 100; 
int f1;
int f2;
int f3;
int tettoluce;  
int lucegirata;
int EnginePin1 = D1;
int EnginePin2 = D2;
int EnginePin3 = D4;
int EnginePin4 = D3;
int PhotoPin1 = D7;
int PhotoPin2 = D6;
int PhotoPin3 = D0;
int crescita = 0;
boolean turning = false;
boolean stopped = false;
WiFiClient espClient;
PubSubClient client(espClient);
Scheduler runner;

const char* ssid = "yournetwork";
const char* password = "yourpassword";
const char* mqtt_server = "yourmqttserver";


Task chooseDirection(100 * TASK_MILLISECOND, TASK_FOREVER, choosedirect);

Task stabilizza(100 * TASK_MILLISECOND, TASK_FOREVER,stabilize);

Task publishEffect(1000 * TASK_MILLISECOND, TASK_FOREVER, publisheffect);



void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("connesso");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String strTopic = String(topic);
  if(strTopic=="marcohazan/lightseeker/velocita"){
      String message = (char*)payload;
      long v = message.toInt();
      velocita = v;
      if(!turning){
        startRun();
      }
  }else if(strTopic == "marcohazan/lightseeker/luce"){
    String message = (char*)payload;
      long l = message.toInt();
      tettoluce = l;
      Serial.println("tetto luce impostato");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      //Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("ruck/machine");
      client.subscribe("ruck/machine/velocita");
      client.subscribe("ruck/machine/luce");
      //client.subscribe("ruck/machine/start");
    } else {
      /*Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");*/
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publisheffect(){
  String s = String(f3);
  char fs[10];
  s.toCharArray(fs,10);
  client.publish("ruck/machine",fs);
}

void stop(){
  analogWrite(EnginePin1,0);
  analogWrite(EnginePin2,0);
  analogWrite(EnginePin3,0);
  analogWrite(EnginePin4,0);
}

void stabilize(){
  readFoto3();
  if(turning && f3 > lucegirata - 50){
    analogWrite(EnginePin1,velocita);
    analogWrite(EnginePin2,0);
    analogWrite(EnginePin3,0);
    analogWrite(EnginePin4,velocita);
    //Serial.println("stabilizzato");
    turning = false;
  }
}

void choosedirect(){
  readFoto1();
  readFoto2();
  readFoto3();
  if(f3 >= tettoluce){
    stop();
    stopped = true;
  }
  if(stopped){
    if(f3 < tettoluce - 50){
      startRun();
      stopped = false;
    }
  }
  if(!stopped){
    if(!turning){
      if(f1 + f2 +  f3 < 100){
        if(f1 > f3 + 5 && f1 > f2 + 5){
          turnRight();
        }else if( f2 > f3 + 5 && f2  > f1  + 5){
          turnLeft();
        }
        return;
      }
      if(f1 > f3 + 20 && f1  > f2 + 20){
        turnRight();
        return;
      }else if( f2 > f3 + 20 && f2  > f1  + 20){
        turnLeft();
        return;
      }
      if(f1 > f3+20 && f2 > f3+20){
        turnLeft();
      }
    }
  }
}


void turnLeft(int indice){
  analogWrite(EnginePin4,velocita);
  analogWrite(EnginePin3,0);
  analogWrite(EnginePin1,0);
  analogWrite(EnginePin2,velocita);
  turning = true;
  lucegirata = f2;
}

void turnRight(int indice){
  analogWrite(EnginePin1,velocita);
  analogWrite(EnginePin3,velocita);
  analogWrite(EnginePin2,0);
  analogWrite(EnginePin4,0);
  turning = true;
  lucegirata = f1;
}

void startRun(){
  analogWrite(EnginePin1,velocita);
  analogWrite(EnginePin2,0);
  analogWrite(EnginePin3,0);
  analogWrite(EnginePin4,velocita);
}

void readFoto1(){
  pinMode(PhotoPin2,INPUT);
  pinMode(PhotoPin3,INPUT);
  pinMode(PhotoPin1,OUTPUT);
  digitalWrite(PhotoPin1,HIGH);
  f1 = analogRead(A0);
}


void readFoto2(){
  pinMode(PhotoPin1,INPUT);
  pinMode(PhotoPin3,INPUT);
  pinMode(PhotoPin2,OUTPUT);
  digitalWrite(PhotoPin2,HIGH);
  f2 = analogRead(A0) - 40;
}

void readFoto3(){
  pinMode(D0,OUTPUT);
  pinMode(D5,INPUT);
  pinMode(D6,INPUT);
  digitalWrite(D0,HIGH);
  f3 = analogRead(A0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  runner.init();
  runner.addTask(chooseDirection);
  chooseDirection.enable();
  runner.addTask(updateCrescita);
  updateCrescita.enable();
  runner.addTask(stabilizza);
  stabilizza.enable();
  runner.addTask(publishEffect);
  publishEffect.enable();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  readFoto1();
  readFoto2();
  readFoto3();
  //tara();
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
  runner.execute();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
}
