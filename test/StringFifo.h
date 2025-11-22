#ifndef STRING_FIFO_H
#define STRING_FIFO_H

#include <Arduino.h>

class StringFifo
{
private:
    String* Stack;
    uint8_t Capacity;
    uint8_t Usage;
    uint8_t Head;
    uint8_t Tail;

public:
    StringFifo(uint8_t capacity=16);
    ~StringFifo();
    void Push(String text);
    String Pop();
    bool IsEmpty() const;
};

#endif
