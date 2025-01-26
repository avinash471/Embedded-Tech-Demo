#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL  // CPU clock frequency
#define BAUD_RATE 9600    // UART baud rate
#define UBRR_VALUE ((F_CPU / (16UL * BAUD_RATE)) - 1)
#define EEPROM_SIZE 1024  // EEPROM size for ATmega328P

volatile uint16_t eepromIndex = 0;  // EEPROM write index
volatile uint16_t totalBits = 0;   // Total bits transmitted
volatile char receivedChar;        // Received character buffer

// Function prototypes
void UART_Init(uint16_t baud_rate);
void UART_Transmit(char data);
char UART_Receive(void);
void UART_SendString(const char *str);
void EEPROM_Write(uint16_t address, char data);
char EEPROM_Read(uint16_t address);
void EEPROM_Clear(void);
void SendBackStoredData(void);

int main(void) {
    // Initialize UART with higher baud rate
    UART_Init(BAUD_RATE);

    // Clear EEPROM for a fresh start
    EEPROM_Clear();

    while (1) {
        // Receive a character
        receivedChar = UART_Receive();

        // Store received character in EEPROM
        EEPROM_Write(eepromIndex, receivedChar);
        eepromIndex++;

        // Increment total bits (1 character = 8 bits)
        totalBits += 8;

        // Echo received character back to sender
        UART_Transmit(receivedChar);

        // Check for end of message or EEPROM limit
        if (receivedChar == '\n' || eepromIndex >= EEPROM_SIZE) {
            // Notify that data has been received successfully
            UART_SendString("\nData received successfully!\n");
            UART_SendString("Total Bits Processed: ");
            char bitCountStr[10];
            itoa(totalBits, bitCountStr, 10);
            UART_SendString(bitCountStr);
            UART_SendString("\n");

            // Add a delay to simulate processing time
            _delay_ms(2000);  // 2-second delay before sending data back

            // Send back stored data from EEPROM
            SendBackStoredData();

            // Reset for the next transmission
            eepromIndex = 0;
            totalBits = 0;
        }
    }

    return 0;
}

// Initialize UART with specified baud rate
void UART_Init(uint16_t baud_rate) {
    uint16_t ubrr_value = (F_CPU / (16UL * baud_rate)) - 1;
    UBRR0H = (uint8_t)(ubrr_value >> 8);
    UBRR0L = (uint8_t)ubrr_value;

    // Enable UART transmitter and receiver
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Transmit a character via UART
void UART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait for the transmit buffer to be empty
    UDR0 = data;                      // Send data
}

// Receive a character via UART
char UART_Receive(void) {
    while (!(UCSR0A & (1 << RXC0)));  // Wait for data to be received
    return UDR0;                      // Return received data
}

// Transmit a string via UART
void UART_SendString(const char *str) {
    while (*str) {
        UART_Transmit(*str++);
    }
}

// Write a byte to EEPROM
void EEPROM_Write(uint16_t address, char data) {
    if (address < EEPROM_SIZE) {
        while (EECR & (1 << EEPE));  // Wait for completion of previous write
        EEAR = address;             // Set address register
        EEDR = data;                // Set data register
        EECR |= (1 << EEMPE);       // Enable master write
        EECR |= (1 << EEPE);        // Start EEPROM write
    }
}

// Read a byte from EEPROM
char EEPROM_Read(uint16_t address) {
    if (address < EEPROM_SIZE) {
        while (EECR & (1 << EEPE));  // Wait for completion of previous write
        EEAR = address;             // Set address register
        EECR |= (1 << EERE);        // Start EEPROM read
        return EEDR;                // Return data
    }
    return 0;
}

// Clear EEPROM
void EEPROM_Clear(void) {
    for (uint16_t i = 0; i < EEPROM_SIZE; i++) {
        EEPROM_Write(i, 0);
    }
}

// Send back all stored data from EEPROM
void SendBackStoredData(void) {
    UART_SendString("Sending back stored data:\n");
    for (uint16_t i = 0; i < eepromIndex; i++) {
        char data = EEPROM_Read(i);  // Read data from EEPROM
        UART_Transmit(data);         // Send data back to PC
    }
    UART_SendString("\nData transmission complete!\n");
}
