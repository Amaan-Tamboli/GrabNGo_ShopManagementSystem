#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <ctime>
#include <iomanip>
using namespace std;

#define CODE "SHOP@PROJECT"

ofstream fout;
ifstream fin;

int k = 7, r = 0, flag = 0;
COORD coord = {0, 0};
string username, password;
string LS;

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
struct date
{
    int mm, dd, yy;
};

bool isLoggedIn();
void registration();

class item
{
protected:
    int itemno;
    char name[25];
    date d;

public:
    void add()
    {
        system("cls");
        cout << "\n\n\tItem No : ";
        cin >> itemno;
        cout << "\n\n\tName of the item : ";
        cin >> name;
    m:
        cout << "\n\n\tDate until fresh (dd mm yyyy) : ";
        cin >> d.dd >> d.mm >> d.yy;
        if (d.dd > 31 || d.mm > 12 || (d.yy < 2022 && d.yy > 2030))
        {
            cout << "=====>Wrong date entered!!!";
            goto m;
        }
    }
    void detail()
    {
        cout << "\n\n\tName of the item : ";
        cout << name;
        cout << "\n\tDate until fresh : ";
        cout << d.mm << "/" << d.dd << "/" << d.yy;
    }
    void show()
    {
        cout << "\n\tItem No : ";
        cout << itemno;
        cout << "\n\n\tName of the item : ";
        cout << name;
        cout << "\n\n\tDate : ";
        cout << d.dd << "/" << d.mm << "/" << d.yy;
    }
    void report()
    {
        gotoxy(3, k);
        cout << itemno;
        gotoxy(13, k);
        cout << name;
    }
    int retno()
    {
        return (itemno);
    }
};

class amount : public item
{

    float price, qty, pqty = 0, nettotal = 0, tax, gross = 0, dis, netamt = 0, tdprice = 0;

public:
    void add();
    void show();
    void report(int p);
    void detail();
    void calculate();
    void pay();
    float get_pqty();
    void update(int);
    void totalp(int p);
    int search(int p = 1);
    float retnetamt()
    {
        return (netamt);
    }

} amt;

void amount::add()
{
    item::add();
    cout << "\n\n\tPrice: ";
    cin >> price;
    cout << "\n\n\tQuantity: ";
    cin >> qty;
    if (price > 0 && price <= 1000)
    {
        tax = 4;
        dis = 1;
    }
    else if (price > 1000 && price <= 10000)
    {
        tax = 5;
        dis = 5;
    }
    else
    {
        tax = 7;
        dis = 10;
    }
    calculate();
    fout.write((char *)&amt, sizeof(amt));
    fout.close();
}
void amount::calculate()
{
    gross = (tax / 100);
    gross = price * (gross);
    gross = price + gross;

    netamt = gross;
    netamt = qty * netamt;
    tdprice = (dis / 100);
    tdprice = gross * (tdprice);
    tdprice = gross + tdprice;
}
void amount::show()
{
    fin.open("itemstore.dat", ios::binary);
    fin.read((char *)&amt, sizeof(amt));
    item::show();
    cout << "\n\n\tNet amount: ";
    cout << netamt;
    fin.close();
}
void amount::detail()
{
    fin.open("itemstore.dat", ios::binary);
    fin.read((char *)&amt, sizeof(amt));
    item::detail();
    cout << "\n\tQuantity: " << qty;

    cout << "\n\tPrice(tax not included): " << price;
    cout << "\n\tDiscount: " << dis << "%" << endl
         << endl;
    fin.close();
}

