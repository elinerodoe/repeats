 #include <iostream>
 #include <vector>
 #include <string>
using namespace std;


// annotation struct
struct Repeat {
  int index;  // starting position of repeat
  int period; // length of repeat
  int count;  // number of repeats
  int shift;  // prefix of maximal repeat
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
void findRepeat(const string S) {
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
}


int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Usage: ./repeats <word>\n";
    return 1;
  }
  string S = argv[1];
  findRepeat(S);
  return 0;
}


