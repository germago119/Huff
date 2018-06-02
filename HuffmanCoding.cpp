/* 


Definition of the HuffmanCoding Class


*/

#ifndef HUFFMANCODING_CPP
#define HUFFMANCODING_CPP

///////////// Header Files /////////////////
#include "HuffmanCoding.h"
#include "mergingTree.h"
#include "mergingTree.cpp"
#include "mergingTreeNode.h"
#include "mergingType.h"

#include <ctime>

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::flush;
using std::endl;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;
using std::endl;

#include <stdexcept> // stdexcept header file contains runtime_error 
using std::runtime_error; // standard C++ library class runtime_error

///////////// Functions Definition ////////////////
////////////////////////////////////////////////////////////
//compress input onto output
void HuffmanCoding::compress(string input, string output)
{
    time_t   start, finish;
    codeTree = new mergingTree<mergingType>;
    if(input == output)
        throw runtime_error( "Input and output files are the same!");

    //initalize variables
    inFile = input;
    zeros = 0;
    outFile = output;
    buffer = new char[200];
    fileBuffer = new int[1000];
    counts = new int[256];
    strings = new string [256];
    fileBufferSize = bufferSize = 0;
    for(int i = 0; i < 256; i++)
        counts[i] = 0;

    time( &start );
    //count characters
    cout << "Counting characters...\n" << flush;
    count();

    //create tree and set other variables such as strings array
    cout << "Done\nSetting necessary variables for compressing..." << flush;
    setVariables();

    //open input file
    ifstream inputfile(inFile.c_str(),ios::in | ios::binary);

    if(!inputfile.is_open())
        throw runtime_error( "Cannot open the input file!");

    int ch;

    //write file signature
    cout << "Done\nWriting file's signature..." << flush;
    writeSignature();

    //get file's size and precent to show the progress
    unsigned long inputfilesize = fileSize(),
        currpos = 0, percent;
    percent = inputfilesize / 100;
    int curper = 0;

    cout << "\nFile size is: " 
        << inputfilesize << " bytes";

    cout << "Done\nCoding file...\n0 % (0 Bytes)" << flush;

    //begin compress operation and
    //  fill the buffer
    while(!inputfile.eof())
    {
        ch = inputfile.get();
        if( (currpos++) == percent )
        {
            cout << '\r' << ++curper << " % ("
                << currpos * curper << ") Bytes.";
            currpos = 0;
        }
        if(ch != EOF)
            writeBits(strings[ch]);
    }

    //if buffer is not empty
    // so there must be incomplete byte
    if(bufferSize != 0)
    {
        //fill the rest of byte with zero
        int tmp = bufferSize;
        for(; (bufferSize % 8) != 0; bufferSize++)
            buffer[bufferSize] = '0';
        //zeros hold the number of bits
        //  filled with zero
        zeros = bufferSize - tmp;
        //write the rest of bits to buffer
        flushBuffer();
    }

    //write the last buffer to output file
    writeTheFile();

    //shwo 100% progress
    cout << '\r' << 100 << " %";

    //open output file to write the number of
    // bits filled with zero
    ofstream outputfile(outFile.c_str(),ios::app | ios::binary);

    if(!outputfile.is_open())
        throw runtime_error( "Cannot open the output file!");

    outputfile.put(zeros);
    cout << "\nCompression operation completed successfully.";

    //release dynamic memory
    cout << "\nReleasing memory..." << flush;
    delete [] fileBuffer;
    delete [] buffer;
    delete [] counts;
    delete [] strings;
    delete codeTree;
    cout << "Done" << endl;

    time( &finish );
    unsigned long elapsed_time = (unsigned long)difftime( finish, start );

    cout << "\nProgress took:\n" 
        << (elapsed_time / 3600) << " hours, ";
    elapsed_time %= 3600;

    cout << (elapsed_time / 60) << " minutes, ";
    elapsed_time %= 60;

    cout << elapsed_time << " seconds." << endl;
} //end of compress function

