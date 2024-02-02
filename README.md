# NonBlockingSequence
This liberary is used to build a sequence of tasks. Each task is repeatedly called, until specific trigger, then the next task is called. This trigger can be anything: variable value, input signal, or timer. It is possible to run multiple sequences at the same time, without blocking each other.

This sequence of tasks is similar to PLC for induxterial sequence.
## Introdunctory Example

'''
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
// task 1 code
}

bool Task3(){ 
// task 1 code
}
'''
When the program call the function 'Sequence.DoSequence()', the 'Sequence' will start calling 'Task1()'. And will continue executing task1 until the function call returns true. Then 'Task2()' will be called until it returns true. Then 'Task3()' will be called until it returns true. After that, the sequence will end and each time the function 'Sequence.DoSequence()' is called nothing will happen, unless the program is written to repeat or reset the 'Sequence'.
The sequence execution order 
  Task1()-->Task2()-->Task3()-->End
