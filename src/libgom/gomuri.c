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
#include <Python.h>

#include "config.h"

#include "gom/gomuri.h"

enum {
    PROP_URI = 1,
    PROP_SCHEME,
    PROP_NETLOC,
    PROP_PATH,
    PROP_QUERY,
    PROP_FRAGMENT,
    PROP_USERNAME,
    PROP_PASSWORD,
    PROP_HOSTNAME,
    PROP_PORT
};

typedef struct {
    char *uri;
    char *scheme;
    char *netloc;
    char *path;
    char *query;
    char *fragment;
    char *username;
    char *password;
    char *hostname;
    int port;
} GomURIPrivate;

static PyObject *urlsplit   = NULL;
static PyObject *urlunsplit = NULL;
static PyObject *urljoin    = NULL;
static PyObject *urldefrag  = NULL;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_URI, GomURIPrivate)

static gpointer
init_python_once (gpointer data)
{
    PyObject *name;
    PyObject *mod;

    Py_Initialize();

    name = PyString_FromString ("urlparse");
    mod = PyImport_Import (name);
    Py_DECREF (name);

    if (!mod) {
        PyErr_Print ();
        g_warning (G_STRLOC": could not load urlparse module");
        return NULL;
    }

#define GET_FUNC(f)                                             \
    f = PyObject_GetAttrString (mod, #f);                       \
    if (!f || !PyCallable_Check (f)) {                          \
        if (PyErr_Occurred ()) {                                \
            PyErr_Print ();                                     \
        }                                                       \
        g_warning (G_STRLOC": could not look up "#f);           \
    }

    GET_FUNC (urlsplit);
    GET_FUNC (urlunsplit);
    GET_FUNC (urljoin);
    GET_FUNC (urldefrag);

    Py_DECREF (mod);

    return NULL;
}

static void
init_python (void)
{
    static GOnce python_once = G_ONCE_INIT;
    g_once (&python_once, init_python_once, NULL);
}

gboolean
gom_uri_is_relative (const GomURI *uri)
{
    GomURIPrivate *priv = PRIV (uri);
    return *priv->scheme && *priv->netloc && *priv->path == '/';
}

char *
gom_uri_join (const char *base_uri, const char *relative_uri)
{
    PyObject *args;
    PyObject *ret;
    char *new_uri;

    init_python ();
   
    args = PyTuple_New (2);
    PyTuple_SetItem (args, 0, PyString_FromString (base_uri));
    PyTuple_SetItem (args, 1, PyString_FromString (relative_uri));

    ret = PyObject_CallObject (urljoin, args);
    Py_DECREF (args);
    if (!ret) {
        PyErr_Print ();
        g_warning (G_STRLOC": urljoin(%s, %s) failed", base_uri, relative_uri);
        return NULL;
    }

    new_uri = g_strdup (PyString_AsString (ret));
    Py_DECREF (ret);
    return new_uri;
}

static void
gom_uri_get_property (GObject    *object,
                      guint       property_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
    GomURIPrivate *priv = PRIV (object);
    switch (property_id) {
    case PROP_URI:
        g_value_set_string (value, priv->uri);
        break;
    case PROP_SCHEME:
        g_value_set_string (value, priv->scheme);
        break;
    case PROP_NETLOC:
        g_value_set_string (value, priv->netloc);
        break;
    case PROP_PATH:
        g_value_set_string (value, priv->path);
        break;
    case PROP_QUERY:
        g_value_set_string (value, priv->query);
        break;
    case PROP_FRAGMENT:
        g_value_set_string (value, priv->fragment);
        break;
    case PROP_USERNAME:
        g_value_set_string (value, priv->username);
        break;
    case PROP_PASSWORD:
        g_value_set_string (value, priv->password);
        break;
    case PROP_HOSTNAME:
        g_value_set_string (value, priv->hostname);
        break;
    case PROP_PORT:
        g_value_set_int (value, priv->port);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
free_and_xxx (char *s)
{
    char *c;
    if (!s) {
        return;
    }
    for (c = s; *c; c++) {
        *c = 'X';
    }
    g_free (s);
}

static void
free_priv (GomURIPrivate *priv)
{
    free_and_xxx (priv->uri);
    g_free (priv->scheme);
    free_and_xxx (priv->netloc);
    g_free (priv->path);
    g_free (priv->query);
    g_free (priv->fragment);
    g_free (priv->username);
    free_and_xxx (priv->password);
    g_free (priv->hostname);
    memset (priv, 0, sizeof (GomURIPrivate));
}

static void
uri_split (GomURI *uri, const char *new_uri)
{
    GomURIPrivate *priv = PRIV (uri);
    PyObject *args;
    PyObject *ret;
    PyObject *val;

    free_priv (priv);

    args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, PyString_FromString (new_uri));

    ret = PyObject_CallObject (urlsplit, args);
    Py_DECREF (args);
    if (!ret) {
        PyErr_Print ();
        g_warning (G_STRLOC": urlsplit(%s) failed", new_uri);
        return;
    }

    priv->uri = g_strdup (new_uri);

#define GET_ATTR(a)                                      \
    val = PyObject_GetAttrString (ret, #a);              \
    if (val) {                                           \
        priv->a = g_strdup (PyString_AsString (val));    \
        Py_DECREF (val);                                 \
    }

    GET_ATTR(scheme);
    GET_ATTR(netloc);
    GET_ATTR(path);
    GET_ATTR(query);
    GET_ATTR(fragment);
    GET_ATTR(username);
    GET_ATTR(password);
    GET_ATTR(hostname);

#undef GET_ATTR

    val = PyObject_GetAttrString (ret, "port");
    priv->port = PyInt_AsLong (val);
    Py_DECREF (val);

    Py_DECREF (args);
    Py_DECREF (ret);
}

static void
uri_unsplit (GomURI *uri)
{
    GomURIPrivate *priv = PRIV (uri);
    PyObject *tuple;
    PyObject *args;
    PyObject *ret;

    tuple = PyTuple_New (5);
    PyTuple_SetItem (tuple, 0, PyString_FromString (priv->scheme   ? priv->scheme   : ""));
    PyTuple_SetItem (tuple, 1, PyString_FromString (priv->netloc   ? priv->netloc   : ""));
    PyTuple_SetItem (tuple, 2, PyString_FromString (priv->path     ? priv->path     : ""));
    PyTuple_SetItem (tuple, 3, PyString_FromString (priv->query    ? priv->query    : ""));
    PyTuple_SetItem (tuple, 4, PyString_FromString (priv->fragment ? priv->fragment : ""));

    args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, tuple);

    ret = PyObject_CallObject (urlunsplit, args);
    Py_DECREF (args);
    Py_DECREF (tuple);
    if (!ret) {
        PyErr_Print ();
        g_warning (G_STRLOC": urlunsplit(\"%s\", \"%s\", \"%s, \"%s\", \"%s\") failed\n",
                   priv->scheme, priv->netloc, priv->path,
                   priv->query, priv->fragment);
        return;
    }

    uri_split (uri, PyString_AsString (ret));

    Py_DECREF (ret);
}

static void
fixup_netloc (GomURI *uri)
{
    GomURIPrivate *priv = PRIV (uri);
    free_and_xxx (priv->netloc);

    GString *str = g_string_new (NULL);
    if (priv->username) {
        g_string_append (str, priv->username);
        if (priv->password) {
            g_string_append_c (str, ':');
            g_string_append (str, priv->password);
        }
        g_string_append_c (str, '@');
    }
    if (priv->hostname) {
        g_string_append (str, priv->hostname);
    }
    g_string_append_c (str, ':');
    g_string_append_printf (str, "%d", priv->port);

    priv->netloc = g_string_free (str, FALSE);

    uri_unsplit (uri);
}

static void
gom_uri_set_property (GObject      *object,
                      guint         property_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
    GomURIPrivate *priv = PRIV (object);
    switch (property_id) {
    case PROP_URI:
        uri_split (GOM_URI (object), g_value_get_string (value));
        break;
    case PROP_SCHEME:
        g_free (priv->scheme);
        priv->scheme = g_value_dup_string (value);
        uri_unsplit (GOM_URI (object));
        break;
    case PROP_NETLOC:
        free_and_xxx (priv->netloc);
        priv->netloc = g_value_dup_string (value);
        uri_unsplit (GOM_URI (object));
        break;
    case PROP_PATH:
        g_free (priv->path);
        priv->path = g_value_dup_string (value);
        uri_unsplit (GOM_URI (object));
        break;
    case PROP_QUERY:
        g_free (priv->query);
        priv->query = g_value_dup_string (value);
        uri_unsplit (GOM_URI (object));
        break;
    case PROP_FRAGMENT:
        g_free (priv->fragment);
        priv->fragment = g_value_dup_string (value);
        uri_unsplit (GOM_URI (object));
        break;
    case PROP_USERNAME:
        g_free (priv->username);
        priv->username = g_value_dup_string (value);
        fixup_netloc (GOM_URI (object));
        break;
    case PROP_PASSWORD:
        free_and_xxx (priv->password);
        priv->password = g_value_dup_string (value);
        fixup_netloc (GOM_URI (object));
        break;
    case PROP_HOSTNAME:
        g_free (priv->hostname);
        priv->hostname = g_value_dup_string (value);
        fixup_netloc (GOM_URI (object));
        break;
    case PROP_PORT:
        priv->port = g_value_get_int (value);
        fixup_netloc (GOM_URI (object));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

G_DEFINE_TYPE(GomURI, gom_uri, G_TYPE_OBJECT);

static void gom_uri_init (GomURI *uri) { }

static void
gom_uri_dispose (GObject *object)
{
    free_priv (PRIV (object));

    G_OBJECT_CLASS (gom_uri_parent_class)->dispose (object);
}

static void
gom_uri_class_init (GomURIClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    init_python ();

    g_type_class_add_private (klass, sizeof (GomURIPrivate));

    oclass->get_property = gom_uri_get_property;
    oclass->set_property = gom_uri_set_property;
    oclass->dispose      = gom_uri_dispose;

    g_object_class_install_property (
        oclass, PROP_URI,
        g_param_spec_string ("uri", NULL,
                             "The URI this object represents.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_SCHEME,
        g_param_spec_string ("scheme", NULL,
                             "URL scheme specifier.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
    
    g_object_class_install_property (
        oclass, PROP_NETLOC,
        g_param_spec_string ("netloc", NULL,
                             "Network location part.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_PATH,
        g_param_spec_string ("path", NULL,
                             "Hierarchical path.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_QUERY,
        g_param_spec_string ("query", NULL,
                             "Query component.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_FRAGMENT,
        g_param_spec_string ("fragment", NULL,
                             "Fragment identifier.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_USERNAME,
        g_param_spec_string ("username", NULL,
                             "User name.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_PASSWORD,
        g_param_spec_string ("password", NULL,
                             "Password.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_HOSTNAME,
        g_param_spec_string ("hostname", NULL,
                             "Host name (lower case).",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_PORT,
        g_param_spec_int ("port", NULL,
                          "Port number as integer, if present.",
                          G_MININT, G_MAXINT, 0,
                          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}
