#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>


const char* ssid = "keen";
const char* password = "test1234keen";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "38b631bf-dad9-4560-9aac-04f56f638d88";
const char* mqtt_username = "jVxVETMN3nMxH1DjBHvCJgdrr281grWE";
const char* mqtt_password = "jtWEG1XuJjDqyNVe8zEmDdTL9a9YCtcE";

WiFiClient espClient;
PubSubClient client(espClient);
int c = 0;
SoftwareSerial stmSerial(D1,D2);
char stmBuffer[200];
String tim = "";
String latitude = "";
String longitude = "";
String dustValue = "";
bool received = false;


char msg[500];
long prev = 0;

// void IRAM_ATTR uartISR() {
//   while (stmSerial.available() > 0) {
//    tim = stmSerial.readStringUntil(' ');
//    latitude = stmSerial.readStringUntil(' ');
//    longitude = stmSerial.readStringUntil(' ');
//    dustValue = stmSerial.readStringUntil('\n');
//    received = true;
//   }
//   Serial.println("Received");
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  stmSerial.begin(115200);
  //attachInterrupt(digitalPinToInterrupt(D1), uartISR, CHANGE); 

  Serial.println("Starting...");
  Serial.print("Connecting to ");
  Serial.println(ssid);
 WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }
  Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, mqtt_port);
  

}



  



void loop() {
  // put your main code here, to run repeatedly:
 if (!client.connected()) {
      reconnect();
  }
  // if(stmSerial.available()>0){
  //      String receivedData = gpsSerial.readStringUntil('\n'); // Read the incoming data
  //      Serial.println("Received: " + receivedData); // Print the received data to the serial monitor
  //      Serial.println("Available\n");
  // }
  //Serial.println("Hello World\n");
  //client.loop();
  //long now = millis();

  while (stmSerial.available() > 0) {
   tim = stmSerial.readStringUntil(' ');
   latitude = stmSerial.readStringUntil(' ');
   longitude = stmSerial.readStringUntil(' ');
   dustValue = stmSerial.readStringUntil('\n');
   //received = true;
   String data = "{\"data\": {\"dustValue\" : " + dustValue + " , \"latitude\" : " + latitude + " , \"longitude\" : " + longitude + "}}";
    Serial.println(data);
    Serial.println("Test");
    data.toCharArray(msg, (data.length() + 1));
    
  }
  if(tim != "" && latitude != "" && longitude != "" && dustValue != ""){
    client.publish("@shadow/data/update",msg);
  }
  //Serial.println("Received");

  // if(received){
    
    //received = false;
  //}
  
  delay(2000);


}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection…");
        if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
            Serial.println("connected");
            //client.subscribe("@msg/led");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println("try again in 5 seconds");
            delay(5000);
        }
    }
}
