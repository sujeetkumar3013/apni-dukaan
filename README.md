# C++ Custom Redis-Clone

A lightweight, high-performance in-memory key-value data store built from scratch in **C++**, inspired by Redis architecture. This project implements core database operations, TTL (Time-To-Live) key expiration, and RDB-style file persistence.

## 🚀 Features

- **In-Memory Operations**: Fast data lookup and insertion using STL `std::unordered_map`.
- **RDB-Style Persistence**: Persists in-memory state to disk (`dump.txt`) and automatically loads data on system startup.
- **Key Expiration (TTL)**: Precise expiration management using C++ `<chrono>` library (`EXPIRE` command).
- **Interactive CLI**: Robust terminal interface supporting case-insensitive core commands.

## 🛠️ Supported Commands

| Command | Syntax | Description |
| :--- | :--- | :--- |
| **SET** | `SET <key> <value>` | Stores a key-value pair in memory. |
| **GET** | `GET <key>` | Retrieves the value for a given key (handles expiration checks). |
| **EXPIRE** | `EXPIRE <key> <seconds>` | Assigns a Time-To-Live (TTL) duration to a key. |
| **DEL** | `DEL <key>` | Removes a key-value pair from memory. |
| **SAVE** | `SAVE` | Flushes in-memory data to `dump.txt` on disk. |
| **EXIT** | `EXIT` | Terminates the CLI application cleanly. |

## 💻 Building and Running

### Prerequisites
- C++11 compatible compiler (`g++` / `clang++`)

### Steps
1. Clone the repository:
   ```bash
   git clone [https://github.com/sujeetkumar3013/cpp-redis-clone.git](https://github.com/sujeetkumar3013/cpp-redis-clone.git)
   cd cpp-redis-clone