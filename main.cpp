#include <iostream>
#include <cstdlib>
#include<ctime>
#include <vector>
#include "sqlite3.h"

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
/////ЗАДАНИЕ ДЛЯ ПРОВЕРКИ ИТЕРАТОРА

//Вывести информацию о настройке, чистоте и количестве струн
void Task_It(Iterator<InstrPtr> *it)
{
    int i = 0;
    for(it->First(); !it->IsDone(); it->Next())
    {
        i++;
        cout << i << " ";
        const InstrPtr currentInstrument = it->GetCurrent();
        wcout<< "  "<<PrintInstrumentType(currentInstrument->GetType());
        wcout << L"чистота инструмента: " << (currentInstrument->IsCleaned() ? L"+" : L"-") << "  "<< L" ";
        wcout << L"настройка инструмента: " << (currentInstrument->IsTuned() ? L"+" : L"-") << " || "<< L" ";
        wcout << L"количество струн: " << (currentInstrument->GetStringsNumber()) << endl;
    }
}


///////////////////////////////////////////////
//ЗАДАНИЕ ДЕКОРАТОР

void DecoratorTask(Iterator<InstrPtr> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        const InstrPtr currentInstr = it->GetCurrent();
        wcout << PrintInstrumentType(currentInstr->GetType()) << L" : ";
        wcout << L" чистота" << (currentInstr->IsCleaned() ? L" +" : L" -") << " ";
        wcout << L" настройка" << (currentInstr->IsTuned() ? L" +" : L" -") << " ";
        wcout << L" число струн " << currentInstr->GetStringsNumber() << endl;
    }
}

//////////////////////////////////////////////////
//СОЗДАНИЕ ИНСТРУМЕНТОВ

StringInstrument *CreateInstrument(InstrumentType type)
{
    switch(type)
    {
        case InstrumentType::Violin: return new Violin;
        case InstrumentType::Alto: return new Alto;
        case InstrumentType::Cello: return new Cello;
        case InstrumentType::Contrabass: return new Contrabass;
        default: return nullptr; // Добавим обработку случая по умолчанию
    }
}

StringInstrument *StringInstrument::Create(InstrumentType type)
{
    return CreateInstrument(type);
}


//////////////////////////////////////////////////////////
//ЗАДАНИЕ БД

void Task_DB(Iterator<InstrumentInfo>* it) {
    for (it->First(); !it->IsDone(); it->Next()) {
        const InstrumentInfo currentInstrument = it->GetCurrent();
        wcout << L"Инструмент: " << currentInstrument.name << endl;
        wcout << L"Число струн: " << currentInstrument.strings_count << endl;
        wcout << L"Чистый: " << (currentInstrument.clean ? L"Да" : L"Нет") << endl;
        wcout << L"Настроенный: " << (currentInstrument.tune ? L"Да" : L"Нет") << endl;
        wcout << L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }
}


int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); // генератор случайных чисел
    setlocale(LC_ALL,"Russian");

    ////////////////////////////////////////////////////////////////////////////////////
    //ОБЩАЯ ЧАСТЬ ЗАПОЛНЕНИЯ  КОНТЕЙНЕРОВ

    int randomSize = rand()% 100; //рандомный размер контейнера
    wcout << L"в контейнер добавлено " << randomSize << L" инструментов" << endl << endl;

    //на выбор вектор или массив

    //VectorContainer Box;
    ArrayContainer Box(randomSize);

    //создание объектов в цикле

     for(int i = 0; i < randomSize; i++)
    {
        Box.addInstrument(CreateInstrument(static_cast<InstrumentType>(rand() % 4 )));
    }
    ////////////////////////////////////////////////////////////////////////////////////
    //ОБЫЧНЫЕ КОНТЕЙНЕРЫ

    //Task1(&Box);
    //Task2(&Box);

    /////////////////////////////////////////////////////////////////////////////////////
    //ИТЕРАТОР

    Iterator<InstrPtr> *it = Box.GetIterator();
    Task_It(it);

    ////////////////////////////////////////////////////////////////////////////////////
    //ДЕКОРАТОР

    //Декоратор: чистая, не настроенная скрипка с 2 струнами (не всегда попадается такой набор, так что лучше запустить несколько раз)
    /*Iterator<InstrPtr> *it = new InstrCleanedIteratorDecorator(
                            new InstrTunedIteratorDecorator(
                            new InstrStringsIteratorDecorator(
                            new InstrTypeIteratorDecorator(Box.GetIterator(), InstrumentType::Violin), 2), false), true);



    DecoratorTask(it);*/

    ////////////////////////////////////////////////////////////////////////////////////
    //БД
    /*
    for(int i = 0; i < randomSize; i++)
    {
        Box.addInstrument(CreateInstrument(static_cast<InstrumentType>(rand() % 4 )));
    }

    DatabaseBox dbBox;
    Iterator<InstrumentInfo> *it = dbBox.GetIterator();

    for (int i = 0; i < 20; ++i) {
        dbBox.addRow();
    }

    Task_DB(it); // Для БД


    delete it;
    dbBox.clearTable();
    */

    return 0;
}
