#include<unordered_set>
#include<unordered_map>
#include<iostream>
using namespace std;

class redisHash 
{
private:
    unordered_map<string, unordered_map<string, string>> hashMap;
    unordered_map<string, unordered_set<string>> fieldSet;

public:
    void write(const string& filename, const vector<string>& commands, const vector<string>& keys, const vector<string>& fields, const vector<string>& values)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (size_t i = 0; i < commands.size(); ++i)
                file << commands[i] << " " << keys[i] << " " << fields[i] << " " << values[i] << endl;
            file.close();
            loadHash(filename);
        }
        else
            cout << "File can't open!\n";
    }

    void loadHash(const string&name)
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
            string command, key, field, value;
            str >> command;

            if (command == "HSET")
            {
                str >> key >> field;
                getline(str >> ws, value);
                HSET(key, field, value);
            }
            else if (command == "HGET")
            {
                str >> key >> field;
                cout << HGET(key, field) << endl;
            }
            else if (command == "HDEL")
            {
                str >> key >> field;
                HDEL(key, field);
            }
            else if (command == "HGETALL")
            {
                str >> key;
                HGETALL(key);
            }
            else if (command == "HEXISTS")
            {
                str >> key >> field;
                cout << HEXISTS(key, field) << endl;
            }
            else if (command == "HKEYS")
            {
                str >> key;
                HKEYS(key);
            }
            else if (command == "HVALS")
            {
                str >> key;
                HVALS(key);
            }
            else if (command == "HSETNX")
            {
                str >> key >> field >> value;
                HSETNX(key, field, value);
            }
            else if (command == "MSET")
            {
                string key, field, value;
                str >> key;
                vector<string> fields;
                vector<string> values;
                while (str >> field >> value)
                {
                    fields.push_back(field);
                    values.push_back(value);
                }
                MSET(key, fields, values);
            }
            else if (command == "HMGET")
            {
                str >> key;
                vector<string> fields;
                while (str >> field)
                    fields.push_back(field);
                cout << "MGET: ";
                for (int i = 0; i < fields.size(); i++)
                    cout << HMGET(key, fields)[i] << ", ";
                cout << endl;
            }
            else if (command == "HLEN")
            {
                str >> key;
                cout << HLEN(key) << endl;
            }
            else if (command == "HSTRLEN")
            {
                str >> key >> field;
                cout << HSTRLEN(key, field) << endl;
            }
            else
                cout << "INVALID command: " << command << endl;
        }
        rdr.close();
    }

    void HSET(const string& key, const string& field, const string& value) 
    {
        hashMap[key][field] = value;
        fieldSet[key].insert(field);
    }
     
    string HGET(const string& key, const string& field)
    {
        if (HEXISTS(key, field))
            return hashMap[key][field];
        else
            return "Field not found!";
    }

    void HDEL(const string& key, const string& field) 
    {
        if (HEXISTS(key, field))
        {
            hashMap[key].erase(field);
            fieldSet[key].erase(field);
        }
    }

    void HGETALL(const string& key) 
    {
        if (hashMap.count(key)) 
            for (const auto& field : fieldSet[key]) 
                cout << field << ": " << hashMap[key][field] << endl;
    }

    bool HEXISTS(const string& key, const string& field)
    {
        return (hashMap.count(key) && hashMap[key].count(field));
    }

    void HKEYS(const string& key)
    {
        if (hashMap.count(key))
            for (const auto& field : fieldSet[key])
                cout << field <<  endl;
    }

    void HVALS(const string& key)
    {
        if (hashMap.count(key))
            for (const auto& value : fieldSet[key])
                cout << hashMap[key][value] << endl;
    }

    int HLEN(const string& key)
    {
        return hashMap[key].size();
    }

    vector<string> HMGET(const string& key,const vector<string>& fields)
    {
        vector<string> result;
        for (auto field : fields)
            result.push_back(HGET(key, field));
        return result;
    }

    void MSET(const string& key, const vector<string>& fields, const vector<string>& values)
    {
        if (fields.size() != values.size())
        {
            cout << "\nNumber of keys are not equal to number of values.";
            return;
        }
        for (int i = 0; i < fields.size(); i++)
            HSET(key, fields[i], values[i]);
    }

    void HSETNX(const string& key, const string& field, const string& value)
    {
        if (!HEXISTS(key, field))
            HSET(key, field, value);
    }

    int HSTRLEN(const string& key, const string& field)
    {
        return HGET(key, field).length();
    }
};
