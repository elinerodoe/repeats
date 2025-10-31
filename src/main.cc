#include "../inc/pmr.h"
#include "../inc/cover.h"
#include "../inc/types.h"

 #include <iostream>
 #include <vector>
 #include <string>
 #include <cmath>
 #include <algorithm>
using namespace std;


int main(int argc, char* argv[]) {
  int best_cover = 0;
  vector<Interval> current;
  vector<Interval> best_combination;

  if (argc < 3) {
    cerr << "Usage: ./repeats <algorithm> <word>\n";
    cerr << "Functions: bruteforce | greedy | dynamic\n";
    return 1;
  }
  string algorithm = argv[1];
  string S = argv[2];

  PMR pmr(S);
  const auto& repeats = pmr.getRepeats();

  if (algorithm == "bruteforce") {
    Bruteforce bruteforce(pmr, repeats);
    bruteforce.bruteforceCover(0, -1, best_cover, 0, current, best_combination);
  }
  else if (algorithm == "greedy") {
    Greedy greedy(pmr, repeats);
    greedy.greedyCover(best_cover, best_combination);
  }
  else if (algorithm == "dynamic") {
    Dynamic dynamic(S, repeats);
    dynamic.dynamicCover(best_cover);
  }
  else {
    cerr << "Unknown mode: " << algorithm << endl;
    return 1;
  }

  cout << best_cover << endl;
  return 0;
}
