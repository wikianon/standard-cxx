#include <iostream>

#define MIN(a,b) (a < b ? a : b)

struct Stack
{
 Stack();
 ~Stack();
 void push(int);
 int pop();
 int peek();
 int peekMin();
 friend void print(Stack&);

 private:
 
 struct node
 {
  node *next;
  int data;
  int min;
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
 // currently empty (top is NULL)
 
 if(top == NULL) 
 ptr->min = n;
 else
 ptr->min = MIN(n, top->min); 
 top = ptr;
}

int Stack::pop()
{
 node *tmp = top;
 int val = top->data;
 top = top->next;
 delete tmp;
 std::cout << "pop " << val << std::endl;
 return val;
}

int Stack::peek()
{
 return top->data;
}

int Stack::peekMin()
{
 return top->min;
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
 st->push(40);
 st->push(50);
 st->push(20);
 st->push(10);
 st->push(30);
 print(*st);
 std::cout << "minimum = " << st->peekMin() << std::endl;
 st->pop();
 std::cout << "minimum = " << st->peekMin() << std::endl;
 st->pop();
 std::cout << "minimum = " << st->peekMin() << std::endl;
 st->pop();
 std::cout << "minimum = " << st->peekMin() << std::endl;
 st->pop();
 std::cout << "minimum = " << st->peekMin() << std::endl;
 print(*st);
 std::cout << "current top = " << st->peek() << "\n\n";
 return 0;
}