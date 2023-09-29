/*
* file: arrayheap-student-proj6.h
* author: Michael Mathews
* course: CSI 3334
* assignment: project 6
* due date: April 29, 2022
* version: 11.7
 
 This file implements the ArrayHeap class with an added dataToHeap member.
 This helps allow for using keys to change values in the data array
*/

#ifndef ARRAY_HEAP_STUDENT_PROJ6_H
#define ARRAY_HEAP_STUDENT_PROJ6_H

#include "arrayheap-prof-proj6.h"
#include <algorithm>

using namespace std;

/* ArrayHeap default constructor
*  parameters:
*      none
*  return value: none
*
* This function creates the base ArrayHeap
*/
template <class T>
ArrayHeap<T>::ArrayHeap(){
    this->numItems = 0;
    this->capacity = 5;
    this->data = new T[this->capacity];
    this->heapAndFreeStack = new int[this->capacity];
    this->dataToHeap = new int[this->capacity];
    //initialize free stack and DTH
    for(int i = 0; i < this->capacity; i++){
        this->heapAndFreeStack[i] = i;
        this->dataToHeap[i] = i;
    }
}

/* ArrayHeap copy constructor
*  parameters:
*      none
*  return value: none
*
* This function copies an ArrayHeap
*/
template <class T>
ArrayHeap<T>::ArrayHeap(ArrayHeap<T> const &h){
    //deep copy data members
    this->numItems = h.numItems;
    this->capacity = h.capacity;
    this->data = new T[h.capacity];
    this->heapAndFreeStack = new int[h.capacity];
    this->dataToHeap = new int[h.capacity];
    for(int i = 0; i < h.capacity; i++){
        this->data[i] = h.data[i];
        this->heapAndFreeStack[i] = h.heapAndFreeStack[i];
        this->dataToHeap[i] = h.dataToHeap[i];
    }
}
 
/* ArrayHeap destructor
*  parameters:
*      none
*  return value: none
*
* This function deletes an ArrayHeap
*/
template <class T>
ArrayHeap<T>::~ArrayHeap(){
    delete [] this->data;
    this->data = nullptr;
    delete [] this->heapAndFreeStack;
    this->heapAndFreeStack = nullptr;
    delete [] this->dataToHeap;
    this->dataToHeap = nullptr;
}

/* insert
*  parameters:
*      item - thing to be inserted
*  return value: key to where item is stored in the data array
*
* This function inserts an item to the ArrayHeap
*/
template <class T>
int ArrayHeap<T>::insert(T const &item){
    int key;
    //make sure there is room to insert
    if(this->numItems == this->capacity){
        this->doubleCapacity();
    }
    //get next index from free stack and store item there
    this->data[heapAndFreeStack[numItems]] = item;
    this->dataToHeap[heapAndFreeStack[numItems]] = numItems;
    //save key
    key = heapAndFreeStack[numItems];
    bubbleUp(numItems);
    this->numItems++;
    //location item is stored in the data array
    return key;
}

/* removeMinItem
*  parameters:
*      none
*  return value: none
*
* This function removes the highest priority item from an ArrayHeap
*/
template <class T>
void ArrayHeap<T>::removeMinItem(){
    const int FIRST = 0;
    //make sure there is an item to remove
    if(this->numItems > 0){
        //bring last element to front and decrement size
        swap(heapAndFreeStack[0],heapAndFreeStack[numItems-1]);
        swap(dataToHeap[heapAndFreeStack[0]],
             dataToHeap[heapAndFreeStack[numItems-1]]);
        this->numItems--;
        //bubble down first index
        bubbleDown(FIRST);
    }
}

/* operator=
*  parameters:
*      none
*  return value: none
*
* This function allows for the = operator to be used on ArrayHeaps
*/
template <class T>
ArrayHeap<T> const &ArrayHeap<T>::operator=(ArrayHeap<T> const &ah){
    //make sure not the same; copy data and heap
    if(this != &ah){
        //deallocate memory
        delete [] this->data;
        delete [] this->heapAndFreeStack;
        //deep copy data members
        this->numItems = ah.numItems;
        this->capacity = ah.capacity;
        this->data = new T[this->capacity];
        this->heapAndFreeStack = new int[this->capacity];
        this->dataToHeap = new int[this->capacity];
        for(int i = 0; i < ah.capacity; i++){
            this->data[i] = ah.data[i];
            this->heapAndFreeStack[i] = ah.heapAndFreeStack[i];
            this->dataToHeap[i] = ah.dataToHeap[i];
        }
    }
    return *this;
}

