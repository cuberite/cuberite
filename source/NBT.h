
// NBT.h

// Interfaces to the classes used for NBT representation, parsing and serializing





#pragma once





typedef long long Int64;
typedef int       Int32;
typedef short     Int16;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Representation classes:

class cNBTTag abstract  // The base class for all NBT tags
{
public:
	enum eTagType
	{
		TAG_Min       = 0,  // The minimum value for a tag type
		TAG_End       = 0,
		TAG_Byte      = 1,
		TAG_Short     = 2,
		TAG_Int       = 3,
		TAG_Long      = 4,
		TAG_Float     = 5,
		TAG_Double    = 6,
		TAG_ByteArray = 7,
		TAG_String    = 8,
		TAG_List      = 9,
		TAG_Compound  = 10,
		TAG_IntArray  = 11,
		TAG_Max       = 11,  // The maximum value for a tag type
	} ;

protected:
	cNBTTag * m_Parent;
	eTagType  m_Type;
	AString   m_Name;  // tag name, in UTF-8

public:
	cNBTTag(cNBTTag * a_Parent, eTagType a_Type) : m_Parent(a_Parent), m_Type(a_Type) {}
	cNBTTag(cNBTTag * a_Parent, eTagType a_Type, const AString & a_Name) : m_Parent(a_Parent), m_Type(a_Type), m_Name(a_Name) {}
	virtual ~cNBTTag() {}  // Force a virtual destructor
	
	cNBTTag *       GetParent(void) const {return m_Parent; }	
	eTagType        GetType  (void) const {return m_Type; }
	const AString & GetName  (void) const {return m_Name; }
	void            SetName  (const AString & a_Name) {m_Name = a_Name; }
	
	static cNBTTag * CreateTag(cNBTTag * a_Parent, eTagType a_Type, const AString & a_Name);  // Creates a new instance of a tag specified by iType, uses the correct class
	
	virtual cNBTTag * FindChildByName(const AString & a_Name) const {return NULL; }
	const cNBTTag * FindChildByPath(const AString & a_Path) const;
} ;

typedef cNBTTag cNBTTree;
typedef std::vector<cNBTTag *> cNBTTags;





#define DECLARE_SIMPLE_TAG(TAG,CTYPE) \
class cNBT##TAG : \
	public cNBTTag \
{ \
public: \
	cNBT##TAG(cNBTTag * a_Parent) : cNBTTag(a_Parent, TAG_##TAG) {} \
	cNBT##TAG(cNBTTag * a_Parent, const AString & a_Name) : cNBTTag(a_Parent, TAG_##TAG, a_Name) {} \
	cNBT##TAG(cNBTTag * a_Parent, const AString & a_Name, const CTYPE & a_Value) : cNBTTag(a_Parent, TAG_##TAG, a_Name), m_Value(a_Value) {} \
	CTYPE m_Value; \
}





DECLARE_SIMPLE_TAG(Byte,      char);
DECLARE_SIMPLE_TAG(Short,     Int16);
DECLARE_SIMPLE_TAG(Int,       Int32);
DECLARE_SIMPLE_TAG(Long,      Int64);
DECLARE_SIMPLE_TAG(Float,     float);
DECLARE_SIMPLE_TAG(Double,    double);
DECLARE_SIMPLE_TAG(ByteArray, AString);  // Represent the array as a string for easier manipulation
DECLARE_SIMPLE_TAG(String,    AString);






class cNBTList :
	public cNBTTag
{
	cNBTTags m_Children;
	eTagType m_ChildrenType;
	
public:
	cNBTList(cNBTTag * a_Parent) : cNBTTag(a_Parent, TAG_List), m_ChildrenType(TAG_End) {}
	cNBTList(cNBTTag * a_Parent, const AString & a_Name) : cNBTTag(a_Parent, TAG_List, a_Name), m_ChildrenType(TAG_End) {}
	virtual ~cNBTList() {Clear(); }
	
	void              Clear           (void);
	int               Add             (cNBTTag * a_Tag);
	cNBTTag *         GetChildByIdx   (size_t a_Index);
	const cNBTTags &  GetChildren     (void) const {return m_Children; }
	size_t            GetChildrenCount(void) const {return m_Children.size(); }
	virtual cNBTTag * FindChildByName (const AString & a_Name) const override;
	
	int      SetChildrenType(eTagType a_Type);  // Only valid when list empty
	eTagType GetChildrenType(void) const {return m_ChildrenType; }
} ;





