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
#ifndef GOM_CHARACTER_DATA_H
#define GOM_CHARACTER_DATA_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomCharacterData GomCharacterData; /* dummy object */
typedef struct _GomCharacterDataInterface GomCharacterDataInterface;

G_END_DECLS

#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_CHARACTER_DATA             (gom_character_data_get_type ())
#define GOM_CHARACTER_DATA(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_CHARACTER_DATA, GomCharacterData))
#define GOM_IS_CHARACTER_DATA(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_CHARACTER_DATA))
#define GOM_CHARACTER_DATA_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_CHARACTER_DATA, GomCharacterDataInterface))

struct _GomCharacterDataInterface {
    GTypeInterface parent;

    const char *(*substring_data) (GomCharacterData *cdata,
                                   gulong offset, gulong count,
                                   GError **error);

    void (*append_data) (GomCharacterData *cdata, const char *arg, GError **error);

    void (*insert_data) (GomCharacterData *cdata,
                         gulong offset, const char *arg,
                         GError **error);

    void (*delete_data) (GomCharacterData *cdata,
                         gulong offset, gulong count,
                         GError **error);

    void (*replace_data) (GomCharacterData *cdata,
                          gulong offset, gulong count,
                          const char *arg,
                          GError **error);
};

GType gom_character_data_get_type (void);

const char *gom_character_data_substring_data (GomCharacterData *cdata,
                                               gulong offset, gulong count,
                                               GError **error);

void gom_character_data_append_data (GomCharacterData *cdata, 
                                     const char *arg,
                                     GError **error);

void gom_character_data_insert_data (GomCharacterData *cdata,
                                     gulong offset, const char *arg,
                                     GError **error);

void gom_character_data_delete_data (GomCharacterData *cdata,
                                     gulong offset, gulong count,
                                     GError **error);

void gom_character_data_replace_data (GomCharacterData *cdata,
                                      gulong offset, gulong count,
                                      const char *arg,
                                      GError **error);

#endif /* GOM_CHARACTER_DATA_H */