/* doubleCapacity
*  parameters:
*      none
*  return value: none
*
* This function doubles the capacity of an ArrayHeap
*/
template <class T>
void ArrayHeap<T>::doubleCapacity(){
    //make a larger heapAndFreeStack and data
    int initCap = this->capacity;
    this->capacity *= 2;
    T *tempData = new T[this->capacity];
    int *tempHeap = new int[this->capacity];
    int *tempDTH = new int[this->capacity];
    //initialize free stack and dth
    for(int i = 0; i < this->capacity; i++){
        tempHeap[i] = i;
        tempDTH[i] = i;
    }
    //copy data members to larger array
    for(int i = 0; i < initCap; i++){
        tempData[i] = this->data[i];
        tempHeap[i] = this->heapAndFreeStack[i];
        tempDTH[i] = this->dataToHeap[i];
    }
    //delete original arrays
    delete [] this->data;
    delete [] this->heapAndFreeStack;
    delete [] this->dataToHeap;
    //set implicit data and heap to larger arrays
    this->data = tempData;
    this->heapAndFreeStack = tempHeap;
    this->dataToHeap = tempDTH;
}

/* getMinItem
*  parameters:
*      none
*  return value: templated item from the top of the heap
*
* This function returns the highest priority item from an ArrayHeap
*/
template <class T>
T const &ArrayHeap<T>::getMinItem() const{
    return this->data[heapAndFreeStack[0]];
}

/* getNumItems
*  parameters:
*      none
*  return value: integer that represents the number of items in an ArrayHeap
*
* This function returns the number of items in an ArrayHeap
*/
template <class T>
int ArrayHeap<T>::getNumItems() const{
    return this->numItems;
}

/* bubbleUp
*  parameters:
*      ndx - index in the arrayHeap that points to the data to be bubbled up
*  return value: none
*
* This function bubbles up an item in an ArrayHeap until in the right location
*/
template <class T>
void ArrayHeap<T>::bubbleUp(int ndx){
    bool done = false;
    while(!done){
        //find parent
        int parent = (ndx-1) / 2;
        //make sure not first item
        if(ndx > 0){
            if(data[heapAndFreeStack[ndx]] < data[heapAndFreeStack[parent]]){
                swap(heapAndFreeStack[parent], heapAndFreeStack[ndx]);
                swap(dataToHeap[heapAndFreeStack[parent]],
                     dataToHeap[heapAndFreeStack[ndx]]);
                ndx = parent;
            }
            //process is where it should be
            else{
                done = true;
            }
        }
        //no parent
        else{
            done = true;
        }
    }
}

/* bubbleDown
*  parameters:
*      ndx - index in the arrayHeap that points to the data to be bubbled down
*  return value: none
*
* This function bubbles down an item in an ArrayHeap until in the right location
*/
template <class T>
void ArrayHeap<T>::bubbleDown(int ndx){
    int child1, child2;
    bool done = false;
    
    while(!done){
        child1 = ndx*2 + 1;
        child2 = ndx*2 + 2;
        //check if viable 2 children valid
        if(child2 < this->numItems){
            //check if a child is less than the current value
            if(data[heapAndFreeStack[child1]] < data[heapAndFreeStack[ndx]] or
               data[heapAndFreeStack[child2]] < data[heapAndFreeStack[ndx]]){
                //check which child is smaller
                //if child 1 is smaller than child 2, swap it, else swap child2
                if(data[heapAndFreeStack[child1]] <
                   data[heapAndFreeStack[child2]]){
                    swap(heapAndFreeStack[child1], heapAndFreeStack[ndx]);
                    swap(dataToHeap[heapAndFreeStack[child1]],
                         dataToHeap[heapAndFreeStack[ndx]]);
                    ndx = child1;
                }
                else{
                    swap(heapAndFreeStack[child2], heapAndFreeStack[ndx]);
                    swap(dataToHeap[heapAndFreeStack[child2]],
                         dataToHeap[heapAndFreeStack[ndx]]);
                    ndx = child2;
                }
            }
            else{
                done = true;
            }
        }
        //if only one viable child
        else if(child1 < this->numItems){
            //if child 1 is smaller, swap it, else swap child2
            if(data[heapAndFreeStack[child1]] < data[heapAndFreeStack[ndx]]){
                swap(heapAndFreeStack[child1], heapAndFreeStack[ndx]);
                swap(dataToHeap[heapAndFreeStack[child1]],
                     dataToHeap[heapAndFreeStack[ndx]]);
                ndx = child1;
            }
            else{
                done = true;
            }
        }
        //no children
        else{
            done = true;
        }
    }
}

/* isOnHeap
*  parameters:
*      key - index where the new item is inserted into the data array
*  return value: returns true if item is on heap and false if not
*
* checks to see if a given item is on the heap.
*/
template <class T>
bool ArrayHeap<T>::isOnHeap(int key) const{
    bool isOn = false;
    if(0 <= key and key < capacity){
        if(0 <= dataToHeap[key] and dataToHeap[key] < numItems){
            if(heapAndFreeStack[dataToHeap[key]] == key){
                isOn = true;
            }
        }
    }
    return isOn;
}

/* changeItemAtKey
*  parameters:
*      key - index where the new item is inserted into the data array
*      newItem - item that is to replace existing item at key
*  return value: none
*
* changes the values at a given key in the data array
*/
template <class T>
void ArrayHeap<T>::changeItemAtKey(int key, T const &newItem){
    if(newItem < this->data[key]){
        data[key] = newItem;
        bubbleUp(dataToHeap[key]);
    }
    else{
        data[key] = newItem;
        bubbleDown(dataToHeap[key]);
    }
}


#endif
