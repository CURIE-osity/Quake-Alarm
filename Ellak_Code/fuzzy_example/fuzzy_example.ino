#include"FuzzyAlarm.h"

FuzzyAlarm fa;

float products[9];
float results[9];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println("Waiting for voltage (V)");
  while( Serial.available() == false) {}
  float voltage = Serial.parseFloat(SKIP_ALL, '\n');
  Serial.print("-- voltage: ");
  Serial.println(voltage);
  Serial.println("Waiting for duration (sec)");
  while( Serial.available() == false) {}
  float duration = Serial.parseFloat(SKIP_ALL, '\n');
  Serial.print("-- duration: ");
  Serial.println(duration);
  Serial.println("New Run");
  triVal a = fa.getIntensity(voltage);
  printIntensity(a);
  triVal b = fa.getDistance(duration);
  printDistance(b);
  fa.calculateProducts(products, a, b);
  printProducts();
  fa.calculateResults(products, results);
  printResults();
  float evacuation = fa.getEvacuation(products, results);
  printEvacuation(evacuation);
}

void printIntensity(triVal t) {
  Serial.println("Calculating Intensity");
  Serial.print("> Weak: ");
  Serial.println(t.a);
  Serial.print("> Moderate: ");
  Serial.println(t.b);
  Serial.print("> Strong: ");
  Serial.println(t.c);
  Serial.println("----------");
}

void printDistance(triVal t) {
  Serial.println("Calculating Distance");
  Serial.print("> Near: ");
  Serial.println(t.a);
  Serial.print("> Middle: ");
  Serial.println(t.b);
  Serial.print("> Far: ");
  Serial.println(t.c);
  Serial.println("----------");
}

void printProducts() {
  Serial.println("Calculating Products");
  for (int i=0; i<9; i++) {
    Serial.print("> P");
    Serial.print(i+1);
    Serial.print(" [ ");
    Serial.print(fa.getIntensityLabel(i));
    Serial.print(" - ");
    Serial.print(fa.getDensityLabel(i));
    Serial.print(" ] \t");
    Serial.print(products[i]);
    Serial.print(" (");
    Serial.print(fa.getEvacuationLabel(i));
    Serial.println(")");
  }
  Serial.println("----------");
}

void printResults() {
  Serial.println("Calculating Results");
  for (int i=0; i<9; i++) {
    Serial.print("> P");
    Serial.print(i+1);
    Serial.print(" [ ");
    Serial.print(fa.getIntensityLabel(i));
    Serial.print(" - ");
    Serial.print(fa.getDensityLabel(i));
    Serial.print(" ] \t");
    Serial.print(results[i]);
    if (results[i] == 0) {
      Serial.print(" (Ignored)");
    }
    Serial.println("");
  }
  Serial.println("----------");
}

void printEvacuation(float value) {
  Serial.print(">> Evacuation time: ");
  Serial.print(value);
  Serial.println("sec");
  Serial.println("==========");
}
