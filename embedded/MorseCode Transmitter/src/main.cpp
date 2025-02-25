// Arduino Morse Code Transmitter
// Accepts text input via Serial Monitor and converts to Morse code
// Outputs Morse code via LED blink and buzzer sound

#include <Arduino.h>

const int ledPin = 13;   
const int buzzerPin = 8;  

// Morse code timing (in milliseconds)
const int dotDuration = 200;
const int dashDuration = dotDuration * 3;
const int elementSpace = dotDuration;      
const int letterSpace = dotDuration * 3;   
const int wordSpace = dotDuration * 7;    

// Morse code mapping
const String morseCodeMap[] = {
  ".-",    // A
  "-...",  // B
  "-.-.",  // C
  "-..",   // D
  ".",     // E
  "..-.",  // F
  "--.",   // G
  "....",  // H
  "..",    // I
  ".---",  // J
  "-.-",   // K
  ".-..",  // L
  "--",    // M
  "-.",    // N
  "---",   // O
  ".--.",  // P
  "--.-",  // Q
  ".-.",   // R
  "...",   // S
  "-",     // T
  "..-",   // U
  "...-",  // V
  ".--",   // W
  "-..-",  // X
  "-.--",  // Y
  "--.."   // Z
};

const String morseCodeDigits[] = {
  "-----", // 0
  ".----", // 1
  "..---", // 2
  "...--", // 3
  "....-", // 4
  ".....", // 5
  "-....", // 6
  "--...", // 7
  "---..", // 8
  "----."  // 9
};

String inputMessage = "";
bool messageReady = false;

String charToMorse(char c) {
  if (c >= 'A' && c <= 'Z') {
    return morseCodeMap[c - 'A'];
  } else if (c >= '0' && c <= '9') {
    return morseCodeDigits[c - '0'];
  } else {
    // Handle special characters if needed
    switch (c) {
      case '.': return ".-.-.-";
      case ',': return "--..--";
      case '?': return "..--..";
      case '/': return "-..-.";
      case '@': return ".--.-.";
      default: return "";  // Return empty string for unsupported characters
    }
  }
}

void transmitDot() {
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 800, dotDuration);  // 800 Hz tone
  delay(dotDuration);
  digitalWrite(ledPin, LOW);
}

void transmitDash() {
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 800, dashDuration);  // 800 Hz tone
  delay(dashDuration);
  digitalWrite(ledPin, LOW);
}

void transmitMessage(String message) {
  message.toUpperCase();  // Convert to uppercase for easier processing
  
  for (int i = 0; i < message.length(); i++) {
    char c = message.charAt(i);
    
    if (c == ' ') {
      // Space between words
      delay(wordSpace);
      Serial.print("   ");  // Visual spacing for readability
    } else {
      String morseChar = charToMorse(c);
      
      if (morseChar != "") {
        // Transmit the character in Morse code
        Serial.print(c);
        Serial.print(": ");
        Serial.print(morseChar);
        Serial.print(" ");
        
        for (int j = 0; j < morseChar.length(); j++) {
          char element = morseChar.charAt(j);
          
          if (element == '.') {
            transmitDot();
          } else if (element == '-') {
            transmitDash();
          }
          
          // Space between elements (dots and dashes) within a character
          if (j < morseChar.length() - 1) {
            delay(elementSpace);
          }
        }
        
        // Space between letters
        delay(letterSpace);
      }
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Arduino Morse Code Transmitter");
  Serial.println("Enter a message and press Send");
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    // Add character to message if it's not a newline or carriage return
    if (c != '\n' && c != '\r') {
      inputMessage += c;
    } else if (c == '\n' && inputMessage.length() > 0) {
      // Set message as ready when we receive a newline
      messageReady = true;
    }
  }

  // Process the message if ready
  if (messageReady) {
    Serial.print("Message received: ");
    Serial.println(inputMessage);
    Serial.println("Transmitting in Morse code...");
    
    transmitMessage(inputMessage);
    
    // Reset for the next message
    inputMessage = "";
    messageReady = false;
    
    Serial.println("\nReady for next message");
  }
}