////////////////////////////////////////////////////////////
//count file's characters
void HuffmanCoding::count()
{
    //open input file
    ifstream file(inFile.c_str(),ios::in | ios::binary);

    if(!file.is_open())
        throw runtime_error( "Cannot open the input file!");

    unsigned long inputfilesize = fileSize(),
        currpos = 0, percent;
    percent = inputfilesize / 100;
    int curper = 0;
    int ch;
    //until end of file
    while(!file.eof())
    {
        if( (currpos++) == percent )
        {
            cout << '\r' << ++curper << " % ("
                << currpos * curper << ") Bytes.";
            currpos = 0;
        }
        ch = file.get();
        if(ch != EOF) //do not enter EOF character
            counts[ch]++;
    }
    cout << '\r' << "100 % ("
        << inputfilesize << ") Bytes.";
}

////////////////////////////////////////////////////////////
//set necessary variables to compress
void HuffmanCoding::setVariables()
{
    //insert nonzero counts into mergingTree's heap
    for(int i = 0; i < 256; i++)
    {
        if(counts[i] != 0)
        {
            mergingType tmp(i,counts[i]);
            codeTree->insertNode(tmp);
        }
    }

    //create mergingTree's tree
    // and strings
    codeTree->createTree();
    codeTree->createHuffmanArrays();
    vector<mergingType> a = codeTree->HuffmanElements();
    vector<string> b = codeTree->getHuffmanNumbers();

    // copy strings into strings array
    int size = (int)a.size();
    for(int i = 0; i < size; i++)
        strings[ (a[i]).data ] = b[i];
}

////////////////////////////////////////////////////////////
// write one string containing 1s and 0s into buffer
void HuffmanCoding::writeBits(string str)
{
    int sz = (int)str.size();
    if( (bufferSize + sz) <= 200 ) //if buffer has enough space
    {
        for(int i = 0 ; i < sz; i++) //copy characters
            buffer[i + bufferSize] = str[i];
        bufferSize += sz;
    }
    else //buffer is full
    {
        flushBuffer(); //flush buffer into file's buffer
        writeBits(str); //copy str's Bits
    }
}

////////////////////////////////////////////////////////////
//flush buffer into file's buffer
void HuffmanCoding::flushBuffer()
{
    int i,numTmp, 
        rest = bufferSize % 8, //bits that cannot create a byte
        ops = bufferSize / 8; //number of loops depending on
    // buffer's size
    ops *= 8;
    for(i = 0; i < ops; i += 8)
    {
        //reset the number
        numTmp = 0;
        //for each 8 characters
        // check each character and if its 1 
        // use bitwise or to create proper number
        // for example if the first character is 1
        // then or the number with 128 so we have 1
        // at the 7th bit of byte and each time divide
        // k (number to use with bitwise or) by 2
        for(int j = 0, k = 128; j < 8; j++, k /= 2)
            if(buffer[i + j] == '1')
                numTmp |= k;
        //insert created number into file's buffer
        fileBuffer[fileBufferSize++] = numTmp;

        //if file's buffer is full so
        // write it into the file
        if(fileBufferSize == 1000)
            writeTheFile();
    }

    //copy characters that cannot create
    // a full byte to the first of buffer
    // so they can be used next time
    for(int j = 0; j < rest; j++)
        buffer[j] = buffer[i + j];
    bufferSize = rest;
}

////////////////////////////////////////////////////////////
// write the file's buffer onto the output file
void HuffmanCoding::writeTheFile()
{
    //open the output file and move
    // the file's pointer to the end of it
    ofstream outputfile(outFile.c_str(),ios::app | ios::binary);

    if(!outputfile.is_open()) //if cannot open the file
        throw runtime_error( "Cannot open the output file!");

    //using put function write each character
    for(int i = 0; i < fileBufferSize; i++)
        outputfile.put(fileBuffer[i]);

    //reset file's buffer size
    fileBufferSize = 0;
}

