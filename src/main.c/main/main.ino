#include <AcceleroMMA7361.h>

// States of state machine
enum GAME_STATE {
  FIRULA,
  GAME_ON,
  FORCE_MEAS,
  ATUALIZE_MAX,
  CHICKEN_FIRULA,
  STRONG_FIRULA,
  MY_ERROR
};

enum ACCEL_COMPONENT{
  X,
  Y,
  Z
};

//Accelerometer class
AcceleroMMA7361 accelero;

//Globl concepts for the whole program
const int ON = HIGH;
const int OFF = LOW;

////////////////////////////////////////////////////////////////////////////////////////////////
// Game Variables
////////////////////////////////////////////////////////////////////////////////////////////////
GAME_STATE state              = GAME_STATE::FIRULA;    //Starts the game with the Firula State
int game_max_score            = 0;         //The maximum score obtained in all attemps of the game
int game_qty_level            = 10;        //Indicates how many steps has the score of the game
int game_medium_border_level  = 4;         //Indicates which level starts the medium level
int game_hard_border_level    = 8;         //Indicates which level starts the hard level
int game_level_step;                       //This variable contains the steps score for the game calculated based on the scale predefined
volatile int game_buttonState = OFF;       //Declare as volatile in order to it can be used inside interruption. Check https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/ for more details

////////////////////////////////////////////////////////////////////////////////////////////////
// Relative variables for each attempt
////////////////////////////////////////////////////////////////////////////////////////////////
// Score Values
int relative_score;                       //Is the score for the relative attemp
int relative_level;                       //Number that variates on the levels of the game. Deppends on the quantity of steps

// Accelerometers Values
int attempt_aux;                          //Indicates the higher value obtained by user in the measured loop
int attempt;                              //Indicates the higher value obtained by user in that attemp

////////////////////////////////////////////////////////////////////////////////////////////////
// Accelerometers Parameters
////////////////////////////////////////////////////////////////////////////////////////////////
ACCEL_COMPONENT acc_comp     = ACCEL_COMPONENT::Z;
int max_scale                = 2000;     //Higher value calibrated. From this value I can infer the strenth of beating
int min_scale                = 0;     //Lower value calibrated. From this value I can infer the strenth of beating
int min_offset_scale         = 400;      // Offset that activates the machine counting

double g_const               = 9.8;       // Gravity constant to transform to m/s^2
double accel_voltage         = 5;         //Sets the voltage used in the Accelerometer. Could be 3.3 (more precision) or 5 (lower precision)
double accel_sensitivity     = 0.8;       //Accelerometer Sensitivity (800 mV/g for HIGH Precision and 206 mV/g for LOW precision
bool accel_sensitivity_mode  = LOW;       //Sets the sensibility for the accelerometer (HIGH = +/-1.5G, LOW = +/-6G)
bool accel_end_loop          = false;     //This variables indicates when stop the game loop state
int axe_direction            = -1;        //This value is -1 if the measured axe moviment is in the inverted direction, otherwise is 1
int accel_sample_value;


//Firula Parameters
int firula_counter           = 0;         //Used to indicate which firula will run. Firula is a show light.

////////////////////////////////////////////////////////////////////////////////////////////////
// Harware I/O Pinout
////////////////////////////////////////////////////////////////////////////////////////////////
const int start_button_pin   = 2;          //Button pin to say if we can start the game (INTERRUPT PIN)
//int point_pin[10];                         //LED point pins vector
int point_pin[10] = {13,12,11,10,9,8,7,6,5,4}; //LED point pins vector

//point_pin[0]                 = 13;         //LED pin from 000 to 100 points
//point_pin[1]                 = 12;         //LED pin from 100 to 200 points
//point_pin[2]                 = 11;         //LED pin from 200 to 300 points
//point_pin[3]                 = 10;         //LED pin from 300 to 400 points
//point_pin[4]                 = 9;          //LED pin from 400 to 500 points
//point_pin[5]                 = 8;          //LED pin from 500 to 600 points
//point_pin[6]                 = 7;          //LED pin from 600 to 700 points
//point_pin[7]                 = 6;          //LED pin from 700 to 800 points
//point_pin[8]                 = 5;          //LED pin from 800 to 900 points
//point_pin[9]                 = 4;          //LED pin from 900 to 1000 points

