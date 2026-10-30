#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <chrono>

// Key ke sath uska expiration time track karne ke liye structure
struct CacheItem {
    std::string value;
    std::chrono::steady_clock::time_point expires_at;
    bool has_expiry = false;
};

class KeyValueStore {
private:
    std::unordered_map<std::string, CacheItem> store;
    const std::string filename = "dump.txt";

public:
    KeyValueStore() {
        loadFromFile();
    }

    // 1. SET Command
    void set(const std::string& key, const std::string& value) {
        store[key] = {value, {}, false};
        std::cout << "OK" << std::endl;
    }

    // 2. EXPIRE Command: Kisi key ko seconds baad expire karne ke liye
    void expire(const std::string& key, int seconds) {
        auto it = store.find(key);
        if (it != store.end()) {
            auto expiry_time = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
            it->second.expires_at = expiry_time;
            it->second.has_expiry = true;
            std::cout << "(integer) 1" << std::endl;
        } else {
            std::cout << "(integer) 0" << std::endl;
        }
    }

    // 3. GET Command (Expiration check ke sath)
    std::string get(const std::string& key) {
        auto it = store.find(key);
        if (it == store.end()) {
            return "(nil)";
        }

        // Check karein ki key expire toh nahi ho gayi
        if (it->second.has_expiry) {
            if (std::chrono::steady_clock::now() > it->second.expires_at) {
                store.erase(it); // Expire ho gayi toh delete kar do
                return "(nil)";
            }
        }

        return it->second.value;
    }

    // 4. DEL Command
    void del(const std::string& key) {
        if (store.erase(key)) {
            std::cout << "(integer) 1" << std::endl;
        } else {
            std::cout << "(integer) 0" << std::endl;
        }
    }

    // 5. SAVE Command
    void saveToFile() {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cout << "ERR saving to file" << std::endl;
            return;
        }

        auto now = std::chrono::steady_clock::now();
        for (auto it = store.begin(); it != store.end(); ) {
            // Save karte waqtexpired items ko hata do
            if (it->second.has_expiry && now > it->second.expires_at) {
                it = store.erase(it);
            } else {
                outFile << it->first << " " << it->second.value << "\n";
                ++it;
            }
        }
        std::cout << "OK (Data persisted to disk)" << std::endl;
    }

    void loadFromFile() {
        std::ifstream inFile(filename);
        if (!inFile) {
            return;
        }

        std::string key, value;
        while (inFile >> key >> value) {
            store[key] = {value, {}, false};
        }
    }
};

int main() {
    KeyValueStore kv;
    std::string command, key, value;
    int seconds;

    std::cout << "--- Mini Redis Store (Phase 3 with TTL) Initialized ---" << std::endl;

    while (true) {
        std::cout << "> ";
        if (!(std::cin >> command)) {
            break;
        }

        if (command == "SET" || command == "set") {
            std::cin >> key >> value;
            kv.set(key, value);
        } 
        else if (command == "EXPIRE" || command == "expire") {
            std::cin >> key >> seconds;
            kv.expire(key, seconds);
        }
        else if (command == "GET" || command == "get") {
            std::cin >> key;
            std::cout << kv.get(key) << std::endl;
        } 
        else if (command == "DEL" || command == "del") {
            std::cin >> key;
            kv.del(key);
        } 
        else if (command == "SAVE" || command == "save") {
            kv.saveToFile();
        } 
        else if (command == "EXIT" || command == "exit") {
            std::cout << "Exiting..." << std::endl;
            break;
        } 
        else {
            std::cout << "ERR unknown command: " << command << std::endl;
        }
    }

    return 0;
}