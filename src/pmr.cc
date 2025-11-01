#include "../inc/pmr.h"
#include "../inc/types.h"

 #include <iostream>
 #include <vector>
 #include <string>
 #include <cmath>
 #include <algorithm>
 #include <chrono> // voor timing
using namespace std;

bool PMR::isRotation(const std::vector<Repeat>& repeats, const int i, const int j) {
  for (const Repeat& r : repeats) {
    if (r.shift != 0 && r.period == j && r.index < i && (r.index + r.shift) >= i) {
      return true;
    }
  }
  return false;
} // PMR::isRotation

//******************************************************************************

bool PMR::isPrimitive (const std::vector<Repeat>& repeats, 
                      const int i, 
                      const int j, 
                      const int count) {
  for (const Repeat& r : repeats) {
    if (
        r.index <= i && 
        r.index + (r.period * r.count) > i && 
        r.index + (r.period * r.count) + r.shift >= i + (j * count) && 
        r.period <= j
      ) {
      return false;
    }
  }
  return true;
} // PMR::isPrimitive

//******************************************************************************

vector<Repeat> PMR::findRepeat(const string S) {
  using namespace std::chrono;
  auto time_a = high_resolution_clock::now();

  int max_period;
  int count;
  int shift;
  string unit;
  int length_S = S.length();

  for (int i = 0; i < length_S; i++) {
    max_period = (length_S - i) / 2;
    for (int j = 1; j <= max_period; j++) {
      // checks rotation
      if (isRotation(repeats, i, j)){
        continue;
      }
      unit = S.substr(i, j);
      count = 1;
      // checks how many times substring is repeated
      while ((i + j + (count * j) <= length_S) && (S.substr(i + (count * j), j)) == unit) {
        count++;
      }
      shift = 0;
      if (count  > 1) {
        // check primitive
        if (!isPrimitive(repeats, i, j, count)){
          continue;
        }
        // calculate shift
        
        while (((i + (count * j) + shift + 1) <= length_S) && (S[i + (count * j) + shift] == S[i + shift])) {
          shift++;
        }
        repeats.push_back({i, j, count, shift});
        // cout << "(" << i << "," << j << "," << count << "," << shift << ") ";
      }
    }
  }

  auto time_b = chrono::high_resolution_clock::now();
  duration<double> elapsed = time_b - time_a;
  cout << "timer findRepeat(): " << elapsed.count() << " seconds\n";

  return repeats;
} // PMR::findRepeat

//******************************************************************************

void PMR::shiftIntervals(vector<vector<Interval>>& covers, 
                        int shift, 
                        int start, 
                        int end, 
                        int pmr, 
                        int length) {
  for (int i = 0; i <= shift; i++) {
    covers[pmr].push_back({start + i, end + i, length, pmr});
  }
} // PMR::shiftIntervals

//******************************************************************************

vector<vector<Interval>> PMR::getInterval(const vector<Repeat> repeats){
  using namespace std::chrono;
  auto time_a = high_resolution_clock::now();

  vector<vector<Interval>>covers;
  int start;
  int end;
  int shift;
  int length;
  int end_full_repeat;

  // vector<Repeat> repeats = findRepeat(S);

  // loop through all repeats
  for (long unsigned int i = 0; i < repeats.size(); i++) {
    covers.push_back({});
    const auto& repeat = repeats[i];

    if (repeat.count <= 2) {
      start = repeat.index;
      length = repeat.period * repeat.count; 
      end = start + length - 1;
      shift = repeat.shift;
      
      shiftIntervals(covers, shift, start, end, i, length);
    }
    else {
      end_full_repeat = repeat.index + (repeat.period * repeat.count) - 1;
      for (int k = 0; k <= repeat.count - 2; k++) {
        for (int t = 2; t <= repeat.count - k; t++) {
          start = repeat.index + (k * repeat.period);
          end = repeat.index + (k + t) * repeat.period - 1;
          length = end - start + 1;

          if (end_full_repeat == end) {
            shift = repeat.shift;
            shiftIntervals(covers, shift, start, end, i, length);
          }
          else {
            shift = min(repeat.period - 1, end_full_repeat - end);
            shiftIntervals(covers, shift, start, end, i, length);
          }
        }
      }
    }
  }
  auto time_b = chrono::high_resolution_clock::now();
  duration<double> elapsed = time_b - time_a;
  cout << "timer getInterval(): " << elapsed.count() << " seconds\n";

  return covers;
} // PMR::getInterval