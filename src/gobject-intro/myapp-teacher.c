#include "myapp-teacher.h"

#include <stdio.h>

// ----------------------------------------------------------------------------

/* Instance private data
 * =====================
 */

typedef struct {
  gchar* subject;
} MyappTeacherPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(MyappTeacher, myapp_teacher, MYAPP_TYPE_PERSON)

#define GET_PRIVATE(instance) \
    myapp_teacher_get_instance_private(MYAPP_TEACHER(instance))

// ----------------------------------------------------------------------------

/* Instance properties
 * ===================
 */

enum {
  PROP_SUBJECT = 1,
  N_PROPERTIES
};

// static GParamSpec* obj_properties[N_PROPERTIES] = { 0 };

void
myapp_teacher_set_property(GObject* object, guint property_id,
                          const GValue* value, GParamSpec* pspec)
{
  MyappTeacherPrivate* priv = GET_PRIVATE(object);

  switch (property_id) {
  case PROP_SUBJECT:
    g_free(priv->subject);
    priv->subject = g_value_dup_string(value);
    printf("myapp_teacher_set_property(), subject: %s\n", priv->subject);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;
  }
}

void
myapp_teacher_get_property(GObject* object, guint property_id,
                          GValue* value, GParamSpec* pspec)
{
  printf("myapp_teacher_get_property(), property_id: %u\n", property_id);

  MyappTeacherPrivate* priv = GET_PRIVATE(object);

  switch (property_id) {
  case PROP_SUBJECT:
    g_value_set_string(value, priv->subject);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;
  }
}

// ----------------------------------------------------------------------------

/* Class methods
 * =============
 */

static void
myapp_teacher_say_impl(MyappTeacher* self, char* words)
{
  printf("myapp_teacher_say_impl()\n");

  MyappTeacherPrivate* priv = GET_PRIVATE(self);

  char* name = NULL;
  int age = 0;

  g_object_get (self, "name", &name, "age", &age, NULL);

  printf("I am Teacher '%s', aged '%d', and I say study a lot of '%s'!\n",
      name, age, priv->subject);

  g_free (name); name = NULL;
}

// ----------------------------------------------------------------------------

/* Class destructor
 * ================
 *
 * Unreference all objects in `dispose()`,
 * and free all memory or close file descriptors in `finalize()`.
 */

static void
myapp_teacher_dispose(GObject* object)
{
  printf("myapp_teacher_dispose()\n");

  // Chain up
  G_OBJECT_CLASS(myapp_teacher_parent_class)->dispose(object);
}

static void
myapp_teacher_finalize(GObject* object)
{
  printf("myapp_teacher_finalize()\n");

  MyappTeacherPrivate* priv = GET_PRIVATE(object);

  // Free all memory allocated by this object
  g_free(priv->subject); priv->subject = NULL;
  // Alternative:
  //g_clear_pointer(&priv->subject, g_free);

  // Chain up
  G_OBJECT_CLASS(myapp_teacher_parent_class)->finalize(object);
}

// ----------------------------------------------------------------------------

/* Class constructor
 * =================
 */

static void
myapp_teacher_init(MyappTeacher* self)
{
  printf("myapp_teacher_init()\n");

  MyappTeacherPrivate* priv = GET_PRIVATE(self);

  // Initialize all public and private members to reasonable default values.
  // They are all automatically initialized to 0 to begin with.
  priv->subject = g_strdup("Conocimiento del Medio");
}

static void
myapp_teacher_class_init(MyappTeacherClass* klass)
{
  GParamSpec* obj_properties[N_PROPERTIES] = { 0 };

  printf("myapp_teacher_class_init()\n");

  // Override virtual methods from GObject
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->set_property = myapp_teacher_set_property;
  object_class->get_property = myapp_teacher_get_property;
  object_class->dispose = myapp_teacher_dispose;
  object_class->finalize = myapp_teacher_finalize;

  // Override virtual methods from MyappPerson
  MyappPersonClass* person_class = MYAPP_PERSON_CLASS(klass);
  person_class->say = myapp_teacher_say_impl;

  // Register object properties
  obj_properties[PROP_SUBJECT] =
      g_param_spec_string("subject",
                          "teacher's Subject",
                          "Peferred subject of this teacher",
                          NULL,  // Default value
                          G_PARAM_READWRITE);

  g_object_class_install_properties(object_class,
                                    N_PROPERTIES, obj_properties);
}

// ----------------------------------------------------------------------------