const int x_accel_pin        = A0;         //Analog pin for the accelerometer x value
const int y_accel_pin        = A1;         //Analog pin for the accelerometer y value
const int z_accel_pin        = A2;         //Analog pin for the accelerometer z value
//const int sleep_pin          = A3;         //Number indicating to which pin the sleep port is attached.     [DIGITAL OUT]
//const int selfTest_pin       = ?;         //Number indicating to which pin the selftest port is attached.  [DIGITAL OUT]
//const int zeroG_pin          = ?;         //Number indicating to which pin the ZeroGpin is connected to.   [DIGITAL IN]
//const int gSelect_pin        = ?;         //Number indication to which pin the Gselect is connected to.    [DIGITAL OUT]

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  //Serial.setTimeout(50);
  
  pinMode(start_button_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(start_button_pin), button_State_Interruption, CHANGE);
  
  pinMode(point_pin[0], OUTPUT);
  pinMode(point_pin[1], OUTPUT);
  pinMode(point_pin[2], OUTPUT);
  pinMode(point_pin[3], OUTPUT);
  pinMode(point_pin[4], OUTPUT);
  pinMode(point_pin[5], OUTPUT);
  pinMode(point_pin[6], OUTPUT);
  pinMode(point_pin[7], OUTPUT);
  pinMode(point_pin[8], OUTPUT);
  pinMode(point_pin[9], OUTPUT);

  //accelero.begin(sleep_pin, selfTest_pin, zeroG_pin, gSelect_pin, x_accel_pin, y_accel_pin, z_accel_pin);
  //accelero.begin();
  accelero.begin(x_accel_pin, y_accel_pin, z_accel_pin);
  accelero.setARefVoltage(accel_voltage);
  accelero.setAveraging(20);
  //accelero.setSensitivity(accel_sensitivity_mode);
  accelero.calibrate();


  // Fixing and atributing parameters
//  max_scale          *= axe_direction;
//  min_scale          *= axe_direction;
//  min_offset_scale   *= axe_direction;
  game_level_step     = (max_scale - (min_scale + min_offset_scale))/game_qty_level;
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
    case MY_ERROR:
      my_error();
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////
// GAME STATES
////////////////////////////////////////////////////////////////////////////////////////////////

void firula(){
  interrupts();
  if(game_buttonState){
    state = GAME_STATE::GAME_ON;
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
    state = GAME_STATE::FIRULA;
  }
}


void gameOn(){
  noInterrupts();
  game_buttonState = OFF;
  digitalWrite(point_pin[0], OFF);
  digitalWrite(point_pin[1], OFF);
  digitalWrite(point_pin[2], OFF);
  digitalWrite(point_pin[3], OFF);
  digitalWrite(point_pin[4], OFF);
  digitalWrite(point_pin[5], OFF);
  digitalWrite(point_pin[6], OFF);
  digitalWrite(point_pin[7], OFF);
  digitalWrite(point_pin[8], OFF);
  digitalWrite(point_pin[9], OFF);
  state = GAME_STATE::FORCE_MEAS;
}

