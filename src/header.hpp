// #ifndef HEADER
// #define HEADER
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <algorithm>
using namespace std;

typedef vector<pair<int, int>> RservInterval;
typedef map<string, map<int, shared_ptr<RservInterval>>> Reservation;
class System;

// #endif