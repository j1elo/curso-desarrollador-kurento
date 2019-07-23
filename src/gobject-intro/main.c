#include "myapp-person.h"
#include "myapp-student.h"
#include "myapp-teacher.h"

// ----------------------------------------------------------------------------

int main(void)
{
    // MyappPerson* person1 = g_object_new(MYAPP_TYPE_PERSON,
    //                                     "name", "Pepe", "age", 33, NULL);
    MyappPerson* person1 = g_object_new(MYAPP_TYPE_PERSON, NULL);
    g_object_set (person1, "name", "Pepe", "age", 33, NULL);

    myapp_person_say(person1, "Hello, World!");

    g_object_set (person1, "name", "Pepito Perez", NULL);

    char* name;
    g_object_get (person1, "name", &name, NULL);
    g_print ("El nombre nuevo es: %s\n", name);
    g_free (name);

    g_object_unref(person1);

    // ------------

    MyappPerson* person2 = g_object_new(MYAPP_TYPE_PERSON,
                                        "name", "Luis", NULL);
    myapp_person_say(person2, "How are you?");
    g_object_unref(person2);

    // ------------

    MyappPerson* person3 = g_object_new(MYAPP_TYPE_STUDENT,
                                        "name", "Rolo", NULL);
    myapp_person_say(person3, "Let's go karting");
    g_object_unref(person3);

    // ------------

    MyappPerson* teacher = g_object_new(MYAPP_TYPE_TEACHER, NULL);
    myapp_person_say(teacher, "Da igual porque se ignora");
    g_object_unref(teacher);

    // ------------

    return 0;
}

// ----------------------------------------------------------------------------
