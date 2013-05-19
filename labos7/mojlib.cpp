#include "mojlib.h"
using namespace std;

vector< string > split(string s,string delim) {
  vector< string > result;

  string curr;

  for (int i = 0; i < (int)s.length(); ++i) {
    if (s.substr(i, delim.length()) == delim) {
      if ((int)curr.length()) {
        result.push_back(curr);
      }

      curr = "";
      i += delim.length() - 1;

    } else {
      curr += s.at(i);
    }
  }

  if ((int)curr.length()) {
    result.push_back(curr);
  }

  return result;
}

double nCr(int n, int k) {
  if (k > n) return 0;
  if (k * 2 > n) return nCr(n, n - k);
  if (k == 0) return 1;

  double res = n;
  for (int i = 2; i <= k; ++i) {
    res *= (n - i + 1);
    res /= i;
  }
  return res;
}


