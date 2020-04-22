// browserExtract.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
//#include <Windows.h>
//#include <cstdio>

using namespace std;

ifstream image;

int main()
{
    unsigned char data[8];
    short length;
    short selection;
    string imageLoc;

    // getting file location
    //cout << "Please enter the full path for the 3DS file: ";
    //cin >> imageLoc;
    imageLoc = "C:\\Users\\Trang\\Documents\\Class\\Spring 2020\\Capstone\\6_added_to_internet\\NAND.fat16.bin";
    // changing file location input for opening
    replace(imageLoc.begin(), imageLoc.end(), '\\', '/');
    image.open(imageLoc, ios::binary);

    if (image.is_open())
    {
        cout << "\n\nFile opened successfully!\n\n";
        int i = 0;
        while (!image.eof())
        {
            image.seekg(8 * i);
            image >> data;
            // 0x0100000080DF0A00
            if ((int)data[0] == 0xF0 && (int)data[1] == 0xC9 && (int)data[2] == 0xBB
                && (int)data[3] == 0x3C && (int)data[4] == 0xB2 && (int)data[5] == 0xE7
                && (int)data[6] == 0xC5 && (int)data[7] == 0X9B)
            {
                cout << data[1] << endl;
            }
            i++;
        }
    }
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
