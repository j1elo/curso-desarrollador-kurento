#ifndef MYAPP_STUDENT_H
#define MYAPP_STUDENT_H

#include "myapp-person.h"
#include <glib-object.h>

G_BEGIN_DECLS

// ----------------------------------------------------------------------------

/* Class public declaration
 * ========================
 */

// GType macro convention: <PREFIX>_TYPE_<CLASS>
#define MYAPP_TYPE_STUDENT myapp_student_get_type()

/* This helper macro declares the following:
 *
 * GType myapp_student_get_type(void);
 * typedef struct _MyappStudent MyappStudent;
 * typedef struct { MyappPersonClass parent_class; } MyappStudentClass;
 *
 * static inline MyappStudent* MYAPP_STUDENT(gpointer ptr) {
 *   return G_TYPE_CHECK_INSTANCE_CAST(ptr, myapp_student_get_type(), MyappStudent);
 * }
 * static inline gboolean MYAPP_IS_STUDENT(gpointer ptr) {
 *   return G_TYPE_CHECK_INSTANCE_TYPE(ptr, myapp_student_get_type());
 * }
 */
G_DECLARE_FINAL_TYPE(MyappStudent, myapp_student, MYAPP, STUDENT, MyappPerson)

// ----------------------------------------------------------------------------

G_END_DECLS

#endif  // MYAPP_STUDENT_H
