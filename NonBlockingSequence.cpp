#include "NonBlockingSequence.h"

    bool NonBlockingSequence::DoSequence(){
        
        if(_time_mil>0){
            if ( millis() - _last_time_mil_sequence_do > _time_mil ){
                Restart();
            }
                _last_time_mil_sequence_do = millis() ;
        }

        if (_end == false){
        switch(steps.get_element().my_step_type){

            case NonBlockingSequence::function_call:
                if(first_time_executing_step==true){
                    my_func_ptr=steps.get_element().fun_ptr;
                    if( (my_func_ptr)() ){
                        NextStep();
                    }else{
                        first_time_executing_step=false;
                    }
                }else{
                    if( (my_func_ptr)() ){
                    NextStep();
                    } 
                }
   
            break;

            case NonBlockingSequence::pause:
                if(first_time_executing_step==true){
                    start_pause_time=millis();
                    first_time_executing_step=false;
                }else if( millis()-start_pause_time > pauses_time.get_element() ){
                    // prepare for the next pause
                    pauses_time.next();
                    // go to next step
                    NextStep();                    
                }else{
                    // do nothing
                    
                }
            break;

            case NonBlockingSequence::repeat:
                Restart();
            break;

            case NonBlockingSequence::repeat_n_times:
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

    void NonBlockingSequence::AddNewStep( func_ptr_type func_ptr ){
        step mystep;
        mystep.my_step_type= NonBlockingSequence::function_call;
        mystep.fun_ptr=func_ptr;
        steps.add_element(mystep);
    }

    void NonBlockingSequence::AddDelayInMillis(unsigned long val){
        pauses_time.add_element(val);
        step mystep ;
        mystep.my_step_type= NonBlockingSequence::pause;
        steps.add_element(mystep);
    }



    void NonBlockingSequence::Restart(){
        steps.from_begining();
        pauses_time.from_begining();
        _end=false;
        first_time_executing_step=true;
        _n_counter = repetition;
        _last_time_mil_sequence_do = millis() ;
    }

    void NonBlockingSequence::Repeat(){
        step mystep ;
        mystep.my_step_type= NonBlockingSequence::repeat;
        steps.add_element(mystep);
    }

    void NonBlockingSequence::Repeat(unsigned int n){
        step mystep ;
        mystep.my_step_type= NonBlockingSequence::repeat_n_times;
        repetition = n - 1 ;
        _n_counter = n - 1 ;
        steps.add_element(mystep);
    }



    bool NonBlockingSequence::isFinish(){
        return _end;
    }

    bool NonBlockingSequence::NextStep(){
        if (steps.next()){ 
            first_time_executing_step=true;
            return true;
        }
        else{ _end=true; return false; }
    }

    unsigned int NonBlockingSequence::PassSteps(unsigned int n){
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

    bool NonBlockingSequence::FirstTimeStepExecuting(){
        return first_time_executing_step;
    }


    void NonBlockingSequence::AutoRestart(unsigned long time_mil){
        _time_mil=time_mil;
    }