void forceMeasure(){
  accel_end_loop = false;
  attempt_aux = 0;
  bool valid_play = false;

  noInterrupts();
  while(!accel_end_loop)       // Stop Condition
  {
    if(acc_comp == ACCEL_COMPONENT::X){
     accel_sample_value = accelero.getXAccel();
    }
    else if(acc_comp == ACCEL_COMPONENT::Y){
      accel_sample_value = accelero.getYAccel();
    }
    else{
      accel_sample_value = accelero.getZAccel();
    }
  
    Serial.print(accel_sample_value);
    Serial.print(" - ");
    Serial.print(min_scale);
    Serial.print(" (");
    Serial.print(abs(accel_sample_value - min_scale));
    Serial.print(") > ");
    Serial.print(min_offset_scale);
    Serial.println(" ?");
    
    if(abs(accel_sample_value - min_scale)  > min_offset_scale){
      Serial.println("true");
      valid_play = true;
      if(abs(accel_sample_value - min_scale) > attempt_aux){
        attempt_aux = (abs(accel_sample_value - min_scale));
      }
    }
    else if(valid_play){
      attempt = attempt_aux;
      Serial.print("Mlnax_Attempt: ");
      Serial.println(attempt);
      relative_level = compute_relative_level(attempt);
      Serial.print("Relative_Level: ");
      Serial.println(relative_level);
      relative_score = attempt - min_offset_scale;
      Serial.print("Relative_Score: ");
      Serial.println(relative_score);

      Serial.print(relative_score);
      Serial.print(" > ");
      Serial.print(game_max_score);
      Serial.println(" ?");
      if(relative_score > game_max_score){
        state = GAME_STATE::ATUALIZE_MAX;
        accel_end_loop = true;
        Serial.println("true");
      }
      else{
        Serial.println("false");
        if(relative_level > game_hard_border_level){
          Serial.println("Strong");
          state = GAME_STATE::STRONG_FIRULA;
        }
        else if(game_medium_border_level){
          Serial.println("Chicken");
          state = GAME_STATE::CHICKEN_FIRULA;
        }
        else{
          Serial.println("Chicken");
          state = GAME_STATE::CHICKEN_FIRULA;
        }
        accel_end_loop = true;
      }
    }
  
   delay(10);
  }
}

void atualizeMax(){
  noInterrupts();
  game_max_score = relative_score;

  Serial.print("NEW_MAX: ");
  Serial.println(game_max_score);

  if(relative_level > game_hard_border_level){
    Serial.println("Strong");
    state = GAME_STATE::STRONG_FIRULA;
  }
  else if(game_medium_border_level){
    Serial.println("Chicken");
    state = GAME_STATE::CHICKEN_FIRULA;
  }
  else{
    Serial.println("Chicken");
    state = GAME_STATE::CHICKEN_FIRULA;
  }
}

void chickenFirula(){
  noInterrupts();
  int i;
  for(i=0;i<game_qty_level; i++){
    digital_off_with_delay(point_pin[i], 0);
  }
  for(int i=0; i<7; i++){
    digital_vector_on_with_delay(point_pin, 0, relative_level, 500);
    digital_vector_off_with_delay(point_pin, 0, relative_level, 500);
  }
  state = GAME_STATE::FIRULA;
}

void strongFirula(){
  noInterrupts();
  int i;
  for(i=0;i<game_qty_level; i++){
    digital_off_with_delay(point_pin[9], 0);
  }
  for(i=0;i<7; i++){
    digital_on_with_delay(point_pin[9], 500);
    digital_off_with_delay(point_pin[9], 500);
  }
  state = GAME_STATE::FIRULA;
}

