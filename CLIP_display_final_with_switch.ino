// PLDC pins
const int pinA = 4;
const int pinB = 5;
// LED Strip pins
const int orangePin = 3;
const int whitePin = 11;

// Physical switch pins
const int switchPinFast = 7; 
const int switchPinSlow = 8; 

// Experimentally Chosen minimum and maximum values for cycling
const int minVal = 1;
const int maxVal = 59;

// State identifiers
const int MODE_AMBIENT = 0;
const int MODE_FAST = 7;
const int MODE_SLOW = 8;

int currentMode = MODE_AMBIENT;

// Encoder state table
const int states[4][2] = {
  {LOW, LOW},   
  {HIGH, LOW},  
  {HIGH, HIGH}, 
  {LOW, HIGH}   
};

int encoderPosition = 0;

// Read the state (fast or slow)
int readPhysicalMode() {
  if (digitalRead(switchPinFast) == LOW) return MODE_FAST;
  if (digitalRead(switchPinSlow) == LOW) return MODE_SLOW;
  return MODE_AMBIENT;
}

// custom delay function to allow for interrupting a cycle
bool isInterrupted(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (readPhysicalMode() != currentMode) {
      return true; 
    }
    delay(1); 
  }
  return false;
}

/*
  GENERAL FUNCTIONS (UPDATE PINS & ROTATION)
*/  
void updatePins() {
  digitalWrite(pinA, states[encoderPosition][0]);
  digitalWrite(pinB, states[encoderPosition][1]);
}

void rotateCW() {
  encoderPosition = (encoderPosition + 1) % 4;
  updatePins();
}

void rotateCCW() {
  encoderPosition = (encoderPosition - 1 + 4) % 4;
  updatePins();
}

// Startup calibration
void calibrate(){
  for (int i = 0; i < 300; i++) { rotateCCW(); delay(5); }
  for (int i = 0; i < 200; i++) { rotateCW(); delay(5); }
  
  // Initial Ambient State
  analogWrite(orangePin, 30);
  analogWrite(whitePin, 220);
}

// setup loop for calibration and startup
void setup() {
  Serial.begin(9600);
  
  pinMode(orangePin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);

  pinMode(switchPinFast, INPUT_PULLUP);
  pinMode(switchPinSlow, INPUT_PULLUP);

  updatePins();
  calibrate();
  currentMode = readPhysicalMode();

  
}

// continuous loop
void loop() {
  int newMode = readPhysicalMode();
  if (newMode != currentMode) {
    calibrate();
    currentMode = newMode;
  }

  // each mode for the case
  switch (currentMode) {
    case MODE_FAST:
      fastCycle();
      break;
    case MODE_SLOW:
      slowCycle();
      break;
    default:
      ambientCycle();
      break;
  }
}

/*
  FUNCTIONS FOR TIME CYCLES (FAST, SLOW, AMBIENT)
  loops from minValue to maxValue, increasing and decreasing the orange and white led duty cycles inversely
*/

void fastCycle() {
  Serial.println("Mode: FAST");
  analogWrite(orangePin, 0);
  analogWrite(whitePin, 255);
  if (isInterrupted(1000)) return;
  
  for (int i = 0; i < 59; i++) {
    rotateCW();
    float x_orange = (float)(i - minVal) / (maxVal - minVal); 
    int pwmOrange = 255 * pow(x_orange, 2.2); 
    analogWrite(orangePin, pwmOrange);

    if (i > 34) {
      int startPwm = 255 - 10 * (i - 34 - 1);
      for (int k = 1; k <= 5; k++) {
         analogWrite(whitePin, startPwm - (k * 2));
         if (isInterrupted(160)) return; 
      }
    } else {
      analogWrite(whitePin, 255);
      if (isInterrupted(800)) return; 
    }
  }

  if (isInterrupted(6500)) return;
  
  Serial.println("BACK DOWN");
  for (int i = 59; i > 0; i--) {
    rotateCCW();
    float x_orange = (float)(i - minVal) / (maxVal - minVal);
    float x_white = 1.0 - x_orange; 
    analogWrite(orangePin, 255 * pow(x_orange, 2.2));
    analogWrite(whitePin, 255 * pow(x_white, 2.2));
    if (isInterrupted(25)) return; 
  }
}

void slowCycle() {
  Serial.println("Mode: SLOW (3 Hours)");
  const float m = 197.414; 
  
  analogWrite(orangePin, 0);
  analogWrite(whitePin, 255);
  if (isInterrupted(1000 * m)) return;
  
  for (int i = 0; i < 59; i++) {
    rotateCW();
    float x_orange = (float)(i - minVal) / (maxVal - minVal); 
    analogWrite(orangePin, 255 * pow(x_orange, 2.2));

    if (i > 34) {
      int startPwm = 255 - 10 * (i - 34 - 1);
      for (int k = 1; k <= 5; k++) {
         analogWrite(whitePin, startPwm - (k * 2));
         if (isInterrupted(160 * m)) return; 
      }
    } else {
      analogWrite(whitePin, 255);
      if (isInterrupted(800 * m)) return; 
    }
  }

  if (isInterrupted(1283189)) return; 

  Serial.println("BACK DOWN");
  for (int i = 59; i > 0; i--) {
    rotateCCW();
    float x_orange = (float)(i - minVal) / (maxVal - minVal);
    float x_white = 1.0 - x_orange; 
    analogWrite(orangePin, 255 * pow(x_orange, 2.2));
    analogWrite(whitePin, 255 * pow(x_white, 2.2));
    if (isInterrupted(25)) return; 
  }
}

void ambientCycle() {
  static int currentOrange = 30;
  static int currentWhite = 220;
  
  Serial.println("Mode: AMBIENT");
  analogWrite(orangePin, currentOrange);
  analogWrite(whitePin, currentWhite);
  if (isInterrupted(10000)) return;
  // int randNumber = random(10); // 0 to 9

  // // 1/10 Chance: Clouds passing (Opaque pulse)
  // if (randNumber == 0) {
  //   Serial.println("Ambient: Clouds Passing");
  //   for (int i = 0; i < 30; i++) {
  //     rotateCW();
  //     currentWhite = max(0, currentWhite - 4); // Quick darkening
  //     analogWrite(whitePin, currentWhite);
  //     if (isInterrupted(40)) return;
  //   }
  //   if (isInterrupted(500)) return; // Brief hold at peak opacity
  //   for (int i = 0; i < 30; i++) {
  //     rotateCCW();
  //     currentWhite = min(220, currentWhite + 4); 
  //     analogWrite(whitePin, currentWhite);
  //     if (isInterrupted(40)) return;
  //   }
  }
  
  // // 1/10 Chance: Sunny (Orange bloom)
  // else if (randNumber == 1) {
  //   Serial.println("Ambient: Getting Sunny");
  //   // Transition to Sunny
  //   for (int i = 0; i < 50; i++) {
  //     currentOrange = min(120, currentOrange + 2); // Increase orange
  //     analogWrite(orangePin, currentOrange);
  //     if (isInterrupted(100)) return; 
  //   }
    
  //   // Stay sunny for a while (e.g., 30 seconds)
  //   if (isInterrupted(30000)) return; 
    
  //   // Transition back to normal
  //   for (int i = 0; i < 50; i++) {
  //     currentOrange = max(30, currentOrange - 2);
  //     analogWrite(orangePin, currentOrange);
  //     if (isInterrupted(100)) return; 
  //   }
  // }
//}
