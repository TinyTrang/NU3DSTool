// ssidExtract.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <cstdio>


using namespace std;

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
    cout << "Please enter the full path for the 3DS file: ";
    cin >> imageLoc;
    //imageLoc = "I:/data/6994afbaa1ed5977a4c2fab49e7bd863/sysdata/00010017/00000000";
    // changing file location input for opening
    replace(imageLoc.begin(), imageLoc.end(), '\\', '/');
    image.open(imageLoc, ios::binary);

    if (image.is_open()) {

        cout << "\n\nFile opened successfully!\n\n";
        cout << "Is the file for the New 3DS/2DS or the Old 3DS/2DS?\n";
        cout << "1: Old 3DS/2DS\n2: New 3DS/2DS\n3: Unsure\nSelection: ";
        cin >> selection;
        
        switch (selection)
        {
        case 3: //runs the following two cases

        case 2: // NEW 3DS wifi extraction
            cout << "\n\nTrying to extract wifi config based on NEW 3DS format:\n";
            for (int i = 0; i < 3; i++)
            {
                // seeking the ssid length (B608)46600
                image.seekg(0xB608 + (i*0xC00));
                // reading in the length of the ssid
                image >> data;
                // turning length into short
                length = (short)data;

                // checking to see if there is any wifi info
                if (length == 0)
                    cout << "\nNo data for wifi config " << i + 1 << "\n";
                else
                {
                    cout << "\n***Wifi config "<< i+1 << " information***\n";
                    // getting ssid (B5E8)
                    getSSID(0xB5E8 + (i * 0xC00), length);
                    // getting password (B60C)
                    getPassword(0xB60C + (i * 0xC00));
                }
            }
            if(selection == 2)
                break;

        case 1: // OLD 3DS wifi extraction
            cout << "\n\nTrying to extract wifi config based on OLD 3DS format:\n";
            for (int i = 0; i < 3; i++)
            {
                // seeking the ssid length (B748)
                image.seekg(0xB748 + (i * 0xC00));
                // reading in the length of the ssid
                image >> data;
                // turning length into short
                length = (short)data;

                // checking to see if there is any wifi info
                if (length == 0)
                    cout << "\nNo data for wifi config " << i + 1 << "\n";
                else
                {
                    cout << "\n***Wifi config " << i + 1 << " information***\n";
                    // getting ssid (B728)
                    getSSID(0xB728 + (i * 0xC00), length);
                    // getting password (B74C)
                    getPassword(0xB74C + (i * 0xC00));
                }
            }
            break;
               
        default:
            cout << "Incorrect or no selection made, please try again.";
            image.close();
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
