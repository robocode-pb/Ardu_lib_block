#include <Arduino.h>

#define S0 7
#define S1 8
#define S2 A1
#define SIG A0

#define DISTANCE_LEFT_SENSOR A3
#define DISTANCE_FORWARD_SENSOR A6
#define DISTANCE_RIGHT_SENSOR A7

class QuantLine {
  public:
   
    QuantLine();

    void begin();
    void begin(bool debug);
	void setLevel(uint8_t level);
	
    uint16_t readLineSensor(uint8_t channel);
	bool readLineSensorBool(uint8_t channel);

  private:
    void initLineSensors(bool debug);
	uint16_t _level = 150;
};

class QuantDistance {
  public:
    QuantDistance();

    uint16_t readDistanceSensor(uint8_t channel);
};
