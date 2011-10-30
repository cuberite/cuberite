#include "cNBTData.h"
#include <string> // memcpy
#include <stdio.h>
#include "zlib.h"
#include <assert.h>

#ifndef _WIN32
#include <cstring>
#include <netinet/in.h>
#endif

#ifdef _WIN32
#include <WinSock2.h>
#endif

cNBTData::~cNBTData()
{
    // TODO: Delete all compounds and stuff in it
    Clear();
}

cNBTData::cNBTData( char* a_Buffer, unsigned int a_BufferSize )
    : cNBTCompound( 0 )
{
    m_NumUnnamedElements = 0;
    for(int i = 0; i < TAG_NumTags; i++)
    {
        m_ParseFunctions[i] = 0;
    }
    m_ParseFunctions[TAG_Byte]      = &cNBTData::ParseByte;
    m_ParseFunctions[TAG_Short]     = &cNBTData::ParseShort;
    m_ParseFunctions[TAG_Int]       = &cNBTData::ParseInt;
    m_ParseFunctions[TAG_Long]      = &cNBTData::ParseLong;
    m_ParseFunctions[TAG_String]    = &cNBTData::ParseString;
    m_ParseFunctions[TAG_List]      = &cNBTData::ParseList;
    m_ParseFunctions[TAG_Compound]  = &cNBTData::ParseCompound;
    m_ParseFunctions[TAG_ByteArray] = &cNBTData::ParseByteArray;


    m_Buffer = a_Buffer;
    m_BufferSize = a_BufferSize;
    m_Index = 0;

    m_CurrentCompound = this;

    m_bDecompressed = true;
}

bool cNBTData::OpenCompound( std::string a_Name )
{
    cNBTCompound* Compound = GetCompound( a_Name );
    if( Compound )
    {
        m_CurrentCompound = Compound;
        return true;
    }
    printf("WARNING: Could not open NBT Compound %s\n", a_Name.c_str() );
    return false;
}

bool cNBTData::CloseCompound()
{
    if( m_CurrentCompound->GetParentCompound() )
    {
        m_CurrentCompound = m_CurrentCompound->GetParentCompound();
        return true;
    }
    printf("WARNING: Could not close NBT Compound, already at root!\n" );
    return false;
}

bool cNBTCompound::OpenList( std::string a_Name )
{
    if( GetList( a_Name ) )
    {
        m_CurrentList = GetList( a_Name );
        return true;
    }
    printf("WARNING: Could not open NBT List %s\n", a_Name.c_str() );
    return false;
}

bool cNBTCompound::CloseList()
{
    if( m_CurrentList )
    {
        m_CurrentList = m_CurrentList->GetParentList();
        return true;
    }
    printf("WARNING: Could not close NBT List, no list open!\n" );
    return false;
}

bool cNBTData::OpenList( std::string a_Name )
{
    return m_CurrentCompound->OpenList( a_Name );
}

bool cNBTData::CloseList()
{
    return m_CurrentCompound->CloseList();
}

void cNBTData::Compress()
{
    //printf("Before Compress size: %i\n", m_BufferSize );//re
    const int MAXNBTSIZE = 1024 * 1024 * 120;

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char* Compressed = new unsigned char[MAXNBTSIZE];

    /* allocate deflate state */
    strm.zalloc     = Z_NULL;
    strm.zfree      = Z_NULL;
    strm.opaque     = Z_NULL;
    strm.avail_in   = m_BufferSize;
    strm.avail_out  = MAXNBTSIZE;
    strm.next_in    =(Bytef*)m_Buffer;
    strm.next_out   = Compressed;
    strm.total_in   = 0;
    strm.total_out  = 0;
    ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15+MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK)
    {
        printf("deflateInit2 returned NOT OK\n");
        return;
    }



    /* run deflate() on input until output buffer not full, finish
       compression if all of source has been read in */

    ret = deflate(&strm, Z_FULL_FLUSH);    /* no bad return value */
    if( ret != Z_OK )
    {
        printf("WARNING: deflate returned NOT OK\n");
    }
    assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
    have = strm.total_out;

    assert(strm.avail_in == 0);     /* all input will be used */

    if( ret != Z_STREAM_END )
    {
        printf("WARNING: Compressing didn't go to end of stream\n");//re
    }

    if(m_Buffer)
    {
        delete [] m_Buffer;
        m_Buffer = 0;
    }

    //printf("Compressed size: %i\n", have );//re

    m_BufferSize = have;
    m_Buffer = new char[ m_BufferSize ];
    memcpy( m_Buffer, Compressed, m_BufferSize );
    delete Compressed;

    /* clean up and return */
    deflateEnd(&strm);
    m_bDecompressed = false;
    return;
}

