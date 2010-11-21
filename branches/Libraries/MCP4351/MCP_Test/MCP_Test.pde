#include <MCP4351.h>

void setup(){
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    MCP.init();
    Serial.println("MCP Initialized");
    
}

void loop(){
    uint8_t h = MCP.setCurrent(1, 1.0);
    Serial.println("Returned byte:  ");
    Serial.println(h,HEX);
    delay(1000);
    
    
}
