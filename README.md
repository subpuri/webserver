# 🌐 Multithreaded HTTP Web Server in C++

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20C++17-blue)]()

> A lightweight, multithreaded HTTP server written in **C++17**, using **POSIX sockets**, for learning low-level networking and concurrency on Linux.

---

## 📦 Features

- 🧵 **Multithreaded**: Each client request is handled in a separate thread.
- ⚡ **Non-blocking**: Fast and responsive even under concurrent requests.
- 🧱 **CMake-based**: Portable and structured build system.
- 🔌 **Low-level TCP sockets**: No external dependencies.
- 🌐 **Simple HTML response**: Serves basic static content.
- 📂 Easy to extend for serving real HTML/CSS/JS content.

---

## 🗂️ Directory Structure

```
webserver/
├── include/
│   └── http_tcpServer_linux.h     # TcpServer class declaration
├── src/
│   ├── http_tcpServer_linux.cpp   # TcpServer class implementation
│   └── server_linux.cpp           # Main entry point
├── build/                         # Build artifacts (added to .gitignore)
├── CMakeLists.txt                 # CMake project definition
├── .gitignore                     # Exclude build, object files, etc.
├── LICENSE                        # MIT License
└── README.md                      # Project documentation
```

---

## ⚙️ Getting Started

### ✅ Prerequisites

- Linux (Ubuntu or other)
- `g++` with C++17 support
- `cmake` ≥ 3.10
- `make`

### 🔧 Build Instructions

```bash
# 1. Clone the repo
git clone git@github.com:subpuri/webserver.git
cd webserver

# 2. Create build directory
mkdir build && cd build

# 3. Run cmake and make
cmake ..
make

# 4. Run the server
./webserver
```

> The server will start and listen on port `8080`.

---

## 🌍 Test the Server

### 🧪 1. Curl or browser

```bash
curl http://localhost:8080
```

**Output:**
```html
<!DOCTYPE html><html lang="en"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>
```

### 🧪 2. Python Multi-client Test Script

Install requests:

```bash
pip install requests
```

Save this as `test_connections.py`:

```python
import threading
import requests

def make_request(i):
    try:
        r = requests.get("http://localhost:8080")
        print(f"Client {i}: {r.status_code}")
    except Exception as e:
        print(f"Client {i} failed:", e)

threads = []
for i in range(20):  # Number of parallel clients
    t = threading.Thread(target=make_request, args=(i,))
    t.start()
    threads.append(t)

for t in threads:
    t.join()
```

Run the script:

```bash
python3 test_connections.py
```

You should see multiple simultaneous connections being handled by the server.

---

## 🎨 Planned Enhancements

- [ ] Serve static HTML/CSS/JS from files
- [ ] Add request parsing (GET, POST)
- [ ] Implement routing
- [ ] Logging improvements
- [ ] Graceful shutdown support

---

## 📝 License

This project is licensed under the [MIT License](LICENSE).

---

## 👨‍💻 Author

Built by [subpuri](https://github.com/subpuri) — learning systems one socket at a time.

---

## ❤️ Feedback

Have suggestions or found an issue? [Open an Issue](https://github.com/subpuri/webserver/issues) or contribute via a PR!
