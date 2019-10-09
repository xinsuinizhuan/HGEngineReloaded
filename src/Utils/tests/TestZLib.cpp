#include <HG/Utils/zlib.hpp>
#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>
#include <string_view>

using namespace std::literals;

TEST(Utils, ZLibDeflateDefault)
{
    auto data        = "SOME EXAMPLE DATA TO DEFLATE DEFLATE DEFLATE DEFLATE"sv;
    uint8_t result[] = {0x78, 0xda, 0x0b, 0xf6, 0xf7, 0x75, 0x55, 0x70, 0x8d, 0x70, 0xf4, 0x0d, 0xf0,
                        0x71, 0x55, 0x70, 0x71, 0x0c, 0x71, 0x54, 0x08, 0xf1, 0x57, 0x70, 0x71, 0x75,
                        0xf3, 0x71, 0x0c, 0x71, 0xc5, 0x45, 0x03, 0x00, 0x6c, 0x05, 0x0d, 0xb2};

    std::vector<std::byte> target;

    HG::Utils::ZLib::Deflate(data.data(), data.length(), target, HG::Utils::ZLib::CompressionLevel::BestCompression);

    ASSERT_EQ(target.size(), 38);

    for (std::size_t i = 0; i < 38; ++i)
    {
        ASSERT_EQ((uint8_t)result[i], (uint8_t)target[i]) << "Deflating of \"" << data << "\" is wrong at index " << i;
    }
}

TEST(Utils, ZLibDeflateHuge)
{
    auto dataSample = "SIZE OF THIS STRING IS 32  BYTES"sv;

    std::vector<char> data;

    for (int i = 0; i < (20 * 1024 / 32); ++i)
    {
        for (char j : dataSample)
        {
            data.push_back(j);
        }
    }

    uint8_t result[] = {0x78, 0xda, 0xed, 0xc9, 0xb1, 0x09, 0x80, 0x40, 0x0c, 0x00, 0xc0, 0x55, 0x32, 0x83, 0x6e,
                        0x20, 0xbc, 0x9a, 0x46, 0xc1, 0xa4, 0xd1, 0xfd, 0x07, 0xd1, 0x1d, 0x6c, 0xaf, 0x3b, 0xb8,
                        0xca, 0x67, 0xc4, 0xb9, 0x46, 0xef, 0x59, 0x51, 0x7d, 0xe5, 0xb1, 0xc5, 0xa7, 0x79, 0x8a,
                        0x58, 0xee, 0x1e, 0x55, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde,
                        0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde,
                        0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde,
                        0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xff, 0xfd, 0x2f, 0x67, 0xe2, 0xb2, 0xad};

    std::vector<std::byte> target;

    HG::Utils::ZLib::Deflate(data.data(), data.size(), target, HG::Utils::ZLib::CompressionLevel::BestCompression);

    ASSERT_EQ(target.size(), 105);

    for (std::size_t i = 0; i < 105; ++i)
    {
        ASSERT_EQ((uint8_t)result[i], (uint8_t)target[i]) << "Deflating of huge amount of data is wrong at index " << i;
    }
}

TEST(Utils, ZLibInflateHuge)
{
    auto resultSample = "SIZE OF THIS STRING IS 32  BYTES"sv;

    std::vector<uint8_t> data = {
        0x78, 0xda, 0xed, 0xc9, 0xb1, 0x09, 0x80, 0x40, 0x0c, 0x00, 0xc0, 0x55, 0x32, 0x83, 0x6e, 0x20, 0xbc, 0x9a,
        0x46, 0xc1, 0xa4, 0xd1, 0xfd, 0x07, 0xd1, 0x1d, 0x6c, 0xaf, 0x3b, 0xb8, 0xca, 0x67, 0xc4, 0xb9, 0x46, 0xef,
        0x59, 0x51, 0x7d, 0xe5, 0xb1, 0xc5, 0xa7, 0x79, 0x8a, 0x58, 0xee, 0x1e, 0x55, 0xde, 0x7b, 0xef, 0xbd, 0xf7,
        0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef,
        0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xf7, 0xde,
        0x7b, 0xef, 0xbd, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xff, 0xfd, 0x2f, 0x67, 0xe2, 0xb2, 0xad};

    std::vector<uint8_t> target;

    ASSERT_TRUE(HG::Utils::ZLib::Inflate(data.data(), data.size(), target)) << "Inflating failed";

    ASSERT_EQ(target.size(), 20 * 1024);

    for (int i = 0; i < (20 * 1024 / 32); ++i)
    {
        ASSERT_EQ(target[i], resultSample[i % resultSample.size()]);
    }
}

TEST(Utils, ZLibInflateStreamToStream)
{
    std::ifstream input("zlib/stream.bin");

    ASSERT_TRUE(input.is_open());

    std::stringstream output;

    std::size_t inflated = 0;
    ASSERT_NO_THROW(inflated = HG::Utils::ZLib::InflateStreamToStream(input, output, 1024));

    ASSERT_EQ(inflated, 19);
    ASSERT_EQ(output.str(), "THIS IS SAMPLE TEXT");

    output.str("");

    ASSERT_NO_THROW(inflated = HG::Utils::ZLib::InflateStreamToStream(input, output, 1024));

    ASSERT_EQ(inflated, 17);
    ASSERT_EQ(output.str(), "SOME_ANOTHER_DATA");
}

TEST(Utils, ZLIbDeflateStreamToStream)
{
    uint8_t expected[] = {0x78, 0xda, 0xb,  0xf6, 0xf7, 0x75, 0x55, 0x8,  0x76, 0xf4, 0xd, 0xf0,
                          0x71, 0x55, 0xf0, 0xf1, 0xf7, 0x73, 0x57, 0x8,  0x71, 0x8d, 0x8, 0x51,
                          0x8,  0x1e, 0x15, 0x1d, 0xdc, 0xa2, 0x0,  0x9d, 0x1a, 0x76, 0x3d};

    std::ifstream input("zlib/some_text.txt");

    ASSERT_TRUE(input.is_open());

    std::stringstream output;

    std::size_t deflated = 0;
    ASSERT_NO_THROW(deflated = HG::Utils::ZLib::DeflateStreamToStream(input, output, 1024));

    ASSERT_EQ(deflated, 35);

    auto string = output.str();

    ASSERT_EQ(string.size(), 35);

    for (std::size_t i = 0; i < string.size(); ++i)
    {
        ASSERT_EQ(static_cast<uint8_t>(string[i]), expected[i]) << i << " byte is differ";
    }
}
