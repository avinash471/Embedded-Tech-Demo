import serial
import time
import os

try:
    print("Starting script...")
    
    ser = serial.Serial('COM9', 9600, timeout=5)
    print(f"Connected to {ser.port} at {ser.baudrate} baud")

    # Path to the text file
    file_path = "data.txt" 

    # Check if the file exists
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"File '{file_path}' not found in the current folder.")

    # Read the text from the file
    with open(file_path, 'r', encoding='utf-8') as file:
        text_to_send = file.read()

    total_bits_sent = 0
    total_bits_received = 0

    # Sending data
    print("Sending data to MCU...")
    start_time_send = time.time()

    for char in text_to_send:
        ser.write(char.encode())
        total_bits_sent += 8  # 1 character = 8 bits

        # Real-time speed tracking during sending
        elapsed_time_send = time.time() - start_time_send
        if elapsed_time_send > 0:
            speed_send = total_bits_sent / elapsed_time_send
            print(f"Sending Speed: {speed_send:.2f} bits/second", end="\r")

        time.sleep(0.01)  # Simulate delay

    # Completed sending
    elapsed_time_send = time.time() - start_time_send
    print("\nData Transmission to MCU Completed!")
    print(f"Total Bits Sent: {total_bits_sent}")
    print(f"Elapsed Time for Sending: {elapsed_time_send:.3f} seconds")
    print(f"Average Sending Speed: {total_bits_sent / elapsed_time_send:.2f} bits/second")

    # Receiving data
    print("\nReceiving data from MCU...")
    received_data = ""
    start_time_receive = time.time()

    while True:
        if ser.in_waiting > 0:  # Check if data is available
            char = ser.read().decode()
            received_data += char
            total_bits_received += 8  # 1 character = 8 bits

            # Real-time speed tracking during receiving
            elapsed_time_receive = time.time() - start_time_receive
            if elapsed_time_receive > 0:
                speed_receive = total_bits_received / elapsed_time_receive
                print(f"Receiving Speed: {speed_receive:.2f} bits/second", end="\r")

        # Stop receiving when the full message is received
        if len(received_data) >= len(text_to_send):
            break

    # Completed receiving
    elapsed_time_receive = time.time() - start_time_receive
    print("\nData Reception from MCU Completed!")
    print(f"Total Bits Received: {total_bits_received}")
    print(f"Elapsed Time for Receiving: {elapsed_time_receive:.3f} seconds")
    print(f"Average Receiving Speed: {total_bits_received / elapsed_time_receive:.2f} bits/second")

    # Print received data (optional)
    print("\nReceived Data:")
    print(received_data)

except FileNotFoundError as e:
    print(f"File error: {e}")

except serial.SerialException as e:
    print(f"Serial error: {e}")

except Exception as e:
    print(f"An unexpected error occurred: {e}")

finally:
    # Close the serial connection
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Serial connection closed.")
print("Script ended.")