bool cNBTData::Decompress()
{
    if( m_bDecompressed )
    {
        printf("WARNING: Decompress called, while it has already been decompressed\n");
        return false;
    }
    if( m_BufferSize == 0 )
    {
        printf("WARNING: Decompress called, with m_BufferSize of 0\n");
        return false;
    }

    //printf("Before Decompress size: %i\n", m_BufferSize );//re

    const int MAXNBTSIZE = 1024 * 1024 * 120 ;

    int ret;
    z_stream strm;
    unsigned char* out = new unsigned char[MAXNBTSIZE];

    /* allocate inflate state */
    strm.zalloc     = Z_NULL;
    strm.zfree      = Z_NULL;
    strm.opaque     = Z_NULL;
    strm.avail_in   = Z_NULL;
    strm.next_in    = Z_NULL;
    strm.avail_in   = m_BufferSize;
    strm.avail_out  = Z_NULL;
    strm.next_in    = (Bytef*)m_Buffer;
    strm.next_out   = Z_NULL;
    strm.avail_out  = MAXNBTSIZE;
    strm.next_out   = out;
    strm.total_in   = 0;
    strm.total_out  = 0;

    ret = inflateInit2(&strm, 16+MAX_WBITS);
    if (ret != Z_OK)
    {
        printf("inflateInit2 returned NOT OK\n");
        delete out;
        return false;
    }

    if( (ret = inflate(&strm, Z_NO_FLUSH)) != Z_STREAM_END)
    {
        assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
        printf("ret != Z_STREAM_END\n");
    }
    unsigned UncompressedSize = strm.total_out; //MAXNBTSIZE - strm.avail_out;

    m_Buffer = new char[ UncompressedSize ];
    memcpy( m_Buffer, out, UncompressedSize );
    m_BufferSize = UncompressedSize;

    inflateEnd(&strm);
    delete [] out;

    if( ret != Z_STREAM_END )
    {
        printf("WARNING: NBT Data received was too big! (More than %i bytes)\n", MAXNBTSIZE);
    }

    //printf("Decompressed Size: %i\n", UncompressedSize );//re
    m_bDecompressed = true;
    return (ret == Z_STREAM_END) ? true : false;
}

void cNBTCompound::AppendShort( std::string & a_Buffer, short a_Value )
{
    a_Buffer.push_back( (char)((a_Value>>8)&0xff) );
    a_Buffer.push_back( (char)((a_Value)&0xff) );
}

void cNBTCompound::AppendInteger( std::string & a_Buffer, int a_Value )
{
    int NetVal = htonl( a_Value );
    a_Buffer.append( (char*)&NetVal, sizeof( int ) );
}

