#include <NonBlockingSequence.h>

visit https://github.com/AhmedYousryM/NonBlockingSequence

// Pin used to wire the leds
#define led1 13
#define led2 12

// two new sequences are defined
NonBlockingSequence Sequence1;
NonBlockingSequence Sequence2;

void setup() {
  
  // Define steps for Sequence1
  Sequence1.AddNewStep(&led1_on);
  Sequence1.AddDelayInMillis(400);
  Sequence1.AddNewStep(&led1_off);
  Sequence1.AddDelayInMillis(400);
  Sequence1.Repeat();

  // Difinition for pin usage
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  
  // Define steps for Sequence2
  Sequence2.AddNewStep(&led2_on);
  Sequence2.AddDelayInMillis(1500);
  Sequence2.AddNewStep(&led2_off);
  Sequence2.AddDelayInMillis(1500);
  Sequence2.Repeat();

}

void loop() {
  // put your main code here, to run repeatedly:
  Sequence1.DoSequence();
  Sequence2.DoSequence();
}

bool led1_on(){
  digitalWrite(led1,1);
  return true;
}
bool led1_off(){
  digitalWrite(led1,LOW);
  return true;
}
bool led2_on(){
  digitalWrite(led2,1);
  return true;
}
bool led2_off(){
  digitalWrite(led2,LOW);
  return true;
}
