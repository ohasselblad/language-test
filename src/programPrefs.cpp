/*
 *  programPrefs.cpp
 *  
 *  Description:
 *     Functions to control the environment variables of ltest
 *
 *
 *  Created by Añdy Eschbacher on 10/6/11.
 *
 */

#include "programPrefs.h"

void progPrefs::cmdLineInput(int argc, char **argv, char * inFile, bool &verbose, bool &debug)
{
    int c;
    while ( (c = getopt(argc, argv, ":i:vhdl")) != -1)
    {
        switch (c)
        {
            case 'i': // Input non-default dictionary
                strcpy(inFile,argv[optind-1]);
                break;
            case ':':
                if (optopt == 'i')
                {   // Hmm, this is slightly redundant with what input() does
                    cerr << "Warning: Option '-i' must have more than one argument." << endl;
                    cout << "Type a new file name to continue or 'exit' to exit program." << endl;
                    cin >> inFile;
                    if ( ltest::exitProg(inFile,cin.eof()) ) // if 'exit', exit program
                        exit(0);
                    cin.clear();
                    cin.ignore(10,'\n');
                }
            case 'v': // Verbose output
                verbose = true;
                break;
            case 'h': // Print usage info then exit
                cout << ltest::help(argv[0]);
                exit(0);
            case 'd': // Show debug output info
                debug = true;
                break;
            case 'l': // List available dictionaries
                strcpy(inFile,listDicts().c_str());
                break;
            case '?':
                cerr << "Option '-" << static_cast<char> (optopt) << "' is not valid." << endl;
                break;
            default:
                cerr << "Invalid commandline options." << endl;
                ltest::help(argv[0]);
                exit(0);
                break;
        }
    }
}

void progPrefs::changeProgOptions(Account & acct)
{
    cout << "The following variables are set: " << endl;
    cout << "1: verbose = " << (verbose?"on":"off") << endl;
    cout << "2: User = " << acct.getName() << endl;
    cout << endl;
    cout << "Enter a number to change its value" << endl;
}

// EOF
