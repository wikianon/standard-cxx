#include <iostream>

//prototypes
void printline(char, int);
bool name_valid(std::string);
bool mob_valid(std::string);

struct contact
{
     //Initialize the contact by a default value
        contact(): name(""), mob(""){}
        
        // Shows all contacts
        bool show()
        {
            if(name != "")
            {
                std::cout << name << "\t" << mob << std::endl;
                return 1; //Indicates success
            }
            else
                return 0; //Indicates failure
        }
        
        //Search
        bool show(std::string search_term)
        {
            if(search_term == name)
            {
                std::cout << name << "\t" << mob << std::endl;
                return 1;
            }
            else
                return 0;
        }
        
        //Checks whether the name exists or not
        bool name_exists(std::string tname)
        {
            if(tname == name)
                return 1;
            else
                return 0;
        }
        
        //The contact object is initialized by valid values
        bool add(std::string new_name, std::string new_mob)
        {
            if(name=="")
            {
                name = new_name;
                mob = new_mob;
                return 1; // Success
            }
            else
                return 0; // Failure
    
        }
        
        //Edits the contact details
        bool edit(std::string);
        
        //Sets the contact details to default values
        //That is, the contact details are thus erased
        bool erase(std::string new_name)
        {
            if(new_name==name)
            {       
                name = "";
                mob = "";
                return 1;
            }
            else
                return 0;
        }
  private:
   std::string name;
   std::string mob;
};

//Edits the contact
bool contact::edit(std::string new_name)
{
 std::string new_mob;
 
 if(new_name == name)
  {
   std::cout << "Enter new name: ";
   getline(std::cin,new_name);
        
   std::cout << "Enter new mobile no: ";
   getline(std::cin,new_mob);
        
   name = new_name;
   mob = new_mob;
   
   return 1;
  }
  else
  return 0;
}



