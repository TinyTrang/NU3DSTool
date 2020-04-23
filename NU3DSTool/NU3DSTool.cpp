// NU3DSTool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <WinSock2.h>
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
        cout << "Sorry, no browser history was found" << endl;
}

int main()
{
    short selection;
    int pos = 0;
    string imageLoc = "";
    short segment = 0;

    cout << "Welcome to the NU3DSTool!!" << endl;
    cout << "What would you like to do?" << endl;
    cout << "1: Extract browser data" << endl;
    cout << "2: Extract wifi data" << endl;
    cout << "3: All the above" << endl;
    cout << "Selection: ";
    cin >> selection;

    switch (selection)
    {
    case 3: //runs the following cases

    case 1: //extract browser data
        int length;
        
        cout << "\n\n*** Browser Data Extraction ***\n\n";
        // getting file location
        cout << "Please enter the full path for the decrypted fat16 3DS file.\nInput is not case sensitive, but MUST NOT contain any whitespace.\nex. \"C:Users\\ConsoleX\\nand.fat16.bin\": ";
        cin >> imageLoc;
        // changing file location input for opening
        replace(imageLoc.begin(), imageLoc.end(), '\\', '/');
        image.open(imageLoc, ios::binary);
        if (image.is_open())
        {
            cout << "\nFile opened successfully!\n\n";

            image.seekg(0, image.end);
            length = image.tellg();
            image.seekg(0, image.beg);
            uint64_t* data = new uint64_t[length / 8];;
            uint64_t temp;


            cout << "Working on extraction...\n";
            // data gets read in backwards; first hex is now last
            image.read((char*)data, length);


            // flipping hex values
            for (int i = 0; i < length/8; i++)
            {
                data[i] = htonll(data[i]);
            }

            // searching for the bookmarks header; 0x0100 0000 80DF 0A00
            while (pos < length/8)
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
            cout << "\n\nBroswer Data Extraction Complete!\nFile closed\n\n";
        }
        else
            cout << "\n\nFile failed to open, please try again. File path must NOT contain any whitespace." << endl;
        if (selection == 1)
            break;
    
    case 2: //extract wifi data
        char data;
        length;
        imageLoc = "";
        selection = 0;

        cout << "\n\n*** Wifi Data Extraction ***\n\n";
        cout << "Please enter the full path to the 3DS sysdata file,\nex. \"I:\\data\\6994afbaa1ed5977a4c2fab49e7bd863\": ";
        cin >> imageLoc;
        // adding the remaining file path to config file
        imageLoc += "/sysdata/00010017/00000000";
        // changing file location input for opening
        replace(imageLoc.begin(), imageLoc.end(), '\\', '/');
        image.open(imageLoc, ios::binary);

        if (image.is_open()) {

            cout << "\n\nFile opened successfully!\n\n";
            cout << "Is the file for the Old 3DS/2DS or the New 3DS/2DS?\n";
            cout << "1: Old 3DS/2DS\n2: New 3DS/2DS\n3: Unsure\nSelection: ";
            cin >> selection;

            switch (selection)
            {
            case 3: //runs the following two cases

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
                if (selection == 1)
                {
                    cout << "\n\nWifi Data Extraction Complete!\nFile closed\n\n";
                    break;
                }
                

            case 2: // NEW 3DS wifi extraction
                cout << "\n\nTrying to extract wifi config based on NEW 3DS format:\n";
                for (int i = 0; i < 3; i++)
                {
                    // seeking the ssid length (B608)46600
                    image.seekg(0xB608 + (i * 0xC00));
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
                        // getting ssid (B5E8)
                        getSSID(0xB5E8 + (i * 0xC00), length);
                        // getting password (B60C)
                        getPassword(0xB60C + (i * 0xC00));
                    }
                }
                cout << "\n\nWifi Data Extraction Complete!\nFile closed\n\n";
                break;
                

            default:
                cout << "Incorrect or no selection made, please try again.";
                image.close();
                break;
            }

        }
        else
            cout << "\n\nCouldn't open file, please try again.";
        cout << "\n\nAll Extractions Complete!\n\n";
        break;
            
    default:
        cout << "\nIncorrect selection made, please try again\nExiting program...\n";
    }

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
