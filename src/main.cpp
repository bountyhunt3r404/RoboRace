#include <Arduino.h>

#include <PS4Controller.h>

/*Four Wheel Drive Using Cytron MDD10A motor driver
  and ESP32 */


//MOTOR PIN CONNECTIONS
#define pwmB1 21
#define pwmB2 18
#define pwmA1 33
#define pwmA2 26

#define dirB1 19
#define dirB2 5
#define dirA1 25
#define dirA2 27

int const_limit = 5;

/* Function to stop the Bot*/
void bot_stop() {
  digitalWrite(dirA1, 0);
  digitalWrite(dirA2, 0);
  analogWrite(pwmA1, 0);
  analogWrite(pwmA2, 0);

  digitalWrite(dirB1, 0);
  digitalWrite(dirB2, 0);
  analogWrite(pwmB1, 0);
  analogWrite(pwmB2, 0);
}

/* Function for movement of left track*/
void left_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(dirA1, 1);
    digitalWrite(dirA2, 1);
   }
   else {
    digitalWrite(dirA1, 0);
    digitalWrite(dirA2, 0);
   }

   if (abs_power*2>240){
      abs_power = 127;
   }
   analogWrite(pwmA1, abs_power*2);
   analogWrite(pwmA2, abs_power*2);

   Serial.printf("L_PWM: %d\t", abs_power*2);
}

/* Function for movement of right track*/
void right_track(int power) {
  int abs_power = abs(power);
   if (power>0){
    digitalWrite(dirB1, 1);
    digitalWrite(dirB2, 1);
   }
   else { 
    digitalWrite(dirB1, 0);
    digitalWrite(dirB2, 0);
   }
   if (abs_power*2>240){
      abs_power = 127;
   }

   analogWrite(pwmB1, abs_power*2);
   analogWrite(pwmB2, abs_power*2);

   Serial.printf("R_PWM: %d\n", abs_power*2);
}


/* Debugging PS4 Data*/
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
  PS4.begin("2C:CF:67:00:0F:F2");
  Serial.begin(115200);

  pinMode(pwmB1, OUTPUT);
  pinMode(pwmB2, OUTPUT);

  pinMode(pwmA1, OUTPUT);
  pinMode(pwmA2, OUTPUT);

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

    left_track(L_Y);
    right_track(-R_Y);
  }

  else{
    bot_stop();
    Serial.println("NO PS4");
  }
}