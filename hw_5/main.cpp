// Shady Boukhary
// Arshia Clare
// Intro to Operating Systems 4103
// HW 4-5 Paging Simulation
// Does what the title above suggests

#include <stdint.h>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

const uint8_t PAGE_NUM = 16;

struct Ipt {
  int32_t frames[PAGE_NUM];
  uint8_t validBits[PAGE_NUM];
  uint8_t modifiedBits[PAGE_NUM];
  uint8_t referenceBits[PAGE_NUM];
  uint8_t ops[PAGE_NUM];
};

int thirdChancePageFaults(string file, Ipt &ipt);
void thirdChanceReplacement(int address, int op, Ipt &ipt);
inline int findPage(int address, const Ipt &ipt);
void printPages(const Ipt &ipt);
void pushBack(Ipt &ipt, int32_t current[4]);
void insertPageToFrame(int address, Ipt &ipt, int op);
inline bool frameAvailable(const Ipt &ipt);
void swapPages(Ipt &ipt, int32_t address, uint8_t op);
void removePage(Ipt &ipt, int i);
void printRow(const Ipt &ipt, int counter, int op);
void printTable(const Ipt &ipt);
int thirdChancePageFaults(string file, Ipt &ipt);

int main() {
  Ipt ipt;
  // set frames to -1 and everything else to 0
  memset(ipt.frames, -1, PAGE_NUM * sizeof(int32_t));
  memset(ipt.validBits, 0, PAGE_NUM * sizeof(uint8_t));
  memset(ipt.modifiedBits, 0, PAGE_NUM * sizeof(uint8_t));
  memset(ipt.referenceBits, 0, PAGE_NUM * sizeof(uint8_t));
  int32_t faults = thirdChancePageFaults("../data.dat", ipt);
  cout << "Faults " << faults;
  return 0;
}

void thirdChanceReplacement(int address, int op, Ipt &ipt) {
  // Insert to available frame
  if (frameAvailable(ipt)) {
    insertPageToFrame(address, ipt, op);
    if (op == 1) {
      ipt.modifiedBits[PAGE_NUM - 1] = 1;
    }
  } else {
    // Replacement algorithm
    if (ipt.referenceBits[0] == 1) {
      swapPages(ipt, address, op);
      ipt.referenceBits[PAGE_NUM - 1] = 0;
    } else if (ipt.modifiedBits[0] == 1) {
      swapPages(ipt, address, op);
      ipt.modifiedBits[PAGE_NUM - 1] = 0;
    } else {
      // Remove the page with mod=0 and ref=0
      removePage(ipt, 0);
      insertPageToFrame(address, ipt, op);
      if (op == 1) {
        ipt.modifiedBits[PAGE_NUM - 1] = 1;
      }
    }
  }
}

// linear search :(
inline int findPage(int address, const Ipt &ipt) {
  for (int i = 0; i < PAGE_NUM; i++) {
    if (ipt.frames[i] == address) return i;
  }
  return -1;
}

void printPages(const Ipt &ipt) {
  for (int i = 0; i < PAGE_NUM; i++) {
    cout << ipt.frames[i] << " ";
  }
  cout << endl;
}

// fake queue
void pushBack(Ipt &ipt, int32_t current[4]) {
  int next[4] = {0};

  for (int i = PAGE_NUM - 1; i >= 0; i--) {
    // frame
    next[0] = ipt.frames[i];
    ipt.frames[i] = current[0];
    current[0] = next[0];

    // modification bit
    next[1] = ipt.modifiedBits[i];
    ipt.modifiedBits[i] = current[1];
    current[1] = next[1];

    // valid bit
    next[2] = ipt.validBits[i];
    ipt.validBits[i] = current[2];
    current[2] = next[2];

    // operation
    next[3] = ipt.ops[i];
    ipt.ops[i] = current[3];
    current[3] = next[3];
  }
}

void insertPageToFrame(int address, Ipt &ipt, int op) {
  int32_t current[4]{-1, 0, -1, 0};
  pushBack(ipt, current);
  // reset all reference bits to 0
  memset(ipt.referenceBits, 0, sizeof(uint8_t) * PAGE_NUM);
  ipt.frames[PAGE_NUM - 1] = address;
  ipt.referenceBits[PAGE_NUM - 1] = 1;
  ipt.validBits[PAGE_NUM - 1] = 1;
  if (op == 1) {
    ipt.modifiedBits[PAGE_NUM - 1] = 1;
  }
}

inline bool frameAvailable(const Ipt &ipt) { return ipt.frames[0] == -1; }

void swapPages(Ipt &ipt, int32_t address, uint8_t op) {
  int addr = address;
  uint8_t mod = 0;
  uint8_t val = 1;
  int current[4]{addr, mod, val, 0};
  // insert in the back
  pushBack(ipt, current);
  ipt.referenceBits[PAGE_NUM - 1] = 1;
  if (op == 1) {
    ipt.modifiedBits[PAGE_NUM - 1] = 1;
  }
}

void removePage(Ipt &ipt, int i) {
  ipt.frames[i] = -1;
  ipt.modifiedBits[i] = 0;
  ipt.validBits[i] = 0;
  ipt.referenceBits[i] = 0;
  ipt.ops[i] = 0;
}

int thirdChancePageFaults(string file, Ipt &ipt) {
  ifstream infile(file);
  uint64_t faults = 0;
  uint64_t counter = 0;
  int operation;
  int address;

  // read and simulate paging
  while (infile >> operation) {
    infile >> hex >> address;
    address = address >> 12;
    int i = findPage(address, ipt);
    // if page exists, no page fault
    if (i > -1) {
      ipt.referenceBits[i] = 1;
      ipt.ops[i] = operation;
      if (operation == 1) {
        ipt.modifiedBits[i] = 1;
      }
      // not in memory
    } else {
      thirdChanceReplacement(address, i, ipt);
      ipt.ops[PAGE_NUM - 1] = operation;
      faults++;
    }
    ++counter;
  }
  printTable(ipt);
  return faults;
}

void printTable(const Ipt &ipt) {
  cout << right << setw(25) << "Time (data record number) " << setw(10)
       << "Data code" << setw(18) << "Data address" << setw(20) << "Comments\n";
  cout << string(92, '-');
  cout << "\n";
  for (int i = 0; i < PAGE_NUM; ++i) {
    printRow(ipt, i, ipt.ops[i]);
  }
}

void printRow(const Ipt &ipt, int counter, int op) {
  char comment[100];
  sprintf(comment, "(Ref: %i) (Mod: %i) (Valid: %d)",
          ipt.referenceBits[counter], ipt.modifiedBits[counter],
          ipt.validBits[counter]);

  cout << left << setw(27) << counter << setw(15) << op << setw(22)
       << ipt.frames[counter] << comment << "\n";
}
