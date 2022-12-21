#include <iostream>
/*
 Example 7D - Stack Class Using Linked List with Query for Minimum Value (additional stack) 
*/
struct node
{
 node *next;
 int data;
};

struct Stack
{
 Stack();
 virtual ~Stack();
 virtual void push(int);
 virtual int pop();
 virtual int peekMin();
 int peek();
 friend void print(Stack&);

 private:
 
 bool empty();
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
 node *ptr = new node;
 ptr->next = top;
 ptr->data = n;
 top = ptr;
}

int Stack::pop()
{
 node *tmp = top;
 if(!empty())
  {
   int val = top->data;
   top = top->next;
   delete tmp;
   std::cout << "pop " << val << std::endl;
   return val;
  }
 else
  {
   std::cout << "empty! " << std::endl;
   return -1;
  }
}

int Stack::peek()
{
 if(!empty()) return top->data;
 return -1;
}

bool Stack::empty()
{
 if(top == NULL) return true;
 return false;
}

int Stack::peekMin()
{
 return -1; 
}

void print(Stack &s)
{
 node *cur = s.top;
 std::cout << std::endl;
 while(cur)
 {
  std::cout << cur->data << " ";
  cur = cur->next;
 }
 std::cout << std::endl;
}

struct StackWithMin : Stack
{
 StackWithMin();
 ~StackWithMin();
 void push(int);
 int pop();
 int peekMin();
 
 private:
 bool empty();
 node* top;
};

StackWithMin::StackWithMin()
{
 top = NULL;
}

StackWithMin::~StackWithMin()
{
 while(top)
  {
   node *tmp = top;
   top = top->next;
   delete tmp;
  }
}

void StackWithMin::push(int n)
{
 if(top)
  {
   // push only if it's smaller than the top min
   if(n < top->data)
    {
     node *ptr = new node;
     ptr->next = top;
     ptr->data = n;
     top = ptr;
    }
 }
 // if empty, just push the new element
 else
 {
  node *ptr = new node;
  ptr->next = top;
  ptr->data = n;
  top = ptr;
 }

 Stack::push(n);
}

int StackWithMin::pop()
{
 int popped = Stack::pop();
 if(empty())
  {
   std::cout << "empty min stack" << std::endl;
   return -1;
  }
 
 if(popped == top->data)
  {
   node *tmp = top;
   
   if(top->next)
    {
     top = top->next;
    }
   else
    {
     top = NULL;
    }  
   delete tmp;
  }
 return popped;
}

int StackWithMin::peekMin()
{
 if(!empty()) return top->data;
 std::cout << "empty min stack!" << std::endl;
 return -1;
}

bool StackWithMin::empty()
{
 if(top == NULL) return true;
 return false;
}

int main()
{
 Stack *st = new StackWithMin();
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
 st->pop();
 std::cout << "minimum = " << st->peekMin() << std::endl;
 print(*st);
 std::cout << "current top=" << st->peek();
 return 0;
}