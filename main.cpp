#include <iostream>

#include <fstream>
#include <cstring>
#include <iomanip>
#include <ctime>        // for timestamp in export
using namespace std;

const int MAX_TASKS = 100;
const int STR_LEN   = 105;

struct Task {
    char name[STR_LEN];
    int start;      // e.g., 930 = 9:30 AM
    int duration;   // minutes
    int end;        // auto-calculated
    int priority;   // 1=High, 2=Medium, 3=Low
};

Task tasks[MAX_TASKS];
int totalTasks = 0;