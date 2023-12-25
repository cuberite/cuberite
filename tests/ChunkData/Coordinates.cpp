
#include "Globals.h"
#include <gtest/gtest.h>
#include "ChunkData.h"




/** Performs the entire cChunkData coordinates test. */
TEST(ChunkDataCoordinates, SetAndGetOutOfBounds){
    ChunkBlockData buffer;

    // Empty chunks
    buffer.SetBlock({ 0, 0, 0 }, 0xAB);
    EXPECT_EQ(buffer.GetBlock({ 0, 0, 0 }), 0xAB);
    buffer.SetMeta({ 0, 16, 0 }, 0xC);
    EXPECT_EQ(buffer.GetMeta({ 0, 16, 0 }), 0xC);

    // loaded but not written segments
    EXPECT_EQ(buffer.GetBlock({ 1, 0, 0 }), 0x0);
    EXPECT_EQ(buffer.GetMeta({ 1, 16, 0 }), 0x0);

    // Notloaded segments
    EXPECT_EQ(buffer.GetBlock({ 0, 32, 0 }), 0x0);
    EXPECT_EQ(buffer.GetMeta({ 0, 48, 0 }), 0x0);

    // Out of range GetBlock
    EXPECT_ANY_THROW(
        buffer.GetBlock({ -1, 0, 0 });
    );
    EXPECT_ANY_THROW(
        buffer.GetBlock({ 0, -1, 0 });
    );
    EXPECT_ANY_THROW(
        buffer.GetBlock({ 0, 0, -1 });
    );
    EXPECT_ANY_THROW(
        buffer.GetBlock({ 256, 0, 0 });
    );
    EXPECT_ANY_THROW(
        buffer.GetBlock({ 0, 256, 0 });
    );
    EXPECT_ANY_THROW(
        buffer.GetBlock({ 0, 0, 256 });
    );

    // Out of range SetBlock
    EXPECT_ANY_THROW(
        buffer.SetBlock({ -1, 0, 0 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetBlock({ 0, -1, 0 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetBlock({ 0, 0, -1 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetBlock({ 256, 0, 0 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetBlock({ 0, 256, 0 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetBlock({ 0, 0, 256 }, 0)
    );

    // Out of range GetMeta
    EXPECT_ANY_THROW(
        buffer.GetMeta({ -1, 0, 0 });
    );
    EXPECT_ANY_THROW(
        buffer.GetMeta({ 0, -1, 0 });
    );
    EXPECT_ANY_THROW(
        buffer.GetMeta({ 0, 0, -1 });
    );
    EXPECT_ANY_THROW(
        buffer.GetMeta({ 256, 0, 0 });
    );
    EXPECT_ANY_THROW(
        buffer.GetMeta({ 0, 256, 0 });
    );
    EXPECT_ANY_THROW(
        buffer.GetMeta({ 0, 0, 256 });
    );

    // Out of range SetMeta
    EXPECT_ANY_THROW(
        buffer.SetMeta({ -1, 0, 0 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetMeta({ 0, -1, 0 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetMeta({ 0, 0, -1 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetMeta({ 256, 0, 0 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetMeta({ 0, 256, 0 }, 0)
    );
    EXPECT_ANY_THROW(
        buffer.SetMeta({ 0, 0, 256 }, 0)
    );
}

TEST(ChunkDataCoordinates, SetBlockTypeAndMeta)
{
    ChunkBlockData buffer;

    // Zero's
    buffer.SetBlock({ 0, 0, 0 }, 0x0);
    buffer.SetBlock({ 0, 0, 1 }, 0xab);
    EXPECT_EQ(buffer.GetBlock({ 0, 0, 0 }), 0x0);
    EXPECT_EQ(buffer.GetBlock({ 0, 0, 1 }), 0xab);

    buffer.SetMeta({ 0, 16, 0 }, 0x0);
    buffer.SetMeta({ 0, 16, 1 }, 0xc);
    EXPECT_EQ(buffer.GetMeta({ 0, 16, 0 }), 0x0);
    EXPECT_EQ(buffer.GetMeta({ 0, 16, 1 }), 0xc);
}

TEST(ChunkDataCoordinates, Copy)
{
    // Operator =
    ChunkBlockData buffer;
    buffer.SetBlock({ 0, 0, 0 }, 0x42);
    ChunkBlockData copy;
    copy = std::move(buffer);
    EXPECT_EQ(copy.GetBlock({ 0, 0, 0 }), 0x42);
}
