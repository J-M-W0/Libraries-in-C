# 📚 Libraries-in-C

A collection of self-designed libraries written in C, compatible with Unix-based systems. These libraries aim to bring modern programming concepts like generics, move semantics, and ergonomic data structures into plain C development.

---

## 📦 Included Libraries

### 🎨 `crayon.h`  
Macros for printing colorful terminal output using ANSI escape codes.

---

### 🧪 `assertf.h`, `assertf.c`  
A custom assertion library offering formatted error messages and debugging context.

---

### 🧠 `cgeneric.h`, `cgeneric.c`  
A generic programming utility for C, including a **self-designed borrow-checker** to simulate safe memory access patterns.

---

### ⚙️ `cenv.h`, `cenv.c`  
A lightweight library for **parsing environment-style configuration files**, ideal for UNIX `.env` style setups.

---

### 🔑 `ckvmap.h`, `ckvmap.c`  
A key-value map (hash map) data structure implementation with basic collision handling.

---

### 📦 `cvector.h`, `cvector.c`  
A dynamic array (vector) structure that supports resizing and memory-safe operations.

---

### ✂️ `cstring.h`, `cstring.c`  
A safer string handling library built on top of `char*`, featuring length-tracking and buffer protection.

---

### 🔗 `clinkedlist.h`, `clinkedlist.c`  
A flexible and extensible linked list implementation supporting both singly and doubly linked variants.

---

### 🚚 `cmove.h`, `cmove.c`  
Implements basic **move semantics** in C to simulate resource ownership transfer.

---

### 🌐 `chttp.h`, `chttp.c`  
A minimal HTTP client library for sending GET/POST requests in C with socket-based networking.

---

## 🔧 Example Usage

```bash
clang -c -O3 /opt/localc/csources/assertf.c -o assertf.o
ar rcs libassertf.a assertf.o
