#include "strings.h"
#include <string.h>

uint16_t strncasecmp(const char* a, const char* b, size_t len) {
	return _strnicmp(a,b,len);
}
