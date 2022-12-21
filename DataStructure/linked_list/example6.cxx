#include <iostream>
//Example 6 - Stack Using Linked List

struct Element
{
 void *data;
 Element *next;
};

bool push(Element **top, void *data)
{
 Element *elem = new Element();
 if(!elem) return false;

 elem->data = data;
 elem->next = *top;
 *top = elem;
 return true;
}

bool createStack(Element **top)
{
 *top = NULL;
 return true;
}

bool pop(Element **top, void **data )
{
 Element *elem;
 if(!(elem = *top)) return false;

 *data = elem->data;
 *top = elem->next;
 delete elem;
 return true;
}

bool deleteStack(Element **top)
{
 Element *elem;
 while(*top)
 {
  elem = (*top)->next;
  delete *top;
  *top = elem;
 }
 return true;
}

void printStack(Element *top) 
{
 if(top==NULL)
 {
  std::cout << "Empty!\n";
 }
 
 Element *cur = top;
 
 while(cur)
 {
  std::cout << *(static_cast<int *>(cur->data)) << " ";
  cur = cur->next;
 }
 std::cout << "\n\n";
}

int main()
{
 Element *head;
 createStack(&head);
 int n1 = 10, n2 = 20, n3 = 30, n4 = 40, n5 = 50;
 push(&head, &n1);
 push(&head, &n2);
 push(&head, &n3);
 push(&head, &n4);
 push(&head, &n5);

 printStack(head);

 void * n;
 pop(&head, &n);
 std::cout << "popped " << *(static_cast<int*>(n)) << std::endl;
 pop(&head, &n);
 std::cout << "popped " << *(static_cast<int*>(n)) << std::endl;
 std::cout << std::endl;

 printStack(head);

 std::cout << "deleting stack...\n";
 deleteStack(&head);
 printStack(head);
}