const int pinA = 4;
const int pinB = 5;
const int orangePin = 3;
const int whitePin = 11;
const int minVal = 1;
const int maxVal = 59;
const int state1 = 7;
const int state2 = 8;

// possible states of rotary encoder
const int states[4][2] = {
  {LOW, LOW},   // no contact
  {HIGH, LOW},  // pinA contact
  {HIGH, HIGH}, // pinA & pinB contact 
  {LOW, HIGH}   // pinB contact
};

int encoderPosition = 0;


void updatePins() {
  int currentStateA = states[encoderPosition][0];
  int currentStateB = states[encoderPosition][1];

  // Set pins to the current state values
  digitalWrite(pinA, currentStateA);
  digitalWrite(pinB, currentStateB);

  // for visualization purposes, prints to the serial plotter
  Serial.print(currentStateA);
  Serial.print(",");
  Serial.println(currentStateB);
}

// one CW rotation
void rotateCW() {
  encoderPosition = (encoderPosition + 1) % 4;
  updatePins();
}

// One CCW rotation
void rotateCCW() {
  encoderPosition = (encoderPosition - 1 + 4) % 4;
  updatePins();
}



void setup() {
  Serial.begin(9600);
  Serial.println("Send 1 for full CW cycle, 0 for full CCW cycle");

  pinMode(orangePin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  
  // Set initial states to match the start of the loop
  // Orange starts at 0 (min), White starts at 255 (max)


  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);

  // Startup Opacity (Resets down to 0, Sets to a starting opacity value); these values were chosen for the purposes of this project but can be adjusted easily
  updatePins();
  for (int i = 0; i < 300; i++) {
    rotateCCW();
    delay(5); 
      }
  for (int i = 0; i < 200; i++) {
    rotateCW();
    delay(5); 
      }
    analogWrite(orangePin, 0);
    analogWrite(whitePin, 255);

}

void loop() {
    analogWrite(orangePin, 0);
    analogWrite(whitePin, 255);
    delay(1000);
    
    for (int i = 0; i < 59; i++) {
      rotateCW();
      // Calculate the normalized progress (0.0 to 1.0)
      float x_orange = (float)(i - minVal) / (maxVal - minVal); 
    
      // Calculate the inverse for white (1.0 to 0.0)
      float x_white = 1.0 - x_orange; 

      // Calculate PWM using Gamma Correction for Orange
      int pwmOrange = 255 * pow(x_orange, 2.2); 
      analogWrite(orangePin, pwmOrange);

      if (i > 34){
        int val = i - 34;
        // Calculate the value we are starting FROM (the value at the end of the previous loop)
        int startPwm = 255 - 10 * (val - 1);

        // Run 5 smaller steps instead of 1 big step
        // 800ms total delay / 5 steps = 160ms per step
        // 10 PWM drop total / 5 steps = 2 PWM drop per step
        for (int k = 1; k <= 5; k++) {
           analogWrite(whitePin, startPwm - (k * 2));
           delay(160); 
        }
      }
      else{
        analogWrite(whitePin, 255);
        delay(800); 
      }
    }

    delay(6500);
    Serial.println("BACK DOWN");
    for (int i = 59; i > 0; i--) {
      rotateCCW();
      // x_orange goes from 1.0 to 0.0 here
      float x_orange = (float)(i - minVal) / (maxVal - minVal);
    
      // x_white goes from 0.0 to 1.0 here
      float x_white = 1.0 - x_orange; 

      int pwmOrange = 255 * pow(x_orange, 2.2);
      int pwmWhite = 255 * pow(x_white, 2.2);

      analogWrite(orangePin, pwmOrange);
      analogWrite(whitePin, pwmWhite);
      delay(25); 
      }
    
}
