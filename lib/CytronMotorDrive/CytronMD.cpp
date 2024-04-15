#include <CytronMD.h>

void CytronMD::initialize(int dirPin, int pwmPin) {
    this->dirPin = dirPin;
    this->pwmPin = pwmPin;
    this->currentPower = 0.0;
    
    pinMode(dirPin, OUTPUT);
    pinMode(pwmPin, OUTPUT);
    analogWrite(pwmPin, 0);
}

void CytronMD::setPower(float val) {

    if (val > 0) {
      digitalWrite(this->dirPin, HIGH);
    } else {
      digitalWrite(dirPin, LOW);
    }

    analogWrite(pwmPin, abs(val) * 255);

    this->currentPower = val;

}