#include "project.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// 12) Add a new meeting (take topic/date/time as input inside)
void addMeeting()
{
    if (meetingCount >= MAX_MEETINGS)
    {
        cout << "Meeting list is full\n";
        return;
    }
    Meeting &m = meetings[meetingCount];
    cout << "Enter Meeting ID: ";
    cin >> m.id;
    cin.ignore(1000, '\n'); // Peechay bachi newline ko saaf kar raha hoon, warna next input galat parhta.
    cout << "Enter Topic: ";
    cin.getline(m.topic, 50);
    cout << "Enter Date (YYYY-MM-DD): ";
    cin.getline(m.date, 11);
    cout << "Enter Time (HH:MM): ";
    cin.getline(m.time, 6);
    m.participantCount = 0;
    m.participantCapacity = 50;
    m.participantIds = new int[m.participantCapacity];
    meetingCount++;
}

// 13) Schedule a meeting (check clashes before confirming).
int scheduleMeeting(int meetingIndex)
{
    if (meetingIndex < 0 || meetingIndex >= meetingCount)
        return 0;
    for (int i = 0; i < meetingCount; i++)
    {
        if (i == meetingIndex)
            continue;
        if (hasTimeClash(meetingIndex, i) == 1)
        {
            cout << "Time clash with meeting index " << i << "\n";
            return 0;
        }
    }
    return 1;
}

// 14) Display details of a meeting at a given index
void displayMeeting(int index)
{
    if (index < 0 || index >= meetingCount)
    {
        cout << "Meeting not found\n";
        return;
    }
    Meeting &m = meetings[index];
    cout << "ID: " << m.id << ", Topic: " << m.topic
         << ", Date: " << m.date << ", Time: " << m.time << '\n';
    cout << "Participants (" << m.participantCount << "): ";
    for (int i = 0; i < m.participantCount; i++)
    {
        if (i)
            cout << ' ';
        cout << m.participantIds[i];
    }
    cout << '\n';
}

// 15) Add a participant (employee ID) to a meeting by index
void addParticipant(int meetIdx, int empId)
{
    if (meetIdx < 0 || meetIdx >= meetingCount)
    {
        cout << "Meeting not found\n";
        return;
    }
    Meeting &m = meetings[meetIdx];
    if (m.participantCount >= m.participantCapacity)
    {
        cout << "Participant list is full\n";
        return;
    }
    m.participantIds[m.participantCount++] = empId;
}

// 16) Check if two meetings clash in date/time.
int hasTimeClash(int indexA, int indexB)
{
    if (indexA < 0 || indexA >= meetingCount)
        return 0;
    if (indexB < 0 || indexB >= meetingCount)
        return 0;
    Meeting &a = meetings[indexA];
    Meeting &b = meetings[indexB];
    if (strcmp(a.date, b.date) == 0 && strcmp(a.time, b.time) == 0)
        return 1;
    return 0;
}

// FILE HANDLING – MEETINGS (TEXT FILE)

void loadMeetingsFromText(const char *filename)
{
    ifstream fin(filename);
    meetingCount = 0;
    while (meetingCount < MAX_MEETINGS)
    {
        Meeting &m = meetings[meetingCount];
        fin >> m.id >> m.topic >> m.date >> m.time >> m.participantCount;
        if (fin.fail())
            break;

        if (m.participantIds)
            delete[] m.participantIds;
        if (m.participantCount > 0)
        {
            m.participantCapacity = m.participantCount;
            m.participantIds = new int[m.participantCapacity];
            for (int i = 0; i < m.participantCount; i++)
                fin >> m.participantIds[i];
        }
        else
            m.participantIds = nullptr;

        meetingCount++;
    }
}
void saveMeetingsToText(const char *filename)
{
    ofstream fout(filename);
    for (int i = 0; i < meetingCount; i++)
    {
        Meeting &m = meetings[i];
        fout << m.id << ' ' << m.topic << ' ' << m.date << ' ' << m.time << ' ' << m.participantCount;
        for (int j = 0; j < m.participantCount; j++)
            fout << ' ' << m.participantIds[j];
        fout << '\n';
    }
}
