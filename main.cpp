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

int main(){
    return 0;
}

void addTask() {
    if(totalTasks >= MAX_TASKS) { cout << "Max tasks reached!\n"; return; }
    cout << "Task Name: "; cin.getline(tasks[totalTasks].name, STR_LEN);
    cout << "Start Time (e.g., 930): "; cin >> tasks[totalTasks].start;
    cout << "Duration (min): "; cin >> tasks[totalTasks].duration;
    cout << "Priority (1=High, 2=Med, 3=Low): "; cin >> tasks[totalTasks].priority;
    cin.ignore();
    calculateEndTimes();
    totalTasks++;
    cout << "Task added!\n";
}

// 2. Calculate end times
void calculateEndTimes() {
    for(int i = 0; i < totalTasks; i++) {
        int h = tasks[i].start / 100;
        int m = tasks[i].start % 100 + tasks[i].duration;
        h += m / 60;
        m %= 60;
        tasks[i].end = h * 100 + m;
    }
}