void amount::report(int p)
{
    item::report();
    gotoxy(23, k);
    cout << price;
    gotoxy(33, k);
    cout << (int)qty;
    gotoxy(44, k);
    cout << tax;

    if (p == 1)
    {
        gotoxy(52, k);
        cout << netamt;
    }
    else if (p == 2)
    {
        gotoxy(52, k);
        cout << dis;
        gotoxy(64, k);
        cout << tdprice;
    }

    k = k + 1;
    if (k == 50)
    {
        gotoxy(25, 50);
        cout << "PRESS ANY KEY TO CONTINUE...";
        getch();
        k = 7;
        system("cls");
        gotoxy(30, 3);
        cout << " ITEM DETAILS ";
        gotoxy(3, 5);
        cout << "NUMBER";
        gotoxy(13, 5);
        cout << "NAME";
        gotoxy(23, 5);
        cout << "PRICE";
        gotoxy(33, 5);
        cout << "QUANTITY";
        gotoxy(44, 5);
        cout << "TAX";
        gotoxy(52, 5);
        cout << "DEDUCTION";
        gotoxy(64, 5);
        cout << "NET AMOUNT";
    }
}

void amount::totalp(int p)
{
    int ff;
    float gtotal;
    system("cls");
    gotoxy(30, 3);
    cout << " ITEM DETAILS ";
    gotoxy(3, 5);
    cout << "ITEM NO";
    gotoxy(13, 5);
    cout << "NAME";
    gotoxy(23, 5);
    cout << "PRICE";
    gotoxy(33, 5);
    cout << "QUANTITY";
    gotoxy(44, 5);
    cout << "TAX %";

    if (p == 1)
    {
        gotoxy(52, 5);
        cout << "NET AMOUNT";
    }
    if (p == 2)
    {
        gotoxy(52, 5);
        cout << "DISCOUNT %";
        gotoxy(64, 5);
        cout << "MRP";
    }

    fin.open("itemstore.dat", ios::binary);
    fin.seekg(0);
    gtotal = 0;
    while (!fin.eof())
    {
        fin.read((char *)&amt, sizeof(amt));
        if (!fin.eof())
        {
            amt.report(p);
            gtotal += amt.retnetamt();
            ff = 0;
        }
        if (ff != 0)
            gtotal = 0;
    }

    if (p == 1)
    {
        gotoxy(17, 15);
        cout << "\n\n\n\t\t\tGrand Total=" << gtotal;
    }
    getch();
    fin.close();
}
void amount::pay()
{
    time_t ttime = time(0);
    char *dt = ctime(&ttime);
    cout << "\n\n\n\t\t*********************************************";
    cout << "\n\t\t                 DETAILS                  ";
    cout << "\n\t\t*********************************************";
    cout << "\n\n\t\tITEM NAME                   : " << name;
    cout << "\n\t\tPRICE                       : Rs. " << price;
    cout << "\n\t\tQUANTITY                    : " << (int)pqty;
    cout << "\n\t\tTAX                         : " << tax << " %";
    cout << "\n\t\tDISCOUNT                    : " << dis << " %";
    cout << "\n\n\n\t\tTOTAL AMOUNT              : Rs. " << nettotal;
    cout << "\n\n\t\t\t\t" << setw(15) << dt;
    cout << "\n\t\t*********************************************" << endl;

    system("PAUSE");
}
float amount::get_pqty()
{
    fin.open("order.dat", ios::binary | ios::in);
    fin.read((char *)&amt, sizeof(amt));
    float q = pqty;
    fin.close();
    return q;
}
void amount::update(int c)
{

    float q = get_pqty();
    fout.open("itemstore.dat", ios::binary | ios::in);
    fout.seekp(c * sizeof(amt));
    qty = qty - q;
    calculate();
    fout.write((char *)&amt, sizeof(amt));
    fout.close();
}

