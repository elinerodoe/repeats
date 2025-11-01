#include "../inc/pmr.h"
#include "../inc/cover.h"
#include "../inc/types.h"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <chrono> // voor timing
using namespace std;


void printOutput(const string S, 
                const int best_cover, 
                const vector<vector<Interval>>best_combinations) {

  // print indeces
  for (size_t i = 0; i < S.length(); ++i) {
    cout << setw(2) << i << " ";
  }
  cout << endl;

  // print string
  for (char c : S) {
    cout << setw(2) << c << " ";
  }
  cout << endl;

  // print maximal cover
  cout << "maximal cover: " << best_cover << endl;

  cout << best_cover << endl;

  // print combinations
  for (long unsigned int i = 0; i < best_combinations.size(); ++i) {
    cout << "combination " << i + 1 << ": ";
    for (const auto& interval : best_combinations[i]) {
      cout << "[" << interval.start << "," << interval.end << "] ";
    }
  }
  cout << endl;

}


int main(int argc, char* argv[]) {
  int best_cover = 0;
  vector<Interval> current;
  vector<Interval> best_combination;
  vector<vector<Interval>> best_combinations;

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

    using namespace std::chrono;
    auto time_a = high_resolution_clock::now();

    bruteforce.bruteforceCover(0, -1, best_cover, 0, current, best_combinations);

    auto time_b = chrono::high_resolution_clock::now();
    duration<double> elapsed = time_b - time_a;
    cout << "timer bruteforceCover(): " << elapsed.count() << " seconds\n";
  }
  else if (algorithm == "greedy") {
    Greedy greedy(pmr, repeats);
    greedy.greedyCover(best_cover, best_combination);
  }
  else if (algorithm == "dynamic") {
    if (!S.empty()) {
      Dynamic dynamic(S, repeats);
      dynamic.dynamicCover(best_cover);
      // cout << best_cover << endl;
    }
  }
  else {
    cerr << "Unknown mode: " << algorithm << endl;
    return 1;
  }

  printOutput(S, best_cover, best_combinations);

  return 0;
}
