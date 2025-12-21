#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <ctime>       
using namespace std;

const int MAX_TASKS = 100;
const int STR_LEN   = 105;


char taskNames[MAX_TASKS][STR_LEN];
int taskStarts[MAX_TASKS];
int taskDurations[MAX_TASKS];
int taskEnds[MAX_TASKS];
int taskPriorities[MAX_TASKS];
int totalTasks = 0;

void addTask();
void calculateEndTimes();
bool hasConflict(int i, int j);
void detectConflicts();
void sortByPriority();
void sortByDuration();
void swapTasks(int i, int j);
void displaySchedule();
void saveToFile();
void loadFromFile();
void adminClearAll();
void adminExportWithTimestamp();
int findTaskRecursively(char target[], int low, int high);
void printTime(int t);


int main(){
    cout << "==========================================\n";
    cout << "   DAILY TASK SCHEDULER - CS-101 F25     \n";
    cout << "==========================================\n";

    int choice;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Add Task\n";
        cout << "2. Detect Conflicts\n";
        cout << "3. Optimize by Priority\n";
        cout << "4. Optimize by Shortest Duration\n";
        cout << "5. Display Schedule\n";
        cout << "6. Save Schedule\n";
        cout << "7. Load Schedule\n";
        cout << "8. Search Task by Name (Recursive)\n";
        cout << "9. Admin: Clear All Tasks\n";
        cout << "10. Admin: Export Schedule with Timestamp\n";
        cout << "11. Exit\n";
        cout << "Choice: ";         
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: addTask(); break;
            case 2: detectConflicts(); break;
            case 3: sortByPriority(); cout << "Sorted by Priority!\n"; break;
            case 4: sortByDuration(); cout << "Sorted by Duration!\n"; break;
            case 5: displaySchedule(); break;
            case 6: saveToFile(); break;
            case 7: loadFromFile(); break;
            case 8: {
                if(totalTasks == 0) { cout << "No tasks!\n"; break; }
                // Sort names alphabetically first (bubble sort)
                for(int i = 0; i < totalTasks-1; i++)
                    for(int j = 0; j < totalTasks-i-1; j++)
                        if(strcmp(taskNames[j], taskNames[j+1]) > 0)
                            swapTasks(j, j+1);

                char search[STR_LEN];
                cout << "Enter task name to search: ";
                cin.getline(search, STR_LEN);

                int idx = findTaskRecursively(search, 0, totalTasks-1);
                if(idx != -1) {
                    cout << "Found at position " << idx+1 << "\n";
                    printTime(taskStarts[idx]); cout << " - "; printTime(taskEnds[idx]);
                    cout << " | P" << taskPriorities[idx] << " | " << taskNames[idx] << endl;
                } else cout << "Task not found!\n";
                break;
            }
            case 9: adminClearAll(); break;
            case 10: adminExportWithTimestamp(); break;
            case 11: cout << "Thank you!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 11);

    return 0;
}



void addTask() {
    if(totalTasks >= MAX_TASKS) { cout << "Max tasks reached!\n"; return; }
    cout << "Task Name: "; cin.getline(taskNames[totalTasks], STR_LEN);
    cout << "Start Time (e.g., 930): "; cin >> taskStarts[totalTasks];
    cout << "Duration (min): "; cin >> taskDurations[totalTasks];
    cout << "Priority (1=High, 2=Med, 3=Low): "; cin >> taskPriorities[totalTasks];
    cin.ignore();
    calculateEndTimes();
    totalTasks++;
    cout << "Task added!\n";
}


void calculateEndTimes() {
    for(int i = 0; i < totalTasks; i++) {
        int h = taskStarts[i] / 100;
        int m = taskStarts[i] % 100 + taskDurations[i];
        h += m / 60;
        m %= 60;
        taskEnds[i] = h * 100 + m;
    }
}


bool hasConflict(int i, int j) {
    return (taskStarts[i] < taskEnds[j] && taskEnds[i] > taskStarts[j]);
}


void detectConflicts() {
    cout << "\n=== CONFLICTS ===\n";
    bool found = false;
    for(int i = 0; i < totalTasks; i++)
        for(int j = i+1; j < totalTasks; j++)
            if(hasConflict(i, j)) {
                found = true;
                cout << taskNames[i] << " overlaps with " << taskNames[j] << "\n";
                printTime(taskStarts[i]); cout << " - "; printTime(taskEnds[i]);
                cout << "  vs  "; printTime(taskStarts[j]); cout << " - "; printTime(taskEnds[j]);
                cout << "\n";
            }
    if(!found) cout << "No conflicts!\n";
}


