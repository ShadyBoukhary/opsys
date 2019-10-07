#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>

using std::cin;
using std::cout;

void _tmain(int argc, TCHAR *argv[]) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  WCHAR cmd[80];
  while (true) {
    wscanf_s(L"%s", cmd, 80);
    int x, y;
    if (wcscmp(cmd, L"+") == 0) {
      cin >> x >> y;
      cout << x + y << "\n";
    } else if (wcscmp(cmd, L"-") == 0) {
      cin >> x >> y;
      cout << x - y << "\n";
    } else if (wcscmp(cmd, L"%") == 0) {
      return;
    } else {
      // Start the child process.
      if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si,
                         &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
      }

      WaitForSingleObject(pi.hProcess, INFINITE);
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }
  }
}