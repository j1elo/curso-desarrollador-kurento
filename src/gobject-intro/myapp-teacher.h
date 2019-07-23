#ifndef MYAPP_TEACHER_H
#define MYAPP_TEACHER_H

#include "myapp-person.h"
#include <glib-object.h>

G_BEGIN_DECLS

// ----------------------------------------------------------------------------

/* Class public declaration
 * ========================
 */

// GType macro convention: <PREFIX>_TYPE_<CLASS>
#define MYAPP_TYPE_TEACHER myapp_teacher_get_type()
G_DECLARE_DERIVABLE_TYPE(MyappTeacher, myapp_teacher, MYAPP, TEACHER, MyappPerson)

struct _MyappTeacherClass
{
    MyappPersonClass parent_class;
};

// ----------------------------------------------------------------------------

/* Class methods
 * =============
 */

// ----------------------------------------------------------------------------

G_END_DECLS

#endif  // MYAPP_TEACHER_H
