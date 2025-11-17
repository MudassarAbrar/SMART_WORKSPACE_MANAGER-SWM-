#include "project.h"
#include <iostream>
#include <fstream>

using namespace std;

static bool stringsEqual(const char* a, const char* b)
{
    int idx = 0;
    while (a[idx] != '\0' || b[idx] != '\0')
    {
        if (a[idx] != b[idx])
            return false;
        idx++;
    }
    return true;
}

static bool stringEmpty(const char* text)
{
    return text[0] == '\0';
}

static int findMeetingIndex(int meet_id)
{
    for (int i = 0; i < meeting_count; i++)
    {
        if (meetings_list[i].meet_id == meet_id)
            return i;
    }
    return -1;
}

static bool employeeExists(int emp_id)
{
    for (int i = 0; i < employee_count; i++)
    {
        if (employees_list[i].emp_id == emp_id)
            return true;
    }
    return false;
}

static bool validateDate(const char* date)
{
    // Expect format YYYY-MM-DD
    int len = 0;
    while (date[len] != '\0')
        len++;
    if (len != 10)
        return false;
    return date[4] == '-' && date[7] == '-';
}

static bool validateTime(const char* time)
{
    int len = 0;
    while (time[len] != '\0')
        len++;
    if (len != 5)
        return false;
    return time[2] == ':';
}

static bool collectMeetingDetails(Meeting& slot)
{
    cout << "Enter Meeting ID: ";
    cin >> slot.meet_id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid meeting ID.\n";
        return false;
    }
    if (findMeetingIndex(slot.meet_id) != -1)
    {
        cout << "Meeting ID already exists.\n";
        cin.ignore(1000, '\n');
        return false;
    }

    cin.ignore(1000, '\n');
    cout << "Enter Topic: ";
    cin.getline(slot.meet_topic, 50);
    if (stringEmpty(slot.meet_topic))
    {
        cout << "Topic cannot be empty.\n";
        return false;
    }

    cout << "Enter Date (YYYY-MM-DD): ";
    cin.getline(slot.meet_date, 11);
    if (!validateDate(slot.meet_date))
    {
        cout << "Invalid date format.\n";
        return false;
    }

    cout << "Enter Time (HH:MM): ";
    cin.getline(slot.meet_time, 6);
    if (!validateTime(slot.meet_time))
    {
        cout << "Invalid time format.\n";
        return false;
    }

    slot.participant_count = 0;
    slot.participant_capacity = 10;
    slot.participant_ids = nullptr;
    return true;
}

