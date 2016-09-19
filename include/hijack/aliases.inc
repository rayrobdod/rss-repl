/*
 * Blahblahblah something-something perl is a thing that exists only in theory
 * 
 * A stub version of what `parserutils/build/make-aliases.pl` would produce
 */

#define MIBENUM_IS_UNICODE(x) (x == 106 || (x >= 1013 && x <= 1015))
static const uint16_t charset_aliases_canon_count = 5;
static const uint16_t charset_aliases_count = 2;


static parserutils_charset_aliases_canon canonical_charset_names[] = {
	{106, 5, "UTF-8"},
	{1015, 6, "UTF-16"},
	{1013, 8, "UTF-16LE"},
	{1014, 8, "UTF-16BE"},
	{3, 8, "US-ASCII"}
};

typedef struct {
	uint16_t name_len;
	const char *name;
	parserutils_charset_aliases_canon *canon;
} parserutils_charset_aliases_alias;

static parserutils_charset_aliases_alias charset_aliases[] = {
	{4, "utf8", &canonical_charset_names[0]},
	{5, "ASCII", &canonical_charset_names[4]}
};

