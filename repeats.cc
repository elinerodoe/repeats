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
  int length;
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


// get all possible covers with shift
void shiftIntervals(vector<vector<Interval>>& covers, int shift, int start, int end, int pmr, int length) {
  for (int i = 0; i <= shift; i++) {
    covers[pmr].push_back({start + i, end + i, length});
  }
}


// get all possible covers
vector<vector<Interval>> getInterval(const string S){
  vector<vector<Interval>>covers;
  int start;
  int end;
  int shift;
  int length;
  int end_full_repeat;

  vector<Repeat> repeats = findRepeat(S);

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
  return covers;
}


// find all possible combinations
void findCover(vector<vector<Interval>> & covers, int depth, int prev_end, int &best_cover, int cover_size, vector<Interval>& current, vector<Interval>& best_combination) {
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
      findCover(covers, depth + 1, cover.end, best_cover, cover_size + cover.length, current, best_combination);
      current.pop_back();
    }
  }
  findCover(covers, depth + 1, prev_end, best_cover, cover_size, current, best_combination);
}


// get maximal cover of string S with brute force
int maximalCover(const string S) {
  int best_cover = 0;
  vector<Interval> current;
  vector<Interval> best_combination;

  vector<vector<Interval>> covers = getInterval(S);

  // print all possible covers
  for (size_t i = 0; i < covers.size(); ++i) {
    cout << "pmr " << i << ": ";
    for (const auto& cover : covers[i]) {
        cout << "[" << cover.start << "," << cover.end << "," << cover.length << "] ";
    }
    cout << "\n";
  }

  findCover(covers, 0, -1, best_cover, 0, current, best_combination);

  // print maximal cover
  cout << "best cover: " << best_cover << " --> ";
  for (const auto& i : best_combination) {
    cout << "[" << i.start << "," << i.end << "] ";
  }
  return best_cover; 
}



int main(int argc, char* argv[]) {
  vector<Repeat> repeats;
  if (argc < 2) {
    cerr << "Usage: ./repeats <word>\n";
    return 1;
  }
  string S = argv[1];
  // repeats = findRepeat(S); // print pmrs

  cout << maximalCover(S) << endl;

  return 0;
}