int main()
{
 contact person[100];
            
 std::string temp_name, temp_mob;
 int choice, counter;
 bool flag;
 bool cancel_flag;
    
    std::cout << "\n\t**** PHONEBOOK ****";
    
    do
    {   
        std::cout << "\n\t";
        printline('-', 19);
        std::cout << "\n\t0. Show contacts"
                     "\n\t1. Add Contact"
                     "\n\t2. Edit Contact"
                     "\n\t3. Delete Contact"
                     "\n\t4. Search"
                     "\n\t5. Quit"
                     "\n\tYour choice...";
        std::cin >> choice;
        
        printline('-', 20);
        cancel_flag = 0;
        flag = 0;
        counter = 0;
        
        switch(choice)
        {
            case 0: 
                std::cout << "\n\tShowing Contacts\n";
                printline('-', 20);
                
                for(int i=0; i<100; i++)
                    if(person[i].show())
                        flag = 1;
                
                if(!flag)
                    std::cout << "\n\tNo contacts found!\n";
                break;
                
            case 1:
                std::cout << "\n\tAdd New Contact\t\t\t\tpress $ to cancel\n";
                printline('-', 20);
                counter = 0;
                
                //Loop until correct name and mobile number are entered
                do
                {
                 flag = 0;
                 
                 if(counter)std::cout << "\n\tTry again\t\t\t\tpress $ to cancel\n"; 
                      
                 //counts how many times the do-while loop executes
                 counter++; 
                        
                 std::cout << "Name: ";
                 getline(std::cin,temp_name);
                 //Cancel operation
                 
                 if(temp_name=="$")
                  {
                   cancel_flag = 1;
                   break;
                  }
                 
                 std::cout << "Mobile No.: ";
                 getline(std::cin,temp_mob);
                    
                 //Cancel operation
                 if(temp_mob=="$")
                  {
                   cancel_flag = 1;
                   break;
                  }
                    
                  //Check whether name exists
                  for(int i=0; i<100; i++)
                  if(person[i].name_exists(temp_name))
                   {
                    std::cout << "\n\tThe name you entered is already there" 
                                 "in the phonebook, enter a different name.\n";
                    flag = 1;
                    break;
                   }
                    
                }while(!name_valid(temp_name) || flag || !mob_valid(temp_mob));
                
                if(cancel_flag)
                {
                 break;
                }
                
                //This code adds the contact to phonebook    
                for(int i=0; i<100; i++)
                 if(person[i].add(temp_name, temp_mob))
                  {
                   std::cout << "\n\tContact added successfully!" << std::endl;
                   flag = 1;
                   break;
                  }
                
                if(!flag)
                 std::cout << "\n\tMemory full! Delete some contacts first.\n";
                break;
                
            case 2:
                std::cout << "\n\tEnter a contact name to edit: "
                             "\t\t\t\tpress $ to cancel\n";
                             std::cin >> temp_name;
                
                //Cancel Operation
                if(temp_name=="$")
                {
                 break;
                }
                
                for(int i=0; i<100; i++)
                 if(person[i].edit(temp_name))
                  {
                   std::cout << "\n\tEdited Successfully!\n";
                   flag = 1;
                   break;
                  }
                 
                 if(!flag)
                 std::cout << "\n\tContact name not found!\n";
                 break;
                
            case 3:
                do
                {
                 if(counter)
                  std::cout << "Try again\n";
                  counter++;
                  
                  std::cout << "\n\tEnter a contact name to delete:" 
                               "\t\t\tpress $ to cancel\n";
                  getline(std::cin,temp_name);
                
                  //Cancel Operation
                  if(temp_name=="$")
                   {
                    break;
                   }

                  //Final Confirmation
                  for(int i=0; i<100; i++)
                  if(person[i].name_exists(temp_name))
                   {
                    int yes; 
                    flag = 1;
                    std::cout << "\n\tAre you sure you want to delete? (1/0)\n";
                    std::cin >> yes;
                    
                    if(!yes)
                     {
                      cancel_flag = 1;
                     }
                    break;
                   }
                   
                  if(!flag)std::cout << "\n\tContact name not found!\n";
                    
                  if(cancel_flag)break;
                
                  // This code deletes the contact
                  if(flag)
                   {
                    for(int i=0; i<100; i++)
                    if(person[i].erase(temp_name))
                     {
                      std::cout << "\n\tDeleted successfully!\n";
                      break;
                     }
                   }
                    
                }while(!flag);
                break;
                
            case 4:
                do{
                   if(counter)std::cout << "\n\tTry again\n";
                   counter++;
                   
                   std::cout << "Search a name: \t\t\t\tpress $ to cancel\n";
                   std::cin >> temp_name;
                   
                    //Cancel Operation
                   if(temp_name=="$")
                    {
                     break;
                    }
                    
                    for(int i=0; i<100; i++)
                     if(person[i].show(temp_name))
                      {
                       flag = 1;
                       break;
                      }
                      
                   if(!flag)std::cout << "\n\tContact name not found\n";
                }while(!flag);
                    
                break;
                
            case 5:
                return 0;
                break;  
        }
        
    }while(1);
    
    getchar();
    return 0;
}

//prints a line
void printline(char ch, int size)
{
    for(int i=0; i<size; i++)
     std::cout << ch;
      std::cout << "\n";
}


//Contact name validation
bool name_valid(std::string tname)
{
    if(tname.size()>20)
    {
     std::cout << "\n\tInvalid Name!\n\tEnter a name within 20 characters!\n";
     return 0;
    }
    else
    if(tname == "")
    {
     std::cout << "\n\tInvalid Name!\n\tName cannot be blank!\n";
     return 0;
    }
    else
    return 1;
}

//mobile number validation
bool mob_valid(std::string tmob)
{
 if(tmob.size() > 13 || tmob.size() < 10)
  {
   std::cout << "\n\tInvalid mobile no."
                "\n\tEnter a no. between"
                " 10 and 13 digits\n";
   return 0;
  }
  else 
  if(tmob == "")
  {
   std::cout << "\n\tInvalid mobile no."
                "\n\tMobile no cannot be blank\n";
   return 0;
  }
  else
  return 1;
}