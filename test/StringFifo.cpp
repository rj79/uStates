#include "StringFifo.h"

StringFifo::StringFifo(uint8_t capacity)
{
    if (capacity < 1) {
        capacity = 1;
    }
    Capacity = capacity;
    Head = 0;
    Tail = 0;
    Usage = 0;
    Stack = new String[Capacity];
}

StringFifo::~StringFifo()
{
    delete[] Stack;
}

void StringFifo::Push(String text)
{
    if (Usage == Capacity) {
        abort();
    }
    Stack[Head] = text;
    ++Usage;
    ++Head;
    if (Head == Capacity) {
        Head = 0;
    }
}

String StringFifo::Pop()
{
    if (Usage == 0) {
        return "";
    }
    String result = Stack[Tail];
    --Usage;
    ++Tail;
    if (Tail == Capacity) {
        Tail = 0;
    }
    return result;
}