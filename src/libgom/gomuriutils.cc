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

#include "gom/gomuriutils.h"

#include <nsCOMPtr.h>
#include <nsIChannel.h>
#include <nsIIOService.h>
#include <nsIInputStream.h>
#include <nsIURI.h>
#include <nsServiceManagerUtils.h>
#include <nsStringAPI.h>

#include <glib/garray.h>
#include <glib/gmessages.h>
#include <glib/gstrfuncs.h>
#include <glib/gtestutils.h>
#include <glib/gthread.h>

#include <stdio.h>

#include "gommacros.h"

G_BEGIN_DECLS

GOM_DEFINE_QUARK (nsresult_exception_error);

char *
gom_uri_join (const char *uri, const char *base_uri, GError **error)
{
    nsCAutoString spec;
    nsCOMPtr<nsIURI> base_nsuri;
    nsCOMPtr<nsIIOService> io;
    nsCOMPtr<nsIURI> nsuri;
    nsresult rv;

    io = do_GetService ("@mozilla.org/network/io-service;1", &rv);
    if (!io) {
	g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
		     "Could not get IO Service");
	return NULL;
    }

    if (base_uri) {
	rv = io->NewURI (nsCAutoString (base_uri), "UTF-8", NULL, getter_AddRefs (base_nsuri));
	if (!base_nsuri) {
	    g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
			 "Could not get base uri");
	    return NULL;
	}
    }

    rv = io->NewURI (nsCAutoString (uri), "UTF-8", base_nsuri, getter_AddRefs (nsuri));
    if (!nsuri) {
	g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
		     "Could not get uri");
	return NULL;
    }

    rv = nsuri->GetSpec (spec);
    if (NS_FAILED (rv)) {
	g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
		     "Could not get spec");
	return NULL;
    }

    return g_strdup (spec.get());
}

gboolean
gom_uri_get_contents (const char  *uri,
		      const char  *base_uri,
		      char       **contents,
		      gsize       *length,
		      GError     **error)
{
    GByteArray *buf;
    PRUint32 bytes_read;
    nsCOMPtr<nsIURI> base_nsuri;
    nsCOMPtr<nsIChannel> chan;
    nsCOMPtr<nsIIOService> io;
    nsCOMPtr<nsIInputStream> stream;
    nsresult rv;
    
    io = do_GetService ("@mozilla.org/network/io-service;1", &rv);
    if (!io) {
	g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
		     "Could not get IO Service");
	return FALSE;
    }

    if (base_uri) {
	rv = io->NewURI (nsCAutoString (base_uri), "UTF-8", NULL, getter_AddRefs (base_nsuri));
	if (!base_nsuri) {
	    g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
			 "Could not get base uri");
	    return NULL;
	}
    }

    rv = io->NewChannel (nsCAutoString(uri), "UTF-8", base_nsuri, getter_AddRefs (chan));
    if (!chan) {
	g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
		     "Could not create channel");
	return FALSE;
    }

    rv = chan->Open (getter_AddRefs (stream));
    if (!stream) {
	g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
		     "Could not open channel");
	return FALSE;
    }
   
    buf = g_byte_array_sized_new (BUFSIZ);
    while (1) {
	rv = stream->Read ((char *)buf->data + buf->len, BUFSIZ, &bytes_read);
	if (NS_FAILED (rv)) {
	    if (rv == NS_BASE_STREAM_WOULD_BLOCK) {
		continue;
	    }
	    stream->Close();
	    g_byte_array_free (buf, TRUE);
	    g_set_error (error, GOM_NSRESULT_EXCEPTION_ERROR, rv,
			 "Error reading stream");
	    return FALSE;
	}
	if (bytes_read == 0) {
	    stream->Close();
	    g_byte_array_set_size (buf, buf->len + 1);
	    buf->data[buf->len - 1] = '\0';
	    if (length) {
		*length = buf->len - 1;
	    }
	    *contents = (char *)g_byte_array_free (buf, FALSE);
	    return TRUE;
	}
	/*
	 * something should really be said here but i just haven't the
	 * words
	 */
	buf->len += bytes_read;
	g_byte_array_set_size (buf, buf->len + BUFSIZ);
	buf->len -= BUFSIZ;
    }
    g_assert_not_reached ();
}

G_END_DECLS
