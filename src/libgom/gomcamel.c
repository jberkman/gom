/*
The MIT License

Copyright (c) 2008 jacob berkman <jacob@ilovegom.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "config.h"

#include "gom/gomcamel.h"

#include <glib/gstrfuncs.h>

#include <string.h>

const char *
gom_camel_case (const char *s)
{
    int si, ri;
    char *r = (char *)s;
    gboolean upper = FALSE;
    for (ri = si = 0; s[si]; si++) {
        switch (s[si]) {
        case '-':
        case '_':
            upper = TRUE;
            break;
        default:
            if (upper) {
                if (r == s) {
                    r = g_malloc (strlen (s) + 1);
                    memcpy (r, s, ri);
                }
                r[ri++] = g_ascii_toupper (s[si]);
                upper = FALSE;
            } else if (r != s) {
                r[ri++] = s[si];
            } else {
                ri++;
            }
            break;
        }
    }
    if (r != s) {
        r[ri] = s[si];
    }
#if 0
    g_print ("\n"G_STRLOC": %s -> %s\n", s, r);
#endif
    return (const char *)r;
}

const char *
gom_camel_uncase (const char *s)
{
    int si, ri;
    char *r = (char *)s;
    for (si = 0; s[si]; si++) {
        if (g_ascii_isupper (s[si])) {
            if (r == s) {
                r = g_malloc (si + 2 * strlen (s+si) + 1);
                memcpy (r, s, si);
                ri = si;
            }
            r[ri++] = '_';
            r[ri++] = g_ascii_tolower (s[si]);
        } else if (r != s) {
            r[ri++] = s[si];
        }
    }
    if (r != s) {
        r[ri] = s[si];
    }
#if 0
    g_print (G_STRLOC": %s -> %s\n", s, r);
#endif
    return (const char *)r;
}
