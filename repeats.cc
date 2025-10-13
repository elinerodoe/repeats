 #include <iostream>
 #include <vector>
 #include <string>
 #include <cmath>
using namespace std;


// annotation struct
struct Repeat {
  int index;  // starting position of repeat
  int period; // length of repeat
  int count;  // number of repeats
  int shift;  // prefix of maximal repeat
};


// ...
struct Interval {
  int start;
  int end; 
};


// checks if repeat is a rotation of another repeat
bool isRotation(const std::vector<Repeat>& repeats, const int i, const int j) {
  for (const Repeat& r : repeats) {
    if (r.shift != 0 && r.period == j && r.index < i && (r.index + r.shift) >= i) {
      return true;
    }
  }
  return false;
}

// checks if repeat is primitive
bool isPrimitive (const std::vector<Repeat>& repeats, const int i, const int j, const int count) {
  for (const Repeat& r : repeats) {
    if (r.index <= i && r.index + (r.period * r.count) > i && r.index + (r.period * r.count) + r.shift >= i + (j * count) && r.period <= j) {
      return false;
    }
  }
  return true;
}

// find all primitive maximal repeats in string S
vector<Repeat> findRepeat(const string S) {
  vector <Repeat> repeats;
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
        cout << "(" << i << "," << j << "," << count << "," << shift << ") ";
      }
    }
  }
  return repeats;
}



void shiftIntervals(vector<Interval>& intervals, int shift, int start, int end) {
  int new_start = 0;
  int new_end = 0;
  for (int i = 0; i <= shift; i++) {
    new_start = start + i;
    new_end = end + i;
    intervals.push_back({new_start, new_end});
  }
}


// get all intervals
vector<Interval> getInterval(const string S){
  vector<Interval> intervals;
  vector<Repeat> repeats = findRepeat(S);
  int start;
  int end;
  int end_repeat;
  int shift;

  // loop over repeats heen
  for (Repeat repeat : repeats) {
    if (repeat.count > 2) {
      end_repeat = repeat.index + (repeat.period * repeat.count) - 1;
      for (int k = 0; k <= repeat.count - 2; k++) {
        for (int t = 2; t <= repeat.count - k; t++) {
          start = repeat.index + (k * repeat.period);
          end = repeat.index + (k + t) * repeat.period - 1;
          if (end_repeat == end) {
            shift = repeat.shift;
            shiftIntervals(intervals, shift, start, end);
          }
          else {
            shift = min(repeat.period - 1, end_repeat - end);
            shiftIntervals(intervals, shift, start, end);

          }
        }
      }
    }

    start = repeat.index;
    end = start + (repeat.period * repeat.count) - 1; 
    shiftIntervals(intervals, repeat.shift, start, end);

  }
  return intervals;
}


int findCover(vector <Interval> intervals, int combination) {
  int cover_mask = 0;
  int number_cover = 0;
  int number_intervals = intervals.size();

  for (int bit = 0; bit < number_intervals; bit++){
    
    if (!(combination &(1 << bit))) { // als bit niet 1 is
      continue;
    }

    // string coveren
    for (int pos = intervals[bit].start; pos <= intervals[bit].end; pos++) {
      if (cover_mask & (1 << pos)) { // overlap
        return -1;
      }
      cover_mask |= (1 << pos); // zet digit om naar 1
      number_cover++;
    }
  } 

  return number_cover;
}


int maximalCover(const string S) {
  vector<Interval> intervals = getInterval(S);
  int number_combinations = pow(2, intervals.size());
  int number_cover;
  int best_cover = -1;
  int best_combination = -1;
  bool valid;
  
  for (int combination = 0; combination < number_combinations; combination++) {
    number_cover = findCover(intervals, combination);


    if (best_cover < number_cover) {
      best_cover = number_cover;
      best_combination = combination;
    }
  }

  return best_cover; 
}

  // print check
  // cout << "best cover: " << best_cover << endl;
  // cout << "best combination: " << best_combination << endl;

  // for (int bit = 0; bit < intervals.size(); bit++){
    
  //   if (!(best_combination &(1 << bit))) { // als bit niet 1 is
  //     continue;
  //   }
  //   cout << intervals[bit].start << " " << intervals[bit].end << endl;

  // } 






int main(int argc, char* argv[]) {
  vector<Repeat> repeats;
  if (argc < 2) {
    cerr << "Usage: ./repeats <word>\n";
    return 1;
  }
  string S = argv[1];
  // repeats = findRepeat(S);

  cout << maximalCover(S) << endl;

  return 0;
}


