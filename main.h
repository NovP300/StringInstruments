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
    static StringInstrument *Create(InstrumentType type); //реализация метода в main.cpp
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


/////////////////////////////////////////////////////////////
//РАЗДЕЛ ДЕКОРАТОРЫ

class InstrTunedIteratorDecorator : public IteratorDecorator<InstrPtr> //декоратор настройки инструмента
{
private:
    bool TunedOK;

public:
    InstrTunedIteratorDecorator(Iterator<InstrPtr> *it, bool tunedOK) : IteratorDecorator(it)
    {
        TunedOK = tunedOK;
    }

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsTuned() != TunedOK)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->IsTuned() != TunedOK);
    }
};

class InstrTypeIteratorDecorator : public IteratorDecorator<InstrPtr> // декоратор типа инструмента
{
private:
    InstrumentType TargetType;

public:
    InstrTypeIteratorDecorator(Iterator<InstrPtr> *it, InstrumentType targetType) : IteratorDecorator(it)
    {
        TargetType = targetType;
    }

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetType() != TargetType)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->GetType() != TargetType);
    }

    InstrPtr GetCurrent() const override { return It->GetCurrent(); }
};

class InstrCleanedIteratorDecorator : public IteratorDecorator<InstrPtr> // декоратор чистоты инструмента
{
private:
    bool CleanedOK;

public:
    InstrCleanedIteratorDecorator(Iterator<InstrPtr> *it, bool cleanedOK) : IteratorDecorator(it)
    {
        CleanedOK = cleanedOK;
    }

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsCleaned() != CleanedOK)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->IsCleaned() != CleanedOK);
    }
};

class InstrStringsIteratorDecorator : public IteratorDecorator<InstrPtr> // декоратор чистоты инструмента
{
private:
    int Strings_count;

public:
    InstrStringsIteratorDecorator(Iterator<InstrPtr> *it, int strings_count) : IteratorDecorator(it)
    {
        Strings_count = strings_count;
    }

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetStringsNumber() != Strings_count)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->GetStringsNumber() != Strings_count);
    }
};


/////////////////////////////////////////////////////
//РАЗДЕЛ БАЗА ДАННЫХ

struct InstrumentInfo {
    std::wstring name;
    int strings_count;
    bool clean;
    bool tune;
};


class DatabaseBox
{
private:
    sqlite3* Database;
    char *errmsg;
    int openResult;
public:
    DatabaseBox()
    {
        openResult = sqlite3_open16(
            L"C:\\CodeBlocksProjects\\StringInstruments\\Instruments.db",
            &Database);
        if(openResult != SQLITE_OK)
        {
            wcout << L"Ошибка открытия базы данных!" << endl;
        }
    }
    virtual void addRow();
    virtual void clearTable();
    ~DatabaseBox() { sqlite3_close(Database); }
};

void DatabaseBox::addRow()
{
  InstrumentType type = static_cast<InstrumentType>(rand()%4);
  string name;
  switch(type)
  {
      case InstrumentType::Violin: name = "Violin"; break;
      case InstrumentType::Alto: name = "Alto"; break;
      case InstrumentType::Cello: name = "Cello"; break;
      case InstrumentType::Contrabass: name = "Contrabass"; break;
  }

  int strings_count = rand() % 5;
  bool clean = rand() % 2;
  bool tune = rand() % 2;
  string insert_query = "INSERT INTO StringsInstruments (name, strings_count, clean, tune) VALUES ('" +name+ "'," +to_string(strings_count)+ ","
        +to_string(clean)+ "," +to_string(tune)+ ");";

  int execResult = sqlite3_exec(Database, insert_query.c_str(), NULL, NULL, &errmsg);
  if (execResult != SQLITE_OK)
  {
    cerr << errmsg << endl;
    cerr << "Ошибка добавления строки" << endl;
    return;
  }
}

void DatabaseBox::clearTable()
{
    string delete_query = "DELETE FROM StringsInstruments;";
    int execResult = sqlite3_exec(Database, delete_query.c_str(), NULL, NULL, &errmsg);
    if (execResult != SQLITE_OK)
    {
        cout << errmsg << endl;
        wcout << L"Ошибка очистки таблицы" << endl;
    }
}



#endif // MainH
