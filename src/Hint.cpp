/*
 *  Hint.cpp
 *  
 *  Description:
 *
 *
 *  Created by KEPS on 2/10/11.
 *
 */

#include "Hint.h"

/* TODO: What to do about the verbose variables in Hint.cpp and Hint.h? */
Hint::Hint(string answerKey, bool selectVerbose)
     :key(answerKey), hintNum(0), verbose(selectVerbose), hint(key.size(),' ')
{
}

// Default constructor
Hint::Hint()
     :key(" "), hintNum(0), verbose(false), hint(" ")
{
}


void Hint::setKey(string answerKey)
{
    key = answerKey;
    hintNum = 0;
    hint = "";
    while (hint.size() <= key.size())
        hint += " ";
}


void Hint::addLetter(usInt numLetters)
{
    // Add letters if any ' ' are found in substring
    numLetters += numOfSpaces(string(key,hintNum,string::npos),numLetters);

    if (isAccented(string(key,hintNum,numLetters+1))) numLetters++; // not sure how to do this

    if ( hintNum < key.size() )
    {
        if (hintNum + numLetters > key.size()) // Limit num of letters to key size
            numLetters = key.size() - hintNum;
        hint.replace(hintNum, numLetters, string(key,hintNum,numLetters));
        hintNum += numLetters;
    }
    if (hintNum == key.size())
        hint += " <-";
}


int Hint::numOfSpaces(string str, int numLetters)
{
    usInt isLetter = 0, search = numLetters;
    for (short ii = 0; ii < search && search <= str.size(); ii++)
    {
        if (str.at(ii) != ' ')
            isLetter++;
        else
            search++;
    }
    return search - numLetters;
}


bool Hint::isAccented(string str)
{
    vector<string> set{"á", "é", "í", "ó", "ñ", "ä", "ö", "ü", "å", "ß"};
    vector<string>::const_iterator it;
    bool accent = false;

    for (it = set.begin(); it != set.end(); it++)
    {
        if (str.find(*it) != string::npos)
        {
            accent = true;
            break;
        }
    }
    return accent;
}


void Hint::addLetter()
{
    if ( hintNum <= key.size() )
    {
        hint.replace(hintNum,1,string(1,key[hintNum]));
        hintNum++;
    }

    if (key.at(hintNum) == ' ')
        addLetter();

    if (hintNum == key.size())
        hint += " <-";
}


void Hint::fillLetterPlaces()
{
    for (usInt i = hintNum; i < key.size(); i++)
    {
        if (key.at(i) != ' ')
            hint.replace(i,1,"-");
    }
}


void Hint::answer(void)
{
    hint = key + " <-";
}


void Hint::disableHints(void)
{
}


string Hint::example(void)
{
    string hint = "an example of usage";
    return hint;
}


string Hint::help(size_t num)
{
    stringstream hint;
    string ws = ltest::printWhitespace(num);
    hint << ws << '\a' << endl;
    hint << ws << "Want a letter?  Type '-l'." << endl;
    hint << ws << "Want more than one letter? Type '-l#', where # = a number 1 - 9." << endl;
    hint << ws << "Want the answer?  Type '-a'." << endl;
    hint << ws << "Want the number of letters?  Type '-n'." << endl;
    hint << ws << "Want to skip this word?  Type '-s'." << endl;
    hint << ws << "Want help?  Type '-h'." << endl;
    hint << ws << "Want out of here?  Type exit." << endl;
    hint << ws << "Want to turn these messages off?  Type '-d' to disable." << endl;
    return hint.str();
}


string Hint::handle(string hintType, bool & disableHintMsg)
{
    stringstream out;
    switch (hintType[1])
    {
        case 'l':
            if (hintType[2] == '\0')
                addLetter(1);
            else if (atoi(&hintType[2]) < 10 && atoi(&hintType[2]) > 0)
                addLetter(atoi(&hintType[2]));
            else
                addLetter(1);
            out << "-> " << hint << endl;
            break;
        case 'a':
            answer();
            out << "-> " << hint << endl;
            break;
        case 'n':
            fillLetterPlaces();
            out << "-> " << hint << endl;
            break;
        case 'h':
            out << help();
            break;
        case 'd':
            disableHintMsg = !disableHintMsg;
    }
  return out.str();
}

// skip word should be handled elsewhere...
// if synonym hint is selected, create new Hint object with new key
// and perform answer() on it.
// string Hint::synonym()
