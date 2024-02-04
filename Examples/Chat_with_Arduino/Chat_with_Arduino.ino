#include <NonBlockingSequence.h>

//visit https://github.com/AhmedYousryM/NonBlockingSequence

NonBlockingSequence chat;
NonBlockingSequence subsequence;
String TotalString;
String SubSeqString;
unsigned int char_no = 0;
unsigned int SubSeqchar_no = 0;
String UserName;

void setup() {
  // put your setup code here, to run once:

    Serial.begin(9600);
    //delay(1500);
  Add_sentence("Hi.");
  chat.AddDelayInMillis(1000);
  Add_sentence("I hope you have a good day.");
  chat.AddDelayInMillis(1000);
  Add_sentence("My name is chatichat.");
  chat.AddDelayInMillis(1000);
  Add_sentence("What is your name?");
  chat.AddDelayInMillis(1000);
  chat.AddNewStep( &NewLine );
  chat.AddNewStep( &RecieveName );
  chat.AddNewStep( &ifNameisRecieved );
  chat.AddNewStep( &subSeq_Respond );
  chat.AddDelayInMillis(1000);
  Add_sentence("I can count from 1 to 10");
  chat.AddDelayInMillis(1000);
  Add_sentence("1  2  3  4  5  6  7  8  9  10");
  chat.AddDelayInMillis(1000);
  Add_sentence("Do you want to repeat this conversation? (y/n)");
  chat.AddDelayInMillis(1000);
  chat.AddNewStep( &Respond );
  Add_sentence("Good bye.");
  chat.AddDelayInMillis(1000);

  Add_sentence_to_subsequence("Nice Name");
  subsequence.AddDelayInMillis(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  chat.DoSequence();
}

void Add_sentence_to_subsequence(String str){
  SubSeqString=SubSeqString+str;
  for(unsigned int i = 0; i<str.length(); i++){
    subsequence.AddNewStep( &SubSeqWriteChar );
    subsequence.AddDelayInMillis(100);
  }
    subsequence.AddNewStep( &NewLine );
}

void Add_sentence(String str){
  TotalString=TotalString+str;
  for(unsigned int i = 0; i<str.length(); i++){
    chat.AddNewStep( &WriteChar );
    chat.AddDelayInMillis(100);
  }
    chat.AddNewStep( &NewLine );
}

bool WriteChar(){
  Serial.print(TotalString.charAt(char_no));
  char_no++;
  return true;
}

bool SubSeqWriteChar(){
  Serial.print(SubSeqString.charAt(SubSeqchar_no));
  SubSeqchar_no++;
  return true;
}

bool NewLine(){
  Serial.println();
}

bool subSeq_Respond(){
  subsequence.DoSequence();
  return subsequence.Finish();
}

bool Respond(){
  if (chat.first_time_exexuting_step==true){
      while (Serial.available() > 0) {Serial.read();}
  }

  if (Serial.available()){
    char ch=Serial.read();
    if ((ch=='y')|(ch=='Y')){
      chat.Restart();
      subsequence.Restart();
      char_no = 0;
      SubSeqchar_no = 0;
      UserName="";
      return 0;
    }else if((ch=='n')|(ch=='N')){
      return 1;
    }else{
      return 0;
    }
  }else{
    return 0;
  }
}

bool ifNameisRecieved(){
  if (UserName.length()<=3){chat.PassSteps(2);}
  return 1;
}


unsigned long mm;
bool RecieveName(){

    if (chat.first_time_exexuting_step==true){
      mm=millis();
      while (Serial.available() > 0) {Serial.read();}
      return false;
    }else{
      if ((millis()-mm)>10000){
        return true;
      }else{
          if (Serial.available()){
            UserName=Serial.readString();
            Serial.println(UserName);
            return true;
          }else{
            return false;
          }
      }
    }
    

}