void sortByPriority() {
    calculateEndTimes();
    for(int i = 0; i < totalTasks-1; i++) {
        int best = i;
        for(int j = i+1; j < totalTasks; j++)
            if(taskPriorities[j] < taskPriorities[best] ||
               (taskPriorities[j] == taskPriorities[best] && taskDurations[j] < taskDurations[best]))
                best = j;
        if(best != i) swapTasks(i, best);
    }
}


void sortByDuration() {
    calculateEndTimes();
    for(int i = 0; i < totalTasks-1; i++) {
        int best = i;
        for(int j = i+1; j < totalTasks; j++)
            if(taskDurations[j] < taskDurations[best])
                best = j;
        if(best != i) swapTasks(i, best);
    }
}

void swapTasks(int i, int j) {
    // Swap names
    char tempName[STR_LEN];
    strcpy(tempName, taskNames[i]);
    strcpy(taskNames[i], taskNames[j]);
    strcpy(taskNames[j], tempName);


    int *p1, *p2, temp;

    p1 = &taskStarts[i]; p2 = &taskStarts[j];
    temp = *p1; *p1 = *p2; *p2 = temp;

    p1 = &taskDurations[i]; p2 = &taskDurations[j];
    temp = *p1; *p1 = *p2; *p2 = temp;

    p1 = &taskEnds[i]; p2 = &taskEnds[j];
    temp = *p1; *p1 = *p2; *p2 = temp;

    p1 = &taskPriorities[i]; p2 = &taskPriorities[j];
    temp = *p1; *p1 = *p2; *p2 = temp;
}


void displaySchedule() {
    if(totalTasks == 0) { cout << "No tasks!\n"; return; }
    cout << "\n========================================\n";
    cout << "        DAILY SCHEDULE                  \n";
    cout << "========================================\n";
    for(int i = 0; i < totalTasks; i++) {
        cout << i+1 << ". ";
        printTime(taskStarts[i]); cout << " - "; printTime(taskEnds[i]);
        cout << " | " << setw(3) << taskDurations[i] << " min | P" << taskPriorities[i];
        cout << " | " << taskNames[i] << "\n";
    }
}


void printTime(int t) {
    cout << setfill('0') << setw(2) << t/100 << ":" << setw(2) << t%100;
}

void saveToFile() {
    ofstream f("schedule.dat");
    f << totalTasks << "\n";
    for(int i = 0; i < totalTasks; i++) {
        f << taskNames[i] << "\n";
        f << taskStarts[i] << " " << taskDurations[i] << " " << taskPriorities[i] << "\n";
    }
    f.close();
    cout << "Saved!\n";
}

void loadFromFile() {
    ifstream f("schedule.dat");
    if(!f) { cout << "No saved file!\n"; return; }
    f >> totalTasks; f.ignore();
    for(int i = 0; i < totalTasks; i++) {
        f.getline(taskNames[i], STR_LEN);
        f >> taskStarts[i] >> taskDurations[i] >> taskPriorities[i];
        f.ignore();
    }
    f.close();
    calculateEndTimes();
    cout << "Loaded " << totalTasks << " tasks!\n";
}


void adminClearAll() {
    cout << "ADMIN: All tasks cleared!\n";
    totalTasks = 0;
}


void adminExportWithTimestamp() {
    time_t now = time(0);
    char filename[100];
    strftime(filename, sizeof(filename), "Schedule_%Y%m%d_%H%M%S.txt", localtime(&now));
    ofstream f(filename);
    f << "Daily Schedule Export - " << ctime(&now) << "\n";
    for(int i = 0; i < totalTasks; i++) {
        f << i+1 << ". " << taskStarts[i] << " - " << taskEnds[i] << " | ";
        f << taskDurations[i] << " min | P" << taskPriorities[i] << " | ";
        f << taskNames[i] << "\n";
    }
    f.close();
    cout << "Exported to " << filename << "\n";
}


int findTaskRecursively(char target[], int low, int high) {
    if(low > high) return -1;                          // Base case
    int mid = low + (high - low) / 2;
    if(strcmp(taskNames[mid], target) == 0) return mid; // Found
    if(strcmp(taskNames[mid], target) < 0)
        return findTaskRecursively(target, mid + 1, high);
    return findTaskRecursively(target, low, mid - 1);
}
