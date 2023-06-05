#pragma once

namespace yuki::unicode{
char32_t to_upper(char32_t);
char32_t to_lower(char32_t);
char32_t to_title(char32_t);

char32_t case_fold(char32_t);
}