int amount::search(int p)
{

    int flag = 0;
    int c = 0;
    int n;
    if (p == 2)
    {
        system("cls");
    }
    if (p == 1 || p == 2)
    {
        cout << "\nEnter item id :";
        cin >> n;
    }
    if (p == 3)
    {
        totalp(2);

        cout << "\n\n\nEnter item id to be purchased : ";
        cin >> n;
    }

    fin.open("itemstore.dat", ios::binary);

    if (!fin)
    {
        cout << "\nFile Not Found...";
    }
    fin.seekg(0);

    while (fin.read((char *)&amt, sizeof(amt)))
    {
        int x = amt.item::retno();
        if (x == n)
        {
            if (p == 1)
            {
                cout << "\nItem found!!!" << endl
                     << "Item Details :- " << endl;
                amt.detail();
                flag = 1;
                break;
            }
            else if (p == 2)
            {
                flag = 1;
                fout.open("itemstore.dat", ios::binary | ios::in);
                fout.seekp(c * sizeof(amt));
                cout << "\n\nCurrent details :- " << endl;
                show();
                cout << "\nContinue to enter new details" << endl
                     << endl;
                system("PAUSE");
                add();
                break;
            }
            else if (p == 3)
            {
                flag = 1;
                fout.open("order.dat", ios::binary | ios::out);
                cout << "\nEnter quantity to be purchased : ";
                cin >> pqty;
                nettotal = tdprice * pqty;
                fout.write((char *)&amt, sizeof(amt));
                fout.close();
                break;
            }
        }
        c++;
    }
    if (flag == 0)
    {
        cout << "\nItem with id \'" << n << "\' does not exist!!!" << endl;
    }
    fin.close();
    return c;
}

int main()
{
    registration();
    int c;

    fstream tmp("temp.dat", ios::binary | ios::out);
    if (LS == "E")
    {

    menu:
        system("cls");
        cout << "\n\t\t1. Total goods\n\n";
        cout << "\t\t2. Add/Remove/Edit Item\n\n";
        cout << "\t\t3. Show Item Details\n\n";
        cout << "\t\t4. Purchase items\n\n";
        cout << "\t\t0. Logout\n\n";
        cout << "\tPlease Enter your choice : ";
        int ch;

        cin >> ch;
        switch (ch)
        {
        case 1:
            amt.totalp(1);
            goto menu;
            break;

        case 2:
        db:
            system("cls");
            gotoxy(25, 1);
            cout << "==============\n\n";
            gotoxy(25, 2);
            cout << "Item Edit Page";
            gotoxy(25, 3);
            cout << "==============\n\n";
            cout << "\n\t\t1.Add Item Details\n\n";
            cout << "\t\t2.Edit Item Details\n\n";
            cout << "\t\t3.Delete Item Details\n\n";
            cout << "\t\t0.Back to Main Menu\n\n";
            cout << "\tPlease Enter your choice : ";
            int apc;
            cin >> apc;
            switch (apc)
            {
            case 1:

                fout.open("itemstore.dat", ios::binary | ios::app);
                amt.add();
                cout << "\n\t\tItem Added Successfully!";
                getch();
                goto db;

            case 2:
                amt.search(2);
                goto db;

            case 3:
                flag = 0;
                int ino;
                cout << "\n\n\tEnter Item Number to be deleted :";
                cin >> ino;
                fin.open("itemstore.dat", ios::binary);
                if (!fin)
                {
                    cout << "\n\nFile Not Found...";
                    goto menu;
                }
                fin.seekg(0);
                while (fin.read((char *)&amt, sizeof(amt)))
                {
                    int x = amt.item::retno();
                    if (x != ino)
                        tmp.write((char *)&amt, sizeof(amt));
                    else
                    {
                        flag = 1;
                    }
                }
                fin.close();
                tmp.close();
                fout.open("itemstore.dat", ios::trunc | ios::binary);
                fout.seekp(0);
                tmp.open("temp.dat", ios::binary | ios::in);
                if (!tmp)
                {
                    cout << "Error in File";
                    goto db;
                }
                while (tmp.read((char *)&amt, sizeof(amt)))
                    fout.write((char *)&amt, sizeof(amt));
                tmp.close();
                fout.close();
                if (flag == 1)
                    cout << "\n\t\tItem Succesfully Deleted";
                else if (flag == 0)
                    cout << "\n\t\tItem does not Exist! Please Retry";
                getch();
                goto db;
            case 0:
                goto menu;
            default:
                cout << "\n\n\t\tWrong Choice!!! Retry";
                getch();
                goto db;
            }
            break;

        case 3:
            amt.search();
            system("PAUSE");
            goto menu;
            break;

        case 4:
            c = amt.search(3);
            amt.update(c);
            fin.open("order.dat", ios::binary | ios::in);
            fin.read((char *)&amt, sizeof(amt));
            amt.pay();
            goto menu;

        case 0:
            main();
        default:
            break;
        }
        return 0;
    }
    else if (LS == "C")
    {
    Cmenu:
        system("cls");
        cout << "\n\t\t1. Display items\n\n";
        cout << "\t\t2. Purchase items\n\n";
        cout << "\t\t3. Last Bill\n\n";
        cout << "\t\t4. Show Item Details\n\n";
        cout << "\t\t0. Logout\n\n";
        cout << "\tPlease Enter Required Option: ";
        int ch;
        cin >> ch;
        switch (ch)
        {
        case 1:
            amt.totalp(2);
            goto Cmenu;

        case 2:
            c = amt.search(3);
            amt.update(c);
            fin.open("order.dat", ios::binary | ios::in);
            fin.read((char *)&amt, sizeof(amt));
            amt.pay();

        case 3:
            fin.open("order.dat", ios::binary | ios::in);
            fin.read((char *)&amt, sizeof(amt));
            amt.pay();
            goto Cmenu;
            break;

        case 4:
            c = amt.search();
            system("PAUSE");
            goto Cmenu;
        case 0:
            main();
        default:
            break;
        }
    }

    return 0;
}

