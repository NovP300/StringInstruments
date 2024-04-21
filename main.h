#ifndef MainH
#define MainH

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

///////////////////////////////////


#endif // MainH
