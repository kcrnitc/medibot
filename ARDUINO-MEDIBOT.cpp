#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 4
#define RST_PIN 5

#define IR_SENSOR_1 A0
#define IR_SENSOR_2 A1
#define IR_SENSOR_3 A2

#define MOTOR_PIN_1 2
#define MOTOR_PIN_2 3
#define MOTOR_PIN_3 4

#define MOTOR_SPEED 255

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverAddress = "YOUR_SERVER_ADDRESS";
const int serverPort = 80;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(IR_SENSOR_1, INPUT);
  pinMode(IR_SENSOR_2, INPUT);
  pinMode(IR_SENSOR_3, INPUT);
  
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(MOTOR_PIN_3, OUTPUT);
  analogWrite(MOTOR_PIN_1, MOTOR_SPEED);
  analogWrite(MOTOR_PIN_2, MOTOR_SPEED);
  analogWrite(MOTOR_PIN_3, MOTOR_SPEED);

  connectToWiFi();
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String username = readCardData(); // Read username from card data
    String response = getRequest("/lookup?username=" + username); // Send HTTP request to server
    
    int springToRotate = parseResponse(response); // Parse response to get spring-to-rotate value
    
    rotateSpring(springToRotate); // Rotate the specified spring
  }
}

String readCardData() {
  String username = ""; // Placeholder for username
  // Read username from RFID card data
  return username;
}

String getRequest(String endpoint) {
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed");
    return "";
  }
  
  Serial.println("Connected to server");
  client.print(String("GET ") + endpoint + " HTTP/1.1\r\n" +
               "Host: " + serverAddress + "\r\n" +
               "Connection: close\r\n\r\n");
  
  delay(100);
  
  String response = "";
  while (client.available()) {
    response += char(client.read());
  }
  client.stop();
  
  return response;
}

int parseResponse(String response) {
  // Parse response and extract spring-to-rotate value
  int springToRotate = 1; // Example value
  return springToRotate;
}

void rotateSpring(int springToRotate) {
  if (springToRotate == 1) {
    analogWrite(MOTOR_PIN_1, 0); // Stop motor 1
  } else if (springToRotate == 2) {
    analogWrite(MOTOR_PIN_2, 0); // Stop motor 2
  } else if (springToRotate == 3) {
    analogWrite(MOTOR_PIN_3, 0); // Stop motor 3
  }
}

void connectToWiFi() {
  delay(10);
  Serial.println();
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
}
