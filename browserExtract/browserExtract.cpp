// browserExtract.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <array>
//#include <Windows.h>
//#include <cstdio>

using namespace std;



int main()
{
    int length = 0;
    short selection = 0;
    int pos = 0;
    string imageLoc;
    ifstream image;

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
        
        image.seekg(0, image.end);
        length = image.tellg();
        image.seekg(0, image.beg);
        unsigned char * data = new unsigned char[length];
        
        while (!image.eof())
        {
            image >> data[pos];
            
            //cout << data[pos];
            pos++;
        }
        cout << length;
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
