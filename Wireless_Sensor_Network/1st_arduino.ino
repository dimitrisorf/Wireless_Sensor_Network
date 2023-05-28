// CLIENT_1

#include <WiFiNINA.h>
#include <WiFi.h>

const char* ssid = "Dimitris";
const char* password = "dimitris0510";
const char* host = "192.168.43.152"; // Change this to the IP address of your server
IPAddress serverIP(192, 168, 43, 152); // IP address of the server board
const int numValuesToSend = 1; // stathera 1: ousiastika auth h timh einai to numValuesSent
float averageResponseTime;
float throughput;
float throughput_bytes;
float packetLoss;
int photoPin = A0;

WiFiClient client;
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
    delay(10000);
  }

  Serial.println("Connected to wifi");
  Serial.println(WiFi.localIP());

  // connect to the server
  if (client.connect(serverIP, 80)) {
    Serial.println("Connected to Server 2");
  } else {
    Serial.println("Connection to Server 2 failed");
  }
}

void loop() {
  unsigned long startMillis = millis();
  int numValuesSent = 0;
  int numValuesReceived = 0;
  float totalResponseTime = 0;
  int sizebytes = 0;

  // Check WiFi connection status
  int status = WiFi.status();
  if (status != WL_CONNECTED) {
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.println("Lost internet connection!");
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
    while (1); // halt the program
  }
 
  for (int i = 0; i < numValuesToSend; i++) {
    if (client.connect(host, 80)) {

      unsigned long endMillis = millis();
      unsigned long responseTime = endMillis - startMillis;
      totalResponseTime += responseTime;

      while (client.connected()) {
        int light = analogRead(photoPin);
        // Serial.println(light);

        String mess = String(light);
        size_t size = sizeof(mess);
        sizebytes = sizebytes + size;

        int sentBytes = client.write(mess.c_str(), mess.length());
        if (sentBytes == mess.length()) {
          // Serial.println("Message sent successfully");
          numValuesSent++;
          Serial.print("Value: ");
          Serial.println(mess);
         } else {
          Serial.println("Message sending failed");
        }

        String line = client.readStringUntil('\n');
        if (line == nullptr){
          Serial.println("No message received from Server 1!!!");
        } else{
        Serial.print("Received message from Server 1: ");
        Serial.println(line);          
        }

        if (line == nullptr) {
        } else{
          numValuesReceived++;
        }
       
        delay(4000); // DELAY BETWEEN THE PACKETS
        if (line == "\r") {
          break;
        }
      }

      client.stop();
    }
    else {
      Serial.println("Connection to server failed");
    }

    Serial.println();
    Serial.print("Total bytes: ");
    Serial.println(sizebytes);

    delay(1000); // Wait 1 second before sending the next value
  }

  if (numValuesSent > 0) {
    averageResponseTime = totalResponseTime / numValuesSent;
    throughput = numValuesSent / averageResponseTime;
    throughput_bytes = throughput * sizebytes / numValuesSent;
    packetLoss = 1 - ((float)numValuesReceived / numValuesSent);

    Serial.print("Sent ");
    Serial.print(numValuesSent);
    Serial.print(" values, Received ");
    Serial.print(numValuesReceived);
    Serial.print(" values, Average Response Time: ");
    Serial.print(averageResponseTime);
    Serial.print(" ms, Throughput: ");
    Serial.print(throughput);
    Serial.print(" values/sec, or ");
    Serial.print(throughput_bytes);
    Serial.print(" bytes/sec, Packet Loss: ");
    Serial.println(packetLoss);
    Serial.println();
    Serial.println("----------------------------------------------------------------------------------------------------------------------------------------------");
    Serial.println();
  }
}
