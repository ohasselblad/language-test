/*
 *  flcrd_quiz.h
 *  
 *
 *  Created by Añdy Eschbacher on 10/19/11.
 *
 */

#ifndef FLCRD_QUIZ_H
#define FLCRD_QUIZ_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

#include <boost/chrono.hpp>
#include <readline/readline.h>

#include "functions.h"
#include "Flashcard.h"
#include "testResults.h"
#include "Hint.h"
#include "SmartPicker.h"
#include "listDicts.h"
#include "ltest_typedefs.h"
#include "Account.h"

using std::string;
using std::cout;
using std::vector;
using std::endl;

extern bool exitToMain;

namespace chr = boost::chrono;

int flcrd_quiz(Account&);

#endif // FLCRD_QUIZ_H
