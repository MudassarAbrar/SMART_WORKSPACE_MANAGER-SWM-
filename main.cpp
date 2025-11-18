#include "project.h"
#include <iostream>

using namespace std;

void showMainMenu();
void handleEmployeeManagement();
void handleDepartmentManagement();
void handleMeetingManagement();

// Ye chhota sa helper sirf int input safely lene ke liye hai
static int readInt(const char* prompt)
{
    int value;
    cout << prompt;
    cin >> value;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        return -1;
    }

    cin.ignore(1000, '\n');
    return value;
}

void showMainMenu()
{
    cout << "--------------------------------------------\n";
    cout << "                MAIN MENU                   \n";
    cout << "--------------------------------------------\n";
    cout << " 1. Employee Management\n";
    cout << " 2. Department Management\n";
    cout << " 3. Meeting Management\n";
    cout << " 4. Exit\n";
    cout << "--------------------------------------------\n";
}

void handleEmployeeManagement()
{
    bool back = false;

    while (!back)
    {
        cout << "\n--- Employee Management ---\n";
        cout << " 1. Add Employee\n";
        cout << " 2. Remove Employee\n";
        cout << " 3. Mark Attendance\n";
        cout << " 4. Update Performance\n";
        cout << " 5. Calculate Bonus\n";
        cout << " 6. Display Employee\n";
        cout << " 0. Back\n";
        cout << "Select option: ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice)
        {
        case 1:
            addEmployee();
            break;

        case 2:
        {
            int empId = readInt("Enter Employee ID to remove: ");
            if (empId != -1)
                removeEmployee(empId);
            break;
        }

        case 3:
        {
            int empId = readInt("Enter Employee ID to mark attendance: ");
            if (empId != -1)
                markAttendance(empId);
            break;
        }

        case 4:
        {
            int empId = readInt("Enter Employee ID to update performance: ");
            if (empId == -1)
                break;

            double score;
            cout << "Enter new performance score (0 - 100): ";
            cin >> score;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid score.\n";
                break;
            }
            cin.ignore(1000, '\n');

            updatePerformance(empId, score);
            break;
        }

        case 5:
        {
            int empId = readInt("Enter Employee ID to calculate bonus: ");
            if (empId != -1)
                calculateBonus(empId);
            break;
        }

        case 6:
        {
            int empId = readInt("Enter Employee ID to display: ");
            if (empId != -1)
                displayEmployee(empId);
            break;
        }

        case 0:
            back = true;
            break;

        default:
            cout << "Invalid option.\n";
            break;
        }
    }
}

void handleDepartmentManagement()
{
    bool back = false;

    while (!back)
    {
        cout << "\n--- Department Management ---\n";
        cout << " 1. Add Department\n";
        cout << " 2. Remove Department\n";
        cout << " 3. Display Department\n";
        cout << " 4. Find Top Performer\n";
        cout << " 5. Department Average Performance\n";
        cout << " 6. Check Department Capacity\n";
        cout << " 0. Back\n";
        cout << "Select option: ";

        int choice;
        cin >> choice;

        if (cin.fail()) // cin.fail() check karega input galat to nahi
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        char deptName[30];

        switch (choice)
        {
        case 1:
        {
            cout << "Enter Department Name: ";
            cin.getline(deptName, 30);

            int capacity = readInt("Enter Maximum Capacity: ");
            if (capacity > 0)
                addDepartment(deptName, capacity);
            else
                cout << "Invalid capacity.\n";

            break;
        }

        case 2:
            cout << "Enter Department Name to remove: ";
            cin.getline(deptName, 30);
            removeDepartment(deptName);
            break;

        case 3:
            cout << "Enter Department Name to display: ";
            cin.getline(deptName, 30);
            displayDepartment(deptName);
            break;

        case 4:
        {
            cout << "Enter Department Name to find top performer: ";
            cin.getline(deptName, 30);

            int topId = findTopPerformer(deptName);
            if (topId == -1)
                cout << "No employees found or department missing.\n";
            else
                cout << "Top performer ID: " << topId << '\n';

            break;
        }

        case 5:
            cout << "Enter Department Name to get average performance: ";
            cin.getline(deptName, 30);
            cout << "Average performance: " << getDeptAvgPerformance(deptName) << '\n';
            break;

        case 6:
            cout << "Enter Department Name to check capacity: ";
            cin.getline(deptName, 30);
            displayDeptCapacity(deptName);
            break;

        case 0:
            back = true;
            break;

        default:
            cout << "Invalid option.\n";
            break;
        }
    }
}

void handleMeetingManagement()
{
    bool back = false;

    while (!back)
    {
        cout << "\n--- Meeting Management ---\n";
        cout << " 1. Add Meeting\n";
        cout << " 2. Add Participant\n";
        cout << " 3. Display Meeting\n";
        cout << " 0. Back\n";
        cout << "Select option: ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice)
        {
        case 1:
            // addMeeting already time clash check karta hai meeting.cpp mein
            addMeeting();
            break;

        case 2:
        {
            int meetId = readInt("Enter meeting ID: ");
            int empId  = readInt("Enter employee ID to add as participant: ");

            if (meetId >= 0 && empId >= 0)
            {
                // Yahan pe hum ID se index dhoondh rahe hain
                int meetIdx = -1;
                for (int i = 0; i < meeting_count; i++)
                {
                    if (meetings_list[i].meet_id == meetId)
                    {
                        meetIdx = i;
                        break;
                    }
                }
                // Agar meeting nahi milti, meetIdx -1 rahega, function handle karega
                addParticipant(meetIdx, empId);
            }
            break;
        }

        case 3:
        {
            int meetId = readInt("Enter meeting ID to display: ");
            if (meetId >= 0)
            {
                int meetIdx = -1;
                for (int i = 0; i < meeting_count; i++)
                {
                    if (meetings_list[i].meet_id == meetId)
                    {
                        meetIdx = i;
                        break;
                    }
                }
                displayMeeting(meetIdx);
            }
            break;
        }

        case 0:
            back = true;
            break;

        default:
            cout << "Invalid option.\n";
            break;
        }
    }
}

int main()
{
    // Sabse pehle dynamic memory allocate karwa lete hain
    initializeMemory();

    // File se data load karo (agar files existing hain to)
    loadEmployeesFromText("employee.txt");
    loadDepartmentsFromText("departments.txt");
    loadMeetingsFromText("meetings.txt");

    cout << "============================================\n";
    cout << "      SMART WORKSPACE MANAGER \n";
    cout << "============================================\n";

    bool running = true;

    while (running)
    {
        showMainMenu();
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice)
        {
        case 1:
            handleEmployeeManagement();
            break;
        case 2:
            handleDepartmentManagement();
            break;
        case 3:
            handleMeetingManagement();
            break;
        case 4:
            running = false;
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
            break;
        }

        cout << '\n';
    }

    // Program band hone se pehle data save karwa lo
    saveEmployeesToText("employee.txt");
    saveDepartmentsToText("departments.txt");
    saveMeetingsToText("meetings.txt");

    // Saari dynamic memory free
    freeMemory();

    return 0;
}
