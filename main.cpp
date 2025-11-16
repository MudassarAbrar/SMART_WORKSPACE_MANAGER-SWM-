#include "project.h"
#include <iostream>
using namespace std;

// Only used in this file
void showMainMenu();

int main()
{
    int choice;

    // 1) Allocate dynamic memory
    initializeMemory();

    // 2) Load data from files
    //    Employees from binary, departments & meetings from text
    loadEmployeesFromBinary("employees.bin");
    loadDepartmentsFromText("departments.txt");
    loadMeetingsFromText("meetings.txt");

    cout << "============================================\n";
    cout << "      SMART WORKSPACE MANAGER (STRUCTURED)\n";
    cout << "============================================\n";

    // Main loop
    do
    {
        showMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(1000, '\n'); // clear buffer

        if (choice == 1)
        {
            addEmployee();
        }
        else if (choice == 2)
        {
            int id;
            cout << "Enter Employee ID to remove: ";
            cin >> id;
            cin.ignore(1000, '\n');
            removeEmployee(id);
        }
        else if (choice == 3)
        {
            int id;
            cout << "Enter Employee ID to mark attendance: ";
            cin >> id;
            cin.ignore(1000, '\n');
            markAttendance(id);
        }
        else if (choice == 4)
        {
            int id;
            double score;
            cout << "Enter Employee ID to update performance: ";
            cin >> id;
            cout << "Enter new performance score (0 - 100): ";
            cin >> score;
            cin.ignore(1000, '\n');
            updatePerformance(id, score);
        }
        else if (choice == 5)
        {
            int id;
            cout << "Enter Employee ID to calculate bonus: ";
            cin >> id;
            cin.ignore(1000, '\n');
            calculateBonus(id);
        }
        else if (choice == 6)
        {
            int id;
            cout << "Enter Employee ID to display: ";
            cin >> id;
            cin.ignore(1000, '\n');
            displayEmployee(id);
        }
        else if (choice == 7)
        {
            char deptName[30];
            cout << "Enter Department Name: ";
            cin.getline(deptName, 30);
            addDepartment(deptName);
        }
        else if (choice == 8)
        {
            char deptName[30];
            cout << "Enter Department Name to remove: ";
            cin.getline(deptName, 30);
            removeDepartment(deptName);
        }
        else if (choice == 9)
        {
            char deptName[30];
            cout << "Enter Department Name to display: ";
            cin.getline(deptName, 30);
            displayDepartment(deptName);
        }
        else if (choice == 10)
        {
            char deptName[30];
            cout << "Enter Department Name to find top performer: ";
            cin.getline(deptName, 30);
            int topId = findTopPerformer(deptName);
            if (topId == -1)
            {
                cout << "No employees found in this department or department not found.\n";
            }
            else
            {
                cout << "Top performer in department " << deptName
                     << " is Employee ID: " << topId << endl;
            }
        }
        else if (choice == 11)
        {
            char deptName[30];
            cout << "Enter Department Name to get average performance: ";
            cin.getline(deptName, 30);
            double avg = getDeptAvgPerformance(deptName);
            cout << "Average performance of department " << deptName
                 << " is: " << avg << endl;
        }
        else if (choice == 12)
        {
            addMeeting();
        }
        else if (choice == 13)
        {
            int meetIndex;
            cout << "Enter meeting index to schedule (0 to " << (meetingCount - 1) << "): ";
            cin >> meetIndex;
            cin.ignore(1000, '\n');
            int ok = scheduleMeeting(meetIndex);
            if (ok == 1)
            {
                cout << "Meeting scheduled successfully (no time clash).\n";
            }
            else
            {
                cout << "Meeting clash detected. Could not schedule.\n";
            }
        }
        else if (choice == 14)
        {
            int meetIdx, empId;
            cout << "Enter meeting index: ";
            cin >> meetIdx;
            cout << "Enter employee ID to add as participant: ";
            cin >> empId;
            cin.ignore(1000, '\n');
            addParticipant(meetIdx, empId);
        }
        else if (choice == 15)
        {
            int index;
            cout << "Enter meeting index to display: ";
            cin >> index;
            cin.ignore(1000, '\n');
            displayMeeting(index);
        }
        else if (choice == 16)
        {
            int a, b;
            cout << "Enter first meeting index: ";
            cin >> a;
            cout << "Enter second meeting index: ";
            cin >> b;
            cin.ignore(1000, '\n');
            int clash = hasTimeClash(a, b);
            if (clash == 1)
            {
                cout << "The two meetings have a time clash.\n";
            }
            else
            {
                cout << "No time clash between the two meetings.\n";
            }
        }
        else if (choice == 0)
        {
            cout << "Exiting program...\n";
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }

        cout << endl;

    } while (choice != 0);

    saveEmployeesToBinary("employees.bin");
    saveDepartmentsToText("departments.txt");
    saveMeetingsToText("meetings.txt");

    freeMemory();

    return 0;
}

void showMainMenu()
{
    cout << "--------------------------------------------\n";
    cout << "                 MAIN MENU                  \n";
    cout << "--------------------------------------------\n";
    cout << " 1. Add Employee\n";
    cout << " 2. Remove Employee\n";
    cout << " 3. Mark Attendance\n";
    cout << " 4. Update Performance\n";
    cout << " 5. Calculate Bonus for Employee\n";
    cout << " 6. Display Employee\n";
    cout << " 7. Add Department\n";
    cout << " 8. Remove Department\n";
    cout << " 9. Display Department\n";
    cout << "10. Find Top Performer in Department\n";
    cout << "11. Get Department Average Performance\n";
    cout << "12. Add Meeting\n";
    cout << "13. Schedule Meeting (check for clash)\n";
    cout << "14. Add Participant to Meeting\n";
    cout << "15. Display Meeting\n";
    cout << "16. Check Time Clash Between Two Meetings\n";
    cout << " 0. Exit\n";
    cout << "--------------------------------------------\n";
}
// 1st Change  :  MAKE 4 OPTIONS 1-EMP 2-DEP 3-MARKS 4-EXIT
// 2nd change  : file handling in department in last two functions
// 3rd change  : emp me sari text file handling krni h
// optional change 4  : graph for emp and department
// 5th change  : check for meetings and rest
