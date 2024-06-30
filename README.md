# Redis Database Implementation 

# Project Overview
This project implements various Redis data structures using C++ as part of the final project for the Data Structures and Algorithms course. The implementation includes several key functionalities for Redis data structures such as Strings, Hashes, Sets, Sorted Sets, and Lists.

# Features
# Redis Strings
- SET: Sets a key with a corresponding string value.
- GET: Retrieves the string value associated with a key.
- DEL: Deletes a key-value pair.
- APPEND: Appends a string to the existing value associated with a key.
- GETDEL: Retrieves and deletes the value associated with a key.
- EXISTS: Checks if a key exists in the data structure.
- SETNX: Sets a key only if it does not already exist.
- MGET: Retrieves values for multiple keys.
- MSET: Sets multiple key-value pairs.
- MSETNX: Sets multiple key-value pairs only if none of the keys already exist.
- GETRANGE/SUBSTR: Retrieve a substring of the value associated with a key based on specified indices.
- STRLEN: Returns the length of the string value associated with a key.

# Redis Hashes
- HSET: Sets the value of a field for a given key.
- HGET: Retrieves the value of a specific field for a given key.
- HDEL: Deletes a specific field and its value for a given key.
- HGETALL: Retrieves all fields and their values for a given key.
- HEXISTS: Checks if a specific field exists for a given key.
- HKEYS: Retrieves all field names for a given key.
- HVALS: Retrieves all values for a given key.
- HLEN: Returns the number of fields for a given key.
- HMGET: Retrieves values for multiple fields of a given key.
- HSETNX: Sets the value of a field only if it does not already exist.
- HSTRLEN: Returns the length of the string value of a specific field for a given key.

# Redis Sets
- SADD: Adds one or more members to a set associated with a key.
- SCARD: Returns the cardinality (number of members) of a set.
- SDIFF: Computes the set difference between the first set and one or more other sets.
- SDIFFSTORE: Stores the set difference in a new set.
- SINTER: Computes the set intersection of multiple sets.
- SINTERCARD: Returns the cardinality of the set intersection of multiple sets.
- SINTERSTORE: Stores the set intersection in a new set.
- SISMEMBER: Checks if a member exists in a set.
- SMEMBERS: Retrieves all members of a set.
- SMISMEMBER: Checks for multiple members' existence in a set.
- SMOVE: Moves a member from one set to another.
- SPOP: Removes and returns one or more random members from a set.
- SREM: Removes one or more members from a set.
- SUNION: Computes the set union of multiple sets.
- SUNIONSTORE: Stores the set union in a new set.

# Redis Sorted Sets
- ZADD: Adds a member with a specified score to the sorted set. If the member already exists, it updates the score.
- ZREM: Removes a member from the sorted set.
- ZDIFF, ZINTER, ZUNION: Computes the set difference, intersection, and union of two sorted sets, respectively.
- ZDIFFSTORE, ZINTERSTORE, ZUNIONSTORE: Stores the result of set difference, intersection, and union in a new sorted set.
- ZINTERCARD: Returns the cardinality (number of members) in the result of set intersection.
- ZPOPMIN: Removes and returns members with the lowest scores from the sorted set.
- ZPOPMAX: Removes and returns members with the highest scores from the sorted set.
- ZSCORE: Returns the score of a given member.
- ZRANK: Returns the rank (position) of a member in the sorted set.
- ZCOUNT: Returns the count of members with scores within a specified range.

# Redis Lists
- LPUSH/RPUSH: Add a new node with the given value to the head or tail, respectively.
- LPUSHX/RPUSHX: Add a new node with the given value only if the list exists.
- LPOP/RPOP: Remove and return one or more elements from the head or tail.
- LINDEX: Retrieve the element at a specified index.
- LSET: Set the value of the element at a specified index.
- LMOVE: Move the head of the list to another list.
- LLEN: Get the length of the list.
- LTRIM: Trim the list to a specified range of elements.
- LREM: Remove elements with a specified value, up to a specified count.
- LRANGE: Retrieve a range of elements from the list.
- printList: Print the elements of the list.
- DESTRUCTOR: Safely deallocates memory by deleting all nodes in the list.

# How to Use
1. **Build the Project**:
   - Ensure you have a C++ compiler installed.
   - Compile the project using the provided makefile or manually compile the source files.
2. **Run the Program**:
   - Execute the main program.
   - Follow the menu to select and run the desired data structure operations.
3. **Test the Commands**:
   - Use the provided text files to load and test various commands for each data structure.
   - Modify the text files to test different scenarios and check the performance of all commands.
   - Syntax for commands should be in capital words and follow the format similar to Redis files.

# Author
**Kainat Umar** - *Developer of the `Redis-Database` Project in CPP using Data Structures.*
