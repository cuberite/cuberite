#pragma once

#include <map>
#include <list>
#include <string>

class cNBTList;
class cNBTData;

class cNBTCompound
{
public:
    cNBTCompound( cNBTCompound* a_ParentCompound ) : m_ParentCompound( a_ParentCompound ), m_CurrentList(0) { }
    virtual ~cNBTCompound() {}
public:
#ifdef _WIN32
    enum ENUM_TAG : unsigned char
#else
    enum ENUM_TAG
#endif
    {
        TAG_End = 0,
        TAG_Byte,
        TAG_Short,
        TAG_Int,
        TAG_ByteArray = 7,
	TAG_String,
        TAG_List,
        TAG_Compound,
        TAG_NumTags // Not a real tag, but contains number of tags
    };

    void Clear();

    void PutByte( std::string Name, char Value ) { m_Bytes[Name] = Value; }
    void PutShort( std::string Name, short Value ) { m_Shorts[Name] = Value; }
    void PutInteger( std::string Name, int Value ) { m_Integers[Name] = Value; }
    void PutString( std::string Name, std::string Value ) { m_Strings[Name] = Value; }
    void PutByteArray( std::string Name, char Value ) { m_ByteArrays[Name] = Value; }
    void PutCompound( std::string Name );
    void PutList( std::string Name, ENUM_TAG Type );

    char             GetByte( std::string Name ) { return m_Bytes[Name]; }
    short            GetShort( std::string Name ) { return m_Shorts[Name]; }
    int              GetInteger( std::string Name ) { return m_Integers[Name]; }
    std::string      GetString( std::string Name ) { return m_Strings[Name]; }
    char             GetByteArray( std::string Name ) { return m_ByteArrays[Name]; }
    cNBTCompound*    GetCompound( std::string Name );
    cNBTList*        GetList( std::string Name ) { return m_Lists[Name]; }

    cNBTList*        GetCurrentList() { return m_CurrentList; }
    cNBTCompound*    GetParentCompound() { return m_ParentCompound; }

    bool OpenList( std::string a_Name );
    bool CloseList();

    void Serialize(std::string & a_Buffer);

    void PrintData( int a_Depth, std::string a_Name );
private:
    void AppendShort( std::string & a_Buffer, short a_Value );
    void AppendInteger( std::string & a_Buffer, int a_Value );

    cNBTCompound* m_ParentCompound;
    cNBTList*        m_CurrentList;

    typedef std::map<std::string, char>             ByteMap;
    typedef std::map<std::string, short>            ShortMap;
    typedef std::map<std::string, int>              IntegerMap;
    typedef std::map<std::string, std::string>      StringMap;
    typedef std::map<std::string, char>             ByteArrayMap;
    typedef std::map<std::string, cNBTCompound*>    CompoundMap;
    typedef std::map<std::string, cNBTList*>        ListMap;
    ByteMap            m_Bytes;
    ShortMap           m_Shorts;
    IntegerMap         m_Integers;
    StringMap          m_Strings;
    ByteArrayMap       m_ByteArrays;
    CompoundMap        m_Compounds;
    ListMap            m_Lists;
};

class cNBTList
{
public:
    cNBTList( cNBTList* a_ParentList, cNBTCompound::ENUM_TAG a_Type ) : m_ParentList( a_ParentList ), m_Type( a_Type ) {}
    void AddToList( void* a_Item ) { m_List.push_back( a_Item ); }
    void* GetLastElement() { return m_List.back(); }
    cNBTCompound::ENUM_TAG GetType() { return m_Type; }
    cNBTList* GetParentList() { return m_ParentList; }

    unsigned int GetSize() { return m_List.size(); }

    void Serialize(std::string & a_Buffer);
    void PrintData(int a_Depth, std::string a_Name);
    typedef std::list<void*> VoidList;
    VoidList GetList() { return m_List; }

    void Clear();
private:
    cNBTList* m_ParentList;
    cNBTCompound::ENUM_TAG m_Type;
    VoidList m_List;
};

class cNBTData : public cNBTCompound
{
public:
    cNBTData( char* a_Buffer, unsigned int a_BufferSize );
    virtual ~cNBTData();

    void Clear();

    void PrintData();

    void ParseData();

    bool OpenCompound( std::string a_Name );
    bool CloseCompound();

    bool OpenList( std::string a_Name );
    bool CloseList();

    void PutByte( std::string Name, char Value )            { m_CurrentCompound->PutByte( Name, Value ); }
    void PutShort( std::string Name, short Value )          { m_CurrentCompound->PutShort( Name, Value ); }
    void PutInteger( std::string Name, int Value )          { m_CurrentCompound->PutInteger( Name, Value ); }
    void PutString( std::string Name, std::string Value )   { m_CurrentCompound->PutString(Name, Value); }
    void PutByteArray( std::string Name, char Value )       { m_CurrentCompound->PutByteArray( Name, Value ); }
    void PutCompound( std::string Name )                    { m_CurrentCompound->PutCompound( Name ); }
    void PutList( std::string Name, ENUM_TAG Type )         { m_CurrentCompound->PutList( Name, Type ); }

    int              GetInteger( std::string Name )   { return m_CurrentCompound->GetInteger(Name); }
    std::string      GetString( std::string Name )    { return m_CurrentCompound->GetString(Name); }
    char             GetByteArray( std::string Name ) { return m_CurrentCompound->GetByteArray(Name); }
    cNBTCompound*    GetCompound( std::string Name )  { return m_CurrentCompound->GetCompound(Name); }
    cNBTList*        GetList( std::string Name )      { return m_CurrentCompound->GetList(Name); }

    char* GetBuffer() { return m_Buffer; }
    unsigned int GetBufferSize() { return m_BufferSize; }

    void Compress();
    bool Decompress();

    void Serialize();
private:
    int m_NumUnnamedElements;
    bool m_bDecompressed;

    void ParseTags();
    void ParseCompound( bool a_bNamed );
    void ParseList( bool a_bNamed );
    void ParseString( bool a_bNamed );
    void ParseByte( bool a_bNamed );
    void ParseByteArray( bool a_bNamed );
    void ParseInt( bool a_bNamed );
    void ParseShort( bool a_bNamed );

    short ReadShort();
    std::string ReadName();
    char ReadByte();
    int ReadInt();

    cNBTCompound*    m_CurrentCompound;

    char* m_Buffer;
    unsigned int m_BufferSize;
    unsigned int m_Index;

    void (cNBTData::*m_ParseFunctions[TAG_NumTags])(bool);
};
