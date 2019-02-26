// Import required libraries
#include "WiFi.h"
#include <aREST.h>



const int trigPin1 = 2;
const int echoPin1 = 4;
const int trigPin2 = 18;
const int echoPin2 = 19;
const int trigPin3 = 27;
const int echoPin3 = 26;


long duration=0;
int distance1;
int distance2;
int distance3;

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "Praan";
const char* password = "nishu311";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
//int temperature;
//int humidity;

void setup(void)
{  
  // Start Serial
  Serial.begin(9600);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  // Init DHT 
  //dht.begin();
  
  // Init variables and expose them to REST API
  rest.variable("distance1",&distance1);
  rest.variable("distance2",&distance2);
  rest.variable("distance3",&distance3);
  
    
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("sensor_module");
  
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
  
}

void loop() {
  
  // Reading temperature and humidity
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration = pulseIn(echoPin1, HIGH);
  distance1 = (duration*.0343)/2;

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  duration = pulseIn(echoPin2, HIGH);
  distance2 = (duration*.0343)/2;

  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);

  duration = pulseIn(echoPin3, HIGH);
  distance3 = (duration*.0343)/2;
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}
