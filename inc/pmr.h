#ifndef pmrH
#define pmrH

#include "types.h"

 #include <iostream>
 #include <vector>
 #include <string>
 #include <cmath>
 #include <algorithm>
using namespace std;

class PMR {
  private:
    string S;
    vector<Repeat> repeats;

  public:
    // constructor: initializes the input string and calculates all repeats
    PMR(const string& input): S(input) {
        repeats = findRepeat(S);
    }
    
    // returns input string
    const string& getString() const { return S; }

    // returns repeats
    const vector<Repeat>& getRepeats() const { return repeats; }

    // checks if repeat is a rotation of another repeat
    bool isRotation(const std::vector<Repeat>& repeats, const int i, const int j);

    // checks if repeat is primitive
    bool isPrimitive (const std::vector<Repeat>& repeats, const int i, 
                      const int j, const int count);

    // find all primitive maximal repeats in string S
    vector<Repeat> findRepeat(const string S);

    // get all possible covers with shift
    void shiftIntervals(vector<vector<Interval>>& covers, int shift, int start, 
                        int end, int pmr, int length);
    
    // get all possible covers
    vector<vector<Interval>> getInterval(const vector<Repeat> repeats);
    
}; // PMR


class PMR_linear {
  private:

  public:
    PMR_linear() {};

    void suffixTree();

    void fractorization ();

};


#endif