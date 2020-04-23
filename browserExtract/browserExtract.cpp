// browserExtract.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <winsock2.h>
#include <stdio.h>
#include <string>
//#include <Windows.h>
//#include <cstdio>

using namespace std;

ifstream image;

void getBookmarks(int offset)
{
    char data;
    string url = "";
    string name = "";
    int i = 0;

    //Finding Timestamp
    // 0xE0 == length from bookmarks header to start of timestamp
    int tsOffset = offset + 0xE0;
    //cout << (hex) << tsOffset << endl;
    cout << "Timestamp: ";
    for (i = 0; i < 8; i++)
    {
        image.seekg(tsOffset + i);
        image >> data;
        cout << (int) data;
    }

    // Finding Bookamrks counter
    cout << "\nCounter: ";
    // counter is 13 bytes from start of timestamp
    int counterOffset = tsOffset + 13;
    image.seekg(counterOffset);
    image >> data;
    cout << (int)data;
    
    
    // Finding URL 
    // start of url is 0x210 bytes from start of timestamp
    int urlOffset = tsOffset + 0x210;    
    image.seekg(urlOffset);
    image >> data;
    i = 0;
    while (data != NULL && i < 400)
    {
        image.seekg(urlOffset + i);
        image >> data;        
        url += data;
        i++;
    }
    cout << "\nURL: " << url;


    // Finding Bookmarks Name
    // start of bookmarks name is 0x610 bytes from start of timestamp
    int nameOffset = tsOffset + 0x610;
    image.seekg(nameOffset);
    image >> data;
    i = 0;
    while (i < 200)
    {
        image.seekg(nameOffset + i);
        image >> data;
        name += data;
        i+=2;
    }
    cout << "\nBookmark name: " << name;


    /*
    // timestamp: 28 8 byte chunks from bookmarks header
    cout << (hex) << data[(pos + 28)] << endl;
    // Counter: 29 8 byte chunks from bookmarks header
    cout << (hex) << data[(pos + 29)] << endl;
    // URL: 55 8 byte chunks from bookmarks header
    cout << (char) data[(pos + 55)] << endl;
    */
    
}

int main()
{
    int length = 0;
    short selection = 0;
    int pos = 0;
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
        
        image.seekg(0, image.end);
        length = image.tellg();
        image.seekg(0, image.beg);
        uint64_t * data = new uint64_t[length];
        uint64_t temp;

        // data gets read in backwards; first hex is now last
        image.read((char*)data, length);
        
        
        // flipping hex values
        for (int i = 0; i < length; i++) 
        {
            data[i] = htonll(data[i]);
        }
        
        bool found = FALSE;
        // searching for the bookmarks header; 0x0100 0000 80DF 0A00
        while (!found)
        {
            if (data[pos] == 0x0100000080DF0A00)
            {
                found = TRUE;
                //cout << pos << endl;
                // pos * 8 == offset of start of header
                getBookmarks(pos * 8);
            }
            pos++;
        }

        
    }
    image.close();
    cout << "\n\nFile closed\n\n";
    
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
