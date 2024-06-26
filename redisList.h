#include <iostream>
#include <string>
using namespace std;

class listNode 
{
public:
    string data;
    listNode* next;

    listNode(const string& value)
    {
        data = value;
        next = nullptr;
    }
};

class redisList 
{
private:
    listNode* head;
    listNode* tail;
    int length;
    unordered_map<string, listNode*> lists;

public:
    void write(const string& filename, const vector<string>& commands, const vector<string>& keys, const vector<string>& values)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (size_t i = 0; i < commands.size(); ++i)
                file << commands[i] << " " << keys[i] << " " << values[i] << endl;
            file.close();
            loadList(filename);
        }
        else
            cout << "File can't open!\n";
    }

    void loadList(const string& name)
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

            if (command == "LPUSH")
            {
                str >> key;
                vector<string> values;
                while (str >> value)
                    values.push_back(value);
                LPUSH(key, values);
            }
            else if (command == "PRINT")
            {
                str >> key;
                printList(key);
            }
            else if (command == "LPUSHX")
            {
                str >> key;
                vector<string> values;
                while (str >> value)
                    values.push_back(value);
                LPUSHX(key, values);
            }
            else if (command == "LINDEX")
            {
                int ind;
                str >> key >> ind;
                cout << "Element at " << ind << ": " << LINDEX(key, ind) << endl;
            }
            else if (command == "LSET")
            {
                int ind;
                str >> key >> ind >> value;
                cout << LSET(key, ind, value);
            }
            else if (command == "LREM")
            {
                int count;
                str >> key >> count >> value;
                cout << LREM(key, value, count) << endl;
            }
            else if (command == "RPUSH")
            {
                str >> key;
                vector<string> values;
                while (str >> value)
                    values.push_back(value);
                RPUSH(key, values);
            }
            else if (command == "RPUSHX")
            {
                str >> key;
                vector<string> values;
                while (str >> value)
                    values.push_back(value);
                RPUSHX(key, values);
            }
            else if (command == "LPOP")
            {
                int count = 0;
                str >> key >> count;
                cout << LPOP(key, count) << endl;
            }
            else if (command == "LRANGE")
            {
                int start, end;
                str >> key >> start >> end;
                vector<string> results = LRANGE(key, start, end);
                for (const auto& element : results)
                    cout << element << ", ";
                cout << endl;
            }
            else if (command == "RPOP")
            {
                int count;
                str >> key >> count;
                cout << RPOP(key, count);
            }
            else if (command == "LLEN")
            {
                str >> key;
                cout << LLEN(key) << endl;
            }
            else if (command == "LMOVE")
            {
                string dest;
                str >> key >> dest;
                LMOVE(key,dest);
            }
            else if (command == "LTRIM")
            {
                int start, stop;
                str >> key >> start >> stop;
                LTRIM(key, start, stop);
            }
            else
                cout << "INVALID command: " << command << endl;
        }
        rdr.close();
    }

    string LINDEX(const string& key, int index)
    {
        auto it = lists.find(key);
        if (it == lists.end() || !it->second)
            return "List does not exist!\n";
        listNode* current = it->second;
        if (index < 0)
        {
            int size = 0;
            while (current != nullptr)
            {
                size++;
                current = current->next;
            }
            index = size + index;
        }
        for (int i = 0; i < index && current != nullptr; i++)
            current = current->next;
        if (current != nullptr)
            return current->data;
        else
            return "Index not in range!\n";
    }

    string LSET(const string& key, int index, const string& _data)
    {
        auto it = lists.find(key);
        if (it == lists.end() || !it->second)
            return key + " does not exist!\n";
        listNode* current = it->second;
        if (index < 0) 
        {
            int listSize = 0;
            while (current != nullptr) 
            {
                listSize++;
                current = current->next;
            }
            index = listSize + index;
        }
        for (int i = 0; i < index && current != nullptr; ++i)
            current = current->next;
        if (current == nullptr)
            return "Index not in range!\n";
        current->data = _data;
        return "OK\n";
    }

    int LREM(const string& key, const string& element, int count) 
    {
        int remCount = 0;
        auto it = lists.find(key);
        if (it == lists.end() || !it->second) 
            return remCount;
        listNode* current = it->second;
        listNode* prev = nullptr;
        while (current) 
        {
            if (current->data == element) 
            {
                if (prev) 
                    prev->next = current->next;
                else 
                    it->second = current->next;
                listNode* nextNode = current->next;
                delete current;
                current = nextNode;
                remCount++;
                if (count > 0 && remCount >= count) 
                    break;
            }
            else 
            {
                prev = current;
                current = current->next;
            }
        }
        if (!current) 
            tail = prev;
        length -= remCount;
        return remCount;
    }

    void LPUSH(const string& key, const vector<string>& values) {
        auto it = lists.find(key);
        if (it == lists.end()) {
            listNode* head = nullptr;
            for (const auto& value : values) {
                listNode* newNode = new listNode(value);
                newNode->next = head;
                head = newNode;
                length++;
            }
            lists[key] = head;
        }
        else {
            for (const auto& value : values) {
                listNode* newNode = new listNode(value);
                newNode->next = it->second;
                it->second = newNode;
                length++;
            }
        }
    }

    void LPUSHX(const string& key, const vector<string>& values)
    {
        auto it = lists.find(key);
        if (it == lists.end()) 
        {
            cout << "LIST with key '" << key << "' does not exist!\n";
            return;
        }
        for (const auto& value : values) 
        {
            listNode* newNode = new listNode(value);
            newNode->next = it->second;
            it->second = newNode;
        }
    }

    void RPUSH(const string& key, const vector<string>& values) {
        auto it = lists.find(key);
        if (it == lists.end()) {
            lists[key] = nullptr;
            it = lists.find(key);
        }

        for (const auto& value : values) {
            listNode* newNode = new listNode(value);
            if (it->second == nullptr) {
                it->second = newNode;
                tail = newNode;
            }
            else {
                tail->next = newNode;
                tail = newNode;
            }
            length++;
        }
    }

    void RPUSHX(const string& key, const vector<string>& values)
    {
        auto it = lists.find(key);
        if (it == lists.end() || it->second == nullptr)
        {
            cout << key << "' does not exist!\n";
            return;
        }
        for (const auto& value : values)
        {
            listNode* newNode = new listNode(value);
            if (it->second == nullptr)
            {
                it->second = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            length++;
        }
    }

    string LPOP(const string& key, int count = 1)
    {
        auto it = lists.find(key);
        if (it == lists.end() || it->second == nullptr || count <= 0)
            return key + "' does not exist or the count is invalid\n";
        string result;
        for (int i = 0; i < count && it->second != nullptr; i++)
        {
            listNode* temp = it->second;
            it->second = it->second->next;
            temp->next = nullptr;
            if (result.empty())
                result = temp->data;
            else
                result += " " + temp->data;
            delete temp;
            length--;
            if (it->second == nullptr)
                tail = nullptr;
        }
        return result;
    }

    vector<string> LRANGE(const string& key, int start, int stop) const
    {
        vector<string> result;
        if (start < 0 || stop < 0 || start > stop)
            return result;

        int count = stop - start + 1;
        result.reserve(count);

        auto it = lists.find(key);
        if (it == lists.end() || it->second == nullptr)
            return result;

        listNode* temp = it->second;
        for (int i = 0; i < start && temp != nullptr; ++i)
            temp = temp->next;
        for (int i = 0; i < count && temp != nullptr; ++i)
        {
            result.push_back(temp->data);
            temp = temp->next;
        }
        return result;
    }

    string RPOP(const string& key, int count = 1)
    {
        if (count <= 0)
            return "";

        auto it = lists.find(key);
        if (it == lists.end() || it->second == nullptr)
            return "";

        string result;
        for (int i = 0; i < count && it->second != nullptr; i++)
        {
            listNode* temp = it->second;
            listNode* prev = nullptr;
            while (temp->next != nullptr)
            {
                prev = temp;
                temp = temp->next;
            }
            if (prev != nullptr)
            {
                prev->next = nullptr;
                tail = prev;
            }
            else
                it->second = tail = nullptr;
            if (result.empty())
                result = temp->data;
            else
                result += " " + temp->data;
            delete temp;
            length--;
            if (it->second == nullptr)
                tail = nullptr;
        }
        return result;
    }

    int LLEN(const string& key) const 
    {
        auto it = lists.find(key);
        if (it == lists.end() || it->second == nullptr) 
            return 0;
        return length;
    }

    void LMOVE(const string& src, const string& des) 
    {
        auto sourceIt = lists.find(src);
        auto destIt = lists.find(des);
        if (sourceIt == lists.end() || sourceIt->second == nullptr) 
            return; 
        listNode* temp = sourceIt->second;
        sourceIt->second = sourceIt->second->next;
        temp->next = nullptr;
        if (sourceIt->second == nullptr) 
            tail = nullptr;
        if (destIt == lists.end() || destIt->second == nullptr) 
        {
            lists[des] = temp;
            destIt = lists.find(des);
        }
        else 
        {
            temp->next = destIt->second;
            destIt->second = temp;
        }
        length--;
    }

    void LTRIM(const string& key, int start, int stop) 
    {
        auto it = lists.find(key);
        if (it == lists.end() || it->second == nullptr || start >= length || start > stop || stop >= length - 1) 
            return;
        listNode* temp = it->second;
        int count = 0;
        while (count < start - 1) 
        {
            temp = temp->next;
            count++;
        }
        listNode* startTrim = nullptr;
        startTrim = temp->next;
        temp->next = nullptr;
        while (temp->next != nullptr) 
            temp = temp->next;
        it->second = temp;
        while (count < stop) 
        {
            listNode* nextNode = startTrim->next;
            delete startTrim;
            startTrim = nextNode;
            count++;
        }
        length = stop - start + 1;
    }

    void printList(const string& key)
    {
        auto it = lists.find(key);
        if (it == lists.end())
        {
            cout << "Key '" << key << "' does not exist!\n";
            return;
        }
        cout << key << ": ";
        listNode* current = it->second;
        while (current != nullptr)
        {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    ~redisList() 
    {
        for (auto& entry : lists) 
        {
            listNode* current = entry.second;
            while (current != nullptr) 
            {
                listNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
};