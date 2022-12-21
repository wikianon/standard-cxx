#include <iostream>
/*
 Example 9 - Finding Intersection and Union of Two Linked List

 The following code finds intersection/union of two linked list and puts it into a new linked list
 */

struct node
{
 int data;
 node *next;
};

void add(node **head, int n)
{
 node *cur;
 node *no = new node();
 no->data = n;
 no->next = NULL;
 
 if(*head == NULL)
  {
   *head = no;
   return;
  }

 cur = *head;
 while(cur)
  {
   if(cur->next == NULL)
   {
    cur->next = no;
    return;
   }
   cur = cur->next;
  }
}

bool isDuplicate(node *head, int n)
{
 node* cur = head;
 
 while(cur)
 {
  if(cur->data == n) return true;
  cur = cur->next;
 }
 return false;
}

node *getIntersection(node *headA, node *headB)
{
 node *curA = headA;
 node *curB = headB;
 node *result = NULL;
 if(curA == NULL || curB == NULL) return NULL;
 //Not be intersection start
 while(curA)
 {
  while(curB)
   {
    if(curA->data == curB->data)
    {
     add(&result, curA->data);
    }
   }
  curB = curB->next;
 }
 
 curB = headB;
 curA = curA->next;
 
 return result;
}

node *getUnion(node *headA, node *headB)
{
 node *cur;
 node *result = NULL;
 if(headA == NULL && headB == NULL) return NULL;

 cur = headA;
 while(cur)
  {
   add(&result, cur->data);
   cur = cur->next;
  }

 cur = headB;
 while(cur)
  {
   /* check if the new data is already there */
   if(!isDuplicate(result, cur->data))add(&result, cur->data);
   cur = cur->next;
  }
  
 return result;
}

void display(node *head)
{
 if(head == NULL) return;
 node *cur = head;
 while(cur)
  {
   std::cout << cur->data << ' ';
   cur = cur->next;
  }
 std::cout << std::endl;
}

int main()
{
 node *myListA = NULL;
 node *myListB = NULL;
 node *intersectionList = NULL;
 node *unionList = NULL;

 add(&myListA,10);
 add(&myListA,20);
 add(&myListA,30);
 add(&myListA,40);
 add(&myListA,50);
 add(&myListA,60);
 add(&myListA,70);
 std::cout << "List A: ";
 display(myListA);

 add(&myListB,10);
 add(&myListB,30);
 add(&myListB,50);
 add(&myListB,70);
 add(&myListB,90);
 add(&myListB,110);
 add(&myListB,130);
 std::cout << "List B: ";
 display(myListB);

 std::cout << "Intersection of A and B: ";
 intersectionList = getIntersection(myListA, myListB);
 display(intersectionList);

 std::cout << "Union of A and B: ";
 unionList = getUnion(myListA, myListB);
 display(unionList);

 return 0;
}
/*
 List A: 10 20 30 40 50 60 70
List B: 10 30 50 70 90 110 130
Intersection of A and B: 10 30 50 70
Union of A and B: 10 20 30 40 50 60 70 90 110 130
 */