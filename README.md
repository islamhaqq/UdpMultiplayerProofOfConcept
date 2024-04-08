Sure, here's a README.md that summarizes your project and provides guidance on how to build and run the server and clients.

---

# Simple Multiplayer Server-Client Architecture

## Overview

This repository contains a simple proof of concept for a multiplayer server-client architecture using UDP for network communication. The project demonstrates a basic health/damage replication system where multiple clients can connect to a server, simulate taking damage, and have their health states replicated across all connected clients.

The server is responsible for keeping track of each client's health, processing damage reports, and broadcasting updated health information to all clients. Clients are capable of sending random damage reports to the server and listening for broadcasts to update their view of all clients' health.

## Features

- UDP-based networking for simple message passing.
- Server tracks and broadcasts client health.
- Clients send random damage reports and listen for updates from the server.
- Use of Boost Asio for asynchronous IO operations.

## Prerequisites

- C++17 compliant compiler
- Boost libraries (specifically Boost Asio)
- CMake (version 3.5 or higher)

Ensure that the Boost libraries are properly installed and available for your compiler to link against. This project has been tested with Boost 1.75.0, but it should work with other versions that are compatible with Boost Asio.

## Building the Project

1. **Clone the Repository**

   ```
   git clone <repository-url>
   cd <repository-directory>
   ```

2. **Generate Build Files**

   From the root of the project directory, run:

   ```
   cmake .
   ```

   This will generate the necessary build files for your platform.

3. **Build the Server and Client**

   Depending on your build system, the command might vary. For Makefile-based systems, you can simply run:

   ```
   make
   ```

   This should compile both the server and client executables.

## Running the Application

1. **Start the Server**

   Open a terminal and run:

   ```
   ./server
   ```

   This will start the server on UDP port 12345 and wait for incoming connections and messages from clients.

2. **Run Multiple Clients**

   Open one or more separate terminal windows. In each, run:

   ```
   ./client
   ```

   Each client will automatically connect to the server, periodically simulate taking damage, and print updates on all clients' health as broadcasted by the server.

## How It Works

- The server listens for incoming UDP packets on port 12345.
- Clients send a "Damage" message followed by a random damage amount to the server at random intervals.
- The server processes these messages, updates the respective client's health, and then broadcasts the updated health of all clients to every connected client.
- Clients display the broadcasted updates, showing the current health status of all clients, including themselves.

## Limitations and Next Steps

This project is a proof of concept and as such, has several limitations:

- Lack of encryption or secure communication.
- Minimal error handling and no reconnection logic.
- Health updates are broadcasted to all clients, regardless of necessity, leading to potential scalability issues.
