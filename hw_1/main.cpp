#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
int main() {
    char ch;
    int pos;
    int in = open("../data.dat", O_RDONLY);

    while (std::cin >> ch && ch != '%') {
      if (ch == 'P') {
        std::cin >> pos;
        for (int i = 0; i < 4; i++) {
            if (!fork()) {
                lseek(in, pos + i, SEEK_SET);
                char ch;
                read(in, &ch, 1);
                lseek(in, 0, SEEK_SET);
                std::cout << ch;
                std::cout.flush();
                if (i == 3) {
                  std::cout << "\n";
                }
                return 0;
            }
        }
      }
    }
    return 0;
}
