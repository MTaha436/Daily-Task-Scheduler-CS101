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

// 3. Conflict check
bool hasConflict(int i, int j) {
    return (tasks[i].start < tasks[j].end && tasks[i].end > tasks[j].start);
}

// 4. Detect all conflicts
void detectConflicts() {
    cout << "\n=== CONFLICTS ===\n";
    bool found = false;
    for(int i = 0; i < totalTasks; i++)
        for(int j = i+1; j < totalTasks; j++)
            if(hasConflict(i, j)) {
                found = true;
                cout << tasks[i].name << " overlaps with " << tasks[j].name << "\n";
                printTime(tasks[i].start); cout << " - "; printTime(tasks[i].end);
                cout << "  vs  "; printTime(tasks[j].start); cout << " - "; printTime(tasks[j].end);
                cout << "\n";
            }
    if(!found) cout << "No conflicts!\n";
}

// 5. Priority sort (Selection Sort)
void sortByPriority() {
    calculateEndTimes();
    for(int i = 0; i < totalTasks-1; i++) {
        int best = i;
        for(int j = i+1; j < totalTasks; j++)
            if(tasks[j].priority < tasks[best].priority ||
              (tasks[j].priority == tasks[best].priority && tasks[j].duration < tasks[best].duration))
                best = j;
        if(best != i) swapTasks(&tasks[i], &tasks[best]);
    }
}

// 6. Duration sort
void sortByDuration() {
    calculateEndTimes();
    for(int i = 0; i < totalTasks-1; i++) {
        int best = i;
        for(int j = i+1; j < totalTasks; j++)
            if(tasks[j].duration < tasks[best].duration)
                best = j;
        if(best != i) swapTasks(&tasks[i], &tasks[best]);
    }
}

// 7. Pointer-based swap
void swapTasks(Task* a, Task* b) {
    Task temp = *a;
    *a = *b;
    *b = temp;
}