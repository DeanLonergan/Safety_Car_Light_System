#define ST_INIT           0

#define ST_LIGHTBAR_OFF   1
#define ST_LIGHTBAR_ON_Y  2
#define ST_LIGHTBAR_ON_G  3
#define ST_LIGHTBAR_ON_R  4

#define ST_FR_LIGHTS_OFF  1
#define ST_FR_LIGHTS_ON   2

#define FR_LIGHTS_PTNM_IN A0

#define LGTBR_SW_IN      A1

#define LGTBR_RED_BN_IN   A2
#define LGTBR_GRN_BN_IN   A3
#define LGTBR_YLW_BN_IN   A4

#define FR_LIGHTS_OUT     13

#define LGTBR_OUT       12
#define RGB_RED_OUT       6
#define RGB_BLU_OUT       5
#define RGB_GRN_OUT       3

#define ON          1
#define OFF         0
#define MAX         255
#define MIN         0

unsigned long currentTime;
unsigned long previousTime;

int currentState_FR_LIGHTS;
int currentState_LIGHTBAR;
int sensorValue;
int flashInterval;
int flashState;
int redButton;
int greenButton;
int yellowButton;
int lightbarSwitch;

void setup(){
  currentState_FR_LIGHTS = ST_INIT;
  currentState_LIGHTBAR = ST_INIT;
  
  pinMode(FR_LIGHTS_PTNM_IN, INPUT);
  pinMode(LGTBR_SW_IN, INPUT);
  pinMode(LGTBR_RED_BN_IN, INPUT_PULLUP);
  pinMode(LGTBR_GRN_BN_IN, INPUT_PULLUP);
  pinMode(LGTBR_YLW_BN_IN, INPUT_PULLUP);
  
  pinMode(FR_LIGHTS_OUT, OUTPUT);
  pinMode(LGTBR_OUT, OUTPUT);
  pinMode(RGB_RED_OUT, OUTPUT);
  pinMode(RGB_BLU_OUT, OUTPUT);
  pinMode(RGB_GRN_OUT, OUTPUT);
  
  currentState_FR_LIGHTS = ST_FR_LIGHTS_OFF;
  currentState_LIGHTBAR = ST_LIGHTBAR_OFF;
}

void loop(){
  currentTime = millis();
  sensorValue = analogRead(FR_LIGHTS_PTNM_IN);
  lightbarSwitch = digitalRead(LGTBR_SW_IN);
  redButton = !digitalRead(LGTBR_RED_BN_IN);
  greenButton = !digitalRead(LGTBR_GRN_BN_IN);
  yellowButton = !digitalRead(LGTBR_YLW_BN_IN);
  
  //Front-Rear Lights - State 1//
  if (currentState_FR_LIGHTS == ST_FR_LIGHTS_OFF) {
    if (sensorValue > 100) {
      currentState_FR_LIGHTS = ST_FR_LIGHTS_ON;
    } else {
      digitalWrite(FR_LIGHTS_OUT, LOW);
    }
  }
  
  //Front-Rear Lights - State 2//
  if (currentState_FR_LIGHTS == ST_FR_LIGHTS_ON) {
    if (sensorValue < 100) {
      currentState_FR_LIGHTS = ST_FR_LIGHTS_OFF;
    } else {
      if ((currentTime - previousTime) >= flashInterval) {
        previousTime = currentTime;
        if (flashState == OFF) {
          flashState = ON;
        } else {
          flashState = OFF;
        }
      }
      digitalWrite(FR_LIGHTS_OUT, flashState);
    }
    if (sensorValue > 100 && sensorValue < 260) {
      flashInterval = 300;
    } else if (sensorValue > 260 && sensorValue < 430) {
      flashInterval = 250;
    } else if (sensorValue > 430 && sensorValue < 600) {
      flashInterval = 200;
    } else if (sensorValue > 600 && sensorValue < 780) {
      flashInterval = 150;
    } else if (sensorValue > 780 && sensorValue < 1000) {
      flashInterval = 100;
    } else if (sensorValue > 1000){
      flashInterval = 0;
    }
  }
  
  //Lightbar - State 1//
  if (currentState_LIGHTBAR == ST_LIGHTBAR_OFF) {
    if (lightbarSwitch) {
      currentState_LIGHTBAR = ST_LIGHTBAR_ON_Y;
    } else {
      digitalWrite(LGTBR_OUT, OFF);
      analogWrite(RGB_RED_OUT, MIN);
      analogWrite(RGB_BLU_OUT, MIN);
      analogWrite(RGB_GRN_OUT, MIN);
    }
  }
  
  //Lightbar - State 2//
  if (currentState_LIGHTBAR == ST_LIGHTBAR_ON_Y) {
    digitalWrite(LGTBR_OUT, ON);
    analogWrite(RGB_RED_OUT, MAX);
    analogWrite(RGB_BLU_OUT, MIN);
    analogWrite(RGB_GRN_OUT, MAX);
    if (redButton) {
      currentState_LIGHTBAR = ST_LIGHTBAR_ON_R;
    }
    if (greenButton) {
      currentState_LIGHTBAR = ST_LIGHTBAR_ON_G;
    } 
    if (!lightbarSwitch) {
      currentState_LIGHTBAR = ST_LIGHTBAR_OFF;
    } 
  }
  
  //Lightbar - State 3//
  if (currentState_LIGHTBAR == ST_LIGHTBAR_ON_G) {
    analogWrite(RGB_RED_OUT, MIN);
    analogWrite(RGB_BLU_OUT, MIN);
    analogWrite(RGB_GRN_OUT, MAX);
    if (redButton) {
      currentState_LIGHTBAR = ST_LIGHTBAR_ON_R;
    }
    if (yellowButton) {
      currentState_LIGHTBAR = ST_LIGHTBAR_ON_Y;
    }
    if (!lightbarSwitch) {
      currentState_LIGHTBAR = ST_LIGHTBAR_OFF;
    }
  }
  
  //Lightbar - State 4//
  if (currentState_LIGHTBAR == ST_LIGHTBAR_ON_R) {
    analogWrite(RGB_RED_OUT, MAX);
    analogWrite(RGB_BLU_OUT, MIN);
    analogWrite(RGB_GRN_OUT, MIN);
    if (greenButton) {
      currentState_LIGHTBAR = ST_LIGHTBAR_ON_G;
    }
    if (yellowButton) {
      currentState_LIGHTBAR = ST_LIGHTBAR_ON_Y;
    } 
    if (!lightbarSwitch) {
      currentState_LIGHTBAR = ST_LIGHTBAR_OFF;
    }
  }
}
