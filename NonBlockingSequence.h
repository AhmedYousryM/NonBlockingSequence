#pragma once
#ifndef NONBLOCKINGSEQUENCE
#define NONBLOCKINGSEQUENCE

#include "MyLinkedList.h"
#include <Arduino.h>

/*
* This Library is used to build a sequence of commands.
* Arduino can run multiple sequences at the same time.
* visit https://github.com/AhmedYousryM/NonBlockingSequence

Author: Ahmed Yousry
Date: 16/12
*/


class NonBlockingSequence{

    public:

    bool DoSequence();
    // run the sequence (step by step)

    typedef bool (*func_ptr_type)();
    /*    definition of step function

    each step has the form:
                                bool    step_func(){
                                        //do commands
                                        }

        if step_func()  return  true :  go to the next step in the sequence
                                false:  run the current step again
    */

    void AddNewStep( func_ptr_type func_ptr);
    // add new step to the seuence

    void AddDelayInMillis(unsigned long val);
    // add new delay to the sequence

    void Restart();
    // Start the sequence from the begining

    void Repeat();
    // end-line to repeat the sequence for infinity

    void Repeat(unsigned int n);
    //end-line to repeat the sequence for infinity

    bool isFinish();
    // check if the sequence reach its end

    bool NextStep();
    // Force the sequence to go to the next step
    // Run the next step when the function DoSequence() calles

    unsigned int PassSteps(unsigned int n);
    // Pass n future step without run
    // the sequence jump over n future step

    bool FirstTimeStepExecuting();
    // this value is true if the step in its first time calling
    // this value is false if the step is called before

    void AutoRestart(unsigned long time_mil = 2000);
    // Automatically restart the sequence if not called for time longer than time_mil



    private:

    bool first_time_executing_step=true;
    enum step_type{ function_call=1, pause=2, repeat=3, repeat_n_times=4};
    struct step{
        func_ptr_type fun_ptr;
        NonBlockingSequence::step_type my_step_type; 
    };
    MyLinkedList< step > steps;
    MyLinkedList< unsigned long > pauses_time;
    unsigned long start_pause_time;
    func_ptr_type my_func_ptr;
    unsigned int _n_counter;
    unsigned int repetition;
    bool _end=false;
    unsigned long _time_mil=0;
    unsigned long _last_time_mil_sequence_do;
    
};

template <class A_class>
class ClassNonBlockingSequence{

    public:
    
    void AttachedObj(A_class* a);
    bool DoSequence();
    typedef bool (A_class::*func_ptr_type)();
    void AddNewStep( func_ptr_type func_ptr);
    void AddDelayInMillis(unsigned long val);
    void Restart();
    void Repeat();
    void Repeat(unsigned int n);
    bool isFinish();
    bool NextStep();
    unsigned int PassSteps(unsigned int n);
    bool FirstTimeStepExecuting();
    void AutoRestart(unsigned long time_mil = 2000);


    private:
    bool first_time_executing_step=true;
    A_class *a_obj;
    enum step_type{ function_call=1, pause=2, repeat=3, repeat_n_times=4};
    struct step{
        func_ptr_type fun_ptr;
        ClassNonBlockingSequence::step_type my_step_type; 
    };
    MyLinkedList< step > steps;
    MyLinkedList< unsigned long > pauses_time;
    unsigned long start_pause_time;
    func_ptr_type my_func_ptr;
    unsigned int _n_counter;
    unsigned int repetition;
    bool _end=false;
    unsigned long _time_mil=0;
    unsigned long _last_time_mil_sequence_do;
};






    template <class A_class>
    void ClassNonBlockingSequence<A_class>::AttachedObj(A_class* a){
        a_obj=a;
    }

    template <class A_class>
    bool ClassNonBlockingSequence<A_class>::DoSequence(){
        if(_time_mil>0){
            if ( millis() - _last_time_mil_sequence_do > _time_mil ){
                Restart();
            }
                _last_time_mil_sequence_do = millis() ;
        }
        if(_end==false){
        switch(steps.get_element().my_step_type){
            case ClassNonBlockingSequence<A_class>::function_call:
                if(first_time_executing_step==true){
                    my_func_ptr=steps.get_element().fun_ptr;
                    if( (a_obj->*my_func_ptr)() ){
                        NextStep();
                    }else{
                        first_time_executing_step=false;
                    }
                }else{
                    if( (a_obj->*my_func_ptr)() ){
                    NextStep();
                    }    
                }
            break;

            case ClassNonBlockingSequence<A_class>::pause:
                if(first_time_executing_step==true){
                    start_pause_time=millis();
                    first_time_executing_step=false;      
                }else if( millis()-start_pause_time > pauses_time.get_element() ){
                    // prepare for the next pause
                    pauses_time.next();
                    NextStep();                 
                }else{
                    // do nothing                   
                }
            break;

            case ClassNonBlockingSequence<A_class>::repeat:
                Restart();
            break;

            case ClassNonBlockingSequence<A_class>::repeat_n_times:
                if(_n_counter>0){
                    _n_counter--;
                    steps.from_begining();
                    pauses_time.from_begining();
                    first_time_executing_step=true;
                }else{
                    _end=true;
                }
            break;
        }
        }

        return !_end;
    }

    template <class A_class>
    void ClassNonBlockingSequence<A_class>::AddNewStep( func_ptr_type func_ptr ){
        step mystep;
        mystep.my_step_type= ClassNonBlockingSequence<A_class>::function_call;
        mystep.fun_ptr=func_ptr;
        steps.add_element(mystep);
    }

    template <class A_class>
    void ClassNonBlockingSequence<A_class>::AddDelayInMillis(unsigned long val){
        pauses_time.add_element(val);
        step mystep ;
        mystep.my_step_type= ClassNonBlockingSequence<A_class>::pause;
        steps.add_element(mystep);
    }


    template <class A_class>
    void ClassNonBlockingSequence<A_class>::Restart(){
        steps.from_begining();
        pauses_time.from_begining();
        _end=false;
        first_time_executing_step=true;
        _n_counter = repetition;
    }

    template <class A_class>
    void ClassNonBlockingSequence<A_class>::Repeat(){
        step mystep ;
        mystep.my_step_type= ClassNonBlockingSequence<A_class>::repeat;
        steps.add_element(mystep);
    }

    template <class A_class>
    void ClassNonBlockingSequence<A_class>::Repeat(unsigned int n){
        step mystep ;
        mystep.my_step_type= ClassNonBlockingSequence<A_class>::repeat_n_times;
        repetition = n - 1;
        _n_counter = n - 1;
        steps.add_element(mystep);
    }


    template <class A_class>
    bool ClassNonBlockingSequence<A_class>::isFinish(){
        return _end;
    }

    template <class A_class>
    bool ClassNonBlockingSequence<A_class>::NextStep(){
        if (steps.next()){
            first_time_executing_step=true;
            return true;
        }
        else{ _end=true; return false; }
    }

    template <class A_class>
    unsigned int ClassNonBlockingSequence<A_class>::PassSteps(unsigned int n){
        unsigned int pass = 0;
        for(unsigned int i=0; i<n ; i++){
            if (NextStep()) {
               pass++;
            }else{
                break;
            }
        }
        return pass;
    }

    template <class A_class>
    bool ClassNonBlockingSequence<A_class>::FirstTimeStepExecuting(){
        return first_time_executing_step;
    }

    template <class A_class>
    void ClassNonBlockingSequence<A_class>::AutoRestart(unsigned long time_mil){
        _time_mil=time_mil;
    }
#endif
