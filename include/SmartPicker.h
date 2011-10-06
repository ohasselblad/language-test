/*
 *  SmartPicker.h
 *  
 *  Description:
 *
 *
 *  Created by KEPS 4/10/11.
 *
 */

#ifndef _SMARTPICKER_H_
#define _SMARTPICKER_H_

#include <iostream>
#include <numeric>
#include <sstream>
#include <typeinfo>
#include <vector>
#include <string>
#include <ctime>

#include "Flashcard.h"

using std::vector;

class SmartPicker {
private:
    unsigned int currentIndex;
    void setCurrentIndex(int);
public:
    unsigned int nextIndex(bool sideA, vector<Flashcard>);
    unsigned int leastPickedIndex(bool sideA, vector<Flashcard>);
};

#endif // SMARTPICKER_H