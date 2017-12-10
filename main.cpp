#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
  ofstream out("test", ofstream::out);
  out << "c\n";
  out << "c\n";
  out << "c\n";
  int n = 6;
  int nbvar = n * (n - 1);
  int nbclauses = 0;
  nbclauses += n+1;
  nbclauses += (n+1)*n*(n-1) / 2;
  nbclauses += (n)*n*(n+1) / 2;
  out << "p cnf " << nbvar << " " << nbclauses << endl;
  auto x = [&](int i, int j) {
    return n * i + j + 1;
  };
  // c1 
  for (int i = 0; i <= n; ++ i) {
    for (int j = 0; j < n; ++j)
      out << x(i,j) << " ";
    out << "0" << endl;
  }
  // c2 
  for (int i = 0; i <= n; ++ i)
    for (int j = 0; j < n; ++j)
      for (int _j = j + 1; _j < n; ++_j) {
        out << "-" << x(i,j) << " -" << x(i,_j) << " 0\n";
      }
  
  // c3 
  for (int i = 0; i <= n; ++ i)
    for (int _i = i + 1; _i <= n; ++_i)
      for (int j = 0; j < n; ++j) {
        out << "-" << x(i,j) << " -" << x(_i,j) << " 0\n";
      }
  out.close();
  return 0;
}