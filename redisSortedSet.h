#pragma once
#include<unordered_map>
#include<set>
#include<vector>
#include<iostream>
using namespace std;

class redisSortedSet
{
private:
    unordered_map<string, double> scores;
    struct memberInfo
    {
        double score;
        string member;
        bool operator<(const memberInfo& _member)const
        {
            return (score < _member.score) || (score == _member.score) && (member < _member.member);
        }
    };
    unordered_map<string, set<memberInfo>>keys;

    bool EXISTS(const string& member) const
    {
        return (scores.find(member) != scores.end());
    }
public:
    void write(const string& filename, const vector<string>& commands, const vector<string>& keys, const vector<double>& scores, const vector<string>& members)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (size_t i = 0; i < commands.size(); ++i)
                file << commands[i] << " " << keys[i] << " " << scores[i] << " " << members[i] << endl;
            file.close();
            loadSortedSet(filename);
        }
        else
            cout << "File can't open!\n";
    }

    void loadSortedSet(const string&name)
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
            string command, key, member;
            double score;
            str >> command;
            if (command == "ZADD")
            {
                str >> key >> score >> member;
                ZADD(key, member, score);
            }
            else if (command == "ZPRINT")
            {
                str >> key;
                print(key);
            }
            else if (command == "ZDIFF")
            {
                str >> key;
                string otherkey;
                vector<string> keys;
                while (str >> otherkey)
                    keys.push_back(otherkey);
                for (const auto& res : ZDIFF(key, keys))
                    cout << "Member: " << res.member << ", Score: " << res.score << endl;
            }
            else if (command == "ZDIFFSTORE")
            {
                string dest;
                str >> dest >> key;
                string otherkey;
                vector<string> keys;
                while (str >> otherkey)
                    keys.push_back(otherkey);
                ZDIFFSTORE(dest, key, keys);
            }
            else if (command == "ZINTER")
            {
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                cout << "ZINTER:" << endl;
                for (const auto& res : ZINTER(keys))
                    cout << "Member: " << res.member << ", Score: " << res.score << endl;
            }
            else if (command == "ZINTERSTORE")
            {
                string dest;
                str >> dest;
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                ZINTERSTORE(dest, keys);
            }
            else if (command == "ZUNION")
            {
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                cout << "ZUNION:" << endl;
                for (const auto& res : ZUNION(keys))
                    cout << "Member: " << res.member << ", Score: " << res.score << endl;
            }
            else if (command == "SUNIONSTORE")
            {
                string dest;
                str >> dest;
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                ZUNIONSTORE(dest, keys);
            }
            else if (command == "ZINTERCARD")
            {
                vector<string> keys;
                while (str >> key)
                    keys.push_back(key);
                cout << ZINTERCARD(keys) << endl;
            }
            else if (command == "ZCOUNT")
            {
                int min, max;
                str >> key >> min >> max;
                cout << ZCOUNT(key, min, max) << endl;
            }
            else if (command == "ZPOPMIN")
            {
                int count = 0;
                str >> key >> count;
                for (const auto& member : ZPOPMIN(key, count))
                    cout << "Member: " << member.member << ", Score: " << member.score << endl;
            }
            else if (command == "ZPOPMAX")
            {
                int count = 0;
                str >> key >> count;
                for (const auto& member : ZPOPMAX(key, count))
                    cout << "Member: " << member.member << ", Score: " << member.score << endl;
            }
            else if (command == "ZSCORE")
            {
                str >> key >> member;
                cout << ZSCORE(key, member) << endl;
            }
            else if (command == "ZRANK")
            {
                str >> key >> member;
                cout << ZRANK(key, member);
            }
            else if (command == "ZREM")
            {
                str >> key;
                while (str >> member)
                    ZREM(key, member);
            }
            else
                cout << "INVALID command: " << command << endl;
        }
        rdr.close();
    }

    void print(const string& key) const
    {
        if (keys.find(key) != keys.end())
        {
            const set<memberInfo>& memberSet = keys.at(key);
            cout << key << ":" << endl;
            for (const auto& mem : memberSet)
                cout << "Member: " << mem.member << ", Score: " << mem.score << endl;
        }
        else
            cout << "'" << key << "' does not exist!\n";
    }

    void ZADD(const string& key, const string& member, double score)
    {
        if (keys.find(key) == keys.end())
            keys[key] = set<memberInfo>();

        auto& memberSet = keys[key];
        auto it = memberSet.find({ score, member });

        if (it != memberSet.end())
        {
            scores[member] = score;
            memberSet.erase(it);
        }
        else
            scores[member] = score;
        memberSet.insert({ score, member });
    }

    void ZREM(const string& key, const string& member)
    {
        if (keys.find(key) != keys.end())
        {
            auto& memberSet = keys[key];
            auto it = memberSet.find({ scores[member], member });
            if (it != memberSet.end())
            {
                memberSet.erase(it);
                scores.erase(member);
            }
        }
        else
            cout << "'" << key << "' does not exist!\n";
    }

    set<memberInfo> ZDIFF(const string& key, const vector<string>& otherKeys) const
    {
        set<memberInfo> result;
        if (keys.find(key) != keys.end())
        {
            result = keys.at(key);
            for (const auto& otherKey : otherKeys)
                if (keys.find(otherKey) != keys.end())
                {
                    const set<memberInfo>& _set = keys.at(otherKey);
                    set<memberInfo> tempRes;
                    for (const auto& info : result)
                    {
                        auto it = _set.find(info);
                        if (it == _set.end())
                            tempRes.insert(info);
                    }
                    result = tempRes;
                }
        }
        else
            cout << key << " does not exist!\n";
        return result;
    }

    void ZDIFFSTORE(const string& res, const string& set1, const vector<string>& sets)
    {
        keys[res] = ZDIFF(set1, sets);
    }

    set<memberInfo> ZINTER(const vector<string>& otherkeys)
    {
        for (const string& key : otherkeys)
            if (keys.find(key) == keys.end())
                keys[key] = set<memberInfo>();
        set<memberInfo> result;
        if (!otherkeys.empty())
        {
            result = keys[otherkeys[0]];
            for (int i = 1; i < otherkeys.size(); i++)
            {
                set<memberInfo> tempRes;
                for (const memberInfo& member : result)
                    if (keys[otherkeys[i]].find(member) != keys[otherkeys[i]].end())
                        tempRes.insert(member);
                result = tempRes;
                if (result.empty())
                    break;
            }
        }
        return result;
    }
    
    void ZINTERSTORE(const string& res,const vector<string>& set2)
    {
        keys[res] = ZINTER(set2);
    }

    set<memberInfo> ZUNION(const vector<string>& otherkeys)
    {
        for (const string& key : otherkeys)
            if (keys.find(key) == keys.end())
                keys[key] = set<memberInfo>();
        set<memberInfo> result;
        if (!otherkeys.empty())
            for (const string& currentKey : otherkeys)
                for (const memberInfo& member : keys[currentKey])
                    result.insert(member);
        return result;
    }

    void ZUNIONSTORE(const string& res, const vector<string>& otherkeys)
    {
        keys[res] = ZUNION(otherkeys);
    }

    int ZINTERCARD(const vector<string>& otherkeys)
    {
        return ZINTER(otherkeys).size();
    }

    vector<memberInfo> ZPOPMIN(const string& key, int count = 1)
    {
        vector<memberInfo> result;
        if (keys.find(key) == keys.end() || keys[key].empty() || count == 0)
            return result;
        for (int i = 0; i < count; ++i)
        {
            auto it = keys[key].begin();
            result.push_back(*it);
            keys[key].erase(it);
        }
        return result;
    }

    vector<memberInfo> ZPOPMAX(const string& key, int count = 1)
    {
        vector<memberInfo> result;
        if (keys.find(key) == keys.end() || keys[key].empty() || count == 0)
            return result;
        auto it = keys[key].begin();
        while (it != keys[key].end())
            ++it;
        --it;
        for (int i = 0; i < count && it != keys[key].end(); i++)
        {
            result.push_back(*it);
            --it;
        }
        auto eraseIt = it;
        while (eraseIt != keys[key].end())
        {
            auto tempIt = eraseIt;
            ++eraseIt;
            keys[key].erase(tempIt);
        }
        return result;
    }

    double ZSCORE(const string& key, const string& member)
    {
        if (keys.find(key) == keys.end())
            return 'NIL';
        for (const memberInfo& entry : keys[key])
            if (entry.member == member)
                return entry.score;
    }

    int ZRANK(const string& key, const string& member)
    {
        if (keys.find(key) == keys.end())
            return 'NIL';
        int rank = 0;
        bool memberFound = false;
        for (const memberInfo& info : keys[key])
        {
            if (info.member == member)
            {
                memberFound = true;
                break;
            }
            ++rank;
        }
        return memberFound ? rank : 'NIL';
    }

    int ZCOUNT(const string& key, double min, double max)
    {
        if (keys.find(key) == keys.end())
            return 0;
        int count = 0;
        for (const memberInfo& member : keys[key])
            if (member.score >= min && member.score <= max)
                count++;
        return count;
    }
};