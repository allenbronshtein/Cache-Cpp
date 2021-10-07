#ifndef EX2_EX2_H
#define EX2_EX2_H

#include<iostream>
#include<string>
#include <list>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <functional>

using namespace std;

template<class T>
class CacheManager {
    string className;
    int size;
    list<string> listByOrder;
    unordered_map<string, pair<T, list<string>::iterator>> unorderedMap;
public:
    CacheManager(int size) {
        this->size = size;
        className = T::class_name;
    }

    void insert(string key, T object) {
        fstream out(className + "_" + key, ios::out | ios::binary);
        if (unorderedMap.find(key) != unorderedMap.end()) {
            auto temp = unorderedMap.find(key);
            listByOrder.remove(*temp->second.second);
            unorderedMap.erase(key);
        }
        if (listByOrder.size() + 1 > size) {
            string last = listByOrder.back();
            listByOrder.erase(--listByOrder.end());
            unorderedMap.erase(last);
        }
        //insert new value
        listByOrder.push_front(key);
        pair<T, list<string>::iterator> pair;
        pair.first = object;
        pair.second = listByOrder.begin();
        unorderedMap[key] = pair;
        out.write((char *) &object, sizeof(object));
    }

    T get(string key) {
        T object;
        //exists in cache
        if (unorderedMap.find(key) != unorderedMap.end()) {
            auto temp = unorderedMap.find(key);
            listByOrder.remove(*temp->second.second);
            listByOrder.push_front(*temp->second.second);
            return temp->second.first;
        }
        //doesnt exist in cache but exists in file system
        ifstream in(className + "_" + key, ios::in | ios::binary);
        if (!in) { //cannot open file
            throw "Error";
        }
        in.read((char *) &object, sizeof(object));
        insert(key, object);
        return object;
    }

    void foreach(const function<void(T &)> func) {
        vector<string> vec;
        for (string str : listByOrder) {
            vec.push_back(str);
        }
        T object;
        for (std::list<string>::iterator it = listByOrder.begin(); it != listByOrder.end(); it++) {
            func(unorderedMap[*it].first);
        }
    }
};

#endif //EX2_EX2_H
