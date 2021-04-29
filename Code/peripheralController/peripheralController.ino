// Board: Arduino Pro / Pro Mini
// enable additional analog pins

#include <Wire.h>

#define LED4 MISO
#define LED3 MOSI
#define LED2 7
#define LED1 4
#define PWR_EN 1
#define MOTOR_EN 0
#define RIGHT_FORWARD 9
#define RIGHT_BACK 10
#define LEFT_FORWARD 5
#define LEFT_BACK 6

#define PULSE_MAX 2000

uint16_t servo1 = 1000, servo2 = 1000, servo3 = 1000, servo4 = 1000;
byte servo_tick = 0;
byte ir_trigger = 0;
bool lockdown = 0;

void showBatteryLevel(byte level) {
  digitalWrite(LED4, 0);
  digitalWrite(LED3, 0);
  digitalWrite(LED2, 0);
  digitalWrite(LED1, 0);
  switch(level){
    case 4:
      digitalWrite(LED4, 1);
    case 3:
      digitalWrite(LED3, 1);
    case 2:
      digitalWrite(LED2, 1);
    case 1:
      digitalWrite(LED1, 1);
      break;
  }
}

void writeMotors(int8_t left, int8_t right) {
  if(left > 0) {
    digitalWrite(LEFT_BACK, 0);
    analogWrite(LEFT_FORWARD, left << 1);
  } else if(left < 0) {
    digitalWrite(LEFT_FORWARD, 0);
    analogWrite(LEFT_BACK, left << 1);
  } else {
    digitalWrite(LEFT_FORWARD, 0);
    digitalWrite(LEFT_BACK, 0);
  }
  
  if(right > 0) {
    digitalWrite(RIGHT_BACK, 0);
    analogWrite(RIGHT_FORWARD, right << 1);
  } else if(right < 0) {
    digitalWrite(RIGHT_FORWARD, 0);
    analogWrite(RIGHT_BACK, right << 1);
  } else {
    digitalWrite(RIGHT_FORWARD, 0);
    digitalWrite(RIGHT_BACK, 0);
  }
}

//interrupt occurs with 100.24 hz frequency, must be nonblocking because micros is used
ISR(TIMER2_COMPA_vect, ISR_NOBLOCK){
  servo_tick = 1 - servo_tick;
  if(servo_tick) { //do the servo pulse every other isr
    unsigned long loop_timer = micros();
    unsigned long difference;
    // Motor pins high
    PORTC |= 0x0F;
    byte i = 0x0F;
    
    while(i) {
      difference = micros() - loop_timer;
      
      if ((0b00000001 & i) && difference >= servo1) {
        PORTC &= 0b11111110;
        i &= 0b11111110;
      } 
      if ((0b00000010 & i) && difference >= servo2) {
        PORTC &= 0b11111101;
        i &= 0b11111101;
      } 
      if ((0b00000100 & i) && difference >= servo3) {
        PORTC &= 0b11111011;
        i &= 0b11111011;
      } 
      if ((0b00001000 & i) && difference >= servo4) {
        PORTC &= 0b11110111;
        i &= 0b11110111;
      } 
      if(difference >= PULSE_MAX) {
        // shutoff all servo pins
        PORTC &= 0xF0;
        break;
      }
    }
  }
}

ISR(INT0_vect) { //Interrupt from IR receiver
  ir_trigger++;
  if(ir_trigger > 5) {
    lockdown = 1;
  }
}

ISR(INT1_vect) { //Interrupt from motor controller
  digitalWrite(MOTOR_EN, 0); // shutoff motors until restart
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED4, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(PWR_EN, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);
  digitalWrite(PWR_EN, 1);
  digitalWrite(MOTOR_EN, 1);
  ir_trigger = 0;
  lockdown = 0;

  // TIMER 2 for interrupt frequency 100 Hz:
  noInterrupts(); // stop interrupts
  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; // initialize counter value to 0
  // set compare match register for 100 Hz increments
  OCR2A = 155; // = 16000000 / (1024 * 100) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= _BV(WGM21);
  // Set CS22, CS21 and CS20 bits for 1024 prescaler
  TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20); // divide by 1024 = 15625 hz
  TIFR2 |= _BV(OCF2A);      // clear any pending interrupts;
  TIMSK2 |= _BV(OCIE2A);  // enable the output compare interrupt

  EICRA = 0x0F;  // rising edge on INT0 and INT1 both trigger their respective interrupts
  EIMSK = 0b00000011;   // enable INT0 and INT1 interrupts
  interrupts(); // allow interrupts

  showBatteryLevel(0);
}

void loop() {
  if(lockdown) { // needs to receive at least 3 pulses from IR receiver (noise reject)
    digitalWrite(MOTOR_EN, 0); // shutoff motors until restart
    digitalWrite(PWR_EN, 0); // shutoff power until restart
    PORTC &= 0xF0; // shutoff all servos until restart
    writeMotors(0, 0); // shutoff motors until restart
    while(1) { // lockdown into infinite loop
      showBatteryLevel(4);
      delay(250);
      showBatteryLevel(0);
      delay(250);
    }
  }

  digitalWrite(MOTOR_EN, 1);
  
  // put your main code here, to run repeatedly:
  showBatteryLevel(1);
  servo1 = 1000;
  servo2 = 1250;
  servo3 = 1500;
  servo4 = 1750;
  
  writeMotors(100, 100);
  delay(1000);
  showBatteryLevel(2);
  writeMotors(-100, -100);
  delay(1000);
  ir_trigger = 0; //clear this every so often
}
