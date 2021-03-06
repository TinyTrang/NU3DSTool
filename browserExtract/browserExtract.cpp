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

void getBookmarks(int tsOffset) // tsOffset == pos * 8 + 0xE0 on first call
{
    char data;
    string url = "";
    string name = "";
    int i = 0;
    short counter;

    // last value of first line of bookmark start that is always 0x01 is 
    // 15 bytes from start of timestamp, used to check is there is a bookmark
    int checkValOffset = tsOffset + 15;
    image.seekg(checkValOffset);
    image >> data;
    cout << "\nBookmark offset: " << tsOffset << endl;
    if (data == 0x01) {
        //Finding Timestamp
        cout << "Timestamp: ";
        for (i = 0; i < 8; i++)
        {
            image.seekg(tsOffset + i);
            image >> data;
            cout << (hex) << (int)data;
        }

        // Finding Bookmaks counter
        cout << "\nCounter: ";
        // counter is 12 bytes from start of timestamp
        int counterOffset = tsOffset + 12;
        image.seekg(counterOffset);
        image.get(data);
        counter = short(data);
        cout << counter;


        // Finding URL 
        // start of url is 0x210 bytes from start of timestamp
        int urlOffset = tsOffset + 0x210;
        image.seekg(urlOffset);
        image >> data;
        i = 0;
        while (data != NULL && i < 400)
        {
            image.seekg(urlOffset);
            image >> data;
            url += data;
            urlOffset++;
            i++;
        }
        cout << "\nURL: " << url << endl;


        // Finding Bookmarks Name
        // start of bookmarks name is 0x610 bytes from start of timestamp
        int nameOffset = tsOffset + 0x610;
        i = 0;
        while (i < 200)
        {
            image.seekg(nameOffset);
            image.get(data);
            if (data != NULL)
                name += data;
            nameOffset++;
            i++;
        }
        cout << "Bookmark name: " << name << endl;

        // next block of bookmark info is 0x810 from start of previous bookmark timestamp
        int offset = (tsOffset + 0x810);
        getBookmarks(offset);
    }
    else
        cout << "Sorry, no bookmarks were found\n";

    /*
    // timestamp: 28 8 byte chunks from bookmarks header
    cout << (hex) << data[(pos + 28)] << endl;
    // Counter: 29 8 byte chunks from bookmarks header
    cout << (hex) << data[(pos + 29)] << endl;
    // URL: 55 8 byte chunks from bookmarks header
    cout << (char) data[(pos + 55)] << endl;
    */
    
}

void getHistory(int tsOffset) // tsOffset == pos * 8 + 0xE0 + 0x31E30 on first call
{
    char data;
    string url = "";
    string name = "";
    int i = 0;
    short counter;
    
    // first value of second 8 bytes after timestamp is 0x01, seems to indicate 
    // presence of history name
    // used to check if there is history
    int checkValOffset = tsOffset + 8;
    image.seekg(checkValOffset);
    image >> data;
    cout << "\nHistory offset: " << tsOffset << endl;
    if (data == 0x01) 
    {
        // second value of second 8 bytes after timestamp is 0x01, seems to indicate
        // presence of history url
        // used to check if there is history
        checkValOffset = tsOffset + 9;
        image.seekg(checkValOffset);
        image >> data;
        if (data == 0x01)
        {      
            //Finding Timestamp
            cout << "Timestamp: ";
            for (i = 0; i < 8; i++)
            {
                image.seekg(tsOffset + i);
                image >> data;
                cout << (hex) << (int)data;
            }

            // Finding URL 
            // start of url is 0x210 bytes from start of timestamp
            int urlOffset = tsOffset + 0x210;
            image.seekg(urlOffset);
            image >> data;
            i = 0;
            while (data != NULL && i < 400)
            {
                image.seekg(urlOffset);
                image >> data;
                url += data;
                urlOffset++;
                i++;
            }
            cout << "\nURL: " << url << endl;


            // Finding Browser Histroy Name
            // start of browser history name is 0x610 bytes from start of timestamp
            int nameOffset = tsOffset + 0x610;
            i = 0;
            while (i < 200)
            {
                image.seekg(nameOffset);
                image.get(data);
                if ((char)data != NULL)
                    name += data;
                nameOffset++;
                i++;
            }
            cout << "History name: " << name << endl;

            // next block of broswer history info is 0x810 from start of previous broswer history timestamp
            int offset = (tsOffset + 0x810);
            getHistory(offset);
        }
        else
            cout << "Sorry, no browser history was found" << endl;
    }
    else
        cout << "Sorry, no browser history was found"<<endl;
}

int main()
{
    int length = 0;
    short selection = 0;
    int pos = 0;
    string imageLoc;
    short segment = 0;

    // getting file location
    cout << "Please enter the full path for the decrypted fat16 3DS file.\nInput is not case sensitive, but MUST NOT contain any whitespace.\nex. \"C:Users\\ConsoleX\\nand.fat16.bin\": ";
    cin >> imageLoc;
    //imageLoc = "C:\\Users\\Trang\\Documents\\Class\\Spring 2020\\Capstone\\6_added_to_internet\\NAND.fat16.bin";
    // changing file location input for opening
    replace(imageLoc.begin(), imageLoc.end(), '\\', '/');
    
    image.open(imageLoc, ios::binary);
    if (image.is_open())
    {
        cout << "\nFile opened successfully!\n\n";

        image.seekg(0, image.end);
        length = image.tellg();
        image.seekg(0, image.beg);
        uint64_t* data = new uint64_t[length];
        uint64_t temp;


        cout << "Working on extraction...\n";
        // data gets read in backwards; first hex is now last
        image.read((char*)data, length);


        // flipping hex values
        for (int i = 0; i < length; i++)
        {
            data[i] = htonll(data[i]);
        }

        // searching for the bookmarks header; 0x0100 0000 80DF 0A00
        while (pos < length)
        {
            if (data[pos] == 0x0100000080DF0A00)
            {
                segment++;
                // getting bookmarks info
                cout << "\n\n***Segment " << segment << " of Bookmarks***\n";
                //cout << pos << endl;
                // pos * 8 == offset of start of header
                // 0xE0 == length from bookmarks header to start of timestamp
                getBookmarks(pos * 8 + 0xE0);

                // getting browser history info
                cout << "\n\n***Segment " << segment << " of Browser History***\n";
                // first browser history timestamp starts 0x31E40 bytes after first bookmark's timestamp
                getHistory(pos * 8 + 0xE0 + 0x31E40);
            }
            pos++;
        }

        image.close();
        cout << "\n\nExtraction Complete!\nFile closed\n\n";
    }
    else
        cout << "\n\nFile failed to open, please try again. File path must NOT contain any whitespace." << endl;
    
    
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
