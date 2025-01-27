# Firmware Task: UART Communication and EEPROM Storage

## Overview
This project demonstrates a UART-based communication system where text data is transmitted:
1. From a PC to an MCU (ATmega328P).
2. Stored in the MCU's EEPROM.
3. Transmitted back from the MCU to the PC.

The real-time data transmission speed (in bits/second) is displayed during both sending and receiving phases.

## Features
- **Real-Time Speed Calculation**: Displays live transmission speed during UART communication.
- **EEPROM Operations**:
  - Byte-by-byte storage of received data.
  - Retrieval and transmission of stored data back to the PC.
- **Data Integrity**: Ensures data sent matches data received.

## Components Used
- **Microcontroller**: ATmega328P (Arduino Uno)
- **EEPROM**: 1024 bytes (internal to the ATmega328P)
- **PC**: Running Python script for data transmission and reception
- **Communication Interface**: UART at 2400 baud

## System Architecture

1. **PC-Side Script**:
   - Reads text data from `data.txt`.
   - Sends data character-by-character to the MCU over UART.
   - Displays real-time transmission and reception speeds.
   - Logs the received data for verification.

2. **MCU Firmware**:
   - Receives data from the PC and stores it in EEPROM.
   - Sends the stored data back to the PC upon receiving the complete dataset.
   - Calculates and tracks the total bits transmitted.

## How to Use

### Hardware Setup
1. Connect the MCU (ATmega328P) to the PC via a serial interface.
2. Ensure the baud rate is set to 2400.

### Software Setup

#### PC-Side Script
1. Install Python 3.x.
2. Install the required libraries using:
   ```bash
   pip install pyserial
   ```
3. Place the `data.txt` file (containing the text to be transmitted) in the same directory as the script.
4. Run the script:
   ```bash
   python pc_side_script.py
   ```

#### MCU Firmware
1. Flash the provided firmware to the ATmega328P using any AVR programmer or Arduino IDE.
2. Verify the UART connection is configured correctly.

## Expected Outputs

### PC Console Output
- Real-time transmission speed during both sending and receiving phases.
- Total bits transmitted and received.
- Verification message indicating successful data reception.

### Example Log
```
Starting script...
Connected to COM9 at 2400 baud
Sending data to MCU...
Sending Speed: 728.79 bits/second
Data Transmission to MCU Completed!
Total Bits Sent: 8072
Elapsed Time for Sending: 11.086 seconds
Average Sending Speed: 728.10 bits/second

Receiving data from MCU...
Receiving Speed: 175853.76 bits/second
Data Reception from MCU Completed!
Total Bits Received: 8072
Elapsed Time for Receiving: 0.046 seconds
Average Receiving Speed: 175797.15 bits/second

Received Data:
[Text Data]
Data received successfully!
Total Bits Processed: 8192
```

## Challenges and Solutions

### Challenges
1. **Real-Time Speed Measurement**:
   - Calculating speed accurately during high-speed data transmission.
2. **EEPROM Overflow**:
   - Ensured data size does not exceed the 1024-byte EEPROM limit.

### Solutions
- Added checks to prevent data overflow.
- Used precise timing mechanisms to measure transmission speeds.

## Improvements
1. Fine-tune speed calculations to handle high-speed data reception accurately.
2. Add checksum or CRC verification for enhanced data integrity.

## Repository
All code for this project is available on GitHub: [Embedded-Tech-Demo](https://github.com/avinash471/Embedded-Tech-Demo)


