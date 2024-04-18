#include <Arduino.h>

#include <PS4Controller.h>

/*Four Wheel Drive Using L298N motor driver and ESP32 */

/* T0 find the MAC ADDRESS of PS4 USE -> sixaxispairtool */


//MOTOR PIN CONNECTIONS
// PWM PINS
#define pwmA 0
#define pwmB 1

// DIR PINS
#define dirA1 2
#define dirA2 3
#define dirB1 27
#define dirB2 26

int const_limit = 5;
const char ps4_mac_add[18] = "D8:3A:DD:C5:E5:7A";     // Change your PS4 Mac Address Here !!!

// Function Declaration
void bot_stop(void);
void l_track(int);
void r_track(int);
int dead_zone(int val, int limit = const_limit);
void print_ps4_data(void);


void setup() {
  // put your setup code here, to run once:

  PS4.begin(ps4_mac_add);
  Serial.begin(115200);

  pinMode(pwmB, OUTPUT);
  pinMode(pwmA, OUTPUT);

  pinMode(dirA1, OUTPUT);
  pinMode(dirA2, OUTPUT);
  pinMode(dirB1, OUTPUT);
  pinMode(dirB2, OUTPUT);

  Serial.println("Setup Complete");
}


void loop() {
  // put your main code here, to run repeatedly:
  if(PS4.isConnected()) {

    // print_ps4_data();
    int L_Y = dead_zone(PS4.LStickY());
    int R_Y = dead_zone(PS4.RStickY());

    //Serial.printf("%d, %d\n", L_Y, R_Y);

    l_track(L_Y);
    r_track(R_Y);
  }

  else{
    bot_stop();
    Serial.println("NO PS4");
  }
}


// Function Definations

/* Function to stop the Bot*/
void bot_stop(void) {
  digitalWrite(dirA1, 0);
  digitalWrite(dirA2, 0);
  analogWrite(pwmB, 0);

  digitalWrite(dirB1, 0);
  digitalWrite(dirB2, 0);
  analogWrite(pwmA, 0);
}

/* Function for movement of left track*/
void l_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(dirA1, 1);
    digitalWrite(dirA2, 0);
   }
   else {
    digitalWrite(dirA1, 0);
    digitalWrite(dirA2, 1);
   }

   if (abs_power*2>240){
      abs_power = 127;
   }
   analogWrite(pwmA, abs_power*2);
   Serial.printf("L_PWM: %d\t", abs_power*2);
}

/* Function for movement of right track*/
void r_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(dirB1, 1);
    digitalWrite(dirB2, 0);
   }
   else { 
    digitalWrite(dirB1, 0);
    digitalWrite(dirB2, 1);
   }

   if (abs_power*2>240){
      abs_power = 127;
   }
   analogWrite(pwmB, abs_power*2);
   Serial.printf("R_PWM: %d\n", abs_power*2);
}


/* Debugging PS4 Data*/
void print_ps4_data(void) {
      Serial.printf("Left Stick x at %d: ", PS4.LStickX());
      Serial.printf("Left Stick y at %d: ", PS4.LStickY());
      Serial.printf("Right Stick x at %d: ", PS4.RStickX());
      Serial.printf("Right Stick y at %d\n", PS4.RStickY());
}


/* Function to remove deadzone of PS4*/
int dead_zone(int val, int limit) {
  if (val >= -limit && val <= limit) {
    return 0;
  }
  else {
    if (val>0){
      return val-limit;
    }

    else {
      return val+limit;
    }
  }
}