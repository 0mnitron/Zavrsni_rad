#include <MD_CirQueue.h>

// Veličina datoteke - definirana u (Uno)NodeMCU programu (StaticJsonDocument<DATA_SIZE> doc;)
// Veličina - Bytes
//#define DATA_SIZE 550
#define DATA_SIZE 80
char banana[DATA_SIZE]= "https://stackoverflow.com/questions/30101872/sizeofchar-in-c/30102186";

// Broj (Veličina) kopija?
#define QUEUE_SIZE 4

// Define a queue.
MD_CirQueue Q(QUEUE_SIZE, DATA_SIZE);
//MD_CirQueue Q(QUEUE_SIZE, sizeof(banana));

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
  //Q.setFullOverwrite(true);

  // Show status of (empty) ring buffer.
  showStatus();

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