void cNBTCompound::Serialize(std::string & a_Buffer)
{
    printf("cNBTCompound::Serialize()\n");//re
    for( CompoundMap::iterator itr = m_Compounds.begin(); itr != m_Compounds.end(); itr++ )
    {
        if( itr->second == 0 ) continue;
        a_Buffer.push_back( TAG_Compound );
        AppendShort( a_Buffer, (short)itr->first.size() );
        if( itr->first.size() > 0 )
        {
            a_Buffer.append( itr->first.c_str(), itr->first.size() );
        }
        itr->second->Serialize( a_Buffer );
        a_Buffer.push_back( TAG_End );
    }

    for( ListMap::iterator itr = m_Lists.begin(); itr != m_Lists.end(); itr++ )
    {
        if( itr->second == 0 ) continue;
        a_Buffer.push_back( TAG_List );
        AppendShort( a_Buffer, (short)itr->first.size() );
        if( itr->first.size() > 0 )
        {
            a_Buffer.append( itr->first.c_str(), itr->first.size() );
        }
        a_Buffer.push_back( (char)itr->second->GetType() );
        AppendInteger( a_Buffer, itr->second->GetSize() );
        itr->second->Serialize( a_Buffer );
    }

    for( IntegerMap::iterator itr = m_Integers.begin(); itr != m_Integers.end(); itr++ )
    {
        a_Buffer.push_back( TAG_Int );
        AppendShort( a_Buffer, (short)itr->first.size() );
        if( itr->first.size() > 0 )
        {
            a_Buffer.append( itr->first.c_str(), itr->first.size() );
        }
        AppendInteger( a_Buffer, itr->second );
    }

    for( ShortMap::iterator itr = m_Shorts.begin(); itr != m_Shorts.end(); itr++ )
    {
        a_Buffer.push_back( TAG_Short );
        AppendShort( a_Buffer, (short)itr->first.size() );
        if( itr->first.size() > 0 )
        {
            a_Buffer.append( itr->first.c_str(), itr->first.size() );
        }
        AppendShort( a_Buffer, itr->second );
    }

    for( ByteMap::iterator itr = m_Bytes.begin(); itr != m_Bytes.end(); itr++ )
    {
        a_Buffer.push_back( TAG_Byte );
        AppendShort( a_Buffer, (short)itr->first.size() );
        if( itr->first.size() > 0 )
        {
            a_Buffer.append( itr->first.c_str(), itr->first.size() );
        }
        a_Buffer.push_back( itr->second );
    }
}

void cNBTCompound::PrintData( int a_Depth, std::string a_Name )
{
    char* Prefix = new char[a_Depth*4+1];
    for(int i = 0; i < a_Depth*4; i++)
        Prefix[i] = ' ';
    Prefix[ a_Depth*4 ] = 0;

    if( a_Name.size() > 0 )
        printf("%s COMPOUND (%s)\n", Prefix, a_Name.c_str() );
    else
        printf("%s COMPOUND\n", Prefix );

    delete Prefix;
    a_Depth++;
    Prefix = new char[a_Depth*4];
    for(int i = 0; i < a_Depth*4; i++)
        Prefix[i] = ' ';
    Prefix[ a_Depth*4-1 ] = 0;

    for( CompoundMap::iterator itr = m_Compounds.begin(); itr != m_Compounds.end(); itr++ )
    {
        if( itr->second == 0 ) continue;
        itr->second->PrintData( a_Depth, itr->first );
    }

    for( ListMap::iterator itr = m_Lists.begin(); itr != m_Lists.end(); itr++)
    {
        if( itr->second == 0 ) continue;
        itr->second->PrintData( a_Depth, itr->first );
    }

    for( IntegerMap::iterator itr = m_Integers.begin(); itr != m_Integers.end(); itr++ )
    {
        printf("%s INTEGER %s (%i)\n", Prefix, itr->first.c_str(), itr->second );
    }

    for( ShortMap::iterator itr = m_Shorts.begin(); itr != m_Shorts.end(); itr++ )
    {
        printf("%s SHORT %s (%i)\n", Prefix, itr->first.c_str(), itr->second );
    }

    for( ByteMap::iterator itr = m_Bytes.begin(); itr != m_Bytes.end(); itr++ )
    {
        printf("%s BYTE %s (%i)\n", Prefix, itr->first.c_str(), itr->second );
    }

	for( ByteArrayMap::iterator itr = m_ByteArrays.begin(); itr != m_ByteArrays.end(); itr++ )
	{
		printf("%s BYTE ARRAY %s (length: %i)\n", Prefix, itr->first.c_str(), sizeof(itr->second) );
	}

    delete Prefix;
}

