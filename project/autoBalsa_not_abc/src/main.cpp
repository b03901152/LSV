#include <bits / stdc++.h>

int main(int argc, char* argv[]) {
  ifstream ifs;
  ifs.open(inputPath.c_str(), ifstream::in);
  if (!ifs.is_open()) {
    cout << "Error occur when opening file " << (string)inputPath << endl;
    exit(1);
  }
}