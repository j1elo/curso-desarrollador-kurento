#include "myapp-person.h"

#include <stdio.h>

// ----------------------------------------------------------------------------

/* Instance private data
 * =====================
 */

typedef struct {
  gchar* name;
  int age;
} MyappPersonPrivate;

/* This helper macro defines the following:
 *
 * static void myapp_person_init(MyappPerson* self);
 * static void myapp_person_class_init(MyappPersonClass* klass);
 * static gpointer myapp_person_parent_class = NULL;
 * static gint MyappPerson_private_offset;
 *
 * static void
 * myapp_person_class_intern_init(gpointer klass)
 * { ... }
 *
 * static inline gpointer
 * myapp_person_get_instance_private(MyappPerson* self)
 * {
 *   return (G_STRUCT_MEMBER_P(self, MyappPerson_private_offset));
 * }
 *
 * GType
 * myapp_person_get_type(void)
 * { ... }
 *
 * MyappPerson_private_offset =
 *   g_type_add_instance_private(g_define_type_id, sizeof (MyappPersonPrivate));
 */
G_DEFINE_TYPE_WITH_PRIVATE(MyappPerson, myapp_person, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    myapp_person_get_instance_private(MYAPP_PERSON(instance))

// ----------------------------------------------------------------------------

/* Instance properties
 * ===================
 */

enum {
  PROP_NAME = 1,
  PROP_AGE,
  N_PROPERTIES
};

static GParamSpec* obj_properties[N_PROPERTIES] = { 0 };

void
myapp_person_set_property(GObject* object, guint property_id,
                          const GValue* value, GParamSpec* pspec)
{
  MyappPersonPrivate* priv = GET_PRIVATE(object);

  switch (property_id) {
  case PROP_NAME:
    g_free(priv->name);
    priv->name = g_value_dup_string(value);
    printf("myapp_person_set_property(), name: %s\n", priv->name);
    break;
  case PROP_AGE:
    priv->age = g_value_get_int(value);
    printf("myapp_person_set_property(), age: %d\n", priv->age);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    break;
  }
}

void
myapp_person_get_property(GObject* object, guint property_id,
                          GValue* value, GParamSpec* pspec)
{
  printf("myapp_person_get_property(), property_id: %u\n", property_id);

  MyappPersonPrivate* priv = GET_PRIVATE(object);

  switch (property_id) {
  case PROP_NAME:
    g_value_set_string(value, priv->name);
    break;
  case PROP_AGE:
    g_value_set_int(value, priv->age);
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

void
myapp_person_say(MyappPerson* self, char* words)
{
  printf("myapp_person_say()\n");

  g_return_if_fail(MYAPP_IS_PERSON(self));
  g_return_if_fail(words != NULL);

  MyappPersonClass* klass = MYAPP_PERSON_GET_CLASS(self);
  // g_return_if_fail(klass->say != NULL);
  klass->say(self, words);
}

static void
myapp_person_say_impl(MyappPerson* self, char* words)
{
  printf("myapp_person_say_impl()\n");

  MyappPersonPrivate* priv = GET_PRIVATE(self);

  printf("I am Person '%s', aged '%d', and I say '%s'\n", priv->name,
         priv->age, words);
}

// ----------------------------------------------------------------------------

/* Class destructor
 * ================
 *
 * Unreference all objects in `dispose()`,
 * and free all memory or close file descriptors in `finalize()`.
 */

static void
myapp_person_dispose(GObject* object)
{
  printf("myapp_person_dispose()\n");

  //MyappPersonPrivate* priv = GET_PRIVATE(object);

  // Release all types referenced from this object
  //g_object_unref(priv->some_object); priv->some_object = NULL;
  // Alternative:
  //g_clear_object(&priv->some_object);

  // Chain up
  G_OBJECT_CLASS(myapp_person_parent_class)->dispose(object);
}

static void
myapp_person_finalize(GObject* object)
{
  printf("myapp_person_finalize()\n");

  MyappPersonPrivate* priv = GET_PRIVATE(object);

  // Free all memory allocated by this object
  g_free(priv->name); priv->name = NULL;
  // Alternative:
  //g_clear_pointer(&priv->name, g_free);

  // Chain up
  G_OBJECT_CLASS(myapp_person_parent_class)->finalize(object);
}

// ----------------------------------------------------------------------------

/* Class constructor
 * =================
 */

static void
myapp_person_init(MyappPerson* self)
{
  printf("myapp_person_init()\n");

  MyappPersonPrivate* priv = GET_PRIVATE(self);

  // Initialize all public and private members to reasonable default values.
  // They are all automatically initialized to 0 to begin with.
  priv->name = g_strdup("No Name");
  priv->age = 99;
}

static void
myapp_person_class_init(MyappPersonClass* klass)
{
  printf("myapp_person_class_init()\n");

  // Override virtual methods from GObject
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->set_property = myapp_person_set_property;
  object_class->get_property = myapp_person_get_property;
  object_class->dispose = myapp_person_dispose;
  object_class->finalize = myapp_person_finalize;

  // Set default implementation for own methods
  klass->say = myapp_person_say_impl;

  // Register object properties
  obj_properties[PROP_NAME] =
      g_param_spec_string("name",
                          "Full Name",
                          "The full name of this person",
                          "John Doe",  // Default value
                          G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  obj_properties[PROP_AGE] =
      g_param_spec_int("age",
                       "Age",
                       "Age of this person",
                       0,    // Minimum value
                       100,  // Maximum value
                       20,   // Default value
                       G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  g_object_class_install_properties(object_class,
                                    N_PROPERTIES, obj_properties);
}

// ----------------------------------------------------------------------------
