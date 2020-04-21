// ssidExtract.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <cstdio>


using namespace std;

// starting position of first wifi config (B5E4)
#define BLK1  46564
// starting position of second wifi config (C1E4)
#define BLK2  49636
// starting position of third wifi config (CDE4)
#define BLK3  52708

ifstream image;

void getSSID(int pos, short length)
{
    char data;
    string ssid = "";

    // getting the ssid
    for (int i = 0; i < length; i++)
    {
        // seeking the start of ssid (C1E8)
        image.seekg(pos + i);
        image >> data;
        ssid += data;
    }
    cout << "SSID: " << ssid << endl;
}

void getPassword(int pos)
{
    char data = ' ';
    string password = "";

    //getting password, i < 64 because size of memblock = 0x40
    int i = 0;
    while (data != NULL && i < 64)
    {
        // seeking the start of password (C20C)
        image.seekg(pos + i);
        image >> data;
        password += data;
        i++;
    }
    cout << "Password: " << password << endl;
}

int main()
{
    char data;
    short length;
    short selection;
    string imageLoc;

    // getting file location
    //cout << "Please enter the full path for the 3DS file: ";
    //cin >> imageLoc;
    imageLoc = "I:/data/6994afbaa1ed5977a4c2fab49e7bd863/sysdata/00010017/00000000";
    // changing file location input for opening
    replace(imageLoc.begin(), imageLoc.end(), '\\', '/');
    image.open(imageLoc, ios::binary);

    if (image.is_open()) {

        cout << "\n\nFile opened successfully!\n\n";
        cout << "Is the file for the New 3DS/2DS or the Old 3DS/2DS?\n";
        cout << "1: New 3DS/2DS\n2: Old 3DS/2DS\n3: Unsure\nSelection: ";
        cin >> selection;

        switch (selection)
        {
        case 1: // NEW 3DS wifi extraction
            //getting ssid for wifi config 1
            // seeking the ssid length (B608)46600
            image.seekg(46600);
            // reading in the length of the ssid
            image >> data;
            // turning length into short
            length = (short)data;

            // checking to see if there is any wifi info
            if (length == 0)
                cout << "\n\nNo data for wifi config 1\n\n";
            else
            {
                cout << "\n\n***Wifi config 1 information***\n";
                // getting ssid (B5E4)
                getSSID(46568, length);
                // getting password (B60C)
                getPassword(46604);
            }


            //getting ssid for wifi config 2
            // seeking the ssid length (C208)
            image.seekg(49672);
            // reading in the length of the ssid
            image >> data;
            // turning length into short
            length = (short)data;

            // checking to see if there is any wifi info
            if (length == 0)
                cout << "No data for wifi config 2\n\n";
            else
            {
                cout << "\n***Wifi config 2 information***\n";
                // getting ssid (C1E8)
                getSSID(49640, length);
                // getting password (C20C)
                getPassword(49676);
            }


            //getting ssid for wifi config 3
            // seeking the ssid length (CE08)
            image.seekg(52744);
            // reading in the length of the ssid
            image >> data;
            // turning length into short
            length = (short)data;

            // checking to see if there is any wifi info
            if (length == 0)
                cout << "No data for wifi config 3\n\n";
            else
            {
                cout << "\n***Wifi config 3 information***\n";
                // getting ssid (CDE8)
                getSSID(52712, length);
                // getting password (CE0C)
                getPassword(52748);
            }
            break;

        case 2: // OLD 3DS wifi extraction
            // insert code to extract
            break;
        }




    }
    else
        cout << "\n\nCouldn't open file, please try again.";

    image.close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
