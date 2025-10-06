# NonBlockingSequence
This library is used to build a sequence of tasks. Each task is repeatedly called, until specific trigger, then the next task is called. This trigger can be anything: variable value change, input signal, or timer. It is possible to run multiple sequences at the same time, without blocking each other.

This sequence of tasks is similar to PLC programming for industerial sequence, Or similar to state machine with only one path. If it is needed to build a state machine with multiple paths, It is possible to define many sequences, and control which sequence to run by using the if-else condition.
## Introductory Example

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
When the program calls the function `Sequence.DoSequence()`, the `Sequence` will start calling `Task1()`. And will execute task1 again and again, until the function call returns true. Then `Task2()` will be called until it returns true. Then `Task3()` will be called until it returns true. After that, the sequence will end, and each time the function `Sequence.DoSequence()` is called, nothing will happen, unless the program is written to repeat or reset the `Sequence`.

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

Each step ( `led1_on()`, `led1_off()`, `led2_on()` and `led2_off()` ) is only called once. Because each step `return true`. So that, the sequence goes to the next step.
The function `AddDelayInMillis(...)` adds a new step to the sequence. This step has no action to do, but wait until time is over. Then the sequence goes to the next step. The function  `AddDelayInMillis(...)` does not block the program. So that, each led blinking separately without affecting each other. Led1 is blinking faster than Led2. Because 400 ms is less than 1500 ms. The function `Repeat()` make the sequence repeat itself infinite number of times.

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
It is also possible to make a sequence of steps. Where each step is considered as a boolean function member of a class. To learn more about this topic review the Example Class_Function inside library files.


### Add Delay
```c++
void NonBlockingSequence.AddDelayInMillis(unsigned long val)
```
Add a new delay step to the sequence. This delay waits `val` milliseconds. This delay does not block other codes outside the sequence. It only delays the execution of the next step in the same sequence.
This method uses Arduino Timer0. So, to insure this method functions properly, do not change the timer0 regesters.

### Do Sequence Steps
```c++
bool NonBlockingSequence.DoSequence()
```
Run the sequence current step. The steps can either be a boolean function or timer. Each time the function `DoSequence()` is called, a step is called until a trigger action is happened. The trigger action can be:
- for boolean function:       return true.
- for timer           :       running time is over.

After the trigger action is occurred. The sequence start to call the next step.
The function `DoSequence()` should be called inside the `void loop()` method. Because each time `DoSequence()` is called the current steps is executed. As the method `DoSequence()` is called many times, the sequence go forward step by step.

`DoSequence()` return true when it is executed successfully. And return false when it
is finished executing the sequene.

### Sequence end
```c++
bool NonBlockingSequence.isFinish();
```
Check if the sequence reach its end, and finish executing all sequence steps. If the sequence is repeated infinite number of times without end, using `Repeat()` function, then `isFinish` function always return false.

### Restart
```c++
void NonBlockingSequence.Restart();
```
Force the sequence to start from the beginning.

### AutoRestart
```c++
void NonBlockingSequence.AutoRestart(unsigned long time_mil);
```
The Sequency will restart automatically when `DoSequence()` is not called for long time, longer than time_mil.
In regular case, The function `DoSequence()` is called repeatedly inside the arduino `void loop()`. If for any reason it was not called for long time, Then it may be need to be called from start next time. this function possible using `AutoRestart()`.

### Repeat
```c++
void NonBlockingSequence.Repeat();
```
Repeat the sequence for infinite number of times. Usually this method is used after all needed steps are added.

```c++
void NonBlockingSequence.Repeat(unsigned int n);
```
Repeat the previous sequence steps for `n` times. Usually this method is used after all needed steps are added.

### Next Step
```c++
bool NonBlockingSequence.NextStep();
```
Terminate the current step execution. Force the sequence to go to the next step. Next time the function `DoSequence()` is called, the next step is executed.
- if the previous action is done successfully         :        it returns true
- if the current state is the sequence final state    :        it returns false

### Pass Steps
```c++
unsigned int NonBlockingSequence.PassSteps(unsigned int n);
```
the sequence jump over n future step without calling. This function is similar to calling `NextStep()` for `n` time. It also returns number of successful step jumps.

### First Time Executing the Step
```c++
bool NonBlockingSequence.FirstTimeStepExecuting();
```
This function check if the current step is running for the first time. This function can be used to apply number of commands in a step for the first call only.

## Important Notes

### Nested Sequence
You can make many sequences nested with multiple levels.

### Conditional Sequence
It can be done using 2 different methods:
- Defining two different sequences. Use the if-else conditions, to select between them.
- In a single sequence, it is possible to define many adjacent steps, and conditionally jump over these steps without execution using `PassSteps()` method.

### Class Boolean Function
In the previous example, it is only mentioned to add steps of boolean function that do not belong to any class. But, is it possible to deal with class member functions? To answer this question look for the example Class_Function.
