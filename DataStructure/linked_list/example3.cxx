#include <cstring>
#include <iostream>


struct node
{
 char name[20];
 int id;
 node *next;
};

node * initNode(const char *, int);
void displayNode(node *);
void displayList(node *);
void addNode(node *);
node * searchName(node *, const char * );
void deleteNode(node * );
void insertNode(node * );
void deleteList(node * );

node *head = (node *) NULL;
node *tail = (node *) NULL;

// allocates memory for the node
// assign values to the member of the structure
node * initNode(const char *name, int id)
{
 node *ptr = new node;

 // error? then just return
 if( ptr == NULL ) 
 return static_cast<node *>(NULL); 
 // assign it 
 // then return pointer to ne node
 else
 { 
 strcpy(ptr->name, name); 
 ptr->id = id; 
 return ptr; 
 }
}

// adding to the end of list
void addNode(node *newnode) 
{
 // if there is no node, put it to head
 if(head == NULL)
 {
  head = newnode; 
  tail = newnode;
 }

 // link in the new_node to the tail of the list
 // then mark the next field as the end of the list
 // adjust tail to point to the last node

 tail->next = newnode; 
 newnode->next = NULL; 
 tail = newnode; 
}
 
void insertNode(node *newnode)
{
 node *temp, *prev; 

 if( head == NULL )
  { // if an empty list, 
   head = newnode; // set 'head' to it 
   tail = newnode;
   head->next = NULL; // set end of list to NULL 
   return; 
  }
  
 temp = head; // start at beginning of list
 // while currentname < newname 
 while(strcmp(temp->name, newnode->name) < 0)
  {
  // to be inserted then 
  temp = temp->next; // goto the next node in list
  
  if(temp == NULL) // don't go past end of list 
   break;
  }
 
 // set previous node before we insert 
 // first check to see if it's inserting 
 if(temp == head)
  { // before the first node 
   newnode->next = head; // link next field to original list 
   head = newnode; // head adjusted to new node
  }
 else
  { // it's not the first node
   prev = head; // start of the list,
   while(prev->next != temp)
    { // will cycle to node before temp
     prev = prev->next;
    }
   prev->next = newnode; // insert node between prev and next 
   newnode->next = temp;
   
   if(tail == prev) // if the new node is inserted at the 
   tail = newnode; // end of the list the adjust 'end' 
 }
}

node * searchName(node *ptr, const char *name )
{
 while(strcmp( name, ptr->name ) != 0)
  { 
   ptr = ptr->next; 
   
   if( ptr == NULL ) 
   break; 
  }
 return ptr; 
}

node* searchId(node* ptr, int id)
{
 while( id != ptr->id )
  { 
   ptr = ptr->next; 
   if( ptr == NULL ) 
   break; 
  }
 return ptr; 
}

void reverse()
{
 // we need at least two nodes for the reverse to have any effect
 if(head == NULL || head->next == NULL) return;

 // Starting 2nd list as 'me' and 'head' is now 'me->next'
 // and 'head->next' is pointing to NULL
 // So, the 3rd list is now 'child' of 'me'
 node *parent = head;
 node *me = head->next;
 node *child = me->next;

 // convert head to tail
 parent->next = NULL;
 
 while(child != NULL)
  {
   me->next = parent;
   parent = me;
   me = child;
   child = child->next;
  }
   // when me reached the tail
   // me becomes head
   head = me;
   // the head is now pointing to the 2nd last node
   head->next = parent;
}

void deleteNode(node *ptr)
{
 node *temp;
 node *prev;
 temp = ptr; // node to be deleted 
 prev = head; // start of the list, will cycle to node before temp 

 if( temp == prev )
  { // deleting first node? 
   head = head->next; // moves head to next node 
   if( tail == temp ) // is it end, only one node? 
   tail = tail->next; // adjust end as well 
   delete temp ; // free up space 
  }
  else
  { // if not the first node, then 
   while( prev->next != temp )
    { // move prev to the node before
     prev = prev->next; // the one to be deleted 
    }
   prev->next = temp->next; // link previous node to next 
   
   if( tail == temp ) // if this was the end node, 
    tail = prev; // then reset the end pointer 
    delete temp; // free up space
 }
}

void deleteList(node *ptr)
{
 node *temp;

 if( head == NULL ) return; // don't try to delete an empty list 

 if( ptr == head )
 { // if we are deleting the entire list 
  head = NULL; // then reset head and 
  tail = NULL; // end to empty 
 }
 else
 {
  temp = head; // if it's not the entire list, readjust end 
  while( temp->next != ptr ) // locate previous node to ptr 
  temp = temp->next;
  tail = temp; // set end to node before ptr 
 }

 while( ptr != NULL )
 { // while there are still nodes to delete 
  temp = ptr->next; // record address of next node 
  delete ptr; // free this node 
  ptr = temp; // point to next node to be deleted 
 }
}

void displayNode(node *ptr)
{
 std::cout << ptr->id << ": " << ptr->name << std::endl;
}

void displayList(node *ptr)
{
 while( ptr != NULL )
  {
  displayNode( ptr ); 
  ptr = ptr->next; 
 }
}

int main()
{
 int id;
 int ch = 1;
 node *ptr;
 const char* name;
 
 // add
 ptr = initNode( "s1", 1 );
 addNode(ptr);
 ptr = initNode( "s2", 2 );
 addNode(ptr);
 ptr = initNode( "s3", 3 );
 addNode(ptr);
 ptr = initNode( "s4", 4 );
 addNode(ptr);
 ptr = initNode( "s5", 5 );
 addNode(ptr); 
 displayList(head);

 // delete
 name = "s2";
 ptr = searchName(head, name );
 if( ptr == NULL )
 {
  std::cout << "\nName: " << name << " not found\n";
 }
 else 
 {
  std::cout << "\nDeleting a node ... ";
  displayNode(ptr);
  deleteNode( ptr );
 }
 
 displayList(head);

 // insert
 name = "s2";
 id = 2;
 ptr = initNode( name, id );
 insertNode( ptr );
 
 std::cout << "\nInserting a node ... ";
 displayNode(ptr);
 displayList(head);

 // reverse
 std::cout << "\nReversing the list ... \n";
 reverse();
 displayList(head);

 // delete
 std::cout << "\nIn the end, deleting the list ... \n";
 deleteList(head);
 displayList(head);
 
 return 0;
}