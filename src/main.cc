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
                const vector<vector<Interval>>best_combinations,
                vector<Repeat> repeats) {

  // // print indeces
  // for (size_t i = 0; i < S.length(); ++i) {
  //   cout << setw(2) << i << " ";
  // }
  // cout << endl;

  // // print string
  // for (char c : S) {
  //   cout << setw(2) << c << " ";
  // }
  // cout << endl;

  // print maximal cover
  // cout << "maximal cover: " << best_cover << endl;
  // cout << best_cover << endl;
  cout << best_cover << "_";


  int prev_end = -1; // + 1?

  // print combinations
  for (long unsigned int i = 0; i < best_combinations.size(); ++i) {
    for (int j = 0; j < best_combinations[i].size(); j++) {
      const auto& interval = best_combinations[i][j];
      // cout << "start: " << interval.start << " end: " << interval.end << " length: " << interval.length << " group: " << interval.group << endl;

      // print for greedy algorithms
      if (interval.start > prev_end + 1) {
        cout << S.substr(prev_end + 1, interval.start - prev_end - 1) << ".";
      }

      if (interval.group == -1) { // no group
        int start = interval.start;
        int lengte = interval.length;

        int k = j + 1;
        while (k < best_combinations[i].size() && best_combinations[i][k].group == -1){
          lengte += best_combinations[i][k].length;
          k++;
        }
        cout << S.substr(start ,lengte) << ".";
        prev_end = start + lengte;
        j = k - 1;
      }
      else {
        cout << S.substr(interval.start ,repeats[interval.group].period) << "[" << interval.length/repeats[interval.group].period << "].";
        prev_end = interval.end;
      }
    }
    // print for greedy algorithms
    if (prev_end < S.length() - 1){
      cout << S.substr(prev_end + 1, S.length() - prev_end - 1) << ".";
    }

    cout << endl;
  }
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

  // auto start = chrono::high_resolution_clock::now(); // start

  PMR pmr(S);
  const auto& repeats = pmr.getRepeats();

  // auto end = chrono::high_resolution_clock::now(); // end
  // chrono::duration<double> elapsed = end - start; // calculation
  // cout << std::fixed << std::setprecision(9) << elapsed.count() << "_";
  // cout << elapsed.count(); // print

  if (algorithm == "bruteforce") {
    // PMR pmr(S);
    // const auto& repeats = pmr.getRepeats();
    // auto start = chrono::high_resolution_clock::now(); // start

    Bruteforce bruteforce(pmr, repeats);
    bruteforce.bruteforceCover(0, -1, best_cover, 0, current, best_combinations);

    // auto end = chrono::high_resolution_clock::now(); // end
    // chrono::duration<double> elapsed = end - start; // calculation
    // cout << elapsed.count(); // print
  }
  else if (algorithm == "greedy_position") {    
    // auto start = chrono::high_resolution_clock::now(); // start

    Greedy greedy(pmr, repeats);
    greedy.greedyCover(best_cover, best_combinations, "position");

    // auto end = chrono::high_resolution_clock::now(); // end
    // chrono::duration<double> elapsed = end - start; // calculation
    // cout << std::fixed << std::setprecision(9) << elapsed.count();
    // cout << elapsed.count(); // print
  }
  else if (algorithm == "greedy_length") {  
    // auto start = chrono::high_resolution_clock::now(); // start

    Greedy greedy(pmr, repeats);
    greedy.greedyCover(best_cover, best_combinations, "length");

    // auto end = chrono::high_resolution_clock::now(); // end
    // chrono::duration<double> elapsed = end - start; // calculation
    // cout << std::fixed << std::setprecision(9) << elapsed.count();
    // cout << elapsed.count(); // print
  }
  else if (algorithm == "dynamic") {
    if (!S.empty()) {   
      // auto start = chrono::high_resolution_clock::now(); // start

      Dynamic dynamic(S, repeats);
      dynamic.dynamicCover(best_cover, best_combinations);

      // auto end = chrono::high_resolution_clock::now(); // end
      // chrono::duration<double> elapsed = end - start; // calculation
      // cout << std::fixed << std::setprecision(9) << elapsed.count();
      // cout << elapsed.count(); // print
    }
  }
  else if (algorithm == "dynamic_l") {
    if (!S.empty()) {  
      // auto start = chrono::high_resolution_clock::now(); // start

      Dynamic dynamic(S, repeats);
      dynamic.dynamicCover_l(best_cover, best_combinations);

      // auto end = chrono::high_resolution_clock::now(); // end
      // chrono::duration<double> elapsed = end - start; // calculation
      // cout << std::fixed << std::setprecision(9) << elapsed.count();
      // cout << elapsed.count(); // print
    }
  }
  else if(algorithm == "linear_PMR"){
    PMR_linear pmr_linear;
    pmr_linear.LZ77Factorization();
  }
  else {
    cerr << "Unknown mode: " << algorithm << endl;
    return 1;
  }
  printOutput(S, best_cover, best_combinations, repeats);

  return 0;
}
