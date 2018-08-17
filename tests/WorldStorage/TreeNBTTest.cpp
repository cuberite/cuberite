
#include "Globals.h"
#include "WorldStorage/TreeNBT.h"


namespace
{

template <typename T>
const char * StrConst()
{
	if (std::is_const<T>::value)
	{
		return "const ";
	}
	else
	{
		return "";
	}
}





template <typename Expected>
struct sVisitExpect
{
	void operator () (Expected) {}

	template <typename Arg>
	void operator () (Arg &&)
	{
		using Unexpected = Arg &&;
		FLOGERROR("Expected {0}{1}, got {2}{3}",
			StrConst<Expected>(),   typeid(Expected).name(),
			StrConst<Unexpected>(), typeid(Unexpected).name()
		);
		assert_test(!"Visit called with unexpected type");
	}
};





void GetAsExpect(TreeNBT::cTag & a_Tag, eTagType Expected)
{
	// GetAs returns non-null pointer if and only if the tag type is expected
	assert_test((a_Tag.GetAs<TAG_Byte     >() != nullptr) == (Expected == TAG_Byte));
	assert_test((a_Tag.GetAs<TAG_Short    >() != nullptr) == (Expected == TAG_Short));
	assert_test((a_Tag.GetAs<TAG_Int      >() != nullptr) == (Expected == TAG_Int));
	assert_test((a_Tag.GetAs<TAG_Long     >() != nullptr) == (Expected == TAG_Long));
	assert_test((a_Tag.GetAs<TAG_Float    >() != nullptr) == (Expected == TAG_Float));
	assert_test((a_Tag.GetAs<TAG_Double   >() != nullptr) == (Expected == TAG_Double));
	assert_test((a_Tag.GetAs<TAG_ByteArray>() != nullptr) == (Expected == TAG_ByteArray));
	assert_test((a_Tag.GetAs<TAG_String   >() != nullptr) == (Expected == TAG_String));
	assert_test((a_Tag.GetAs<TAG_List     >() != nullptr) == (Expected == TAG_List));
	assert_test((a_Tag.GetAs<TAG_Compound >() != nullptr) == (Expected == TAG_Compound));
	assert_test((a_Tag.GetAs<TAG_IntArray >() != nullptr) == (Expected == TAG_IntArray));
}





template <eTagType ExpectedTypeId, typename ExpectedVisitType>
void AssertHoldsType(TreeNBT::cTag & a_Tag)
{
	assert_test(a_Tag.IsEmpty() == (ExpectedTypeId == TAG_End));
	assert_test(a_Tag.TypeId() == ExpectedTypeId);
	GetAsExpect(a_Tag, ExpectedTypeId);
	a_Tag.Visit(sVisitExpect<ExpectedVisitType &>{});

	const auto & ConstTag = a_Tag;
	ConstTag.Visit(sVisitExpect<const ExpectedVisitType &>{});

}





void TestTagCreation()
{
	{
		TreeNBT::cTag DefaultTag;
		AssertHoldsType<TAG_End, TreeNBT::cEmptyTag>(DefaultTag);
	}
	{
		TreeNBT::cTag ByteTag = Int8{};
		AssertHoldsType<TAG_Byte, Int8>(ByteTag);
	}
	{
		TreeNBT::cTag ShortTag = Int16{};
		AssertHoldsType<TAG_Short, Int16>(ShortTag);
	}
	{
		TreeNBT::cTag IntTag = Int32{};
		AssertHoldsType<TAG_Int, Int32>(IntTag);
	}
	{
		TreeNBT::cTag LongTag = Int64{};
		AssertHoldsType<TAG_Long, Int64>(LongTag);
	}
	{
		TreeNBT::cTag FloatTag = float{};
		AssertHoldsType<TAG_Float, float>(FloatTag);
	}
	{
		TreeNBT::cTag DoubleTag = double{};
		AssertHoldsType<TAG_Double, double>(DoubleTag);
	}
	{
		TreeNBT::cTag ByteArrayTag = TreeNBT::cArray<Int8>{};
		AssertHoldsType<TAG_ByteArray, TreeNBT::cArray<Int8>>(ByteArrayTag);
	}
	{
		TreeNBT::cTag StringTag = AString{};
		AssertHoldsType<TAG_String, AString>(StringTag);

		TreeNBT::cTag StringTag2 = "";
		AssertHoldsType<TAG_String, AString>(StringTag2);
	}
	{
		TreeNBT::cTag ListTag = TreeNBT::cList{};
		AssertHoldsType<TAG_List, TreeNBT::cList>(ListTag);
	}
	{
		TreeNBT::cTag CompoundTag = TreeNBT::cCompound{};
		AssertHoldsType<TAG_Compound, TreeNBT::cCompound>(CompoundTag);
	}
	{
		TreeNBT::cTag IntArrayTag = TreeNBT::cArray<Int32>{};
		AssertHoldsType<TAG_IntArray, TreeNBT::cArray<Int32>>(IntArrayTag);
	}
}





void TestTagAssignment()
{
	const char * const TestString = "Foo Bar Baz";

	// Move assign different type
	{
		TreeNBT::cTag CopyTag;
		TreeNBT::cTag StringTag = TestString;
		CopyTag = std::move(StringTag);

		// Move preserves types
		AssertHoldsType<TAG_String, AString>(StringTag);
		AssertHoldsType<TAG_String, AString>(CopyTag);

		// Move transfers the value
		assert_test(StringTag.GetAs<TAG_String>()->empty());
		assert_test(*CopyTag.GetAs<TAG_String>() == TestString);
	}

	// Copy assign different type
	{
		TreeNBT::cTag CopyTag;
		TreeNBT::cTag StringTag = TestString;
		CopyTag = StringTag;

		// Copying preserves types
		AssertHoldsType<TAG_String, AString>(StringTag);
		AssertHoldsType<TAG_String, AString>(CopyTag);

		// Copying is deep
		assert_test(*StringTag.GetAs<TAG_String>() == TestString);
		assert_test(*CopyTag.GetAs<TAG_String>() == TestString);
		assert_test(StringTag.GetAs<TAG_String>() != CopyTag.GetAs<TAG_String>());
	}

	// Move assign same types
	{
		TreeNBT::cTag CopyTag = "";
		TreeNBT::cTag StringTag = TestString;
		CopyTag = std::move(StringTag);

		// Move preserves types
		AssertHoldsType<TAG_String, AString>(StringTag);
		AssertHoldsType<TAG_String, AString>(CopyTag);

		// Move transfers the value
		assert_test(StringTag.GetAs<TAG_String>()->empty());
		assert_test(*CopyTag.GetAs<TAG_String>() == TestString);
	}

	// Copy assign same types
	{
		TreeNBT::cTag CopyTag = "";
		TreeNBT::cTag StringTag = TestString;
		CopyTag = StringTag;

		// Copying preserves types
		AssertHoldsType<TAG_String, AString>(StringTag);
		AssertHoldsType<TAG_String, AString>(CopyTag);

		// Copying is deep
		assert_test(*StringTag.GetAs<TAG_String>() == TestString);
		assert_test(*CopyTag.GetAs<TAG_String>() == TestString);
		assert_test(StringTag.GetAs<TAG_String>() != CopyTag.GetAs<TAG_String>());
	}
}

}  // namespace (anonymous)




int main(int argc, char ** argv)
{
	FLOG("TreeNBT tests started");

	TestTagCreation();

	TestTagAssignment();

	FLOG("TreeNBT tests finished");
}




