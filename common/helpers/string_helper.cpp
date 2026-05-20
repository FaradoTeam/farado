#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include "string_helper.h"

namespace common
{

std::string toLowerCase(const std::string& value)
{
    icu::UnicodeString unicodeValue = icu::UnicodeString::fromUTF8(
        icu::StringPiece(value.data())
    );
    unicodeValue.toLower();

    std::string result;
    unicodeValue.toUTF8String(result);
    return result;
}

} // namespace common
