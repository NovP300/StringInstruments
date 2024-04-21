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

wstring PrintInstrumentType(const InstrumentType type) // функция для вывода названия инструмента
{
    switch(type)
    {
        case InstrumentType::Violin: return L"Скрипка";
        case InstrumentType::Alto: return L"Альт";
        case InstrumentType::Cello: return L"Виолончель";
        case InstrumentType::Contrabass: return L"Контрабас";
        default: return L"Неизвестно";
    }
}

//////////////////////////////////////
//ЗАДАНИЯ ДЛЯ ПРОВЕРКИ КОНТЕЙНЕРОВ Task1 и Task2

//Вывести все настроенные инструменты
void Task1(InstrumentContainer *Box)
{
    wcout << L"Настроенные инструменты:" << endl;
    for(int i = 0; i < Box->GetCount(); i++)
    {
        const InstrPtr currentInstrument = Box->getInstrument(i);
        if(currentInstrument->IsTuned())
        {
            wcout << i << L" " << PrintInstrumentType(currentInstrument->GetType()) << endl;
        }
    }
}


//Поиграть на всех пригодных (настроенный, чистый, 4 струны) для игры инструментах

void Task2(InstrumentContainer *Box)
{
    wcout << L"Игра на инструментах" << endl;
    for(int i = 0; i < Box->GetCount(); i++)
    {
        const InstrPtr currentInstrument = Box->getInstrument(i);
        if(currentInstrument->IsTuned() && currentInstrument->IsCleaned() && (currentInstrument->GetStringsNumber() == 4))
        {
            wcout << i << L" " ;
            currentInstrument->Play();
            cout << endl;
        }
    }
}

////////////////////////////////////////////////////////

int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); // генератор случайных чисел
    setlocale(LC_ALL,"Russian");

    VectorContainer Box;
    //ArrayContainer Box(60);

    for(int i = 0; i < 15; i++)
    {
        Box.addInstrument(new Violin);
    }
    for(int i = 0; i < 15; i++)
    {
        Box.addInstrument(new Cello);
    }
    for(int i = 0; i < 15; i++)
    {
        Box.addInstrument(new Alto);
    }
    for(int i = 0; i < 15; i++)
    {
        Box.addInstrument(new Contrabass);
    }

    //Task1(&Box);
    Task2(&Box);



    return 0;
}
