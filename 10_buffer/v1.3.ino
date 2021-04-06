#include <MD_CirQueue.h>

// Veličina datoteke - definirana u (Uno)NodeMCU programu (StaticJsonDocument<DATA_SIZE> doc;)
// Veličina - Bytes
//#define DATA_SIZE 550
#define DATA_SIZE 120
char banana1[DATA_SIZE] = "https://stackoverflow.com/questions/30101872/sizeofchar-in-c/30102186";
char banana2[DATA_SIZE] = "https://github.com/0mnitron/Zavrsni_rad/blob/main/10_buffer/v1.2.ino";
char banana3[DATA_SIZE] = "https://github.com/0mnitron/Zavrsni_rad/tree/main/07_Uno-ESP_komunikacija";
char banana4[DATA_SIZE] = "https://components101.com/development-boards/nodemcu-esp8266-pinout-features-and-datasheet";
char banana5[DATA_SIZE] = "https://majicdesigns.github.io/MD_CirQueue/class_m_d___cir_queue.html#a58dff950cf340099cd5dfd5a8e1372cb";  // usefull for later documentation

// Broj kopija
#define QUEUE_SIZE 4

// Define a queue.
MD_CirQueue Q(QUEUE_SIZE, DATA_SIZE);

// Display full/empty status of ring buffer.
void showStatus()
{
  if (Q.isEmpty()) Serial.print("\n->Q EMPTY");
  if (Q.isFull()) Serial.print("\n->Q FULL");
}

void setup()
{
  Serial.begin(57600);
  Serial.print("\n[CQ_Test]");

  Q.begin();
  
  //  push() with a full queue will overwrite the oldest item in the queue
  Q.setFullOverwrite(true);

  // Show status of (empty) ring buffer.
  showStatus();
/*
  // Try pushing 1 more item than can be queued.
  for (uint32_t i = 0; i < QUEUE_SIZE; i++){
    bool b;

    //b = Q.push((uint8_t *)&i);
    b = Q.push((uint8_t *)&banana);
    Serial.print("\nPush ");
    Serial.print(i);
    Serial.print(b ? " ok" : " fail");
    showStatus();
  }
*/
  bool b;
  b = Q.push((uint8_t *)&banana1);
  Serial.print("\nPush 1 ");
  Serial.print(b ? " ok" : " fail");
  showStatus();
  
  b = Q.push((uint8_t *)&banana2);
  Serial.print("\nPush 2 ");
  Serial.print(b ? " ok" : " fail");
  showStatus();
    
  b = Q.push((uint8_t *)&banana3);
  Serial.print("\nPush 3 ");
  Serial.print(b ? " ok" : " fail");
  showStatus();
  
  b = Q.push((uint8_t *)&banana4);
  Serial.print("\nPush 4 ");
  Serial.print(b ? " ok" : " fail");
  showStatus();

  b = Q.push((uint8_t *)&banana5);
  Serial.print("\nPush 5 ");
  Serial.print(b ? " ok" : " fail");
  showStatus();
    
  // Start popping bytes until buffer is empty again.
  while (!Q.isEmpty()){
    char n[DATA_SIZE];
    Q.pop((uint8_t *)&n);
    Serial.print("\nPopped ");
    Serial.print(n);
    showStatus();
  }
}


void loop(){
  
}
