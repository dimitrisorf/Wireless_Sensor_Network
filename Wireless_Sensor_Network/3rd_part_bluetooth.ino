// Brightness and Fire

int photoPin = A0;

void setup() {
  pinMode(2, INPUT); //initialize Flame sensor output pin connected pin as input.
  pinMode(LED_BUILTIN, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);// initialize serial communication @ 9600 baud:
}

void loop() {
  // PHOTORESISTOR
  int light = analogRead(photoPin);
  Serial.print("Photoresistor value: ");
  Serial.println(light);
  delay(100);
  if (light <=  150) {
    Serial.println("***Too Dark***");
  } 
  if (light >  150) {
    Serial.println("Perfect Sun!");
  } 
  Serial.println();

  // FIRE SENSOR
  int fire = digitalRead(2);
  if (fire == 0 )
  {
    digitalWrite(LED_BUILTIN, HIGH); // Led ON
    Serial.println("***Fire detected***");
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW); // Led OFF
    Serial.println("No Fire detected!");
  }
  Serial.println("");
  delay(2000);
}
