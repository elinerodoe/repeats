#include "../inc/cover.h"
#include "../inc/pmr.h"
#include "../inc/types.h"

 #include <iostream>
 #include <vector>
 #include <string>
 #include <cmath>
 #include <algorithm>
using namespace std;


void Bruteforce::bruteforceCover(int depth, 
                                int prev_end, 
                                int &best_cover, 
                                int cover_size, 
                                vector<Interval>& current, 
                                vector<Interval>& best_combination) {
  // reached last pmr
  if (depth == (int)covers.size()) {
    if (cover_size > best_cover) {
      best_cover = cover_size;
      best_combination = current;
    }
    return;
  }
  // loop through all covers of pmr[depth]
  for (const auto& cover : covers[depth]){
    // check for overlap
    if (prev_end < cover.start) {
      current.push_back(cover);
      bruteforceCover(depth + 1, cover.end, best_cover, cover_size + cover.length, current, best_combination);
      current.pop_back();
    }
  }
  bruteforceCover(depth + 1, prev_end, best_cover, cover_size, current, best_combination);
} // Bruteforce::bruteforceCover

//******************************************************************************

bool Greedy::overlaps(const Interval& cover, const Interval& new_cover) {
  if (new_cover.start <= cover.end && cover.start <= new_cover.end) {
    return true;
  }
  return false;
} // Greedy::overlaps

//******************************************************************************

// GREEDY ALGORITHM
void Greedy::greedyCover(int &best_cover, vector<Interval>& best_combination) {
    vector<Interval> covers_1D;
    vector<bool> group_used(covers.size(), false);
    bool overlap_found = false;

    // convert 2D covers vector to 1D covers vector 
    for (int pmr = 0; pmr < (int)covers.size(); ++pmr) {
      for (const auto& cover : covers[pmr]) {
        covers_1D.push_back({cover.start, cover.end, cover.length, pmr});
      }
    }

    // sort 1D covers list by smallest end position and if equal by longest length
    // std::sort(covers_1D.begin(), covers_1D.end(),
    //   [](const Interval& a, const Interval& b) {
    //     if (a.end != b.end) {
    //       return a.end < b.end;
    //     }
    //     return a.length > b.length;
    //   }
    // );

    // sort 1D covers list by longest length and if equal by smallest end position
    std::sort(covers_1D.begin(), covers_1D.end(),
      [](const Interval& a, const Interval& b) {
        if (a.length != b.length) {
            return a.length > b.length;
        }
        return a.end < b.end;
      }
    );

    for (const auto& i : covers_1D) {
      for (const auto& c : best_combination) {
        if (overlaps(c, i)) {
          overlap_found = true;
          break;
        }
        overlap_found = false;
      }
      if (!group_used[i.group] && !overlap_found) {
        best_combination.push_back(i);
        group_used[i.group] = true;
        best_cover += i.length;
      }
    }
} // Greedy::greedyCover

//******************************************************************************

bool Dynamic::sigmaRho(int& sigma, int i, Repeat repeat) {
  int shift = repeat.shift;
  int rho = (i - repeat.index + 1) % repeat.period;
  sigma = (i - repeat.index + 1) / repeat.period;

  bool sigma_bool = false;

  if (sigma >= 2 && sigma <= repeat.count) {
    sigma_bool = true;
    if (sigma < repeat.count) {
      shift = repeat.period - 1;
    }
    
  }
  if (sigma_bool && rho >= 0 && rho <= shift) {
    // cout << "✅ " <<  " sigma= (" << i << " - " << repeat.index << " + 1) /" << repeat.period << " = " << sigma << endl;
    // cout << "✅ " <<  " rho= (" << i << " - " << repeat.index << " + 1) %" << repeat.period << " = " << rho << endl;
    return true;
  }
  // cout << "❌ " <<  " sigma= (" << i << " - " << repeat.index << " + 1) /" << repeat.period << " = " << sigma << endl;
  // cout << "❌ " <<  " rho= (" << i << " - " << repeat.index << " + 1) %" << repeat.period << " = " << rho << endl;
  return false;

} // Dynamic::sigmaRho

//******************************************************************************

void Dynamic::dynamicCover(int& best_cover) {
  int length_S = S.length();
  int sigma = 5;
  int prev_size;
  int cover;
  // int best_cover = -1;

  // int size[length_S];
  std::vector<int> size(length_S);
  size[0] = 0;

  for (int i = 1; i < length_S; i++) {
    // cout << "*********************" << S.substr(0, i+ 1) << "*********************" << endl;
    for (const auto& repeat : repeats) {
      // cout << "_____" << S.substr(repeat.index, repeat.period * repeat.count) << "_____" << endl;
      // sig = sigmaCalc(sigma, i, repeat);
      // rhoo = rhoCalc(i, repeat);

      if (sigmaRho(sigma, i, repeat)) {
        for (int l = 2; l <= sigma; l++) {
          if (i + l * repeat.period < 0) {
            prev_size = 0;
          }
          else {
            prev_size = size[i - l * repeat.period];
            // cout << l << " size[" << i - l * repeat.period << "] = " << prev_size << endl;
          }
          cover = prev_size + l * repeat.period;
          // cout << "cover: " << cover << endl;
          if (cover > best_cover) {
            best_cover = cover;
          }
        }
      }
    }
    // cout << "vorige cover: " << size[i-1] << endl;
    size[i] = max(size[i-1], best_cover);
    // cout << "nieuw cover: " << size[i] << endl;
  }
  // for (int val : size) {
  //   cout << val << " ";
  // }
  // cout << endl;
  best_cover = size[length_S - 1];
} // Dynamic::dynamicCover






