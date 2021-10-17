#include "Arduino.h"
#include "FuzzyAlarm.h"

/*
 * Fuzzy class funtions
 */ 
FuzzyAlarm::FuzzyAlarm() {
  initVars();
}

void FuzzyAlarm::initVars() {
  _ix1 = 0.5;
  _ix2 = 1;
  _ix3 = 1.5;
  _iy = 1;

  _dx1 = 10;
  _dx2 = 20;
  _dx3 = 30;
  _dy = 1;

  _ex1 = 5;
  _ex2 = 10;
  _ex3 = 15;
  _ey = 1;

  _factors[0] = _ex2;
  _factors[1] = _ex3;
  _factors[2] = _ex3;
  _factors[3] = _ex1;
  _factors[4] = _ex1;
  _factors[5] = _ex2;
  _factors[6] = _ex1;
  _factors[7] = _ex1;
  _factors[8] = _ex2;
}

void FuzzyAlarm::setIntensityVars(float x1, float x2, float x3, float y) {
  
}

void FuzzyAlarm::setDensityVars(float x1, float x2, float x3, float y) {

}

void FuzzyAlarm::setEvacuationVars(float x1, float x2, float x3, float y) {

}

float FuzzyAlarm::calculateFactor(float x1, float y1, float x2, float y2) {
  return (y2-y1)/(x2-x1);
}

triVal FuzzyAlarm::getIntensity(float voltage) {
  triVal retVal;
  if (voltage >= 0 && voltage <= _ix1) retVal.a = _iy;
  if (voltage > _ix1 && voltage < _ix2) retVal.a = -2 * voltage + 2;
  if (voltage > _ix1 && voltage <= _ix2) retVal.b = 2 * voltage - 1;
  if (voltage > _ix2 && voltage < _ix3) retVal.b = -2 * voltage + 3;
  if (voltage > _ix2 && voltage <= _ix3) retVal.c = 2 * voltage - 2;
  if (voltage > _ix3) retVal.c = _iy;
  return retVal;
}

triVal FuzzyAlarm::getDistance(float seconds) {
  triVal retVal;
  if (seconds >= 0 && seconds <= _dx1) retVal.a = _iy;
  if (seconds > _dx1 && seconds < _dx2) retVal.a = -0.1 * seconds + 2;
  if (seconds > _dx1 && seconds <= _dx2) retVal.b = 0.1 * seconds - 1;
  if (seconds > _dx2 && seconds < _dx3) retVal.b = -0.1 * seconds + 3;
  if (seconds > _dx2 && seconds <= _dx3) retVal.c = 0.1 * seconds - 2;
  if (seconds > _dx3) retVal.c = _dy;
  return retVal;
}

void FuzzyAlarm::calculateProducts(float* results, triVal intensity, triVal distance) {
  results[0] = (intensity.a < distance.a) ? intensity.a : distance.a;
  results[1] = (intensity.a < distance.b) ? intensity.a : distance.b;
  results[2] = (intensity.a < distance.c) ? intensity.a : distance.c;
  results[3] = (intensity.b < distance.a) ? intensity.b : distance.a;
  results[4] = (intensity.b < distance.b) ? intensity.b : distance.b;
  results[5] = (intensity.b < distance.c) ? intensity.b : distance.c;
  results[6] = (intensity.c < distance.a) ? intensity.c : distance.a;
  results[7] = (intensity.c < distance.b) ? intensity.c : distance.b;
  results[8] = (intensity.c < distance.c) ? intensity.c : distance.c;
}

String FuzzyAlarm::getIntensityLabel(int index) {
  if (index < 3) return "W";
  if (index < 6) return "M";
  return "S";
}

String FuzzyAlarm::getDensityLabel(int index) {
  if (index == 0 || index == 3 || index == 6) return "N";
  if (index == 1 || index == 4 || index == 7) return "M";
  return "F";
}

String FuzzyAlarm::getEvacuationLabel(int index) {
  if (_factors[index] == _ex1) return "Fast";
  if (_factors[index] == _ex2) return "Average";
  if (_factors[index] == _ex3) return "Slow";
}

float FuzzyAlarm::getFactor(int index) {
  return _factors[index];
}

void FuzzyAlarm::calculateResults(float* products, float* results) {
  for (int i=0; i<9; i++) {
    results[i] = products[i] * _factors[i];
  }
}

float FuzzyAlarm::getEvacuation(float* products, float* results) {
  float top = 0, bot = 0;
  for (int i=0; i<9; i++) {
    top += results[i];
    bot += products[i];
  }
  return top / bot;
}