bool isLoggedIn()
{
    string username, password, un, pw;
    system("cls");
    gotoxy(25, 2);
    cout << "Login Tab";
    gotoxy(25, 3);
    cout << "---------\n\n\n\n";
    cout << "\t\tUsername : ";
    cin >> username;
    cout << "\n\t\tPassword : ";
    cin >> password;

    ifstream read(username + ".txt");
    getline(read, un);
    getline(read, pw);
    getline(read, LS);

    if (un == username && pw == password)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void registration()
{
    int c;
    string name, email, contact, code;
    char S;
    system("cls");

    gotoxy(25, 2);
    cout << "\t*********\n\n";
    gotoxy(25, 3);
    cout << "\tGrab N Go";
    gotoxy(25, 4);
    cout << "\t*********\n\n\n\n";
    cout << "\t\t1. Register \n\n\t\t2. Login\n\n\n\n";
    cout << "\t\t\t\tEnter your choice : ";
    cin >> c;
    if (c == 1)
    {
        system("cls");
        gotoxy(25, 2);
        cout << "Registration Tab";
        gotoxy(25, 3);
        cout << "----------------\n\n";
        cout << "\t\tPick a username : ";
        cin >> username;
        ifstream ifile;
        ifile.open(username + ".txt");
        if (ifile)
        {
            cout << "\n\n\t\tUsername already exists!!!!\n\t\tPlease pick another username\n\n";
            system("PAUSE");
            registration();
        }
        cout << "\n\t\tEnter a Password : ";
        cin >> password;
        cout << "\n\t\tEnter your name : ";
        cin >> name;
        cout << "\n\t\tEnter correct email address : ";
        cin >> email;
        cout << "\n\t\tEnter contact number : ";
        cin >> contact;
        cout << "\n\t\tLogin Status(E=Employee, C=Customer) : ";
        cin >> S;

        if (S == 'E')
        {

            cout << "\n\t\tEnter Empoyee's CODE : ";
            cin >> code;
            if (code != CODE)
            {
                cout << "\n\t\tWrong Employee CODE!!!";
                cout << "\n\t\tEnter Details again!!!\n\n";
                system("PAUSE");
                registration();
            }
            else
            {
                cout << "\n\t\tCredentials are saved as a Employee!!!";
            }
        }

        ofstream file;
        file.open(username + ".txt");
        file << username << endl
             << password << endl
             << S << endl
             << name << endl
             << email << endl
             << contact << endl;
        file.close();
        cout << "\n\t\tRegistered successfully!!!\n";
        system("PAUSE");
        main();
    }
    else if (c == 2)
    {
        if (isLoggedIn() == false)
        {
            cout << "\n\n\t\tIncorrect Username or Password!!!\n\n";
            system("PAUSE");
            main();
        }
        else
        {
            cout << "\n\n\t\tLogged in successfully!!!\n"
                 << endl;
            system("PAUSE");
        }
    }
}