void cNBTData::PrintData()
{
    printf("==== STRUCTURED NBT DATA ====\n");
    m_CurrentCompound->PrintData( 0, " " );
    printf("=============================\n");
}

void cNBTData::Serialize()
{
    std::string Buffer;
    m_CurrentCompound->Serialize( Buffer );

    if( m_Buffer )
        delete m_Buffer;
    m_Buffer = new char[Buffer.size()];
    memcpy( m_Buffer, Buffer.c_str(), Buffer.size() );
    m_BufferSize = Buffer.size();

printf("m_BufferSize1: %i\n", m_BufferSize);//re

     //for(unsigned int i = 0; i < m_BufferSize; i++)//re
     //{//re
     //    printf("%02i %02x %3i %c\n", i, (unsigned char)m_Buffer[i], (unsigned char)m_Buffer[i], m_Buffer[i] );//re
     //}//re
}

void cNBTData::ParseData()
{
    if(!m_bDecompressed)
    {
        printf("WARNING: ParseData() called while data was not decompressed\n");
        return;
    }

     m_Index = 0;
     printf("m_BufferSize2: %i\n", m_BufferSize);//re
     printf("cNBTData::ParseData()\n");//re
     //for(unsigned int i = 0; i < m_BufferSize; i++)//re
     for(unsigned int i = 0; i < 70; i++)//re
     {//re
         printf("echo%02i %02x %3i %c\n", i, (unsigned char)m_Buffer[i], (unsigned char)m_Buffer[i], m_Buffer[i] );//re
     }//re

    while( m_Index < m_BufferSize )
    {
	printf("m_BufferSize3: %i\n", m_BufferSize);
	printf("m_Index: %i\n", m_Index);
        ParseTags();
    }
}

void cNBTData::ParseTags()
{
    if( m_Index < m_BufferSize )
    {
        printf("ParseTags idx:%02i %02x %3i %c\n", m_Index, (unsigned char)m_Buffer[m_Index], (unsigned char)m_Buffer[m_Index], m_Buffer[m_Index] );//re
        unsigned char Tag = m_Buffer[m_Index];
        if( Tag > 0 && m_ParseFunctions[ Tag ] )
        {
        printf("m_BufferSize4: %i\n", m_BufferSize);
        printf("m_Index1: %i\n\n\n\n", m_Index);

            m_Index++;
	    printf("Tag: %i\n", Tag);
            (*this.*m_ParseFunctions[ Tag ])(true);
        }
        else if( Tag == TAG_End )
        {
            printf("Tag End");
            m_Index++;
        }
        else
        {
            printf("UNKNOWN TAG %x\n", m_Buffer[m_Index] );
            for(unsigned int i = (m_Index-10 > 0)?m_Index-10:0 ; i < m_Index+10 && i < m_BufferSize; i++)
            {
                printf("%02i %02x %3i %c\n", i, (unsigned char)m_Buffer[i], (unsigned char)m_Buffer[i], m_Buffer[i] );
            }
            m_Index = m_BufferSize;
            return;
        }
    }
}

void cNBTData::ParseCompound( bool a_bNamed )
{
    std::string Name;
    if( a_bNamed ) Name = ReadName();
    printf("OPEN COMPOUND: %s\n", Name.c_str() );//re

    PutCompound( Name );
    OpenCompound( Name );
    while( m_Index < m_BufferSize && m_Buffer[ m_Index ] != TAG_End )
    {
        ParseTags();
    }
    CloseCompound();
    printf("CLOSE COMPOUND\n");//re
}

