int buzzer = 2;           // pin for buzzer
int alt = 3;              // pin for red leds
int go = 4;               // pin for green leds
int pilotLight = 5;       // pin for blue & white leds
int pirSensor = 6;        // pin for PIR sensor

int motor1 = A0;          // analog pin for first motor
int motor2 = A1;          // analog pin for second motor
int motor3 = A2;          // analog pin for third motor
int motor4 = A3;          // analog pin for fourth motor
int zAxes = A6;           // analog pin for zAxes
const int zTimes = 10;
int zVals[zTimes];

// values to read analog ports
int pirVal, motor1Val, motor2Val, motor3Val, motor4Val, zAxesVal, totalVal;     

// sensitivity values for motors and z Axes. Close to 0 means very sensitive.
int motorSensitivity = 4;
int zSensitivity = 5;

/* string variable indicating the system state
 * possible states: idle, green, red
 */
String state = "idle";

void setup() {
  // initialize serial monitoring
  Serial.begin(9600);
  
  // initializing pinModes
  pinMode(buzzer,OUTPUT);
  pinMode(alt,OUTPUT);
  pinMode(go,OUTPUT);
  pinMode(pilotLight,OUTPUT);

  pinMode(pirSensor,INPUT);
  pinMode(motor1,INPUT);
  pinMode(motor2,INPUT);
  pinMode(motor3,INPUT);
  pinMode(motor4,INPUT);
  pinMode(zAxes,INPUT);
  
  // turn off the leds
  digitalWrite(alt,HIGH);
  digitalWrite(go,HIGH);
  digitalWrite(buzzer,HIGH);
  digitalWrite(pilotLight,LOW);

  for (int i=0; i<zTimes; i++) {
    zVals[i] = analogRead(zAxes);
  }
}

void loop() {
  zPush();
  /*
   * check if the earthquake has started
   */
  if(state == "idle" && isMoving()){
    redAlarm();
    delay(1000);
    for (int i=0; i<zTimes; i++) {
      zVals[i] = analogRead(zAxes);
    }
  }
  
  /*
   * check if the earthquake is active
   */
  else if(state == "red"){
    if(zMovement()){
      // wait until the earthquake is over
      while(zMovement()){
        for(int i=0; i<4; i++) {
          zPush();
          delay(500);
        }
      }
      // wait a bit more for aftershocks
      Serial.println("No moving. Waiting for aftershocks");
      for(int i=0; i<10; i++) {
        zPush();
        delay(500);
      }
      // if there is no moving, trigger green alarm
      if(zMovement() == false){
        greenAlarm();
      }
    } else {
      noAlarm();
    }
  }
  
  /* 
   * check if the earthquake is over 
   */
  if(state == "green"){
    // turn on and off the buzzer every 0.5 seconds for 10 seconds
    Serial.println("Waiting for 10 seconds on green state");
    for(int i = 0; i < 20; i++){
      toggleBuzzer(i);
      delay(500);
    }
    // continue turning on and off the buzzer while the PIR sensor detects movement
    Serial.println("Checking for PIR movement every 5 seconds.");
 
    while(digitalRead(pirVal) == HIGH){
      Serial.println("movement detection.");
      for(int i = 0; i < 10; i++){
        toggleBuzzer(i);
        delay(500);
      }
    }
    noAlarm();
  }

  delay(1000);
}

/*
 * Function isMoving:
 * checks if the motors are moving
 * this means that the earthquake has started
 */
bool isMoving(){
  motor1Val = analogRead(motor1);
  motor2Val = analogRead(motor2);
  motor3Val = analogRead(motor3);
  motor4Val = analogRead(motor4);

  totalVal = motor1Val + motor2Val + motor3Val + motor4Val;
  
  movingReport(motor1Val, motor2Val, motor3Val, motor4Val, totalVal);
  
  if(totalVal > motorSensitivity){
    return true;
  }else{
    return false;
  }
}

/**
 * Function zMovement
 * check if there is any movement on z axes
 */
bool zMovement() {
  float dZ = abs(getZAvg() - analogRead(zAxes));

  if (dZ > zSensitivity) {
    return true;
  } else {
    return false; 
  }
}

/**
 * Function movingReport
 */
void movingReport(int motor1Val, int motor2Val, int motor3Val, int motor4Val, int totalVal) {
  Serial.print("State: ");
  Serial.print(state);
  Serial.print(" [M1:");
  Serial.print(motor1Val);
  Serial.print(" M2:");
  Serial.print(motor2Val);
  Serial.print(" M3:");
  Serial.print(motor3Val);
  Serial.print(" M3:");
  Serial.print(motor4Val);
  Serial.print("] total ");
  Serial.print(totalVal);
  if(totalVal>4){
    Serial.println(" Moving");
  }else{
    Serial.println(" Not moving");
  }
}

/*
 * Function redAlarm:
 * turn on the red LEDS and the buzzer
 * gets triggered when the earthquake starts
 */
void redAlarm(){
  Serial.println("Earthquake detected!");
  state = "red";
  digitalWrite(alt,LOW);
  digitalWrite(buzzer,LOW);
}

/*
 * Function greenAlarm:
 * turn off the red LEDS, turn on the green LEDS 
 * gets triggered when the earthquake ends
 */
void greenAlarm(){
  Serial.println("Earthquake is over. Please evacuate.");
  state = "green";
  digitalWrite(alt,HIGH);
  digitalWrite(go,LOW);
}

/*
 * Function noAlarm:
 * turns off everything
 * gets triggered when PIR sensor doesn't detect movements
 */
void noAlarm(){
  Serial.println("Back to normal.");
  state = "idle";
  digitalWrite(alt,HIGH);
  digitalWrite(go,HIGH);
  digitalWrite(buzzer,HIGH);
}

/*
 * Function toggleBuzzer:
 * toggles the buzzer according to repeat value
 */
void toggleBuzzer(int repeat){
  if(repeat%2 == 0){
    digitalWrite(buzzer,LOW);
  }else{
    digitalWrite(buzzer,HIGH);
  }
}

/**
 * Function zPush
 * pushes a new z value in zVals array
 */
void zPush() {
  for(int i=0; i<zTimes-1; i++) {
    zVals[i] = zVals[i+1];
  }
  zVals[zTimes-1] = analogRead(zAxes);
}

/**
 * Function getZAvg
 * returns the average of the zVals array
 */
float getZAvg() {
  int sum = 0;
  for(int i=0; i<zTimes; i++) {
    sum += zVals[i];
  }
  return sum/zTimes;
}
