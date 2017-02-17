// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Gauresh Vanjare

#include <iostream> 
#include "Vector.h"
#include <sstream>
#include <stdio.h>
#include "Vector.h"
#include "String.h"
// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
//Called at first 
   this->count=0;
   this->reserved=0;
   this->elements=NULL;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{ 
  this->count=rhs.count;
  this->reserved=rhs.reserved;
  this->elements=(T*) malloc( this->reserved*sizeof(T));
  for(int i=0;i<count ;i++){
      new(&(this->elements[i])) T(rhs.elements[i]);
  }

}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
//if(this == &rhs){
//  return *this;
//}  
    if( this->elements!=NULL ){
        for(int i=0;i< (this->count) ;i++){
            this->elements[i].~T;
        }
        free(elements);
    }
    this->count=rhs.count;
    this->reserved=rhs.reserved;
    this->elements=(T*) malloc(reserved*sizeof(T));
    for(int i=0;i<count ;i++){
        new(& (this->elements[i])) T(rhs.elements[i]);
    }
    return *this;
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
  this->elements =(T*) malloc(nReserved*sizeof(T));
  this->reserved=nReserved;
  //count =0;

}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"
   this->count=n;
   this->reserved=n;
   this->elements =(T*) malloc(n*sizeof(T));
   for(int i=0;i<count ;i++){
       new(&(this->elements[i])) T(t);
   }
  
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{
  // Using malloc to allocate 'n' amount of memory in heap.
   this->reserved = n;
   this->elements = (T*)malloc(n * sizeof(T));
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
  if( this->elements!=NULL){
  for (int i = 0; i < ( this->count); i++){
     this->elements[i].~T();
  }
  
    free( this->elements);
  
  }
    this->reserved=0  ;
   this->count =0;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
    if( this->count ==  this->reserved){
        this->reserved= this->reserved+1;
        T* tempo=(T*) malloc( this->reserved*sizeof(T));
        this->count= this->count+1;
    
        for(int i=0;i<( this->count-1);i++){
            new(&tempo[i])T( this->elements[i]);
            this->elements[i].~T();
        }
        new(tempo+( this->count-1))T(rhs);
        
        free( this->elements);
       
        this->elements=tempo;
        
    }
    else{
        this->count++;
        new(& (this->elements[count-1]))T(rhs);
    }
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
   if( this->count ==  this->reserved){
       this->reserved= this->reserved+1;
       T* tempo=(T*) malloc( this->reserved*sizeof(T));
       this->count++;
       for(int i=1;i< (this->count);i++){
           new(& (tempo[i]))T( this->elements[i-1]);
           this->elements[i-1].~T();
        }
        new(tempo)T(rhs);
        
        free( this->elements);
        
        this->elements=tempo;
        
    }
    else{
       for(int i=count;i>0;i--){
           new(& (this->elements[i]))T( this->elements[i-1]);
           this->elements[i-1].~T();
       }
    
        new( this->elements)T(rhs);
    
        this->count++;
    }

}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
    if(count!=0){
    
        this->elements[ this->count-1].~T();
        this->count--;
        //free(elements[count]);
    }
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
    if( this->count!=0){
    
        T* tempo=(T*) malloc( this->reserved*sizeof(T));
        
        for(int i=1;i< (this->count);i++){
            new(&tempo[i-1])T( this->elements[i]);
            this->elements[i].~T();
        }
        
        this->elements[0].~T();
        
        free( this->elements);
        
        this->elements=tempo;
        
        this->count--;
    }
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
    return  this->elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
// Return the last element
    int last =  this->count - 1;
    return  this->elements[last];
}

template <typename T>
T& Vector<T>::operator[](size_t i) 
{
    if (i < 0 || i >=  this->count){
        cout << "ERROR !" << endl;
      }
    return (*( this->elements+i));
}

template <typename T>
size_t Vector<T>::Size() const
{
    return  this->count;
}

template <typename T>
bool Vector<T>::Empty() const
{
    if( this->count==0){
        return true;
    }
    else{
        return false;
    }
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
    for(int i=0;i<( this->count);i++){
       this->elements[i].~T();
    }
    this->count=0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
    return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
    return VectorIterator<T>(elements+count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
    int j=0;
    for(VectorIterator<T> i=Begin();i!=it;i++){
      j++;
    }
    
    for(int i=j;i< count-1;i++){
        this->elements[i].~T();
        new(&elements[i])T( this->elements[i+1]);
    }
    
    this->elements[count-1].~T();
    
    count--;
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
   
   T* tempo=(T*) malloc( (count+1)*sizeof(T));
   int j=0;
   for(VectorIterator<T> i=Begin();i!=it;i++){
      j++;
   }
    
   for(int i=0;i<j;i++){
       new(&tempo[i])T( this->elements[i]);
   }
 
   new(&tempo[j])T( rhs);
      
    //this->elements[j].~T();
    //j++;
  
     
    for(int i=j;i<count;i++){
        new(&tempo[i+1])T( this->elements[i]);
       
    }
    for(int i=0;i<count;i++){
        this->elements[i].~T();
    }
      
    free( this->elements);
       
    elements=tempo;
    count=count+1;


}







#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
current=NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
current =c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
current =rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
    return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
    current= current +1;
    return current;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
    T* temp=current;
    current= current +1;
    return temp;
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{

    if(current !=rhs.current){
        return true;
    }
    else{
        return false;
    }
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
if(current !=rhs.current){
  return false;
}
else{
  return true;

}
}




