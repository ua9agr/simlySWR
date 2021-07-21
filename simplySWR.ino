/*
 SWR meter
 
 Read FWR / REV from reflectometer (like N7DDC schematic) and out it as Voltage thru PWM. Button & LEDs for swith mode

  16/07/02021
  UA9AGR
 */


//PIN Def
#define pin_forward_v  A0        // input (analog) - SWR sensor forward voltage
#define pin_reflected_v A1       // input (analog) - SWR sensor reverse voltage

#define pin_forward_led   13
#define pin_reflected_led  12

#define pin_button  10


#define FORWARD_V_TX_SENSE_THRESH 6

//-----------------------------------------------------------------------------------------------------
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

uint16_t Vcc_m

void setup() {
  Serial.begin(9600);

  Serial.println("FWD \t REV \t SWR");
  

   
  // initialize pins.
  pinMode(pin_reflected_led, OUTPUT);
  pinMode(pin_forward_led, OUTPUT);
  
  digitalWrite(pin_forward_v, LOW);
  pinMode(pin_forward_v, INPUT);
  
  digitalWrite(pin_reflected_v, LOW);
  pinMode(pin_reflected_v, INPUT);

  pinMode(pin_button, INPUT_PULLUP);


  analogReference(INTERNAL2V56);
  Vcc_m = analogRead(VCCM);
  analogReference(INTERNAL);
  pinMode(DAC0, ANALOG);
}

// the loop function runs over and over again forever
void loop() {
  int fwd = analogRead(pin_forward_v);
  int rev = analogRead(pin_reflected_v);
  //float swr = measure_swr(fwd, rev);
  float swr = 1.0;

  if (digitalRead(pin_button)) { /// Кнопка нажата 
    digitalWrite(pin_forward_led, 0);
    digitalWrite(pin_reflected_led, 1);
    if (swr < 3)
    {
      analogWrite(DAC0,int(255.0*swr/Vcc_m)); //КСВ  
    }
    else
    {
      analogWrite(DAC0,60); //КСВ BIG
    }
    
  }
  else
  {
    digitalWrite(pin_forward_led, 1);
    digitalWrite(pin_reflected_led, 0);
    analogWrite(DAC0,255); //прямая
  }

  Serial.println(fwd);
  Serial.print("\t");
  Serial.println(rev);
  Serial.print("\t");
  Serial.println(Vcc_m);
  
    delay(100);              // wait for a second
}
