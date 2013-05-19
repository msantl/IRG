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
