/*
 * 
 */

#ifndef FuzzyAlarm_h
#define FuzzyAlarm_h

  #include "Arduino.h"

  typedef struct trival {
    float a = 0, b = 0, c = 0;
  } triVal;
  
  class FuzzyAlarm {
    public:
      FuzzyAlarm();

      void setIntensityVars(float x1, float x2, float x3, float y);
      void setDensityVars(float x1, float x2, float x3, float y);
      void setEvacuationVars(float x1, float x2, float x3, float y);
      
      triVal getIntensity(float voltage);
      triVal getDistance(float seconds);
      void calculateProducts(float* results, triVal intensity, triVal distance);
      void calculateResults(float* products, float* results);
      String getIntensityLabel(int index);
      String getDensityLabel(int index);
      String getEvacuationLabel(int index);
      float getFactor(int index);
      float getEvacuation(float* products, float* results);
      
    private:
      float _ix1, _ix2, _ix3, _iy;
      float _dx1, _dx2, _dx3, _dy;
      float _ex1, _ex2, _ex3, _ey;
      float _factors[9];
      void initVars();
      float calculateFactor(float x1, float y1, float x2, float y2);
  };

#endif
