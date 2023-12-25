
#include "Globals.h"
#include <gtest/gtest.h>
#include "ChunkData.h"
#include "ChunkData.cpp"

constexpr size_t elementCount = ChunkBlockData::SectionBlockCount;


class ChunkDataStoreTest: public ::testing::Test {
protected:
    ChunkDataStore<int, elementCount, 0>* chunkDataStore;

    void SetUp() override {
        chunkDataStore = new ChunkDataStore<int, elementCount, 0>();
    }

    void TearDown() override {
        delete chunkDataStore;
    }
};


/** -------------- Testing setup and no data -------------- */

TEST_F(ChunkDataStoreTest, Constructor_AllSectionsAreNull) {
    ChunkDataStore<int, elementCount, 0> cds;
    for (int i = 0; i < cChunkDef::NumSections; i++) {
        ASSERT_EQ(cds.GetSection(i), nullptr);
    }
}


TEST_F(ChunkDataStoreTest, Get_NotAllocated_ReturnsDefaultValue) {
    int value = chunkDataStore->Get(Vector3i(0, 0, 0));
    ASSERT_EQ(value, 0); // here 0 is the default value
}


/** -------------- Testing valid operations -------------- */


TEST_F(ChunkDataStoreTest, SetSection_ValidPosition_OperationSuccessful) {
    const int source[elementCount] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    chunkDataStore->SetSection(source, 0);
    ChunkDataStore<int, elementCount, 0>::Type* section = chunkDataStore->GetSection(0);
    for (int i = 0; i < elementCount; i++) {
        ASSERT_EQ((*section)[i], source[i]);
    }
}


TEST_F(ChunkDataStoreTest, SetGet_BorderPosition_OperationSuccessful) {
    const Vector3i Position(cChunkDef::Width - 1, 1, 1);
    chunkDataStore->Set(Position, 20);
    int value = chunkDataStore->Get(Position);
    ASSERT_EQ(value, 20);
}


TEST_F(ChunkDataStoreTest, SetAll_OperationSuccessful)
{
    int source[elementCount * cChunkDef::NumSections];
    for (int i = 0; i < elementCount * cChunkDef::NumSections; i++) {
        source[i] = i;
    }
    chunkDataStore->SetAll(source);
    for (size_t y = 0; y < cChunkDef::NumSections; y++) {
        ChunkDataStore<int, elementCount, 0>::Type* section = chunkDataStore->GetSection(y);
        for (size_t i = 0; i < elementCount; i++) {
            ASSERT_EQ((*section)[i], source[y * elementCount + i]);
        }
    }
}


TEST_F(ChunkDataStoreTest, Set_OnlyAllocateUsed)
{
    ASSERT_TRUE(chunkDataStore->GetSection(0) == nullptr);
    ASSERT_TRUE(chunkDataStore->GetSection(1) == nullptr);

    chunkDataStore->Set(Vector3i(0, 0, 0), 10);
    ASSERT_TRUE(chunkDataStore->GetSection(0) != nullptr);
    ASSERT_TRUE(chunkDataStore->GetSection(1) == nullptr);
}


TEST_F(ChunkDataStoreTest, SetAll_EverySectionFilled) {
    // Fill a source array
    int source[elementCount * cChunkDef::NumSections];
    for (int i = 0; i < elementCount * cChunkDef::NumSections; i++) {
        source[i] = i + 1; // let's use i+1, to avoid confusion with the default value 0
    }
    // Set all sections
    chunkDataStore->SetAll(source);
    // Check every last element of every section to ensure they are properly set.
    for (int y = 0; y < cChunkDef::NumSections; y++) {
        auto * section = chunkDataStore->GetSection(y);
        ASSERT_EQ((*section)[elementCount - 1], source[(y + 1) * elementCount - 1]);
    }
}


