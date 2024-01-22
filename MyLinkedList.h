#pragma once
#ifndef MYLINKEDLIST_H
#define MYLINKEDLIST_H

#include <Arduino.h>


template <class T> 
struct Element{
  T data;
  Element<T>* next=0; 
  int id_;
};


  
template <class T> 
class MyLinkedList{
  public:
 
int size_=0;
virtual void add_element(T data);
bool next();
void from_begining();
T get_element();

  private:

Element<T>* first;
Element<T>* last;
Element<T>* current_element;
};

template <class T> 
void MyLinkedList<T>::add_element(T data){
  Element<T>* e = new Element<T>();;
  e->data=data;
  e->id_=size_;
  if (size_==0){
      first=e;
      current_element=e;
  }
  else{
      last->next=e;
  }
  last=e;
  size_++;
}


template <class T> 
bool MyLinkedList<T>::next(){
  if (current_element->next != 0){
      current_element=current_element->next;
      return true;
  }else{ return false; }

}

template <class T>
void MyLinkedList<T>::from_begining(){
  current_element=first;
}

template <class T> 
T MyLinkedList<T>::get_element(){
  return current_element->data;
}

#endif
