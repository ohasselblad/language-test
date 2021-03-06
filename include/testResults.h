/*
 *  testResults.h
 *  
 *  Description: Header file to testResults.cpp
 *
 *
 *  Created by Añdy Eschbacher on 9/15/11.
 *
 */

#ifndef TESTRESULTS_H
#define TESTRESULTS_H

#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

#include "Flashcard.h"
#include "WordData.h"
#include "functions.h"
#include "ltest_typedefs.h"
#include "Account.h"

using std::cout;
using std::endl;
using std::ios;
using std::setw;
using std::vector;

void testResults(vector<Flashcard>, bool);
    // void sorter()
#endif // TESTRESULTS_H
