#pragma once
#include <iostream>
#include <unordered_set>
using namespace std;

class redisSet
{
private:
    unordered_map<string, unordered_set<string>> _set;

    bool EXISTS(const string& key) const
    {
        return (_set.find(key) != _set.end());
    }
public:
    void write(const string& filename, const vector<string>& commands, const vector<string>& keys, const vector<string>& values)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (size_t i = 0; i < commands.size(); ++i)
                file << commands[i] << " " << keys[i] << " " << values[i] << endl;
            file.close();
            loadSet(filename);
        }
        else
            cout << "File can't open!\n";
    }

    void loadSet(const string&name)
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
            if (command == "SADD")
            {
                vector<string> values;
                str >> key;
                while (str >> value)
                    values.push_back(value);
                SADD(key, values);
            }
            else if (command == "SMEMBERS")
            {
                str >> key;
                unordered_set<string> members = SMEMBERS(key);
                cout << key << ": { ";
                for (const auto& member : members)
                    cout << member << ", ";
                cout << "\b\b }" << endl;
            }
            else if (command == "SCARD")
            {
                str >> key;
                cout << SCARD(key) << endl;
            }
            else if (command == "SDIFF")
            {
                str >> key;
                string otherkey;
                vector<string> keys;
                while (str >> otherkey)
                    keys.push_back(otherkey);
                cout << "{ ";
                for (const auto& member : SDIFF(key, keys))
                    cout << member << ", ";
                cout << "\b\b }" << endl;
            }
            else if (command == "SDIFFSTORE")
            {
                string dest;
                str >> dest >> key;
                string otherkey;
                vector<string> keys;
                while (str >> otherkey)
                    keys.push_back(otherkey);
                SDIFFSTORE(dest, key, keys);
            }
            else if (command == "SUNION")
            {
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                cout << "{ ";
                for (const auto& member : SUNION(keys))
                    cout << member << ", ";
                cout << "\b\b }" << endl;
            }
            else if (command == "SUNIONSTORE")
            {
                string dest;
                str >> dest;
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                SUNIONSTORE(dest, keys);
            }
            else if (command == "SINTER")
            {
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                cout << "{ ";
                for (const auto& member : SINTER(keys))
                    cout << member << ", ";
                cout << "\b\b }" << endl;
            }
            else if (command == "SINTERCARD")
            {
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                cout << SINTERCARD(keys) << endl;
            }
            else if (command == "SINTERSTORE")
            {
                string dest;
                str >> dest;
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                SINTERSTORE(dest, keys);
            }
            else if (command == "SISMEMBER")
            {
                str >> key >> value;
                cout << "'" << value << "' is ";
                if (SISMEMBER(key, value))
                    cout << "a member of " << key << endl;
                else
                    cout << "not a member of " << key << endl;
            }
            else if (command == "SMISMEMBER")
            {
                str >> key;
                vector<string> values;
                while (str >> value)
                    values.push_back(value);
                for (const auto& member : SMISMEMBER(key, values))
                    cout << member << " ";
                cout << endl;
            }
            else if (command == "SMOVE")
            {
                string src, des;
                str >> src >> des >> value;
                cout << SMOVE(src, des, value) << endl;
            }
            else if (command == "SPOP")
            {
                int count;
                str >> key >> count;
                cout << "{ ";
                for (const auto& member : SPOP(key, count))
                    cout << member << ", ";
                cout << "\b\b }" << endl;
            }
            else if (command == "SREM")
            {
                str >> key;
                vector<string> values;
                while (str >> value)
                    values.push_back(value);
                cout << SREM(key, values) << endl;
            }
            else
                cout << "INVALID command: " << command << endl;
        }
        rdr.close();
    }

    void SADD(const string& key, const vector<string>& members)
    {
        if (EXISTS(key))
        {
            if (_set.find(key)->second.size() > 0)
                for (const auto& member : members)
                    _set.find(key)->second.insert(member);
            else
                cout << "Error: Key '" << key << "' is not a set." << endl;
        }
        else
        {
            unordered_set<string> newSet;
            for (const auto& member : members)
                newSet.insert(member);
            _set[key] = newSet;
        }
    }

    int SCARD(const string& key)
    {
        if (EXISTS(key))
            return _set[key].size();
        return 0;
    }

    unordered_set<string> SDIFF(const string& firstKey, const vector<string>& otherKeys) const
    {
        if (EXISTS(firstKey) && _set.find(firstKey)->second.size() > 0)
        {
            unordered_set<string> result = _set.find(firstKey)->second;
            for (const auto& key : otherKeys)
            {
                if (EXISTS(key) && _set.find(key)->second.size() > 0)
                    for (const auto& member : _set.find(key)->second)
                        result.erase(member);
            }
            return result;
        }
        else
        {
            cout << "Error: Key '" << firstKey << "' does not exist." << endl;
            return {};
        }
    }

    void SDIFFSTORE(const string& destination, const string& firstKey, const vector<string>& otherKeys)
    {
        _set[destination] = SDIFF(firstKey, otherKeys);
    }

    unordered_set<string> SINTER(const vector<string>& keys) const
    {
        unordered_set<string> result;
        for (const auto& key : keys)
        {
            if (!EXISTS(key) || _set.find(key)->second.empty())
                return {};
            if (result.empty())
                result = _set.find(key)->second;
            else
            {
                unordered_set<string> tempResult;
                for (const auto& member : result)
                    if (_set.find(key)->second.find(member) != _set.find(key)->second.end())
                        tempResult.insert(member);
                result = tempResult;
                if (result.empty())
                    break;
            }
        }
        return result;
    }

    int SINTERCARD(const vector<string> keys)const
    {
        return SINTER(keys).size();
    }

    void SINTERSTORE(const string& destination, const vector<string>& otherKeys)
    {
        _set[destination] = SINTER(otherKeys);
    }

    bool SISMEMBER(const string& key, const string& member) const
    {
        if (!EXISTS(key))
            return false;
        return _set.find(key)->second.find(member) != _set.find(key)->second.end();
    }

    unordered_set<string> SMEMBERS(const string& key)
    {
        if (!EXISTS(key))
            return{};
        return _set.find(key)->second;
    }

    vector<int> SMISMEMBER(const string& key, const vector<string>& members) const
    {
        if (!EXISTS(key))
            return vector<int>(members.size(), 0);
        const unordered_set<string>& keySet = _set.find(key)->second;

        vector<int> result;
        for (const auto& member : members)
            result.push_back(keySet.find(member) != keySet.end() ? 1 : 0);
        return result;
    }

    int SMOVE(const string& sourceKey, const string& destKey, const string& member)
    {
        auto itSource = _set.find(sourceKey);
        auto itDest = _set.find(destKey);

        if (!EXISTS(sourceKey) || itSource->second.find(member) == itSource->second.end())
            return 0;
        itSource->second.erase(member);
        if (!EXISTS(destKey))
            _set[destKey] = { member };
        else
            itDest->second.insert(member);
        return 1;
    }

    vector<string> SPOP(const string& key, size_t count = 1)
    {
        if (!EXISTS(key) || _set.find(key)->second.empty())
            return {};
        vector<string> poppedMembers;
        count = min(count, _set.find(key)->second.size());
        for (size_t i = 0; i < count; ++i)
        {
            auto randomIterator = next(begin(_set.find(key)->second), rand() % _set.find(key)->second.size());
            poppedMembers.push_back(*randomIterator);
            _set.find(key)->second.erase(randomIterator);
        }
        return poppedMembers;
    }

    int SREM(const string& key, const vector<string>& members)
    {
        auto it = _set.find(key);
        if (!EXISTS(key) || it->second.empty())
            return 0;
        int removeCount = 0;
        for (const auto& member : members)
        {
            auto memberIt = it->second.find(member);
            if (memberIt != it->second.end())
            {
                it->second.erase(memberIt);
                ++removeCount;
            }
        }
        return removeCount;
    }

    unordered_set<string> SUNION(const vector<string>& keys) const
    {
        unordered_set<string> unionResult;
        for (const auto& key : keys)
            if (EXISTS(key))
                for (const auto& member : _set.find(key)->second)
                    unionResult.insert(member);
        return unionResult;
    }

    void SUNIONSTORE(const string& destination, const vector<string>& otherKeys)
    {
        _set[destination] = SUNION(otherKeys);
    }
};