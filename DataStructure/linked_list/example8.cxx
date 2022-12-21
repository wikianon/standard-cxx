#include <cstdio>

//Example 8 Queue Struct : Using Linked List
struct node
{
 int data;
 node *next;
};

node *head = NULL;

void push(int n)
{
 node *newNode = new node();
 newNode->data = n;
 newNode->next = NULL;

 if(head == NULL)
 {
  head = newNode;
  return;
 }

 node *cur = head;
 
 while(cur)
  {
   if(cur->next==NULL)
    {
     cur->next = newNode;
     return;
    }
   cur = cur->next;
  }
}

void pop()
{
 if(head==NULL) return;
 
 node *tmp = head;
 head = head->next;
 delete tmp;
}

void display()
{
 node *cur = head;
 while(cur)
  {
   printf("%3d",cur->data);
   cur = cur->next;
  }
 printf("\n");
}

int main()
{
 printf("\n"); 
 push(1);
 push(2);
 push(3);
 push(4);
 push(5);
 display();
 
 pop();
 display();
 
 pop();
 display();
 
 pop();
 display();
 
 pop();
 display();
 
 pop();
 display();
 
 return 0;
}