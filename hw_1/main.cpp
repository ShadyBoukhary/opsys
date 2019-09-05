#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>

int main() {
  char ch;
  int pos;
  std::string str;
  std::ifstream in("../data.dat", std::ios::in);
  in >> str;
  std::cout << "Input String: " + str << "\n";

  while (std::cin >> ch && ch != '%') {
    if (ch == 'P') {
      std::cin >> pos;
      for (int i = 0; i < 4; i++) {
        if (!fork()) {
          std::cout << str[pos + i];
          std::cout.flush();
          if (i == 3) {
            std::cout << "\n";
          }
          return 0;
        }
      }
    }
  }
}
