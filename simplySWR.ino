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

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  Serial.println("FWD \t REV \t SWR");
  
  //Analogue ref 1v1
  analogReference(INTERNAL);
  // initialize pins.
  pinMode(pin_reflected_led, OUTPUT);
  pinMode(pin_forward_led, OUTPUT);
  
  digitalWrite(pin_forward_v, LOW);
  pinMode(pin_forward_v, INPUT);
  
  digitalWrite(pin_reflected_v, LOW);
  pinMode(pin_reflected_v, INPUT);

  pinMode(pin_button, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
  int fwd = analogRead(pin_forward_v);
  int rev = analogRead(pin_reflected_v);
  float swr = measure_swr(fwd, rev);

  if (digitalRead(pin_button)) { /// Кнопка нажата 
    digitalWrite(pin_forward_led, 0);
    analogWrite(0,int(1024.0*swr/5.0)); //КСВ
  }
  else
  {
    digitalWrite(pin_forward_led, 1);
    analogWrite(0,fwd); //прямая
  }

  Serial.println(fwd);
  Serial.print("\t");
  Serial.println(rev);
  Serial.print("\t");
  Serial.println(swr);
  
    delay(100);              // wait for a second
}