////////////////////////////////////////////////////////////
//decompress input onto output
void HuffmanCoding::decompress(string input, string output)
{
    //initalize variables
    time_t   start, finish;
    codeTree = new mergingTree<mergingType>;
    inFile = input;
    outFile = output;
    fileBuffer = new int[1000];
    fileBuffer2 = new int[1000];
    counts = new int[256];
    fileBufferSize = fileBufferSize2 = bufferSize = 0;
    readChars = 0;
    readDone = false;

    for(int i = 0; i < 256; i++)
        counts[i] = 0;

    //start
    time( &start );
    //read file's signature
    cout << "Reading file's signature..." << flush;
    readSignature();

    //Create output file
    ofstream outputfile(outFile.c_str(),ios::out | ios::binary);

    if(!outputfile.is_open())
        throw runtime_error( "Cannot create the output file!");

    outputfile.close();

    //insert nonzero counts into mergingTree's heap
    // and set other variables
    cout << "Done\nSetting necessary variables for decompressing..." << flush;

    int nonzeros = 0;
    for(int i = 0; i < 256; i++)
    {
        if(counts[i] != 0)
        {
            nonzeros++;
            mergingType tmp(i,counts[i]);
            codeTree->insertNode(tmp);
        }
    }

    //get file's size and precent to show the progress
    unsigned long inputfilesize = fileSize(),
        currpos = 0, percent, outputfilesize = 0;
    inputfilesize -= ( (nonzeros * 8) + 5 );
    percent = inputfilesize / 100;
    int curper = 0;

    //create mergingTree's tree
    // and also get a copy of mergingTree's root
    codeTree->createTree();
    codeTree->createHuffmanArrays();
    mergingTreeNode< mergingType > *rootCopy, *tmp;
    rootCopy = codeTree->getRootCopy();
    tmp = rootCopy;

    //each time the file's buffer is empty
    // these variables are hold to keep track
    // of the last operation and continue it's job
    int tmpj,tmpk;
    bool middle = false;

    cout << "Done\nDecoding file...\n0 % (0) Bytes." << flush;

    //if we reach to the end of file
    // the readDone variable sets to true
    while(!readDone)
    {
        readFile(); //read 1000 bytes to file's buffer
        bool done = false; //to check if we reach to one leaf or not
        int i = 0,j = 0,k = 0; //counters

        //go through file's buffer
        for(i = 0; i < fileBufferSize2; i++, currpos++)
        {
            //check each bit
            for(j = 0, k = 128; j < 8; j++, k /= 2)
            {
                //check if we continue track of the last 
                // character of the file's buffer
                if(middle) 
                { //so restore the variables
                    middle = false;
                    j = tmpj;
                    k = tmpk;
                }

                //if all bits are done and the rest is
                // the zeros we filled with zero
                // so break and do not continue
                if( (readDone && (zeros == (9 - j))) &&
                    ( i == (fileBufferSize2 - 1)) )
                {
                    done = true; 
                    break;
                }

                done = false;

                //if current bit is 0
                if( (fileBuffer2[i] & k) == 0 )
                {
                    //if this is a leaf
                    if( tmp->leftPtr == NULL )
                    {
                        done = true;
                        //if file's buffer is full so write it to file
                        if(fileBufferSize == 1000)
                            writeTheFile();
                        //write character to buffer
                        fileBuffer[fileBufferSize++] = (tmp->data).data;
                        outputfilesize++;
                        tmp = rootCopy->leftPtr;
                    }
                    else //go to left (for zeros we go to left)
                        tmp = tmp->leftPtr;
                }
                else
                {
                    //if this is a leaf
                    if( tmp->rightPtr == NULL )
                    {
                        done = true; //for leaves
                        //if file's buffer is full so write it to file
                        if(fileBufferSize == 1000)
                            writeTheFile();
                        //write character to buffer
                        fileBuffer[fileBufferSize++] = (tmp->data).data;
                        outputfilesize++;
                        tmp = rootCopy ->rightPtr;//go to right
                    }
                    else //go to right (for zeros we go to right)
                        tmp = tmp->rightPtr;
                }
            }

            //if done one percent
            if( (currpos % percent) ==  0 )
            {
                cout << '\r' << ++curper << " % ("
                    << currpos << ") Bytes.";
            }
        }
        //we didn't write the last character
        if( (zeros == 0) && readDone )
            fileBuffer[fileBufferSize++] = (tmp->data).data;

        if(done) //nothing to continue
        {
            fileBufferSize2 = 0; //reset file's buffer size
            middle = false; //no need
        }
        else
        {
            if(j != 8) //if we're at the middle of character
            {
                //backup variables
                middle = true;
                tmpj = j - 1;
                tmpk = k * 2;
                fileBuffer2[0] = fileBuffer2[fileBufferSize2];
                fileBufferSize2 = 1;
            }
            else //we've done all the bits
            {
                fileBufferSize2 = 0; //reset file's buffer size
                middle = false; //no need
            }
        }
    }
    //show the 100% progress
    if(fileBufferSize != 0)
        writeTheFile();
    cout << '\r' << "100 % ("
        << inputfilesize << ") Bytes.";
    cout << "\nOutpuf file size: " << outputfilesize;
    cout << "\nCompression operation completed successfully.";

    //release dynamic memory
    cout << "\nReleasing memory..." << flush;

    delete [] fileBuffer2;
    delete [] fileBuffer;
    delete [] counts;
    delete codeTree;
    cout << "Done";

    time( &finish );
    unsigned long elapsed_time = (unsigned long)difftime( finish, start );

    cout << "\nProgress took:\n" 
        << (elapsed_time / 3600) << " hours, ";
    elapsed_time %= 3600;

    cout << (elapsed_time / 60) << " minutes, ";
    elapsed_time %= 60;

    cout << elapsed_time << " seconds." << endl;
}

