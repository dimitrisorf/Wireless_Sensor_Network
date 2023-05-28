// SERVER_1
// CLIENT_2

#include <WiFiNINA.h>
#include <WiFi.h>

const char* ssid = "Dimitris";
const char* password = "dimitris0510";
const int serverPort = 80;
const int serverPort2 = 99;

const char* host = "192.168.43.51"; // Change this to the IP address of your server
IPAddress serverIP(192, 168, 43, 51); // IP address of the server board
const int numValuesToSend = 1; // Change this to the number of values you want to send
float averageResponseTime;
float throughput;
float throughput_bytes;
float packetLoss;

WiFiServer server(serverPort);
WiFiClient client2;
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

  // start the server
  server.begin();
  Serial.println("I (Server 1) started");

  // connect to the server
  if (client2.connect(serverIP, 99)) {
    Serial.println("Connected to Server 2");
  } else {
    Serial.println("Connection to Server 2 failed");
  }
}

void loop() {
  WiFiClient client = server.available();

  // Check WiFi connection status
  int status = WiFi.status();
  if (status != WL_CONNECTED) {
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.println("Lost internet connection!");
    Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!");
    while (1); // halt the program
  }

  if (client) {
    Serial.println("Client 1 connected");
    Serial.println();

    int numValuesSent = 0;
    int numValuesReceived = 0;
    float totalResponseTime = 0; // Total: (numValuesSent) values
    int sizebytes = 0;

    while (client.connected()) {
      unsigned long startMillis = millis();

      if (client.available()) {
        if (client2.connect(host, 99)) {
          unsigned long endMillis = millis();
          unsigned long responseTime = endMillis - startMillis;
          totalResponseTime += responseTime;

          String message = client.readStringUntil('\n');
          Serial.print("Message from Client 1: ");
          Serial.println(message);
          client2.println(message);

          size_t size = sizeof(message);
          sizebytes = sizebytes + size;
          int sentBytes = client.write(message.c_str(), message.length());
          if (sentBytes == message.length()) {
            // Serial.println("Message sent successfully");
            numValuesSent++;
          } else {
            Serial.println("Message sending failed");
          }

          // send a response back to the client
          // client.print("OK\r\n");
          // client.println("Received message: " + message);
          // client.println(message); -----------------------------------------
          String line2 = client2.readStringUntil('\n');

          if (line2 == nullptr) {
            Serial.println("No message received from Server 2!!!");
          } else{
            Serial.println(line2);
            numValuesReceived++;
          }
        }
      }

      if (numValuesReceived >= 10) { // Change this to the number of values you expect to receive
        break;
      }
    }

    client.stop();

    Serial.println();    
    Serial.print("Total bytes: ");
    Serial.println(sizebytes);

    Serial.print("Received ");
    Serial.print(numValuesReceived);
    Serial.println(" values from Server 2");

    if (numValuesSent > 0) {
    averageResponseTime = totalResponseTime / numValuesSent; // Average: for 1 value
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
    Serial.println("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    Serial.println();
    }

  }

}