static bool clashesWithExisting(const Meeting& candidate)
{
    for (int i = 0; i < meeting_count; i++)
    {
        if (stringsEqual(meetings_list[i].meet_date, candidate.meet_date) &&
            stringsEqual(meetings_list[i].meet_time, candidate.meet_time))
        {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// Meeting operations
// ---------------------------------------------------------------------------
void addMeeting()
{
    if (meeting_count >= MAX_MEETINGS)
    {
        cout << "Meeting list is full.\n";
        return;
    }

    Meeting temp;
    if (!collectMeetingDetails(temp))
        return;

    temp.participant_ids = new int[temp.participant_capacity];
    meetings_list[meeting_count] = temp;
    meeting_count++;
    cout << "Meeting added.\n";
}

int scheduleMeeting()
{
    if (meeting_count >= MAX_MEETINGS)
    {
        cout << "Meeting list is full.\n";
        return 0;
    }

    Meeting temp;
    if (!collectMeetingDetails(temp))
        return 0;

    if (clashesWithExisting(temp))
    {
        cout << "Meeting clash detected. Could not schedule.\n";
        return 0;
    }

    temp.participant_ids = new int[temp.participant_capacity];
    meetings_list[meeting_count] = temp;
    meeting_count++;
    cout << "Meeting scheduled successfully.\n";
    return 1;
}

void displayMeeting(int index)
{
    if (index < 0 || index >= meeting_count)
    {
        cout << "Meeting not found.\n";
        return;
    }

    Meeting& m = meetings_list[index];
    cout << "ID: " << m.meet_id << ", Topic: " << m.meet_topic
         << ", Date: " << m.meet_date << ", Time: " << m.meet_time << '\n';
    cout << "Participants (" << m.participant_count << "): ";
    for (int i = 0; i < m.participant_count; i++)
    {
        if (i > 0)
            cout << ' ';
        cout << m.participant_ids[i];
    }
    cout << '\n';
}

void addParticipant(int meetIdx, int empId)
{
    if (meetIdx < 0 || meetIdx >= meeting_count)
    {
        cout << "Meeting not found.\n";
        return;
    }
    if (!employeeExists(empId))
    {
        cout << "Employee ID " << empId << " does not exist.\n";
        return;
    }

    Meeting& m = meetings_list[meetIdx];
    if (m.participant_count >= m.participant_capacity)
    {
        int newCapacity = m.participant_capacity * 2;
        if (newCapacity < 10)
            newCapacity = 10;
        int* newArray = new int[newCapacity];
        for (int i = 0; i < m.participant_count; i++)
            newArray[i] = m.participant_ids[i];
        delete[] m.participant_ids;
        m.participant_ids = newArray;
        m.participant_capacity = newCapacity;
    }

    // Prevent duplicates
    for (int i = 0; i < m.participant_count; i++)
    {
        if (m.participant_ids[i] == empId)
        {
            cout << "Employee already added to meeting.\n";
            return;
        }
    }

    m.participant_ids[m.participant_count++] = empId;
    cout << "Participant added.\n";
}

int hasTimeClash(int indexA, int indexB)
{
    if (indexA < 0 || indexA >= meeting_count)
        return 0;
    if (indexB < 0 || indexB >= meeting_count)
        return 0;

    Meeting& a = meetings_list[indexA];
    Meeting& b = meetings_list[indexB];
    if (stringsEqual(a.meet_date, b.meet_date) && stringsEqual(a.meet_time, b.meet_time))
        return 1;
    return 0;
}

// ---------------------------------------------------------------------------
// File handling (text)
// Format: id topic date time count ids...
// ---------------------------------------------------------------------------
void loadMeetingsFromText(const char* filename)
{
    ifstream fin(filename);
    for (int i = 0; i < meeting_count; i++)
    {
        delete[] meetings_list[i].participant_ids;
        meetings_list[i].participant_ids = nullptr;
        meetings_list[i].participant_count = 0;
        meetings_list[i].participant_capacity = 0;
        meetings_list[i].meet_topic[0] = '\0';
        meetings_list[i].meet_date[0] = '\0';
        meetings_list[i].meet_time[0] = '\0';
    }
    meeting_count = 0;

    if (!fin)
        return;

    while (meeting_count < MAX_MEETINGS)
    {
        Meeting& slot = meetings_list[meeting_count];
        fin >> slot.meet_id >> slot.meet_topic >> slot.meet_date >> slot.meet_time >> slot.participant_count;
        if (fin.fail())
            break;

        if (slot.participant_count < 0)
            slot.participant_count = 0;

        slot.participant_capacity = (slot.participant_count > 0) ? slot.participant_count : 10;
        if (slot.participant_ids != nullptr)
            delete[] slot.participant_ids;
        slot.participant_ids = new int[slot.participant_capacity];

        for (int i = 0; i < slot.participant_count; i++)
        {
            fin >> slot.participant_ids[i];
            if (fin.fail())
            {
                slot.participant_count = i;
                break;
            }
        }

        meeting_count++;
    }
}

void saveMeetingsToText(const char* filename)
{
    ofstream fout(filename);
    if (!fout)
    {
        cout << "Unable to open meetings file for writing.\n";
        return;
    }

    for (int i = 0; i < meeting_count; i++)
    {
        Meeting& m = meetings_list[i];
        fout << m.meet_id << ' ' << m.meet_topic << ' ' << m.meet_date << ' ' << m.meet_time << ' ' << m.participant_count;
        for (int j = 0; j < m.participant_count; j++)
            fout << ' ' << m.participant_ids[j];
        fout << '\n';
    }
}
