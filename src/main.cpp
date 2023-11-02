#include <Arduino.h>
#include <PS4Controller.h>

/*Four Wheel Drive Using Cytron MDD10A motor driver
  and ESP32 */


//MOTOR PIN CONNECTIONS
#define enableA 13
#define enableB 27

#define A_DIR 32
#define B_DIR 33

int set_speed = 200;
int const_limit = 5;


void bot_stop() {
  digitalWrite(A_DIR, 0);
  analogWrite(enableB, 0);

  digitalWrite(B_DIR, 0);
  analogWrite(enableA, 0);
}

void left_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(A_DIR, 1);
   }
   else {
    digitalWrite(A_DIR, 0);
   }

   if (abs_power*2>240){
      abs_power = 127;
   }
   analogWrite(enableA, abs_power*2);
   Serial.printf("R_PWM: %d\t", abs_power*2);
}


void right_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(B_DIR, 1);
   }
   else { 
    digitalWrite(B_DIR, 0);
   }
   if (abs_power*2>240){
      abs_power = 127;
   }

   analogWrite(enableB, abs_power*2);
   Serial.printf("L_PWM: %d\n", abs_power*2);
}



void print_ps4_data(){
      Serial.printf("Left Stick x at %d: ", PS4.LStickX());
      Serial.printf("Left Stick y at %d: ", PS4.LStickY());
      Serial.printf("Right Stick x at %d: ", PS4.RStickX());
      Serial.printf("Right Stick y at %d\n", PS4.RStickY());
    
}

int dead_zone(int val, int limit = const_limit) {
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



void setup() {
  // put your setup code here, to run once:
  PS4.begin("9C:B6:D0:90:37:C2");
  Serial.begin(115200);

  pinMode(enableB, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(A_DIR, OUTPUT);
  pinMode(B_DIR, OUTPUT);

  Serial.println("Setup Complete");
}


void loop() {
  // put your main code here, to run repeatedly:
  if(PS4.isConnected()) {

    // print_ps4_data();
    int L_Y = dead_zone(PS4.LStickY());
    int R_Y = dead_zone(PS4.RStickY());

    //Serial.printf("%d, %d\n", L_Y, R_Y);

    left_track(R_Y);
    right_track(L_Y);

    


  }

  else{
    bot_stop();
    Serial.println("NO PS4");
  }

}