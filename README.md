# NonBlockingSequence
This liberary is used to build a sequence of tasks. Each task is repeatedly called, until specific trigger, then the next task is called. This trigger can be anything: variable value, input signal, or timer. It is possible to run multiple sequences at the same time, without blocking each other.

This sequence of tasks is similar to PLC programming for induxterial sequence, Or similar to state machine with only one path. If it is needed to build a state machine with multiple pathes, It is possible to define many sequences, and control which sequence to run by using if-else condition.
## Introdunctory Example

```C++
#include <NonBlockingSequence.h>

NonBlockingSequence Sequence;

void Setup{
Sequence.AddNewStep(&Task1);
Sequence.AddNewStep(&Task2);
Sequence.AddNewStep(&Task3);
}

void loop() {
  Sequence.DoSequence();
}

bool Task1(){ 
// task 1 code
}

bool Task2(){ 
// task 2 code
}

bool Task3(){ 
// task 3 code
}
```
When the program call the function `Sequence.DoSequence()`, the `Sequence` will start calling `Task1()`. And will continue executing task1 until the function call returns true. Then `Task2()` will be called until it returns true. Then `Task3()` will be called until it returns true. After that, the sequence will end, and each time the function `Sequence.DoSequence()` is called, nothing will happen, unless the program is written to repeat or reset the `Sequence`.

The sequence execution order:  `Task1() --> Task2() --> Task3() --> End`

## Blinking Leds Example

```c++
#include <NonBlockingSequence.h>

// Pin used to wire the leds
#define led1 13
#define led2 12

// two new sequences are defined
NonBlockingSequence Sequence1;
NonBlockingSequence Sequence2;

void setup() {
  Serial.begin(9600);
  
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
```

Each step ( `led1_on()`, `led1_off()`, `led2_on()` and `led2_off()` ) is only called once. Because each step `return true`. So that, the sequence go to the next step.
The function `AddDelayInMillis(...)` adds a new step to the sequence. This step has no action to do, But wait until time is over. Then the sequence go to the next step. The function  `AddDelayInMillis(...)` does not block the program. So that, each led blinking separately without affecting each other. Led1 is blinking faster than Led2. Because 400 ms is less than 1500 ms. The function `Repeat()` make the sequence repeat itself infinite number of times.

The sequence execution order:

Sequence1 :`led1_on() --> DelayInMillis(400) --> led1_off() --> DelayInMillis(400) --> led1_on() --> DelayInMillis(400) --> led1_off() --> DelayInMillis(400) --> led1_on() --> DelayInMillis(400) --> led1_off() --> ... endless-repeat `

Sequence2 :`led2_on() --> DelayInMillis(1500) --> led2_off() --> DelayInMillis(1500) --> led2_on() --> DelayInMillis(1500) --> led2_off() --> DelayInMillis(1500) --> led2_on() --> ... endless-repeat `

## Main Methods

### Add New Step
```c++
void NonBlockingSequence.AddNewStep(&step);
```
Add a new step to the sequence. Each step is a boolean function, that run continounsly, until it returns true.
```c++
bool step(){
// Do tasks
if(step_end_condition){
return true;
}
else{
return false;
}
```

### Add Delay
```c++
void NonBlockingSequence.AddDelayInMillis(unsigned long val)
```
Add a new delay step to the sequence. This delay waits `val` milliseconds. This delay doesnot block other codes outside the sequence. It only delay the execution of the next step in the same sequence.

### Do Sequence Steps
```c++
void NonBlockingSequence.DoSequence()
```
Run the sequence step by step. The steps can either be a boolean function or timer. Each step is called until a trigger action is happend. The trigger action can be:
- for boolean function:       return true.
- for timer           :       running time is over.

After the trigger action is occured. The sequence start to call the neaxt step.

### Sequence end
```c++
bool NonBlockingSequence.Finish();
```
Check if the sequence reach its end, and finish executing all sequence steps.

### Restart
```c++
void NonBlockingSequence.Restart();
```
Force the sequence to start from the begining.

### Repeat
```c++
void NonBlockingSequence.Repeat();
```
Repeat the sequence for infinite number of times. Usually this method is used at after all needed steps are added.

```c++
void NonBlockingSequence.Repeat(unsigned int n);
```
Repeat the previous sequence steps for `n` times. Usually this method is used after all needed steps are added.

### Next Step
```c++
bool NonBlockingSequence.NextStep();
```
Terminate the current step execution. Force the sequence to go to the next step. Next time the function `DoSequence()` is called, the next step is executed.
- if the previous action is done succesfully         :        it returns true
- if the current state is final state                :        it returns false

### Pass Steps
```c++
unsigned int NonBlockingSequence.PassSteps(unsigned int n);
```
the sequence jump over n future step without calling. This function is similar to calling `NextStep()` for `n` time. It also returns number of succesful step jumps.

### first_time_exexuting_step();
```c++
bool NonBlockingSequence.first_time_exexuting_step;
```
This function check if the current step is running for the first time. This function used to apply number of command in a step for the first call only.

## Important Notes

### Nested Sequence
You can many sequences nested with multiple levels.

### Conditional Sequence
It can be done using 2 different methods:
- Defining two different sequences. Use if-else conditions, to select between them.
- In a single sequence, it is possible to define many adjacent steps, and conditionally jump over these steps without execution using `PassSteps()` method.

### Class Boolean Function
In the previous example, it is only mentioned to add steps of boolean function that does not belong to class. But is it possible to deal with class boolean function? To answer this question look for the example Class_Function.
