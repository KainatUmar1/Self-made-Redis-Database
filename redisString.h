#pragma once
#include<unordered_map>
#include<unordered_set>
#include<iostream>
#include<string>
#include<sstream>
using namespace std;

class redisString
{
private:
    unordered_map<string, string> data;

public:

    void write(const string& filename, const vector<string>& commands, const vector<string>& keys, const vector<string>& values)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (size_t i = 0; i < commands.size(); ++i)
                file << commands[i] << " " << keys[i] << " " << values[i] << endl;
            file.close();
            loadString(filename);
        }
        else
            cout << "File can't open!\n";
    }

    void loadString(const string&name)
    {
        ifstream rdr(name);
        if (!rdr.is_open()) 
        {
            cout << "File is'nt opening!\n";
            return;
        }
        string line;
        while (getline(rdr, line))
        {
            istringstream str(line);
            string command, key, value;
            str >> command;

            if (command == "SET")
            {
                str >> key;
                getline(str >> ws, value);
                SET(key, value);
            }
            else if (command == "GET")
            {
                str >> key;
                cout << key << ": " << GET(key) << endl;
            }
            else if (command == "APPEND")
            {
                str >> key >> value;
                APPEND(key, value);
            }
            else if (command == "GETDEL")
            {
                str >> key;
                cout << key << ": " << GETDEL(key) << endl;
            }
            else if (command == "SETNX")
            {
                str >> key >> value;
                SETNX(key, value);
            }
            else if (command == "MGET")
            {
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                cout << "MGET: ";
                for (int i = 0; i < keys.size(); i++)
                    cout << MGET(keys)[i] << ", ";
                cout << endl;
            }
            else if (command == "MSET")
            {
                vector<string> values;
                vector<string> keys;
                while (str >> key >> value)
                {
                    keys.push_back(key);
                    values.push_back(value);
                }
                MSET(keys, values);
            }
            else if (command == "MSETNX")
            {
                vector<string> values;
                vector<string> keys;
                while (str >> key >> value)
                {
                    keys.push_back(key);
                    values.push_back(value);
                }
                MSETNX(keys, values);
            }
            else if (command == "GETRANGE")
            {
                int start = 0, end = 0;
                str >> key >> start >> end;
                cout << GETRANGE(key, start, end) << endl;
            }
            else if (command == "SUBSTR")
            {
                int start = 0, end = 0;
                str >> key >> start >> end;
                cout << SUBSTR(key, start, end) << endl;
            }
            else if (command == "STRLEN")
            {
                str >> key;
                cout << STRLEN(key);
            }
            else
                cout << "INVALID command: " << command << endl;
        }
        rdr.close();
    }

    void SET(const string& key, const string& value)
    {
        data[key] = value;
    }

    string GET(const string& key)
    {
        if (EXISTS(key))
            return data[key].data();
        else
            return "NIL";
    }

    void APPEND(const string& key, const string& value)
    {
        if (EXISTS(key))
            data[key] += " " + value;
        else
            data[key] = " " + value;
    }

    string GETDEL(const string& key)
    {
        if (EXISTS(key))
        {
            auto temp = GET(key);
            data.erase(key);
            return temp;
        }
        else
            return "NIL";
    }

    bool EXISTS(const string& key) const
    {
        return data.find(key) != data.end();
    }

    void SETNX(const string& key, const string& value)
    {
        if (!EXISTS(key))
            SET(key, value);
    }

    vector<string> MGET(const vector<string>& keys)const
    {
        vector<string> result;
        for (auto key : keys)
        {
            if (EXISTS(key))
                result.push_back(data.find(key)->second);
            else
                result.push_back("NIL");
        }
        return result;
    }

    void MSET(const vector<string>& keys, const vector<string>& values)
    {
        if (keys.size() != values.size())
        {
            cout << "\nNumber of keys are not equal to number of values.";
            return;
        }
        for (int i = 0; i < keys.size(); i++)
            SET(keys[i], values[i]);
    }

    void MSETNX(const vector<string>& keys, const vector<string>& values)
    {
        for (auto key : keys)
            if (EXISTS(key))
                return;
        MSET(keys, values);
    }

    string GETRANGE(const string& key, int start, int end)
    {
        string res;
        for (int i = start; i <= end; i++)
            res += data[key][i];
        return res;
    }

    string SUBSTR(const string& key, int start, int end)
    {
        return GETRANGE(key, start, end);
    }

    int STRLEN(const string& key)
    {
        return GET(key).length();
    }
};