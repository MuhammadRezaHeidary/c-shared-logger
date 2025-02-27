# 🚀 C++ Shared Memory Logger  
**Muhammad Reza Heidary**  [![Gmail](https://img.shields.io/badge/Gmail-D14836?logo=gmail&logoColor=white)](mailto:muhammadreza.heidary@gmail.com)  [![LinkedIn](https://img.shields.io/badge/LinkedIn-%230077B5.svg?logo=linkedin&logoColor=white)](https://www.linkedin.com/in/muhammad-reza-heidary/)  
*Embedded Systems Designer | C++ Developer | IoT & CCTV Surveillance Specialist*  

---

## ⚡ About This Repository  
A robust **C++ Shared Memory Logger** designed for logging messages from multiple sources efficiently and safely.  

✅ **Thread-Safe Logging** – Ensures safe concurrent logging without conflicts  
✅ **Shared Memory Utilization** – Uses `/dev/shm` for high-speed inter-process communication  
✅ **Producer-Consumer Model** – Separates data writing (producers) and log writing (consumer)  
✅ **Configurable Log Format** – Supports **Text, HTML, and JSON** outputs  
✅ **Optimized for High-Performance** – Avoids race conditions and ensures smooth logging  

---

## 🛠️ Topics Covered  

🔹 **Shared Memory Management**  
🔹 **Atomic Operations & Synchronization**  
🔹 **Log File Writing from Multiple Sources**  
🔹 **Safe & Conflict-Free Logging**  
🔹 **Producer-Consumer Pattern for Logging**  
🔹 **Customizable Log Levels & Formats**  

---

## 🗒 How This Logger Works  

🔹 **Multiple producer tasks** write log messages into shared memory  
🔹 **A dedicated logger service (consumer)** reads from shared memory and writes to a file  
🔹 **Supports concurrent logging** from multiple files or processes without data corruption  
🔹 **Uses shared memory located at `/dev/shm`** for efficient logging  
🔹 **Ensures safe logging even when multiple tasks write simultaneously**  
🔹 **Log messages are stored in a queue structure with a max size of 10 messages**  

---

## 🔧 Shared Memory Configuration  
Shared memory is used to store log messages before they are written to a file. The configuration includes:

- **Maximum messages**: Defines how many log messages can be stored before being processed. The default value is 10.
- **Message size**: Each log message can have a maximum size of **1MB**.

```cpp
#define MEKATRONIK_LOGGER_MAX_MESSAGES 10
#define MEKATRONIK_LOGGER_MESSAGE_SIZE 1024*1024 // 1 MByte
```

Shared memory is created in `/dev/shm` and can be checked using:
```bash
ls -lh /dev/shm
```
You can also inspect active shared memory segments using:
```bash
ipcs -m
```

---

## ⚙️ Available Log Levels  

```cpp
enum class LogLevel { info, warning, error };
```

---

## 📄 Supported Log Formats  

```cpp
enum class LogFormat { TEXT, HTML, JSON };
```

---

## 📂 Example Usage  
This project includes multiple examples inside the `example/` folder to demonstrate different logging formats.

### 🔧 Building the Project  
This project is built using **CMake**. To compile:
```bash
mkdir build && cd build
cmake ..
make
```

### 🚀 Running Examples  
#### Log in Terminal:  
```bash
cd example/log_terminal/
./log_terminal
```

#### Log in HTML Format:  
```bash
cd example/log_html_file
./log_html_consumer_animal & ./log_html_producer_cat & ./log_html_producer_dog
```

#### Log in JSON Format:  
```bash
cd example/log_json_file
./log_json_consumer_car & ./log_json_producer_irankhodro & ./log_json_producer_saipa
```

#### Log in Text Format (Default):  
```bash
cd example/log_text_file
./log_text_consumer_sports & ./log_text_producer_football & ./log_text_producer_kungfu
```

---

💡 **Let's collaborate on high-performance logging solutions!** 🚀  

