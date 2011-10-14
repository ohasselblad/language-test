/*
 *  wordData.cpp
 *  
 *  Description: functions for the class wordData
 *
 *
 *  Created by Añdy Eschbacher on 9/11/11.
 *
 */

#include "wordData.h"

extern bool debug;

void wordData::populate(int size)
{   // Populates wordData object to initialization values
    numAsked = 0;
    percentRight = 0.0;
    avgTime = 0.0;
    probability = 1.0 / static_cast<double> (size);
}

double wordData::weight(bool wrong, double diff)
{   // weight for answering (non-hints)
    double weight;

    if (wrong)
    {    // Probability increase with response time for wrong answers
         // Quick responses are proportional to smaller probability differentials
      weight = 0.24 * (1 - exp(-0.2 * diff));
    }
    else // if correct
    {    // Probability decreases with response times for correct answers
         // Quick responses are proportional to larger probabilty differentials
      weight = -0.24 * exp(-0.2 * diff);
    }
    
    if (debug) cout << "weight = " << weight << endl;

    return weight;
}

double wordData::weight(char typeOfHint, int numLetReqstd, double currProb)
{   // weight for hints
    double weight = 0.0;

    switch (typeOfHint)
    {   // Bounds on weight: I think they're for p = currProb:
        //  -1/(1-p) < weight < 1/p  -- so the 's' case is the lower bound
        //  and we have a lot more freedom on the upper bound, even up to 1.0
        //  safely since p <= 1.0 by definition
        case 'l':   // get a letter
            weight = 0.3 * static_cast<double> (numLetReqstd);
            if (weight >= 1.0 / currProb)  // to ensure that probability constraints aren't broken
                weight = 1.0 / currProb - 0.1;
            break;
        case 'a':   // get answer
            weight = 0.5;
            break;
        case 'n':   // get number of letters
            weight = 0.05;
            break;
        case 's':   // skip a word (this weight sets probability to 0.0 and raises others)
            weight = - 1.0 / (1.0 - currProb);
            break;
        case 'u':
                // Not yet implemented -- for showing usage of word in question
            break;
        case 'y':
            weight = 0.025;
            break;
        default:
            weight = 0.0; // no effect, shouldn't be triggered
            break;
    }

    if (debug) cout << "weight = " << weight << endl;

    return weight;
}

void wordData::updateProbs_alt(int index, int numOfEntries, double weight, wordData * wordInfo)
{   // Updates probabilities
    
    double pStar = wordInfo[index].probability;
    double alpha = fdim(1.0,pStar), beta = pStar;
    double gamma = 0.001, probUnasked = 0.0;
    static int guard = 1;

    for (int ii = 0; ii < numOfEntries; ii++)
    {
        if (wordInfo[ii].numAsked == 0 && ii != index)
            probUnasked += wordInfo[ii].probability;
    }

    if (guard < numOfEntries / 2)
    {
        beta = pStar;
        guard++;
    }
    else
        beta = (gamma * probUnasked / weight + pStar * alpha) / (alpha - probUnasked);
    
    cout.precision(10);
    cout << "total probability sum = " << sumProbs(wordInfo,numOfEntries) << endl;
    cout << "(alpha,beta) = (" << fma(weight,alpha,1.0) << ", " << fma(-weight,beta,1.0) << ", " << fma(gamma,1.0,1.0) << ")" << endl;
    cout.precision(6);
    
    for (int ii = 0; ii < numOfEntries; ii++)
    {
        if ( ii == index )
            wordInfo[ii].probability *= fma(weight,alpha,1.0);
        else if (wordInfo[ii].numAsked > 0 || (guard < numOfEntries / 2))
            wordInfo[ii].probability *= fma(-weight,beta,1.0);
        else
            wordInfo[ii].probability *= fma(gamma,1.0,1.0);
    }
}

void wordData::updateProbs(int index, int numOfEntries, double weight, wordData * wordInfo)
{   // Updates probabilities

    double probUnasked = 0.0, pStar = wordInfo[index].probability;
    int numOfNumAskedIs0 = 0;
    double alpha = fdim(1.0,pStar), beta;
    double gamma = 0.01, gamWeight = 1.0;

    // Find pJ
    for (int ii = 0; ii < numOfEntries; ii++)
    {
        if (wordInfo[ii].numAsked == 0 && ii != index)
        {
            probUnasked += wordInfo[ii].probability;
            numOfNumAskedIs0++;
        }
    }
    
    cout << "probNumAskedIs0 = " << probUnasked << endl;
    cout << "numOfNumAskedIs0 = " << numOfNumAskedIs0 << endl;

    // fma(x,y,z) = x*y + z correctly rounded -- since probs need to be sum(probs) = 1.0, be careful on rounding errors
    // Should we worry about rounding errors in division?  How big of an issue is this?

    // Divide-by-zero guard
    if (numOfNumAskedIs0 < (numOfEntries - 2))
    {
        gamma = 0.01;
        beta = (gamma * probUnasked / weight + pStar * alpha) / (alpha - probUnasked);
        gamWeight = 1.0;
    }
    else
    {
        beta = pStar;
        gamWeight = -weight;
        gamma = beta;
    }

    cout.precision(10);
    cout << "total probability sum = " << sumProbs(wordInfo,numOfEntries) << endl;
    cout << "(alpha,beta,gamma) = (" << alpha << ", " << beta << ", " << gamma << ")" << endl;
    cout << "alpha * p1 - beta * pI + gamma * pJ / w = " << \
    alpha * pStar - beta * fdim(1.0, pStar + probUnasked) + gamma * probUnasked / weight << endl;
    cout.precision(6);

    for (int ii = 0; ii < numOfEntries; ii++)
    {
        if ( ii == index )
            wordInfo[ii].probability *= fma(weight,alpha,1.0);
        else if ( wordInfo[ii].numAsked != 0 )
            wordInfo[ii].probability *= fma(-weight,beta,1.0);
        else
            wordInfo[ii].probability *= fma(gamWeight,gamma,1.0);
    }
}

void wordData::updateScore(int index, bool wrong, double timeDiff, \
                           int numOfEntries, wordData * wordInfo)
{
    // Update number of individual queries of word
    numAsked++;

    // Update probabilities
    updateProbs(index, numOfEntries, \
                wordData::weight(wrong,timeDiff), wordInfo);

    // Update scoring percentage
    if (numAsked == 1)
        (wrong)?(percentRight = 0.0):(percentRight = 1.0);
    else if (!wrong && numAsked > 1)
        percentRight = reweight(numAsked,percentRight,1.0);
    else if (wrong && numAsked > 1)
        percentRight = reweight(numAsked,percentRight,0.0);

    // Update timing information
    avgTime = reweight(numAsked,avgTime,timeDiff);
}

void wordData::updateScore(int index, int numOfEntries, wordData * wordStats, \
                           char typeOfHint, unsigned int numLetReqstd)
{ // This is the hints variant of this function
    updateProbs(index, numOfEntries, \
                weight(typeOfHint,numLetReqstd,wordStats[index].probability), \
                wordStats);
}

double wordData::sumProbs(wordData * wordStats, int numOfEntries)
{
    double sum = 0.0;
    for (int ii = 0; ii < numOfEntries; ii++)
        sum += wordStats[ii].probability;

    return sum;
}

double wordData::reweight(int num, double old, double newish)
{
    double nd = static_cast<double> (num);
    return ((nd - 1.0) * old + newish)/nd;
}
