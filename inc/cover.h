#ifndef coverH
#define coverH

#include "types.h"
#include "pmr.h"

 #include <iostream>
 #include <vector>
 #include <string>
 #include <cmath>
 #include <algorithm>
using namespace std;


class Bruteforce {
  private:
    vector<vector<Interval>> covers;

  public:
    // constructor: retrieves interval covers based on given repeats
    Bruteforce(PMR& pmr, const vector<Repeat>& repeats)
        : covers(pmr.getInterval(repeats)) {}
    
    // brute force algorithm to find the maximal cover
    void bruteforceCover(int depth, 
                        int prev_end, 
                        int &best_cover, 
                        int cover_size, 
                        vector<Interval>& current, 
                        vector<vector<Interval>>& best_combinations);
}; // Bruteforce


class Greedy {
  private:
    vector<vector<Interval>> covers;

  public:
    // constructor: retrieves interval covers based on given repeats
    Greedy(PMR& pmr, const vector<Repeat>& repeats)
      : covers(pmr.getInterval(repeats)) {}

    // greedy algorithm to find the maximal cover
    void greedyCover(int &best_cover, vector<Interval>& best_combination);

    // checks whether new_cover overlaps with cover
    bool overlaps(const Interval& cover, const Interval& new_cover);
}; // Greedy


class Dynamic {
  private:
    string S;
    const vector<Repeat>& repeats;
    
  public:
    // constructor: initializes the input string and reference to repeats
    Dynamic(const string& input, const vector<Repeat>& r)
      : S(input), repeats(r) {}

    // dynamic algorithm to find the maximal cover
    void dynamicCover(int& best_cover);

    // helper function to checks the sigma/rho condition 
    bool sigmaRho(int& sigma, int i, Repeat repeat);
}; // Dynamic


#endif