#include "myapp-student.h"

#include <stdio.h>

// ----------------------------------------------------------------------------

/* Instance private data
 * =====================
 */

struct _MyappStudent
{
    MyappPerson parent_instance;

    // Other members, including private data
    gchar* favorite_pizza;
};

/* This helper macro defines the following:
 *
 * static void myapp_student_init(MyappStudent* self);
 * static void myapp_student_class_init(MyappStudentClass* klass);
 * static gpointer myapp_student_parent_class = NULL;
 * static gint MyappStudent_private_offset;
 *
 * static void
 * myapp_student_class_intern_init(gpointer klass)
 * { ... }
 *
 * static inline gpointer
 * myapp_student_get_instance_private(MyappStudent* self)
 * {
 *   return (G_STRUCT_MEMBER_P(self, MyappStudent_private_offset));
 * }
 *
 * GType
 * myapp_student_get_type(void)
 * { ... }
 */
G_DEFINE_TYPE(MyappStudent, myapp_student, MYAPP_TYPE_PERSON)

// ----------------------------------------------------------------------------

/* Class methods
 * =============
 */

static void
myapp_student_say_impl(MyappStudent* self, char* words)
{
  printf("myapp_student_say_impl()\n");

  char* name;
  int age;

  g_object_get(self, "name", &name, "age", &age, NULL);

  printf("Hey yo! I'm Student '%s', '%d' years old, listen now: '%s'\n",
         name, age,
         words);

  g_free(name);
}

// ----------------------------------------------------------------------------

/* Class constructor
 * =================
 */

static void
myapp_student_init(MyappStudent* self)
{
  printf("myapp_student_init()\n");

  // Initialize all public and private members to reasonable default values.
  // They are all automatically initialized to 0 to begin with.
  //priv->favorite_pizza = g_strdup("Pepperoni");
}

static void
myapp_student_class_init(MyappStudentClass* klass)
{
  printf("myapp_student_class_init()\n");

  // Override virtual methods from GObject
  //GObjectClass* object_class = G_OBJECT_CLASS(klass);
  //object_class->set_property = myapp_student_set_property;
  //object_class->get_property = myapp_student_get_property;

  // Override virtual methods from MyappPerson
  MyappPersonClass* person_class = MYAPP_PERSON_CLASS(klass);
  person_class->say = myapp_student_say_impl;
}

// ----------------------------------------------------------------------------
