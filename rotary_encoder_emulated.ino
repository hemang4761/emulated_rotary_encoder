// Define the Arduino pins for the emulated encoder channels A and B
const int pinA = 8;
const int pinB = 9;

// Represents the Possible States of Our Rotary Encoder (Derived from the two square functions)
const int states[4][2] = {
  {LOW, LOW},   // Both Pins lose contact
  {HIGH, LOW},  // First Pin in Contact
  {HIGH, HIGH}, // Second Pin (Both) in Contact 
  {LOW, HIGH}   // First Pin looses contact
};

int encoderPosition = 0;


// Updates Pin States
void updatePins() {
  int currentStateA = states[encoderPosition][0];
  int currentStateB = states[encoderPosition][1];

  // Set the digital pins to the current state values
  digitalWrite(pinA, currentStateA);
  digitalWrite(pinB, currentStateB);

  // Print the two values separated by a comma for the Serial Plotter.
  Serial.print(currentStateA);
  Serial.print(",");
  Serial.println(currentStateB);
}

// One clockwise (CW) rotation
void rotateCW() {
  encoderPosition = (encoderPosition + 1) % 4;
  updatePins();
}

// One counter-clockwise (CCW) rotation
void rotateCCW() {
  encoderPosition = (encoderPosition - 1 + 4) % 4;
  updatePins();
}



void setup() {
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Send 1 for full CW cycle, 0 for full CCW cycle");

  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);

  // Startup Opacity (Resets down to 0, Sets to a starting opacity value depending on user needs) 
  updatePins();
  Serial.println("RESETTING TO 0");
  for (int i = 0; i < 300; i++) {
    rotateCCW();
    delay(5); 
      }
    Serial.println("STARTING VALUE");
  for (int i = 0; i < 200; i++) {
    rotateCW();
    delay(5); 
      }

}

void loop() {

    for (int i = 0; i < 59; i++) {
      rotateCW();
      delay(800); 
    }
    delay(15000);
    Serial.println("BACK DOWN");
    for (int i = 0; i < 59; i++) {
      rotateCCW();
      delay(10); 
      }
    delay(1000);
}
