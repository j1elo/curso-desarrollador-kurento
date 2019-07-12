#ifndef MYAPP_PERSON_H
#define MYAPP_PERSON_H

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * Type declaration
 * Convention: <PREFIX>_TYPE_<CLASS>
 */
#define MYAPP_TYPE_PERSON myapp_person_get_type()

/* This helper macro declares the following:
 *
 * GType myapp_person_get_type(void);
 * typedef struct _MyappPerson MyappPerson;
 * typedef struct _MyappPersonClass MyappPersonClass;
 * struct _MyappPerson { GObject parent_instance; };
 *
 * static inline MyappPerson* MYAPP_PERSON(gpointer ptr) {
 *   return G_TYPE_CHECK_INSTANCE_CAST(ptr, myapp_person_get_type(), MyappPerson);
 * }
 * static inline MyappPersonClass* MYAPP_PERSON_CLASS(gpointer ptr) {
 *   return G_TYPE_CHECK_CLASS_CAST(ptr, myapp_person_get_type(), MyappPersonClass);
 * }
 * static inline gboolean MYAPP_IS_PERSON(gpointer ptr) {
 *   return G_TYPE_CHECK_INSTANCE_TYPE(ptr, myapp_person_get_type());
 * }
 * static inline gboolean MYAPP_IS_PERSON_CLASS(gpointer ptr) {
 *   return G_TYPE_CHECK_CLASS_TYPE(ptr, myapp_person_get_type());
 * }
 * static inline MyappPersonClass* MYAPP_PERSON_GET_CLASS(gpointer ptr) {
 *   return G_TYPE_INSTANCE_GET_CLASS(ptr, myapp_person_get_type(), MyappPersonClass);
 * }
 */
G_DECLARE_DERIVABLE_TYPE(MyappPerson, myapp_person, MYAPP, PERSON, GObject)

struct _MyappPersonClass
{
    GObjectClass parent_class;

    // Class virtual function fields
    void (*say)(MyappPerson* self, char* words);
};



// Method declarations
// -------------------

void myapp_person_say(MyappPerson* self, char* words);

G_END_DECLS

#endif  // MYAPP_PERSON_H
