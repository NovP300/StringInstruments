#include <iostream>
#include <cstdlib>
#include<ctime>
#include <vector>

using namespace std;

#include "main.h"

StringInstrument::StringInstrument() // конструктор родительского класса
{                                    // Пусть контейнеры не очень надежные, и инструмент может запачкаться или у него порвутся струны, поэтому заполняются случайными значениями.
    Tuned = bool(rand() % 2);
    Type = InstrumentType::Unknown;
    Cleaned = bool(rand() % 2);
    StringsNumber = rand() % 5;
}

ArrayContainer::ArrayContainer(int maxSize)
{
    Instruments = new InstrPtr[maxSize];
    for(int i = 0; i < maxSize; i++)
    {
        Instruments[i] = NULL;
    }
    CurrentSize = 0;
    MaxSize = maxSize;
}


ArrayContainer::~ArrayContainer()
{
    for(int i = 0; i < MaxSize; i++)
    {
        if(Instruments[i] != NULL)
        {
            delete Instruments[i];
            Instruments[i] = NULL;
        }
    }
    delete[] Instruments;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); // генератор случайных чисел
    setlocale(LC_ALL,"Russian");
    cout << "Hello world!!!" << endl;
    return 0;
}