void cNBTData::ParseList( bool a_bNamed )
{
    std::string Name;
    if( a_bNamed ) Name = ReadName();
    ENUM_TAG TagType = (ENUM_TAG)ReadByte();
    int Length = ReadInt();
    printf("LIST: %s Type: %02x Length: %i\n", Name.c_str(), TagType, Length );//re

     for(unsigned int i = (m_Index-10 > 0)?m_Index-10:0 ; i < m_Index+10 && i < m_BufferSize; i++)//re
     {//re
         printf("%02i %02x %3i %c\n", i, (unsigned char)m_Buffer[i], (unsigned char)m_Buffer[i], m_Buffer[i] );//re
     }//re

    PutList( Name, TagType );
    OpenList( Name );
    for(int i = 0; i < Length && m_Index < m_BufferSize; i++)
    {
        if( m_ParseFunctions[ TagType ] )
        {
            (*this.*m_ParseFunctions[ TagType ] )(false);
            m_Index++;
        }
    }
    CloseList();

}

void cNBTData::ParseByte( bool a_bNamed )
{
    std::string Name;
    if( a_bNamed ) Name = ReadName();
    char Value = ReadByte();

    PutByte( Name, Value );

    printf("BYTE: %s %i\n", Name.c_str(), Value );//re
}

void cNBTData::ParseShort( bool a_bNamed )
{
    std::string Name;
    if( a_bNamed ) Name = ReadName();
    short Value = ReadShort();

    PutShort( Name, Value );

    printf("SHORT: %s %i\n", Name.c_str(), Value );//re
}

void cNBTData::ParseInt( bool a_bNamed )
{
    std::string Name;
    if( a_bNamed ) Name = ReadName();
    int Value = ReadInt();

    PutInteger( Name, Value );

    printf("INT: %s %i\n", Name.c_str(), Value );//re
}

void cNBTData::ParseLong( bool a_bNamed )
{
    std::string Name;
    if( a_bNamed ) Name = ReadName();
    long long Value = ReadLong();

    PutInteger( Name, (int)Value );

    printf("LONG: %s %li\n", Name.c_str(), Value );//re
}

void cNBTData::ParseString( bool a_bNamed )
{
    std::string Name;
    if( a_bNamed ) Name = ReadName();
    std::string String = ReadName();

    PutString( Name, String );

    printf("STRING: %s (%s)\n", Name.c_str(), String.c_str() );//re
}

void cNBTData::ParseByteArray( bool a_bNamed )
{

    std::string Name;
    if( a_bNamed ) Name = ReadName();

    int Length = ReadInt();
    std::string String;

    char* ByteArray = new char[ Length ];
    if( Length > 0 )
    {
		memcpy( ByteArray, &m_Buffer[ m_Index ], Length );
        m_Index += Length;
    }

    PutByteArray( Name, ByteArray );

    printf("VALUE: %s First 5 Chars: (%i,%i,%i,%i,%i)\n", Name.c_str(), ByteArray[0],ByteArray[1],ByteArray[2],ByteArray[3],ByteArray[4] );//re
}

std::string cNBTData::ReadName()
{
printf("crui1 \n");
    short Length = ReadShort();

printf("crui Length: %i\n", Length);
    std::string Name;
    if( Length > 0 )
    {
        for(int i = 0; i < Length; i++, m_Index++)
        {
            Name.push_back( m_Buffer[m_Index] );
        }
    }
    return Name;
}

char cNBTData::ReadByte()
{
    unsigned char Byte = m_Buffer[ m_Index ]; m_Index++;
    return Byte;
}

short cNBTData::ReadShort()
{
    short Length = 0;
    Length |= m_Buffer[ m_Index ] << 8; m_Index++;
    Length |= m_Buffer[ m_Index ]; m_Index++;
    return Length;
}

int cNBTData::ReadInt()
{
    int Value = 0;
    memcpy( &Value, m_Buffer+m_Index, sizeof(int) );
    m_Index+=sizeof(int);

    return ntohl( Value );
}

