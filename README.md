# OSIM: Organizational Simulation & Internal Management System

![Language](https://img.shields.io/badge/Language-C++-00599C?style=for-the-badge&logo=c%2B%2B)
![Security](https://img.shields.io/badge/Security-RBAC%20%7C%20MFA%20%7C%20Audit-red?style=for-the-badge)
![Type](https://img.shields.io/badge/Architecture-No%20STL-lightgrey?style=for-the-badge)

## 🛡️ Project Overview

OSIM is a terminal-based simulation of a secure corporate environment developed in C++. It simulates the internal workflows of an organization, focusing heavily on **Identity and Access Management (IAM)** and **Security Operations**.

Unlike standard C++ applications, this project was built **without the Standard Template Library (STL)**. Every data structure, string manipulation, and file handling operation is manually implemented, demonstrating granular control over memory allocation and buffer management.

## 🔐 Security Features

### 1. Identity & Access Management (IAM)
* **Role-Based Access Control (RBAC):** Implements a strict `PolicyEngine` that governs permissions across four clearance levels: Junior, Manager, Director, and Executive.
* **Multi-Factor Authentication (MFA):** Requires a Time-based One-Time Password (OTP) simulation upon login to verify user identity.
* **Credential Hashing:** User passwords are not stored in plain text. (Note: Academic implementation using shift-based obfuscation).

### 2. Threat Detection & Telemetry
* **Immutable Audit Logging:** All sensitive actions (Logins, Task Reassignments, Permission Changes) are written to a tamper-evident `audit.txt` file.
* **Anomaly Detection Engine:** A dedicated module parses audit logs in real-time to detect specific threat signatures:
    * *Brute Force Attacks:* Detects repeated `FAILED_CREDENTIALS` events.
    * *Suspicious Activity:* Flags unusual `FAILED_OTP` sequences.

### 3. Secure Communications
* **End-to-End Encryption:** The `PrivateMessage` class implements custom encryption logic, ensuring message content is obfuscated on disk (`inbox.txt`) and only decrypted for the intended recipient.
* **Input Masking:** Password entry is masked in the terminal to prevent shoulder surfing.

## 🏗️ Technical Architecture

### Core Constraints
* **No STL:** No `std::vector`, `std::string`, or smart pointers.
* **Manual Memory Management:** extensive use of `new` and `delete` to manage object lifecycles and prevent memory leaks.
* **Polymorphism:** Utilizes virtual functions for the Employee hierarchy (`Junior` -> `Executive`) and Message types.

### Class Structure
* **`PolicyEngine`**: The centralized authority for checking permissions.
* **`CredentialManager`**: Handles authentication, decryption, and session initiation.
* **`AnomalyDetection`**: The "security watchdog" analyzing log patterns.
* **`AuditLogger`**: Singleton-based logger for maintaining the chain of custody for events.

## 🚀 Getting Started

### Prerequisites
* A C++ Compiler (GCC/G++ recommended)

### Compilation
Since this project does not use external libraries, you can compile it directly using g++:

```bash
g++ *.cpp -o osim_secure
