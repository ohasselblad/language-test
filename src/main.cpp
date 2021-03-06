/*
 *  LanguageTest.cpp
 *
 *  Language quiz program.
 *        Takes in a two-column file, stores it, and then quizzes and then quizzes
 *         the user on the translations.
 *
 *
 *  Created by Añdy Eschbacher on 4/29/11.
 *
 *
 *  Create an alternate mode called 'direct feedback' where the user
 *  instantly learns her/his wrong answer -- or even slighly delayed -- kind of
 *  learning-machine like, and not being in the dark and moving onto another
 *  word instead, which could lead to some mental muddiness and focus on
 *  trouble words.
 *
 *  Contained lessons/quizzes.  Lessons that have a clear ending point so
 *  the student will feel a sense of satisfaction on completing a unit.
 *  They can be performance-related, time-related, number-of-items-related,
 *  or more...
 *
 */

#include <cstdlib>
#include <iostream>
/* TODO: make a map container to contain all the program options such as
 * toDoOption, etc.
 *
#include <map>
 */

#include "functions.h"
#include "progPrefs.h"
#include "Account.h"
#include "ltest_typedefs.h"

/* Quizzes */
#include "flcrd_quiz.h"
#include "Numbers.h"

using std::cin;
using std::cout;
using std::endl;

bool exitToMain = false;

int main(int argc, char **argv)
{
    /*****    Initialize Variables    *****/

    Account acct;
    int score; // Do away with this?

    srand(time(NULL)); // What is this initialized for?

    /*****    Take optional input from command line   *****/
    progPrefs::cmdLineInput(argc,argv);

    ltest::welcomeMessage();
    acct.establishAccount();

    while (true)
    {
        usInt toDoOption = ltest::whatDoYouWantToDo(acct.getName());

        switch (toDoOption)
        {
            case 1:
                cout << "you chose flash cards" << endl;
                score = flcrd_quiz(acct);
                break;
            case 2:
                cout << "you chose number quiz" << endl;
                score = numbers(acct);
                break;
            case 3:
                cout << "you chose conjugations!" << endl;
                /* run conjugations program */
                break;
            case 4:
                cout << "you chose fill-in-the-blank" << endl;
                /* run fill-in-the-blank program */
                break;
            case 5:
                cout << "you chose the multiple choice quiz!" << endl;
                /* run multiple-choice program */
                break;
            case 6:
                cout << "you chose to change the user!" << endl;
                acct.establishAccount();
                break;
            case 7:
                cout << "you chose to see your account summary" << endl;
                /* print account summary */
                /* This will include which tests were taken, the dates taken, the scores, etc. */
                cout << "***This feature is not yet implemented."
                     << "***Returing to main menu.";
                break;
            case 8:
                cout << "you chose to change some program options" << endl;
                progPrefs::changeProgOptions(acct);
                break;
            case 9:
                cout << "you chose to exit the program" << endl;
                cout << ltest::goodbye(acct.getName()) << endl;
                exit(0);
            default:
                cout << toDoOption << " is not a valid option." << endl;
                break;
        }
        exitToMain = false;
    }

    return 0;
}
