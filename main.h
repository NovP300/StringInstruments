#ifndef MainH
#define MainH

#include "Pattern.h"

enum class InstrumentType: int
{
    Violin = 0, //скрипка
    Alto = 1, //альт
    Cello = 2, //виолончель
    Contrabass = 3, //контрабас
    Unknown = -1
};

///////////////////////////////////
//РАЗДЕЛ КЛАССЫ ИНСТРУМЕНТОВ


class StringInstrument
{
protected:
    bool Tuned;
    InstrumentType Type;
    int StringsNumber;
    bool Cleaned;

    StringInstrument(); // реализация конструктора в main.cpp

public:
    bool IsTuned() const { return Tuned; }
    bool IsCleaned() const { return Cleaned; }
    virtual InstrumentType GetType() const = 0;
    int GetStringsNumber() const { return StringsNumber; }
    virtual void Play() = 0;
    virtual void Tune()  { Tuned = true; wcout << L"инструмент настроен" << endl; }
    virtual void Clean() { Cleaned = true; wcout << L"инструмент очищен" << endl; }
};

typedef StringInstrument * InstrPtr; // переопределение указателя на класс StringInstrument

class Violin : public StringInstrument //скрипка
{
public:
    Violin() : StringInstrument() { Type = InstrumentType::Violin; };
    InstrumentType GetType() const { return Type; }
    void Play() { wcout << L"игра на скрипке..."; }
};

class Alto : public StringInstrument //альт
{
public:
    Alto() : StringInstrument() { Type = InstrumentType::Alto; };
    InstrumentType GetType() const { return Type; }
    void Play() { wcout << L"игра на альте..."; }
};

class Cello : public StringInstrument //виолончель
{
public:
    Cello() : StringInstrument() { Type = InstrumentType::Cello; };
    InstrumentType GetType() const { return Type; }
    void Play() { wcout << L"игра на виолончели..."; }
};

class Contrabass : public StringInstrument //виолончель
{
public:
    Contrabass() : StringInstrument() { Type = InstrumentType::Contrabass; };
    InstrumentType GetType() const { return Type; }
    void Play() { wcout << L"игра на виолончели..."; }
};

/////////////////////////////////////////////////////////////
//РОДИТЕЛЬСКИЙ КЛАСС КОНТЕЙНЕРОВ

class InstrumentContainer
{
public:
    virtual void addInstrument(InstrPtr newInstrument) = 0;
    virtual InstrPtr getInstrument(int index) const = 0;
    virtual int GetCount() const = 0;
};

//////////////////////////////////////////////////////////////
//ВЕКТОР КОНТЕЙНЕР

class VectorContainerIterator : public Iterator<InstrPtr>
{
private:
    const vector<InstrPtr> *Instruments;
    vector<InstrPtr>::const_iterator it;

public:
    VectorContainerIterator(const vector<InstrPtr> *instruments)
    {
        Instruments = instruments;
        it = Instruments->begin();
    }

    void First() { it = Instruments->begin(); }
    void Next() { it++; }
    bool IsDone() const { return it == Instruments->end(); }
    InstrPtr GetCurrent() const { return *it; }
};

class VectorContainer : public InstrumentContainer // контейнер-вектор
{
private:
    vector<InstrPtr> Instruments;

public:
    void addInstrument(InstrPtr newInstrument) override { Instruments.push_back(newInstrument); }
    int GetCount() const override { return Instruments.size(); }
    InstrPtr getInstrument(int index) const override { return Instruments[index]; }

    Iterator<InstrPtr> *GetIterator()
    {
        return new VectorContainerIterator(&Instruments);
    }
};

//////////////////////////////////////////////////////////////
//МАССИВ КОНТЕЙНЕР

class ArrayContainerIterator : public Iterator<InstrPtr>
{
private:
    const InstrPtr *Instruments;
    int Size;
    int Position;

public:
    ArrayContainerIterator(const InstrPtr *instruments, int size)
    {
        Instruments = instruments;
        Size = size;
        Position = 0;
    }

    void First() { Position = 0; }
    void Next() { Position++; }
    bool IsDone() const { return Position >= Size; }
    InstrPtr GetCurrent() const { return Instruments[Position]; }
};

class ArrayContainer : public InstrumentContainer // контейнер-массив
{
private:
    InstrPtr *Instruments;
    int MaxSize;
    int CurrentSize;

public:
    ArrayContainer(int maxSize); //реализация контейнера в main.cpp
    virtual ~ArrayContainer(); //реализация деструктора в main.cpp
    void addInstrument(InstrPtr newInstrument)  {
        Instruments[CurrentSize] = newInstrument;
        CurrentSize++;
        }
    int GetCount() const  { return CurrentSize; }
    InstrPtr getInstrument(int index) const { return Instruments[index]; }

    Iterator<InstrPtr> *GetIterator()
    {
        return new ArrayContainerIterator(Instruments, MaxSize);
    }
};


#endif // MainH
