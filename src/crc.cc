#include "basic.hpp"
#include "crc/src/checksum.h"
#include "def_export.hpp"

#include "crc/crc.hpp"
#include <fstream>
#include <functional>
#include <memory>
#include <stdint.h>

EXPORT_API(void, crc_init, 0)
()
{
    init_crc16_tab();
    init_crc32_tab();
    init_crc64_tab();
    init_crcccitt_tab();
    init_crcdnp_tab();
    init_crc_tab();
}

EXPORT_API(uint8_t, crc8_update, (parameterSizeof<uint8_t, uint8_t>()))
(uint8_t crc, uint8_t c)
{
    return update_crc_8(crc, c);
}

EXPORT_API(uint16_t, crc16_update, (parameterSizeof<uint16_t, uint8_t>()))
(uint16_t crc, uint8_t c)
{
    return update_crc_16(crc, c);
}

EXPORT_API(uint32_t, crc32_update, (parameterSizeof<uint32_t, uint8_t>()))
(uint32_t crc, uint8_t c)
{
    return update_crc_32(crc, c);
}

EXPORT_API(uint64_t, crc64_update, (parameterSizeof<uint64_t, uint8_t>()))
(uint64_t crc, uint8_t c)
{
    return update_crc_64(crc, c);
}

EXPORT_API(uint16_t, crc_ccitt_update, (parameterSizeof<uint16_t, uint8_t>()))
(uint16_t crc, uint8_t c)
{
    return update_crc_ccitt(crc, c);
}

EXPORT_API(uint16_t, crc_dnp_update, (parameterSizeof<uint16_t, uint8_t>()))
(uint16_t crc, uint8_t c)
{
    return update_crc_dnp(crc, c);
}

EXPORT_API(uint16_t, crc_kermit_update, (parameterSizeof<uint16_t, uint8_t>()))
(uint16_t crc, uint8_t c)
{
    return update_crc_kermit(crc, c);
}

EXPORT_API(uint16_t, crc_sick_update, (parameterSizeof<uint16_t, uint8_t, uint8_t>()))
(uint16_t crc, uint8_t c, uint8_t prev_byte)
{
    return update_crc_sick(crc, c, prev_byte);
}

EXPORT_API(uint64_t, crc_string, (parameterSizeof<crc::Code, cstr_t>()))
(crc::Code code, cstr_t str)
{
    return crc::Crc(code, str).value();
}

EXPORT_API(uint64_t, crc_bytes, (parameterSizeof<crc::Code, uint8_t*, int32_t>()))
(crc::Code code, uint8_t* bytes, int32_t size)
{
    return crc::Crc(code, bytes, bytes + size).value();
}

EXPORT_API(uint64_t, crc_file, (parameterSizeof<crc::Code, cstr_t>()))
(crc::Code code, cstr_t file)
{
    std::ifstream ifs(file, std::ios::binary);
    auto destructor_callback = [](std::ifstream *ifs) {
        if (*ifs) ifs->close();
    };
    std::unique_ptr<std::ifstream, decltype(destructor_callback)> destructor(&ifs, destructor_callback);

    auto value = crc::Crc(code, ifs).value();
    
    return value;
}
