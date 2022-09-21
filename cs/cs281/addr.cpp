#include <iostream.h>

typedef struct
{
        char name[20];
        char city [20];
        char state[20];
} addrStruct;
 
const int MAX = 10;
 
class List
{
        addrStruct list[MAX];
        int numInList;
public:
        List(): numInList(0) // constructor
        {
        }
        ~List() // destructor
        {
        }
        int Full()
        {
                if (numInList >=MAX) return 1; else return 0;
        }
        int Empty()
        {
                if (numInList==0) return 1; else return 0;
        }
        int Size()
        {
                return numInList;
        }
        int Add(addrStruct addr)
        {
                if (!Full())
                {
                        list[numInList++]=addr;
                        return 0;  // returns 0 if OK
                }
                return 1;
        }
        int Get(addrStruct&  addr, int i)
        {
                if (i < Size())
                {
                        addr=list[i];
                        return 0;  // returns 0 if OK
                }
                return 1;
        }
};
//-----------------------------------------------
 
List list;
 
void addName()
{
        addrStruct a;
 
        if (!list.Full())
        {
                cout << "Enter Name: ";
                cin >> a.name;
                cout << "Enter City: ";
                cin >> a.city;
                cout << "enter State: ";
                cin >> a.state;
                list.Add(a);
        }
        else
                cout << "List full\n";
}
void printOneName(addrStruct a)
{
        cout << endl;
        cout << a.name << endl;
        cout << a.city << endl;
        cout << a.state << endl;
}
 
void printNames()
{
        int i;
        addrStruct a;
 
        for (i=0; i < list.Size(); i++)
        {
                list.Get(a,i);
                printOneName(a);
        }
        cout << endl;
}
void findName()
{
        char s[20];
        int i;
        int found=0;
        addrStruct a;
 
        if (list.Size()==0)
                cout << "List empty\n";
        else
        {
                cout << "Enter name to find: ";
                cin >> s;
                for (i=0; i < list.Size(); i++)
                {
                        list.Get(a, i);
                        if (strcmp(s,a.name)==0)
                        {
                                printOneName(a);
                                found=1;
                        }
                }
                if (!found)
                        cout << "No match\n";
        }
}
 
void paintMenu()
{
        cout << "Address list Main Menu\n";
        cout << "  1 - add to list\n";
        cout << "  2 - print list\n";
        cout << "  3 - find name\n";
        cout << "  4 - quit\n";
        cout << "Enter choice: ";
}
int main()
{
        char choice[10];
        int done=0;
 
        while (!done)
        {
                paintMenu();
                cin >> choice;
                switch(choice[0])
                {
                        case '1':
                                addName();
                                break;
                        case '2':
                                printNames();
                                break;
                        case '3':
                                findName();
                                break;
                        case '4':
                                done=1;
                                break;
                        default:
                                cout << "invalid choice.\n";
                }
        }
        return 0;
        // list destroys itself when it goes out of scope.
}

