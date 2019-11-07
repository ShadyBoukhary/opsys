// Shady Boukhary - HW4 - Operating Systems - FIFO paging vs Second chance
#include <time.h>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <unordered_set>
using namespace std;

int fifo(int pages[], const vector<int> &rands, int iters, int frames) {
  unordered_set<int> s;
  queue<int> indexes;
  int faults = 0;

  for (int i = 0; i < iters; i++) {
    int page = rands[i];
    // Check if the set can hold more pages
    if (s.find(page) == s.end()) {
      faults++;
      if (s.size() < frames) {
        s.insert(page);
        indexes.push(page);
      } else {
        int val = indexes.front();
        indexes.pop();
        s.erase(val);
        s.insert(page);
        indexes.push(page);
      }
    }
  }

  return faults;
}

int secondChance(int pages[], const vector<int> &rands, int iters, int frames) {
  set<int> s;
  set<int> secondChance;
  int index = 0;
  int faults = 0;

  for (int i = 0; i < iters; i++) {
    int page = rands[i];
    if (s.find(page) == s.end()) {
      // if page not found
      faults++;
      if (s.size() < frames) {
        s.insert(page);
      } else {
        bool foundReplacement = false;
        while (!foundReplacement) {
          // No second chance, found replacement
          if (secondChance.find(index) == secondChance.end()) {
            foundReplacement = true;
            int val = *std::next(s.begin(), index);
            s.erase(val);
            s.insert(page);
          } else {
            secondChance.erase(index);
          }
          index = (index + 1) % frames;
        }
      }
    } else { secondChance.insert(page); }
  }

  return faults;
}

int main() {
  int pages[100] = {1};
  const int frames = 20;
  const int iters = 10000;
  vector<int> rands(iters);
  srand(time(0));

  for (auto &val : rands) { val = rand() % 100; }
  cout << "FIFO Page faults: " << fifo(pages, rands, iters, frames) << endl;
  cout << "Second Chance Page faults: " << secondChance(pages, rands, iters, frames) << endl;
  return 0;
}