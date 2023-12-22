#include <SPI.h>

const int blueLed = 2;     

char data_received[100];
char count;
volatile bool rx_complete; 

void setup() {
  Serial.begin(115200);

  SPCR |= bit(SPE);      
  pinMode(MISO, OUTPUT); 
  count = 0;
  rx_complete = false;
  SPI.attachInterrupt(); 

  pinMode(blueLed, OUTPUT);
}

void loop() {

  while(rx_complete == false){
    Serial.println(data_received);
  }

  if (rx_complete && data_received == "Enable 2") {
    Serial.println("Complete" );
    Serial.println(data_received);
  }
  else if (rx_complete == false && data_received != "Enable 2"){
    data_received[count] = 0;
    rx_complete = false;
  }

  while(rx_complete){
    Serial.println("Got it!" );
    receivedLed();
  }

    delay(1000);

}


void receivedLed(){
    for (int i = 0; i < 5; i++) {
    digitalWrite(blueLed, HIGH);  // Turn the LED on
    delay(500);                   // Wait for 0.5 seconds
    digitalWrite(blueLed, LOW);   // Turn the LED off
    delay(500);                   // Wait for 0.5 seconds
  }

}

 

// SPI interrupt routine
ISR(SPI_STC_vect) {
  uint8_t oldsrg = SREG;
  cli();
  char var = SPDR;
  if (count < sizeof(data_received)) {
    data_received[count++] = var;
    if (var == '2') { /* Check for newline character as end of msg */
      rx_complete = true;
    }1
  }
  SREG = oldsrg;
}