class cNBTCompound :
	public cNBTTag
{
	cNBTTags m_Children;
public:
	cNBTCompound(cNBTTag * a_Parent) : cNBTTag(a_Parent, TAG_Compound) {}
	cNBTCompound(cNBTTag * a_Parent, const AString & a_Name) : cNBTTag(a_Parent, TAG_Compound, a_Name) {}
	virtual ~cNBTCompound() {Clear(); }
	
	void              Clear           (void);
	int               Add             (cNBTTag * a_Tag);
	cNBTTag *         GetChildByIdx   (size_t a_Index);
	const cNBTTags &  GetChildren     (void) const {return m_Children; }
	size_t            GetChildrenCount(void) const {return m_Children.size(); }
	virtual cNBTTag * FindChildByName (const AString & a_Name) const override;
} ;





class cNBTIntArray :
	public cNBTTag
{
	typedef cNBTTag super;
	
	std::vector<int> m_Values;
	
public:
	cNBTIntArray(cNBTTag * a_Parent) : super(a_Parent, TAG_IntArray) {}
	cNBTIntArray(cNBTTag * a_Parent, const AString & a_Name) : super(a_Parent, TAG_IntArray, a_Name) {}
	
	void Clear(void) {m_Values.clear(); }
	void Add  (int a_Value) {m_Values.push_back(a_Value); }
	int  Get  (int a_Index) const {return m_Values[a_Index]; }
	int  Size (void) const {return m_Values.size(); }
	const std::vector<int> & GetValues(void) const {return m_Values; }
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The parser:

class cNBTParser
{
	static int ReadTag      (const char ** Data, int * Length, cNBTTag::eTagType iType, const AString & a_Name, cNBTTag * iParent, cNBTTag ** oTag);  // Helper
	
	static int ReadByte     (const char ** Data, int * Length, char &    a_Value);
	static int ReadInt16    (const char ** Data, int * Length, Int16 &   a_Value);
	static int ReadInt32    (const char ** Data, int * Length, Int32 &   a_Value);
	static int ReadInt64    (const char ** Data, int * Length, Int64 &   a_Value);
	static int ReadFloat    (const char ** Data, int * Length, float &   a_Value);
	static int ReadDouble   (const char ** Data, int * Length, double &  a_Value);
	static int ReadByteArray(const char ** Data, int * Length, AString & a_Value);
	static int ReadString   (const char ** Data, int * Length, AString & a_Value);
	static int ReadList     (const char ** Data, int * Length, cNBTList     * a_List);
	static int ReadCompound (const char ** Data, int * Length, cNBTCompound * a_Compound);
	static int ReadIntArray (const char ** Data, int * Length, cNBTIntArray * a_Array);
	
public:

	/// Returns the parsed tree, or NULL on failure
	static cNBTTree * Parse(const char * a_Data, int a_Length);
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The serializer:

class cNBTSerializer
{
	static void WriteTag     (AString & a_Out, const cNBTTag * a_Tag);
	
	static void WriteByte     (AString & a_Out, const char      a_Value);
	static void WriteInt16    (AString & a_Out, const Int16     a_Value);
	static void WriteInt32    (AString & a_Out, const Int32     a_Value);
	static void WriteInt64    (AString & a_Out, const Int64     a_Value);
	static void WriteFloat    (AString & a_Out, const float     a_Value);
	static void WriteDouble   (AString & a_Out, const double    a_Value);
	static void WriteByteArray(AString & a_Out, const AString & a_Value);
	static void WriteString   (AString & a_Out, const AString & a_Value);
	static void WriteList     (AString & a_Out, const cNBTList     * a_List);
	static void WriteCompound (AString & a_Out, const cNBTCompound * a_Compound);
	static void WriteIntArray (AString & a_Out, const cNBTIntArray * a_Array);
	
public:

	static void Serialize(const cNBTTree * a_Tree, AString & a_Out);
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dumping the tree (DEBUG-only)

#if (defined(_DEBUG) && defined(_WIN32))
void DumpTree(const cNBTTree * a_Tree, int a_Level = 0);
#endif  // (_DEBUG && _WIN32)




