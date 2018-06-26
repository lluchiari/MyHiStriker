#include <AcceleroMMA7361.h>

// States of state machine
enum GAME_STATE {
  FIRULA,
  GAME_ON,
  FORCE_MEAS,
  ATUALIZE_MAX,
  CHICKEN_FIRULA,
  STRONG_FIRULA
};

AcceleroMMA7361 accelero;

const int ON = HIGH;
const int OFF = LOW;

////////////////////////////////////////////////////////////////////////////////////////////////
// Game Variables
GAME_STATE state             = FIRULA;    //Starts the game with the Firula State
int max_value                = 10;        //Adjust to a calibrated one
int buttonState              = 0;

int accel_voltage            = 3.3;       //Sets the voltage used in the Accelerometer. Could be 3.3 (more precision) or 5 (lower precision)
bool accel_sensibility       = false;     //Sets the sensibility for the accelerometer (HIGH = +/-1.5G, LOW = +/-6G)

int firula_counter = 0;


////////////////////////////////////////////////////////////////////////////////////////////////
// Harware I/O Pinout
const int start_button_pin   = ?;          //Button pin to say if we can start the game

const int point_100_pin      = 13;         //LED pin for 100 points
const int point_200_pin      = 12;         //LED pin for 200 points
const int point_300_pin      = 11;         //LED pin for 300 points
const int point_400_pin      = 10;         //LED pin for 400 points
const int point_500_pin      = 9;          //LED pin for 500 points
const int point_600_pin      = 8;          //LED pin for 600 points
const int point_700_pin      = 7;          //LED pin for 700 points
const int point_800_pin      = 6;          //LED pin for 800 points
const int point_900_pin      = 5;          //LED pin for 900 points
const int point_1000_pin     = 4;          //LED pin for 1000 points

const int sleep_pin          = 3;          //Number indicating to which pin the sleep port is attached.     [DIGITAL OUT]
const int selfTest_pin       = 2;          //Number indicating to which pin the selftest port is attached.  [DIGITAL OUT]
const int zeroG_pin          = 1;          //Number indicating to which pin the ZeroGpin is connected to.   [DIGITAL IN]
const int gSelect_pin        = 0;          //Number indication to which pin the Gselect is connected to.    [DIGITAL OUT]
const int x_accel_pin        = A0;         //Analog pin for the accelerometer x value
const int y_accel_pin        = A1;         //Analog pin for the accelerometer y value
const int z_accel_pin        = A2;         //Analog pin for the accelerometer z value

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  pinMode(start_button_pin, INPUT);
  
  pinMode(point_100_pin, OUTPUT);
  pinMode(point_200_pin, OUTPUT);
  pinMode(point_300_pin, OUTPUT);
  pinMode(point_400_pin, OUTPUT);
  pinMode(point_500_pin, OUTPUT);
  pinMode(point_600_pin, OUTPUT);
  pinMode(point_700_pin, OUTPUT);
  pinMode(point_800_pin, OUTPUT);
  pinMode(point_900_pin, OUTPUT);
  pinMode(point_1000_pin, OUTPUT);

  accelero.begin(sleep_pin, selfTest_pin, zeroG_pin, gSelect_pin, x_accel_pin, y_accel_pin, z_accel_pin);
  accelero.setARefVoltage(accel_voltage);                 //sets the AREF voltage to 3.3V
  accelero.setSensitivity(accel_sensibility);             //sets the sensitivity to +/-6G
  accelero.calibrate();

}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  switch(state){
    case FIRULA:
      firula();
      break;
    case GAME_ON:
      gameOn();
      break;
    case FORCE_MEAS:
      forceMeasure();
      break;
    case ATUALIZE_MAX:
      atualizeMax();
      break;
    case CHICKEN_FIRULA:
      chickenFirula();
      break;
    case STRONG_FIRULA:
      strongFirula();
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void firula(){
  buttonState = digitalRead(_start_button);
  if(buttonState){
    state = GAME_ON;
  }
  else{
    switch (firula_counter){
      case 0:
        firula0();
        firula_counter++;
        break;
      case 1:
        firula1();
        firula_counter = 0;
        break;
    }
    state = FIRULA;
  }
}


void gameOn(){
  
}

void forceMeasure(){
  
}

void atualizeMax(){
  
}

void chickenFirula(){
  
}

void strongFirula(){
  
}


////////////////////////////////////////////////////////////////////////////////////////////////
// FIRULA FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////
void firula0(){
  digitalWrite(point_100_pin, OFF);
  digitalWrite(point_200_pin, OFF);
  digitalWrite(point_300_pin, OFF);
  digitalWrite(point_400_pin, OFF);
  digitalWrite(point_500_pin, OFF);
  digitalWrite(point_600_pin, OFF);
  digitalWrite(point_700_pin, OFF);
  digitalWrite(point_800_pin, OFF);
  digitalWrite(point_900_pin, OFF);
  digitalWrite(point_1000_pin, OFF);
}

void firula1(){
  digitalWrite(point_100_pin, OFF);
  digitalWrite(point_200_pin, OFF);
  digitalWrite(point_300_pin, OFF);
  digitalWrite(point_400_pin, OFF);
  digitalWrite(point_500_pin, OFF);
  digitalWrite(point_600_pin, OFF);
  digitalWrite(point_700_pin, OFF);
  digitalWrite(point_800_pin, OFF);
  digitalWrite(point_900_pin, OFF);
  digitalWrite(point_1000_pin, OFF);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// AUXILIAR FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

void digital_on_with_delay(int _pin, int _delay){  
  digitalWrite(_pin, ON);
  delay(_delay);
}

void digital_off_with_delay(int _pin, int _delay){  
  digitalWrite(_pin, OFF);
  delay(_delay);
}

