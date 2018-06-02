/* 


Test program for HuffmanCoding Class
This program uses both command line and
manual on-screen inputs

Copyright (C) 2007 , Alireza Noori
All rights reserved.


*/


///////////// Header Files /////////////////
#include "HuffmanCoding.h"

#include <string>
using std::string;

#include <windows.h>

#include <stdexcept> // stdexcept header file contains runtime_error 
using std::runtime_error; // standard C++ library class runtime_error

#include <iostream>

using namespace std;

#include <ostream>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::cerr;

#include <conio.h>

#include <cstring>
#include <ctime>

#include <string>
using std::string;

void manual(); //manual operations
void help(); //print help for command line

void RandomTransmission();
void HorizontalTransmission();
void VerticalTransmission();

int gotoxy(int x, int y);
void sleep( clock_t wait );

int compress(string in, string out); //compress in to out
int decompress(string in, string out); //decompress in to out

//set array of function-pointers to perform random transmissions
void (*functions[3])(void) = {RandomTransmission,
                            HorizontalTransmission,
                            VerticalTransmission};

//main function checks arguments and if they are true
// or no argument is entered uses the methods
int main(int argc, char **argv)
{
    srand((unsigned int)time(0));
    if( (argc < 3) || (argc > 4) ) //check arguments count
    {
        (*functions[rand() % 3])(); //run random transmission
        system("CLS");
        cout << "Invalid command line! To use program with manual "
            << "settings press ENTER,\notherwise press any other key "
            << "to see the command line help.";
        char ch;
        ch = _getch();

        if(ch == 13)
        {
            manual();
            return 0;
        }

        (*functions[rand() % 3])(); //run random transmission
        system("CLS");
        help();
        return 0;
    }

    HuffmanCoding hc;
    if(argc == 4) //if there are 3 arguments
    {
        string tmp(argv[3]);
        if( (tmp == "/D") ||  //check the last argument
            (tmp == "/d") )
        {
            decompress(argv[1],argv[2]); //goot to decompress
        }
        else //incorrect argument
        {
            cerr << "Error: 3rd argument is optional and "
                << "only can be \"/d\"";
            return 1;
        }
    }
    else
        compress(argv[1],argv[2]); //the last chance is compress
}

//manual input
void manual()
{
    char ch;
    do //as many times as user wants to do this
    {
        (*functions[rand() % 3])(); //run random transmission
        system("CLS");
        cout << "Welcome to Alireza Noori's Huffman "
            << "compression/decompression program.\n"
            << "copyright (C) 2007\n"
            << "Pick a task to begin...\n\n"
            << "    1.Compress a file.\n"
            << "    2.Decompress a file.\n"
            << "\nPress the related key. " << flush;

        do
        {
            ch = _getch();
        }while( (ch < '1') || (ch > '2') ); //only 1 and 2

        string input,output;

        (*functions[rand() % 3])(); //run random transmission
        system("CLS");
        cout << "Enter the input file's path:\n";
        getline(cin,input);

        cout << "\nEnter the output file's path:\n";
        getline(cin,output);

        (*functions[rand() % 3])(); //run random transmission
        system("CLS");

        if(ch == '1')
            compress(input,output);
        else
            decompress(input,output);

        cout << "\nDo you want to do other operation? (y/n)";

        do
        {
            ch = _getch();
        }while( (ch != 'y') && (ch != 'Y') 
            && (ch != 'n' ) && (ch != 'N') );

    } while ( (ch != 'n' ) && (ch != 'N') ); //check the answer
}

//print command line help
void help()
{
    system("CLS");
    cout << "Command line:\n"
        << "Huffman.EXE (input file) (output file) [/d]\n"
        << "Note:/d - means decompress input file to output file" << endl;
}

//compress using HuffmanCoding
int compress(string in, string out)
{
    cout << "Compressing " << in << " :\n";
    HuffmanCoding hc;
    try
    {
        hc.compress(in,out);
    }
    catch( runtime_error pro )
    {
        cerr << pro.what();
        return 1;
    }
    return 0;
}

//decompress using HuffmanCoding
int decompress(string in, string out)
{
    cout << "Decompressing " << in << " :\n";
    HuffmanCoding hc;
    try
    {
        hc.decompress(in,out);
    }
    catch( runtime_error pro )
    {
        cerr << pro.what();
        return 1;
    }
    return 0;
}

//print random '*' then print random ' '
void RandomTransmission()
{
    system("CLS");
    int i,j;
    for(i = 0; i < 80; i++)
    {
        for(j = 0; j < 25; j++)
        {
            gotoxy(rand() % 80, rand() % 25);
            cout << "*";
        }
        sleep( (clock_t)3 );
    }

    for(i = 0; i < 80; i++)
    {
        for(j = 0; j < 25; j++)
        {
            gotoxy(rand() % 80, rand() % 25);
            cout << " ";
        }
        sleep( (clock_t)3 );
    }
}

//print horizontal '*' then print horizontal ' '
void HorizontalTransmission()
{
    system("CLS");
    int i,j;
    for(i = 0; i < 80; i++)
    {
        for(j = 0; j < 25; j++)
        {
            gotoxy(i,j);
            cout << "*";
        }
        sleep( (clock_t)2 );
    }

    gotoxy(1,1);

    for(i = 0; i < 80; i++)
    {
        for(j = 0; j < 25; j++)
        {
            gotoxy(i,j);
            cout << " ";
        }
        sleep( (clock_t)2 );
    }
}

//print vertical '*' then print vertical ' '
void VerticalTransmission()
{
    system("CLS");
    int i,j;
    for(i = 0; i < 25; i++)
    {
        for(j = 0; j < 80; j++)
            cout << "*";
        sleep( (clock_t)2 );
    }

    gotoxy(1,1);

    for(i = 0; i < 25; i++)
    {
        for(j = 0; j < 80; j++)
            cout << " ";
        sleep( (clock_t)2 );
    }
}

// Pauses for a specified number of milliseconds.
void sleep( clock_t wait )
{
   clock_t goal;
   goal = wait + clock();
   while( goal > clock() )
      ;
}

//go to x and y positions
int gotoxy(int x, int y){

    HANDLE myHandle;
    BOOL successfull;

    COORD coordination;
    coordination.X=x;
    coordination.Y=y;

    myHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (myHandle==INVALID_HANDLE_VALUE)
        return -1;
    successfull = SetConsoleCursorPosition(myHandle, coordination);

    if(!successfull)
        return -1;
    
    return 1;
}

/***************************************************************************\
|                                                                           |
|                                                                           |
|                                                                           |
|                             Alireza Noori �                               |
|                                                                           |
|                                                                           |
|                                                                           |
|                                                                           |
\***************************************************************************/