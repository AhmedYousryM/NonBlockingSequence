#include <NonBlockingSequence.h>

/*
This code illustrate How to declare a new class that run specific sequence

visit https://github.com/AhmedYousryM/NonBlockingSequence
*/

class Blinking_Led{

  private:
  // Declare Sequence
  ClassNonBlockingSequence<Blinking_Led> Sequence;

  // led pin
  byte _pin; 

  // Define step-functions
  bool led_on(){
    digitalWrite(_pin,1);
    return true;
  }
  bool led_off(){
      digitalWrite(_pin,0);
      return true;
  }

  public:
  Blinking_Led(){};  
  void init(byte pin,unsigned long pause){
    
    // define the led pin
    _pin=pin;                    
    pinMode(_pin,OUTPUT);    

    // This line explain important fact for the Sequence
    Sequence.AttachedObj(this);
    // Sequence should use steps defined inside this class only

    // Add steps
    Sequence.AddNewStep(&led_on);
    Sequence.AddDelayInMillis(pause);
    Sequence.AddNewStep(&led_off);
    Sequence.AddDelayInMillis(pause);
    Sequence.Repeat();
    // repeat the sequence infinite number of times
    
  }
  
  Blink(){ // use familiar name for led
    Sequence.DoSequence();
  }
  
};

// Declare new objects
Blinking_Led Led1;
Blinking_Led Led2;

void setup() {

  //  Led.init( pin , blinking time )
  Led1.init(13,200);
  Led2.init(12,1500);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  Led1.Blink();
  Led2.Blink();
}
