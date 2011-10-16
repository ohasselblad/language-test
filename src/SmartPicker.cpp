/*
 *  SmartPicker.cpp
 *  
 *  Description: Algorithms for flashcard selection.
 *      Adaptive: probability of card selection based on past performance
 *      through use of boost random library.  Algorithm inspired by
 *      http://www.boost.org/doc/libs/1_47_0/doc/html/boost_random/tutorial.html
 *
 *  Created by KEPS on 4/10/11.
 *
 */

#include "SmartPicker.h"

/**    SmartPicker members    **/
void SmartPicker::setCurrentIndex(int index)
{
    currentIndex = index;
}

unsigned int SmartPicker::findSmallest(const vector<Flashcard> & deck)
{
    unsigned int currentLowest = deck[0].data.numCorrect, temp;
    
    for (int ii = 1; ii < deck.size(); ii++)
    {
        temp = deck[ii].data.numCorrect;
        if (temp < currentLowest)
            currentLowest = temp;
    }
    return currentLowest;
}

// Default constructor
SmartPicker::SmartPicker(void)
{
    currentIndex = 0;
}

unsigned int SmartPicker::nextIndex(int cardsSize)
{
    return (currentIndex++ % cardsSize); // What's the order of operations here?
                                         // Should currentIndex be increment before the mod is taken?
}

/**    LeastCorrect members    **/
// Experimenting with not generating leastCorrectIndices anew each time
unsigned int LeastCorrect::leastCorrectIndex(const vector<Flashcard> & cards, \
                                             int lastIndex)
{
    srand(time(0));
    list<int> leastCorrectIndices;
    unsigned int currentLowest = findSmallest(cards);

// write some WordData member functions to get various variables, which will be
// set to private
    if (cards[lastIndex].data.numCorrect > currentLowest)
        leastCorrectIndices.remove(lastIndex);

    if ( leastCorrectIndices.empty() )
        for (int i = 0; i < cards.size(); i++)
        {
            if (cards[i].data.numCorrect == currentLowest)
                leastCorrectIndices.push_back(i);
        }
    
    list<int>::iterator it = leastCorrectIndices.begin();
    int indexChoice = *(it + (rand() % leastCorrectIndices.size()));

    if (indexChoice == currentIndex)
        setCurrentIndex((indexChoice+1) % leastCorrectIndices.size());
    else
        setCurrentIndex(indexChoice);

    return currentIndex;  
}

/**    LeastPicked members    **/
unsigned int LeastPicked::leastPickedIndex(const vector<Flashcard> & cards, \
                                           int lastIndex)
{
    srand(time(0));
    vector<unsigned int> leastAskedIndices;
    unsigned int currentLowest = findSmallest(cards);

    for (int ii = 0; ii < cards.size(); ii++)
        if ( cards[ii].data.numAsked == currentLowest )
            leastAskedIndices.push_back(ii);

    int indexChoice = leastAskedIndices[rand() % leastAskedIndices.size()];

    if (indexChoice == currentIndex)
        setCurrentIndex((indexChoice+1) % leastAskedIndices.size());
    else
        setCurrentIndex(indexChoice);

    return currentIndex;
}

/**    Adaptive members    **/

// Default Constructor
Adaptive::Adaptive(int numEntries)
{
    probability.assign(numEntries,inverse(numEntries));
    gen.seed(static_cast<unsigned int>(std::time(0))); // initialize random seed
}

// Desctructor
Adaptive::~Adaptive(void)
{
    probability.clear();
}

double Adaptive::weight(bool wrong, double diff)
{   // weight for answering (non-hints)
    double weight = 0.0;

    if (wrong)
        weight = 0.24 * (1.0 - exp(-0.2 * (diff + levDistance)));
    else // if correct
        weight = -0.24 * exp(-0.2 * diff);

    return weight;
}

void Adaptive::updateProbsAdvanced(int index, bool isWrong, double ansTime, \
                                   const vector<Flashcard> & cards)
{   // Updates probabilities
    double wgt = weight(isWrong,ansTime);
    double probUnasked = 0.0;
    double pStar = probability[index];
    int numOfNumAskedIs0 = 0;
    double alpha = fdim(1.0,pStar), beta;
    double gamma = 0.01, gamWeight = 1.0;   // Experiment with different gammas
    vector<double>::iterator it, itIndex = (probability.begin() + index);
    
    if (debug) cout << "stage 1" << endl;
    for (int ii = 0; ii < probability.size(); ii++)
    {
        if (cards[ii].data.numAsked == 0 && ii != index)
        {
            probUnasked += probability[ii];
            numOfNumAskedIs0++;
        }
    }
    if (debug) cout << "stage 2" << endl;
        // Divide-by-zero guard
    if (numOfNumAskedIs0 < (probability.size() - 2))
    {
        gamma = 0.01;
        beta = (gamma * probUnasked / wgt + pStar * alpha) / (alpha - probUnasked);
        gamWeight = 1.0;
    }
    else
    {
        beta = pStar;
        gamWeight = -wgt;
        gamma = beta;
    }
    if (debug) cout << "stage 3" << endl;
    for (int ii = 0; ii < probability.size(); ii++)
    {
        if ( ii == index )
            probability[ii] *= fma(wgt,alpha,1.0);
        else if ( cards[ii].data.numAsked != 0 )
            probability[ii] *= fma(-wgt,beta,1.0);
        else
            probability[ii] *= fma(gamWeight,gamma,1.0);
    }
    if (debug) cout << "stage 4" << endl;
}

void Adaptive::updateProbsBasic(int index, bool isWrong, double ansTime)
{
    double wgt = weight(isWrong,ansTime);
    double pStar = probability[index];
    double alpha = fdim(1.0,pStar), beta = pStar;
    vector<double>::iterator it, itIndex = (probability.begin() + index);

    for (it = probability.begin(); it != probability.end(); it++)
    {
        if ( it == itIndex )
            *it *= fma(wgt,alpha,1.0);
        else
            *it *= fma(-wgt,beta,1.0);
    }
}

unsigned int Adaptive::adaptiveIndex(const vector<Flashcard> & cards)
{
    // ansTime is a float and weight is a double -- does the precision difference matter?
//    updateProbs(index, weight(isWrong,ansTime));

    static unsigned int lastIndex = UINT_MAX;
    unsigned int currentIndex;
    boost::random::discrete_distribution<> dist(probability);

    do
    {
        currentIndex = dist(gen);
    } while (currentIndex == lastIndex);

    lastIndex = currentIndex;

    return currentIndex;
}

string Adaptive::probabilitySummary(vector<Flashcard> & cards)
{
    stringstream ps;
    ps << "Summary:\n";
    for (int ii = 0; ii < cards.size(); ii++)
    {
        ps << cards[ii].getWord('B',0) << '\t' << static_cast<int> (100 * probability[ii]) << endl;
    }

    return ps.str();
}

void Adaptive::setLevDistance(string str1, string str2)
{
    unsigned int temp1 = levenshtein(str1,str2);

    levDistance = static_cast<double> (temp1);
}
