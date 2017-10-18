#define digitalWrite(...) analogWrite(__VA_ARGS__)
#define HIGH 255
#define LOW 0
#define ONLEDS 16
#define INIT_PIN 0

#include <Wire.h>

typedef struct {
    uint16_t pinNum;
    bool pinUSE = false;
    bool pinON = false;
    bool refresh = false;
    uint8_t level = LOW;
    
} pinLevels;

pinLevels digitPins[ONLEDS];

void setup() {
  Serial.begin(115200);
  digitPins_setOut(0xFFFF);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
}

void loop() {
  delay(100);
  refresh();
}

void receiveEvent(int howMany){
  char operation;
  int pins;
  bool state;
  int level;
  
   if (Wire.available() == 4) { // loop through all but the last
   operation = Wire.read(); // receive byte as a character
   pins = Wire.read();
   state = Wire.read();
   level = Wire.read();
  
   switch(operation){
      case 'a':
        digitPins_setState(pins,state);
        break;
      case 'b':
        digitPins_setLevel(pins,level);
        break;
      default:
        break;
    }
   }  
  }

void digitPins_setOut(int onPins){
  char texto[50];
  Serial.println("LEDS");
  for (int i = 0 ;i<ONLEDS;i++)
  {
      digitPins[i].pinNum = i+INIT_PIN;
      if (onPins & (1<<i))
      {
         digitPins[i].level = LOW;
         digitPins[i].pinUSE = true;
         digitPins[i].pinON = false;
         sprintf(texto,"Pin %d se usa como salida",digitPins[i].pinNum);
         Serial.println(texto);
         pinMode(digitPins[i].pinNum,OUTPUT);
      }else
      {
        digitPins[i].level = LOW;
        digitPins[i].pinUSE = false;
        digitPins[i].pinON = false;
      }
  }
}

void digitPins_setState(int onPins,bool state){
  Serial.println(state);
  for (int i = 0 ;i<ONLEDS;i++)
  {
      if (onPins & (1<<i))
      {
         digitPins[i].pinON = state;
         digitPins[i].refresh = true;
         if(state) {
          digitPins[i].level = HIGH;
         }else{
          digitPins[i].level = LOW;
          }
      }
  }
}

void digitPins_setLevel(int onPins,int level){
  for (int i = 0 ;i<ONLEDS;i++)
  {
      if (onPins & (1<<i) && digitPins[i].pinON)
      {
         digitPins[i].level = level;
         digitPins[i].refresh = true;
      }
  }
}

void digitPins_digitWrite(int pin, int level){
  if(digitPins[pin].pinUSE){
    digitalWrite(digitPins[pin].pinNum,level);
    digitPins[pin].level = level;
    digitPins[pin].refresh = false;
    if(level > 0)
      digitPins[pin].pinON = true;
    else
      digitPins[pin].pinON = true;
    }
  }

void refresh(){
  for (int i = 0 ;i<ONLEDS;i++)
  {
      if (digitPins[i].pinUSE && digitPins[i].pinON && digitPins[i].refresh)
      { 
         digitPins[i].refresh = false;
         Serial.print("Se enciende el led ");
         Serial.println(digitPins[i].pinNum);
         digitalWrite(digitPins[i].pinNum,digitPins[i].level);
      }
      else
      { 
         if (digitPins[i].pinUSE && digitPins[i].refresh)
         {
            Serial.print("Se apaga el led ");
            Serial.println(digitPins[i].pinNum);
            digitPins[i].refresh = false;
            digitalWrite(digitPins[i].pinNum,LOW);
         }
      }
  }
}