TEST_F(ChunkDataStoreTest, GetSection_AllValidIndices_NoNullptr) {
    for (size_t i = 0; i < cChunkDef::NumSections; i++) {
        auto * section = chunkDataStore->GetSection(i);
        EXPECT_TRUE(section == nullptr);  // c++ dumb and cant decide if nullptr or NULL
    }

    for (int i = 0; i < cChunkDef::NumSections; i++)
    {
        chunkDataStore->Set(Vector3i(0, i * cChunkDef::SectionHeight, 0), 10);
    }

    for (size_t i = 0; i < cChunkDef::NumSections; i++) {
        auto * section = chunkDataStore->GetSection(i);
        EXPECT_TRUE(section != nullptr);  // c++ dumb and cant decide if nullptr or NULL
    }
}


TEST_F(ChunkDataStoreTest, Set_UpdateValue) {
    const Vector3i Position(1, 1, 1);
    chunkDataStore->Set(Position, 5);
    chunkDataStore->Set(Position, 10); // update the value

    int value = chunkDataStore->Get(Position);
    ASSERT_EQ(value, 10);
}


TEST_F(ChunkDataStoreTest, SetSection_UpdateSection_OperationSuccessful) {
    const int source1[elementCount] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    chunkDataStore->SetSection(source1, 1);
    const int source2[elementCount] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    chunkDataStore->SetSection(source2, 1);

    ChunkDataStore<int, elementCount, 0>::Type* section = chunkDataStore->GetSection(1);
    for (int i = 0; i < elementCount; i++) {
        ASSERT_EQ((*section)[i], source2[i]);
    }
}


TEST_F(ChunkDataStoreTest, SetAll_UnchangedValues_RemainSame) {
    int source1[elementCount * cChunkDef::NumSections];
    for (int i = 0; i < elementCount * cChunkDef::NumSections; i++) {
        source1[i] = i + 1;
    }
    int source2[elementCount * cChunkDef::NumSections];
    for (int i = 0; i < elementCount * cChunkDef::NumSections; i++) {
        source2[i] = (i % 2 == 0) ? source1[i] : source1[i] * 2; // every odd index value has changed
    }

    chunkDataStore->SetAll(source1);
    chunkDataStore->SetAll(source2);

    for (size_t y = 0; y < cChunkDef::NumSections; y++) {
        ChunkDataStore<int, elementCount, 0>::Type* section = chunkDataStore->GetSection(y);
        for (size_t i = 0; i < elementCount; i++) {
            if (i % 2 == 0) { // For even indices, assertEquals
                ASSERT_EQ((*section)[i], source1[y * elementCount + i]);
            } else { // For odd indices, assertNotEquals
                ASSERT_NE((*section)[i], source1[y * elementCount + i]);
                ASSERT_EQ((*section)[i], source2[y * elementCount + i]);
            }
        }
    }
}


/** -------------- Testing invalid operations -------------- */


TEST_F(ChunkDataStoreTest, GetSection_NotAllocated_ReturnsNullptr) {
    ChunkDataStore<int, elementCount, 0>::Type* section = chunkDataStore->GetSection(1);
    ASSERT_EQ(section, nullptr);
}


TEST_F(ChunkDataStoreTest, SetSection_InvalidSectionIndex) {
    const int source[elementCount] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    chunkDataStore->SetSection(source, -1);

    auto * section = chunkDataStore->GetSection(-1);
    ASSERT_TRUE(section == nullptr);
}


TEST_F(ChunkDataStoreTest, Get_OutOfRangePosition_Assertion) {
    const Vector3i Position(cChunkDef::Width + 1, 0, 0);
    ASSERT_ANY_THROW(chunkDataStore->Get(Position));
}

TEST_F(ChunkDataStoreTest, GetSection_InvalidIndex_ReturnsNullptr) {
    ChunkDataStore<int, elementCount, 0>::Type* section = chunkDataStore->GetSection(cChunkDef::NumSections + 1);
    ASSERT_EQ(section, nullptr);
}


TEST_F(ChunkDataStoreTest, GetSection_OutsideValidRange_ReturnsNullptr) {
    auto * section = chunkDataStore->GetSection(cChunkDef::NumSections);
    ASSERT_EQ(section, nullptr);
}