void my_error(){
  digitalWrite(point_pin[0], OFF);
  digitalWrite(point_pin[1], OFF);
  digitalWrite(point_pin[2], OFF);
  digitalWrite(point_pin[3], OFF);
  digitalWrite(point_pin[4], OFF);
  digitalWrite(point_pin[5], OFF);
  digitalWrite(point_pin[6], OFF);
  digitalWrite(point_pin[7], OFF);
  digitalWrite(point_pin[8], OFF);
  digitalWrite(point_pin[9], OFF);
  while(1){
    digitalWrite(point_pin[0], ON);
    digitalWrite(point_pin[9], ON);
    delay(100);
    digitalWrite(point_pin[0], OFF);
    digitalWrite(point_pin[9], OFF);
    delay(100);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////
// FIRULA FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////
void firula0(){
  digitalWrite(point_pin[0], OFF);
  digitalWrite(point_pin[1], OFF);
  digitalWrite(point_pin[2], OFF);
  digitalWrite(point_pin[3], OFF);
  digitalWrite(point_pin[4], OFF);
  digitalWrite(point_pin[5], OFF);
  digitalWrite(point_pin[6], OFF);
  digitalWrite(point_pin[7], OFF);
  digitalWrite(point_pin[8], OFF);
  digitalWrite(point_pin[9], OFF);
  delay(500);
  digitalWrite(point_pin[0], ON);
  digitalWrite(point_pin[1], ON);
  digitalWrite(point_pin[2], ON);
  digitalWrite(point_pin[3], ON);
  digitalWrite(point_pin[4], ON);
  digitalWrite(point_pin[5], ON);
  digitalWrite(point_pin[6], ON);
  digitalWrite(point_pin[7], ON);
  digitalWrite(point_pin[8], ON);
  digitalWrite(point_pin[9], ON);
  delay(500);
}

void firula1(){
  digitalWrite(point_pin[0], OFF);
  digitalWrite(point_pin[1], OFF);
  digitalWrite(point_pin[2], OFF);
  digitalWrite(point_pin[3], OFF);
  digitalWrite(point_pin[4], OFF);
  digitalWrite(point_pin[5], OFF);
  digitalWrite(point_pin[6], OFF);
  digitalWrite(point_pin[7], OFF);
  digitalWrite(point_pin[8], OFF);
  digitalWrite(point_pin[9], OFF);
  digital_on_with_delay(point_pin[0], 100);
  digital_on_with_delay(point_pin[1], 100);
  digital_on_with_delay(point_pin[2], 100);
  digital_on_with_delay(point_pin[3], 100);
  digital_on_with_delay(point_pin[4], 100);
  digital_on_with_delay(point_pin[5], 100);
  digital_on_with_delay(point_pin[6], 100);
  digital_on_with_delay(point_pin[7], 100);
  digital_on_with_delay(point_pin[8], 100);
  digital_on_with_delay(point_pin[9], 100);
}

void firula_game()
{
  digitalWrite(point_pin[0], OFF);
  digitalWrite(point_pin[1], OFF);
  digitalWrite(point_pin[2], OFF);
  digitalWrite(point_pin[3], OFF);
  digitalWrite(point_pin[4], OFF);
  digitalWrite(point_pin[5], OFF);
  digitalWrite(point_pin[6], OFF);
  digitalWrite(point_pin[7], OFF);
  digitalWrite(point_pin[8], OFF);
  digitalWrite(point_pin[9], OFF);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// AUXILIAR FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

void digital_on_with_delay(int _pin, int _delay){ 
  interrupts();
  digitalWrite(_pin, ON);
  delay(_delay);
  noInterrupts();
}

void digital_off_with_delay(int _pin, int _delay){
  interrupts();
  digitalWrite(_pin, OFF);
  delay(_delay);
  noInterrupts();
}

void digital_vector_on_with_delay(int *vector, int idInicial, int idFinal, int _delay){
  interrupts();
  for(int i=idInicial;i < idFinal; i++){
    digitalWrite(vector[i], ON);
  }
  delay(_delay);
  noInterrupts();
}

void digital_vector_off_with_delay(int *vector, int idInicial, int idFinal, int _delay){
  interrupts();
  for(int i=idInicial; i < idFinal; i++){
    digitalWrite(vector[i], OFF);
  }
  delay(_delay);
  noInterrupts();
}

int compute_relative_level(int _relative_score){
  interrupts();
  int _relative_level = (_relative_score - min_offset_scale)/game_level_step;
  int i;
  for(i=0; i<game_qty_level;i++){
    digital_off_with_delay(point_pin[i], 0);
  }
  for(i=0; i<_relative_level;i++){
    digital_on_with_delay(point_pin[i], 700);
  }
  delay(3000);
  noInterrupts();
  return _relative_level;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPTION FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

//Interruption called when the button is pressed
void button_State_Interruption(){
  game_buttonState = ON;
//  noInterrupts();
}
