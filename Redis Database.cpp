#include<fstream>
#include <array>
#include <windows.h>
#include <stdlib.h>
#include"redisString.h"
#include"redisHash.h"
#include"redisSet.h"
#include"redisSortedSet.h"
#include"redisList.h"
using namespace std;

//write and operate commands on memory
int mainAll()
{
    redisString mystring;
    vector<string> writeCommands, writeKeys, writeValues;
    cout << "Type 'done' to finish and - in place of value if it is not required:\n";
    while (true)
    {
        string command, key, value;
        cout << "Enter command key value: ";
        cin >> command;
        if (command == "done")
            break;
        cin >> key >> value;
        writeCommands.push_back(command);
        writeKeys.push_back(key);
        writeValues.push_back(value);
    }
    mystring.write("string2.txt", writeCommands, writeKeys, writeValues);
    return 0;
}

int mainHash()
{
    redisHash myhash;
    vector<string> writeCommands, writeKeys, writeField, writeValues;
    cout << "Type 'done' to finish and - in place of anything not required:\n";
    while (true)
    {
        string command, key, field, value;
        cout << "Enter command key value: ";
        cin >> command;
        if (command == "done")
            break;
        cin >> key >> field >> value;
        writeCommands.push_back(command);
        writeKeys.push_back(key);
        writeField.push_back(field);
        writeValues.push_back(value);
    }
    myhash.write("hash2.txt", writeCommands, writeKeys, writeField, writeValues);
    return 0;
}

int mainSet()
{
    redisSet myset;
    vector<string> writeCommands, writeKeys, writeValues;
    cout << "Type 'done' to finish and - in place of anything not required:\n";
    while (true)
    {
        string command, key,value;
        cout << "Enter command key value: ";
        cin >> command;
        if (command == "done")
            break;
        cin >> key >> value;
        writeCommands.push_back(command);
        writeKeys.push_back(key);
        writeValues.push_back(value);
    }
    myset.write("set2.txt", writeCommands, writeKeys, writeValues);
    return 0;
}

int mainSortedSet()
{
    redisSortedSet myset;
    vector<string> writeCommands, writeKeys, writeMembers;
    vector<double> writeScores;
    cout << "Type 'done' to finish and - in place of anything not required:\n";
    while (true)
    {
        string command, key, member;
        double score;
        cout << "Enter command key value: ";
        cin >> command;
        if (command == "done")
            break;
        cin >> key >> score >> member;
        writeCommands.push_back(command);
        writeKeys.push_back(key);
        writeScores.push_back(score);
        writeMembers.push_back(member);
    }
    myset.write("sortedset2.txt", writeCommands, writeKeys, writeScores, writeMembers);
    return 0;
}

int mainList()
{
    redisList mylist;
    vector<string> writeCommands, writeKeys, writeValues;
    cout << "Type 'done' to finish and - in place of anything not required:\n";
    while (true)
    {
        string command, key, value;
        cout << "Enter command key value: ";
        cin >> command;
        if (command == "done")
            break;
        cin >> key >> value;
        writeCommands.push_back(command);
        writeKeys.push_back(key);
        writeValues.push_back(value);
    }
    mylist.write("list2.txt", writeCommands, writeKeys, writeValues);
    return 0;
}

int main()
{
    redisString myString;
    redisHash myHash;
    redisSet mySet;
    redisSortedSet mySortedSet;
    redisList myList;
    while (true)
    {
        int n = 0;
        cout << "All functions for each data structure are running!\n(Press -1 to exit!)\nEnter number for the desired main to run!\n" <<
            "1 Redis STRINGS\n2 Redis HASHES\n3 Redis SETS\n4 Redis SORTED SETS\n5 Redis LISTS\n";
        cin >> n;
        switch (n)
        {
        case 1:
            myString.loadString("string.txt");
            break;
        case 2:
            myHash.loadHash("hash.txt");
            break;
        case 3:
            mySet.loadSet("set.txt");
            break;
        case 4:
            mySortedSet.loadSortedSet("sortedSet.txt");
            break;
        case 5:
            myList.loadList("list.txt");
            break;
        default:
            cout << "Invalid Option!\n";
            break;
        }
        char x;
        cout << "\n\nDo you want to continue (y/n): ";
        cin >> x;
        if (x == 'y')
            system("cls");
        else
            break;
    }
    return 0;
}