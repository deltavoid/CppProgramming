#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <string_view>
#include <vector>


template <typename CharT, typename Traits = std::char_traits<CharT>>
using basic_string_view = std::basic_string_view<CharT, Traits>;

using rss_key_type = basic_string_view<uint8_t>;

// Mellanox Linux's driver key
static constexpr uint8_t default_rsskey_40bytes[] = {
    0xd1, 0x81, 0xc6, 0x2c, 0xf7, 0xf4, 0xdb, 0x5b,
    0x19, 0x83, 0xa2, 0xfc, 0x94, 0x3e, 0x1a, 0xdb,
    0xd9, 0x38, 0x9e, 0x6b, 0xd1, 0x03, 0x9c, 0x2c,
    0xa7, 0x44, 0x99, 0xad, 0x59, 0x3d, 0x56, 0xd9,
    0xf3, 0x25, 0x3c, 0x06, 0x2a, 0xdc, 0x1f, 0xfc
};


int main()
{
    printf("main: 1\n");
    rss_key_type _rss_key;

    printf("main: 2\n");
    _rss_key = default_rsskey_40bytes;
    // _rss_key = rss_key_type(default_rsskey_40bytes);
   
    printf("main: 3\n");
    return 0;
}