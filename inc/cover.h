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

    // find combination of maximal cover
    Interval findCombination(int& i, vector<int> size);

    // find all possible combination with maximal cover
    void findAllCombinations(int i, const vector<int>& size, vector<Interval>& current, vector<vector<Interval>>& all_combinations, int& group);

    // helper function to checks the sigma/rho condition 
    bool sigmaRho(int& sigma, int i, Repeat repeat);

    // dynamic algorithm to find the maximal cover
    void dynamicCover(int& best_cover, vector<vector<Interval>>& best_combinations);

    // helper function to calculate the cover
    void bestCount(int count, int period, int i, vector<int>size, int& best_cover);

    // dynamic algorithm to find the maximal cover
    void dynamicCover_l(int& best_cover, vector<vector<Interval>>& best_combinations);

}; // Dynamic


#endif