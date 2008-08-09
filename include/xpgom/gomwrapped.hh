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
#ifndef GOM_WRAPPED_HH
#define GOM_WRAPPED_HH

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomWrapped      GomWrapped;
typedef struct _GomWrappedClass GomWrappedClass;

G_END_DECLS

#include <glib-object.h>
#include <nsISupports.h>

G_BEGIN_DECLS

#define GOM_TYPE_WRAPPED         (gom_wrapped_get_type ())
#define GOM_WRAPPED(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), GOM_TYPE_WRAPPED, GomWrapped))
#define GOM_WRAPPED_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST    ((k), GOM_TYPE_WRAPPED, GomWrappedClass))
#define GOM_IS_WRAPPED(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), GOM_TYPE_WRAPPED))
#define GOM_IS_WRAPPED_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE    ((k), GOM_TYPE_WRAPPED))
#define GOM_WRAPPED_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS  ((i), GOM_TYPE_WRAPPED, GomWrappedClass))

struct _GomWrapped {
    GObject parent;
};

struct _GomWrappedClass {
    GObjectClass parent_class;
};

GType gom_wrapped_get_type (void);

G_END_DECLS

typedef NS_CALLBACK (GomWrappedConstructor) (GObject *object, REFNSIID aIID, void **aResult);

#define GOM_WRAPPED_CONSTRUCTOR_INIT(_InstanceClass, _InitMethod)	\
static NS_IMETHODIMP						        \
_InstanceClass##Constructor(GObject *object, REFNSIID aIID,		\
			    void **aResult)				\
{									\
    nsresult rv;							\
    									\
    _InstanceClass * inst;						\
    									\
    *aResult = NULL;							\
    									\
    NS_NEWXPCOM(inst, _InstanceClass);					\
    if (NULL == inst) {							\
	rv = NS_ERROR_OUT_OF_MEMORY;					\
	return rv;							\
    }									\
    NS_ADDREF(inst);							\
    rv = inst->_InitMethod(object);					\
    if(NS_SUCCEEDED(rv)) {						\
	rv = inst->QueryInterface(aIID, aResult);			\
    }									\
    NS_RELEASE(inst);							\
    									\
    return rv;								\
}									\

void gom_wrapped_register_interface (GType                 g_interface,
				     GType                 g_wrapper,
				     REFNSIID              iid,
				     GomWrappedConstructor ctor);

gpointer     gom_wrap_xpcom    (nsISupports *object,
				GType        requested_interface,
				GError     **error);

nsresult     gom_wrap_g_object (gpointer    object,
				REFNSIID    iid,
				void       **retval);

#endif /* GOM_WRAPPED_HH */