long long cNBTData::ReadLong()
{
    long long Value = 0;
    memcpy( &Value, m_Buffer+m_Index, sizeof(long long) );
    m_Index+=sizeof(long long);

    return ntohl( Value );
}

void cNBTCompound::PutList( std::string Name, ENUM_TAG Type )
{
    m_Lists[Name] = new cNBTList( m_CurrentList, Type );
}

void cNBTCompound::PutCompound( std::string Name )
{
    if( m_CurrentList )
    {
        m_CurrentList->AddToList( new cNBTCompound( this ) );
    }
    else
    {
        m_Compounds[Name] = new cNBTCompound( this );
    }
}

cNBTCompound*    cNBTCompound::GetCompound( std::string Name )
{
    if( m_CurrentList )
    {
        if( m_CurrentList->GetType() != TAG_Compound )
            return 0;

        return (cNBTCompound*)m_CurrentList->GetLastElement();
    }
    return m_Compounds[Name];
}

void cNBTList::PrintData(int a_Depth, std::string a_Name)
{
    char* Prefix = new char[a_Depth*4];
    for(int i = 0; i < a_Depth*4; i++)
        Prefix[i] = ' ';
    Prefix[ a_Depth*4-1 ] = 0;

    if( a_Name.size() > 0 )
        printf("%s LIST (%s)\n", Prefix, a_Name.c_str() );
    else
        printf("%s LIST\n", Prefix );

    delete [] Prefix;

    for( VoidList::iterator itr = m_List.begin(); itr != m_List.end(); itr++)
    {
        switch( m_Type )
        {
        case cNBTCompound::TAG_Compound:
            {
                ((cNBTCompound*)*itr)->PrintData(a_Depth+1, "...");
            }
            break;
        default:
            break;
        }
    }
}

void cNBTList::Serialize(std::string & a_Buffer)
{
    for( VoidList::iterator itr = m_List.begin(); itr != m_List.end(); itr++ )
    {
        switch( m_Type )
        {
        case cNBTCompound::TAG_Compound:
            {
                ((cNBTCompound*)(*itr))->Serialize( a_Buffer );
                a_Buffer.push_back( cNBTCompound::TAG_End );
            }
            break;
        default:
            break;
        }
    }
}

void cNBTData::Clear()
{
    while( m_CurrentCompound != this ) CloseCompound();
    m_CurrentCompound->Clear();

    if( m_Buffer )
    {
        delete m_Buffer;
        m_Buffer = 0;
    }
    m_BufferSize = 0;
}

void cNBTCompound::Clear()
{
    for( CompoundMap::iterator itr = m_Compounds.begin(); itr != m_Compounds.end(); itr++ )
    {
        if( itr->second == 0 ) continue;
        itr->second->Clear();
        delete itr->second;
        itr->second = 0;
    }
    m_Compounds.clear();

    for( ListMap::iterator itr = m_Lists.begin(); itr != m_Lists.end(); itr++ )
    {
        if( itr->second == 0 ) continue;
        itr->second->Clear();
        delete itr->second;
        itr->second = 0;
    }
    m_Lists.clear();
    m_Bytes.clear();
    m_Shorts.clear();
    m_Integers.clear();
    m_Strings.clear();
}

void cNBTList::Clear()
{
    for( VoidList::iterator itr = m_List.begin(); itr != m_List.end(); itr++)
    {
        switch( m_Type )
        {
        case cNBTCompound::TAG_Compound:
            {
                cNBTCompound* Compound = (cNBTCompound*)(*itr);
                Compound->Clear();
                delete Compound;
                *itr = 0;
            }
            break;
        case cNBTCompound::TAG_List:
            {
                cNBTList* List = (cNBTList*)(*itr);
                List->Clear();
                delete List;
                *itr = 0;
            }
            break;
        default:
            break;
        }
    }
    m_List.clear();
}