#include <iostream>
/*
 Example 8B - Queue Class: Using Linked List
 First one becomes head, so when it pops, head will be popped.
 */
struct Queue
{
 Queue();
 ~Queue();
 void push(int);
 int pop();
 void print();

 private:
 struct Node
 {
  Node *next;
  int data;
 };
 Node* head;
};

Queue::Queue()
{
 head = NULL;
}

Queue::~Queue()
{
 if(head == NULL) return;
 Node *cur = head;
 while(cur) {
 Node *ptr = cur;
 cur = cur->next;
 delete ptr;
 }
}

void Queue::push(int n)
{
 if(head == NULL)
 {
  head = new Node;
  head->data = n;
  head->next = NULL;
  return;
 }
 
 Node *cur = head;
 
 while(cur)
  {
   if(cur->next == NULL)
   {
    Node *ptr = new Node;
    ptr->data = n;
    ptr->next = NULL;
    cur->next = ptr;
    return;
   }
   cur = cur->next;
  }
}

void Queue::print()
{
 if(head==NULL) return;
 Node *cur = head;
 
 while(cur)
 {
  std::cout << cur->data << " ";
  cur = cur->next;
 }
 
 std::cout << std::endl;
}

int Queue::pop()
{
 if(head == NULL)
 {
  std::cout << "empty estack!\n";
  return 0;
 }
 
 Node *tmp = head;
 int value = head->data;
 
 if(head->next)
 {
  head = head->next;
 }
 // pop the last element (head)
 else
 {
  delete tmp;
  head = NULL;
 }
 std::cout << "pop: " << value << std::endl;
 return value;
}

int main()
{
 Queue *que = new Queue();
 
 que->push(10);
 que->push(20);
 que->push(30);
 que->push(40);
 que->push(50);
 que->print();
 
 que->pop();
 que->print();
 
 que->pop();
 que->print();
 
 que->pop();
 que->print();
 
 que->pop();
 que->print();
 
 que->pop();
 que->print();
 
 que->pop();
 que->print();
 
 return 0;
}