////////////////////////////////////////////////////////////
//read 1000 bytes to file's buffer
void HuffmanCoding::readFile()
{
    //open input file
    ifstream file(inFile.c_str(),ios::in | ios::binary);

    if(!file.is_open())
        throw runtime_error( "Cannot open the input file!");


    //continue from last character
    file.seekg(readChars);

    for(; (fileBufferSize2 < 1000) && (!file.eof()); fileBufferSize2++)
        fileBuffer2[fileBufferSize2] = file.get();


    if(file.eof())
    {
        readDone = true;
        fileBufferSize2 -= 2; //skip EOF and zeros characters
    }

    //save the position of file's pointer
    readChars = file.tellg();
    file.close(); //close the file
}

////////////////////////////////////////////////////////////
//  (used in compress operation)
//write file's signature in this format:
// first [number of variables]
// then for each variable [character] [count]
void HuffmanCoding::writeSignature()
{
    //count the nonzero counts
    int cnt = 0;
    for(int i = 0; i < 256; i++)
        if(counts[i] != 0)
            cnt++;

    //create the output file
    ofstream outputfile(outFile.c_str(),ios::out | ios::binary);

    if(!outputfile.is_open())
        throw runtime_error( "Cannot open the output file!");

    //write count
    outputfile.write( reinterpret_cast< const char * >( &cnt ),
        sizeof( cnt ) );

    //write characters and counts
    for(int j = 0; j < 256; j++)
    {
        if(counts[j] != 0)
        {
            outputfile.write( reinterpret_cast< const char * >( &j ),
                sizeof( j ) );
            outputfile.write( reinterpret_cast< const char * >( &counts[j] ),
                sizeof( counts[j] ) );
        }
    }

    //close the file
    outputfile.close();
}

////////////////////////////////////////////////////////////
//read file's signature (used in decompress operation)
void HuffmanCoding::readSignature()
{
    //open input file
    ifstream inputfile(inFile.c_str(),ios::in | ios::binary);

    if(!inputfile.is_open())
        throw runtime_error( "Cannot open the input file!");

    //read count
    int cnt;
    inputfile.read( reinterpret_cast< char * >( &cnt ),
        sizeof( cnt ) );

    //read characters and counts
    int j;
    for(int i = 0; i < cnt; i++)
    {
        inputfile.read( reinterpret_cast< char * >( &j ),
            sizeof( j ) );
        inputfile.read( reinterpret_cast< char * >( &counts[j] ),
            sizeof( counts[j] ) );
    }

    readChars = inputfile.tellg();


    //go to the end of file
    inputfile.seekg(-1,ios::end);
    //read zeros count
    zeros = inputfile.get();

    //close the file
    inputfile.close();
}

////////////////////////////////////////////////////////////
//return file's size
unsigned long HuffmanCoding::fileSize()
{
    //open file
    ifstream inputfile(inFile.c_str(),ios::in | ios::binary);

    if(!inputfile.is_open())
        throw runtime_error( "Cannot open the input file!");

    //go to the end of file
    inputfile.seekg(-1,ios::end);

    //save current location
    ios::off_type tmp = inputfile.tellg();
    return tmp;
}

#endif

/***************************************************************************\
|                                                                           |
|                                                                           |
|                                                                           |
|                             Alireza Noori ©                               |
|                                                                           |
|                                                                           |
|                                                                           |
|                                                                           |
\***************************************************************************/