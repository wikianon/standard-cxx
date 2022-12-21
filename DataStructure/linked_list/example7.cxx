#include <iostream>
//Example 7 - Stack Class Using Linked List

struct Stack
{
 Stack();
 ~Stack();
 void push(int);
 int pop();
 int peek();
 friend void print(Stack&);

 private:
 struct node
 {
  node *next;
  int data;
 };

 node *top;
};

Stack::Stack()
{
 top = NULL;
}

Stack::~Stack()
{
 while(top)
 {
  node *tmp = top;
  top = top->next;
  delete tmp;
 }
}

void Stack::push(int n) 
{
 node *ptr = new node();
 ptr->next = top;
 ptr->data = n;
 top = ptr;
}

int Stack::pop()
{
 node *tmp = top;
 int val = top->data;
 top = top->next;
 delete tmp;
 return val;
}

int Stack::peek()
{
 return top->data;
}

void print(Stack &s)
{
 Stack::node *cur = s.top;
 while(cur)
 {
  std::cout << cur->data << " ";
  cur = cur->next;
 }
 std::cout << std::endl;
}

int main()
{
 Stack *st = new Stack();
 
 st->push(10);
 st->push(20);
 st->push(30);
 st->push(40);
 st->push(50);
 
 print(*st);
 st->pop();
 st->pop();
 print(*st);
 
 std::cout << "current top: " << st->peek()<<"\n\n";
 return 0;
}