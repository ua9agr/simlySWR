/*
  SWR meter

  Read FWR / REV from reflectometer (like N7DDC schematic) and out it as Voltage thru PWM. Button & LEDs for swith mode
  16/07/02021
  UA9AGR
*/

float my_vcc_const = 1.1;    // начальное значение константы должно быть 1.1

//PIN Def
#define pin_forward_v  A0        // input (analog) - SWR sensor forward voltage
#define pin_reflected_v A1       // input (analog) - SWR sensor reverse voltage

#define pin_green_led   6
#define pin_red_led  3

#define pin_button  5

#define pin_out 4


#define FORWARD_V_TX_SENSE_THRESH 6   // измерение КСВ начиная от, у.е.
#define mult  10


float measure_swr(int forward_voltage, int reverse_voltage) {

  float swr = 0;
  //forward_voltage = analogRead(pin_forward_v);
  //reverse_voltage = analogRead(pin_reflected_v);
  if (forward_voltage > FORWARD_V_TX_SENSE_THRESH) {
    if (forward_voltage > reverse_voltage) {
      if (reverse_voltage == 0)        reverse_voltage = 1;
      swr = (float(forward_voltage + reverse_voltage) / float(forward_voltage - reverse_voltage)) /*)*/;
    }
    else {
      swr = 100;
    }
  }
  return swr;
}

uint16_t Vcc_m;
float m;

void setup() {
  Serial.begin(9600);
  Serial.println("FWD \t REV \t SWR");
  
   
  // initialize pins.
  pinMode(pin_red_led, OUTPUT);
  pinMode(pin_green_led, OUTPUT);
  digitalWrite(pin_forward_v, LOW);
  pinMode(pin_forward_v, INPUT);
  digitalWrite(pin_reflected_v, LOW);
  pinMode(pin_reflected_v, INPUT);
  pinMode(pin_button, INPUT_PULLUP);


  analogReference(INTERNAL2V56);
  //analogReference(INTERNAL4V096);
  delay(500);
  Vcc_m = analogRead(VCCM);        // 5  1016 (4V096 ref)  
                                  //5,105 В = 2042 (2v56 ref) 
  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);
  m = 2042.0/Vcc_m;
}

// the loop function runs over and over again forever
void loop() {
  int fwd = analogRead(pin_forward_v);
  int rev = analogRead(pin_reflected_v);
  //float swr = measure_swr(fwd, rev);
  float swr = 1.0;

  if (digitalRead(pin_button)) { /// Кнопка не нажата 
    digitalWrite(pin_green_led, 0);
    digitalWrite(pin_red_led, 1);
//    if (swr < 3.5)
//    {
//      analogWrite(DAC0,int(255.0*swr/Vcc_m)); //КСВ  
//    }
//    else
//    {
//      analogWrite(DAC0,60); //КСВ BIG
//    }
      analogWrite(DAC0,91.0*m); //91m -> 1V. 43 -> 0.5V  255-> 3V81
    
  }
  else  /// Кнопка нажата
  {
    digitalWrite(pin_green_led, 1);
    digitalWrite(pin_red_led, 0);
    analogWrite(DAC0,Vcc_m/48); //прямая
  }

  Serial.print(fwd);
  Serial.print("\t");
  Serial.print(rev);
  Serial.print("\t");
  Serial.println((m-1.0)*1000);
  
    delay(100);              // wait for a second
}
