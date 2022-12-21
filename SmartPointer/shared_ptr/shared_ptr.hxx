#include<iostream>

template <typename T>
struct CuShPtr 
{
 CuShPtr(T* _ptr) 
 {
  ptr = _ptr;
  count = new int(1);
 }

 ~CuShPtr() 
 {
  //std::cout << "In ~CuShPtr\n";
  if(count != nullptr) 
  {
   dec_count();
   if(*count == 0) 
   {
    delete ptr;
    delete count;
   }
  }
 }

 CuShPtr(const CuShPtr<T>& other) 
 {
  ptr = other.ptr;
   count = other.count;
  inc_count();
 }

 CuShPtr<T>& operator=(const CuShPtr<T>& other) 
 {
  //only if count != nullptr?
   ptr = other.ptr;
   count = other.count;
   inc_count();
  return *this;
 }

 int use_count() const 
 {
  if(count != nullptr)
   return *count;
   else
  return 0;
 }

 void reset() 
 {
  if(count != nullptr) 
  {
   dec_count();
   if(*count == 0) 
   {
    delete ptr;
    delete count;
   }
   count = nullptr;
   ptr = nullptr;
  }
 }

 T* operator*() 
 {
  //std::cout << "In operator *\n";
  return ptr;
 }

 T* operator ->() 
 {
  //std::cout << "In operator ->\n";
  return ptr;
 }

  private:
  T* ptr;
  int *count;	//shared. shallow copied

 inline void dec_count() 
 {
  (*count)--;
 }

 inline void inc_count() 
 {
  (*count)++;
 }
};