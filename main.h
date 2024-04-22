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

struct InstrumentInfo { //структура для вывода столбцов таблицы
    std::wstring name;
    int strings_count;
    bool clean;
    bool tune;
};


class DatabaseBoxIterator : public Iterator<InstrumentInfo>
{
private:
    sqlite3_stmt *pStatement;

public:
    DatabaseBoxIterator(sqlite3_stmt *statement) : pStatement(statement) {}
    void First() { sqlite3_reset(pStatement); }
    void Next() { sqlite3_step(pStatement); }
    bool IsDone() const { return sqlite3_step(pStatement) != SQLITE_ROW; }

    InstrumentInfo GetCurrent() const {
        InstrumentInfo info;
        info.name = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(pStatement, 0));
        info.strings_count = sqlite3_column_int(pStatement, 1);
        info.clean = sqlite3_column_int(pStatement, 2);
        info.tune = sqlite3_column_int(pStatement, 3);
        return info;
    }
    ~DatabaseBoxIterator() {
    if (pStatement != nullptr) {
        sqlite3_finalize(pStatement); // освобождаем ресурс только если он был выделен
    }
}

};


class DatabaseBox
{
private:
    sqlite3* Database;
    char *errmsg;
    int openResult;
    sqlite3_stmt* statement;
public:
    DatabaseBox()
    {
        openResult = sqlite3_open16(
            L"D:\\Code Blocks Projects\\StringInstruments\\Instruments.db",
            &Database);
        if(openResult != SQLITE_OK)
        {
            wcout << L"Ошибка открытия базы данных!" << endl;
        }
    }
    virtual void addRow();
    virtual void clearTable();
    virtual Iterator<InstrumentInfo>* GetIterator();
    ~DatabaseBox() {
    if (Database != nullptr) {
        sqlite3_close(Database);
    }
    if (statement != nullptr) {
        sqlite3_finalize(statement);
    }
}

};

void DatabaseBox::addRow() //добавление рандомной строки
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

void DatabaseBox::clearTable() // очищение таблицы
{
    string delete_query = "DELETE FROM StringsInstruments;";
    int execResult = sqlite3_exec(Database, delete_query.c_str(), NULL, NULL, &errmsg);
    if (execResult != SQLITE_OK)
    {
        cout << errmsg << endl;
        wcout << L"Ошибка очистки таблицы" << endl;
    }
    else
    {
        wcout << L"Таблица очищена" << endl;
    }
}

Iterator<InstrumentInfo>* DatabaseBox::GetIterator() {

    const char* select_query = "SELECT * FROM StringsInstruments;";
    int prepareResult = sqlite3_prepare_v2(Database, select_query, -1, &statement, NULL);
    if (prepareResult != SQLITE_OK) {
        cout << "Ошибка подготовки запроса: " << sqlite3_errmsg(Database) << endl;
        return nullptr;
    }
    return new DatabaseBoxIterator(statement);
}



#endif // MainH
