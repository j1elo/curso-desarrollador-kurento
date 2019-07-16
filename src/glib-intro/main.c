// <#file>

#include <glib.h>

#include <stdio.h>
#include <string.h>

// ----------------------------------------------------------------------------

/* GList
 * =====
 *
 * Use GList to store some static strings and iterate over it to print them.
 *
 * See:
 * - https://developer.gnome.org/glib/stable/glib-Doubly-Linked-Lists.html
 */

void sum_gpointer(gpointer data, gpointer user_data)
{
    int data_i = GPOINTER_TO_INT(data);
    int* total = user_data;

    *total += data_i;
}

void sum_int(int data, int* total)
{
    *total += data;
}

int compare_int(int a, int b)
{
    if (a < b) return -1;
    else if (a > b) return 1;
    else return 0;
}

void print_int(int data)
{
    printf("--> %d\n", data);
}

void test_list(void)
{
    GList* list = NULL;

    // Print all elements
    {
        list = g_list_append(list, "He");
        list = g_list_append(list, "llo");
        list = g_list_append(list, " wor");
        list = g_list_append(list, "ld!");

        g_list_foreach(list, (GFunc)printf, NULL);
        printf("\n");

        g_list_free(list); list = NULL;
    }

    // Pass state as 'user_data'
    {
        int total = 0;

        list = g_list_append(list, GINT_TO_POINTER(1));
        list = g_list_append(list, GINT_TO_POINTER(2));
        list = g_list_append(list, GINT_TO_POINTER(3));
        list = g_list_append(list, GINT_TO_POINTER(4));

        // Pass data and state as gpointers
        g_list_foreach(list, sum_gpointer, &total);
        printf("total: %d\n", total);

        // Pass data and state as ints
        g_list_foreach(list, (GFunc)sum_int, &total);
        printf("total: %d\n", total);

        g_list_free(list); list = NULL;
    }

    // Sort a list
    {
        list = g_list_append(list, GINT_TO_POINTER(19));
        list = g_list_append(list, GINT_TO_POINTER(92));
        list = g_list_append(list, GINT_TO_POINTER(29));
        list = g_list_append(list, GINT_TO_POINTER(-63));

        printf("Unordered list:\n");
        g_list_foreach(list, (GFunc)print_int, NULL);

        list = g_list_sort(list, (GCompareFunc)compare_int);

        printf("Ordered list:\n");
        g_list_foreach(list, (GFunc)print_int, NULL);

        g_list_free(list); list = NULL;
    }
}

// ----------------------------------------------------------------------------

/* GList with g_list_free_full()
 * =============================
 *
 * Use GList to store some dynamically-allocated strings, and show how they
 * must be free'd explicitly when freeing the list itself.
 *
 * See:
 * - https://developer.gnome.org/glib/stable/glib-String-Utility-Functions.html#g-strdup
 * - https://developer.gnome.org/glib/stable/glib-Doubly-Linked-Lists.html#g-list-free-full
 */

void test_list_malloc(void)
{
    GList* list = NULL;

    list = g_list_append(list, g_strdup("AAA"));  // 4 Bytes
    list = g_list_append(list, g_strdup("BBB"));  // 4 Bytes
    list = g_list_append(list, g_strdup("CCC"));  // 4 Bytes

    g_list_foreach(list, (GFunc)printf, NULL);
    printf("\n");

    //g_list_free(list); list = NULL;  // Leaks 12 Bytes
    g_list_free_full(list, g_free); list = NULL;
}

// ----------------------------------------------------------------------------

/* GHashTable
 * ==========
 *
 * Store some static string into a hash table, and show how lookups work.
 *
 * See:
 * - https://developer.gnome.org/glib/stable/glib-Hash-Tables.html
 */

void test_hash(void)
{
    GHashTable* cities = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(cities, "France", "Paris");
    g_hash_table_insert(cities, "Germany", "Berlin");
    g_hash_table_insert(cities, "Italy", "Rome");
    g_hash_table_insert(cities, "Portugal", "Lisbon");
    g_hash_table_insert(cities, "Spain", "Madrid");

    char* countries[3] = {"Belgium", "Spain", "Poland"};

    for (int i = 0; i < 3; i++) {
        char* country = countries[i];
        gpointer key = country;

        if (g_hash_table_contains(cities, key)) {
            gpointer value = g_hash_table_lookup(cities, key);
            char* city = value;

            printf("Capital of '%s' is: '%s'\n", country, city);
        }
        else {
            printf("Unknown country: '%s'\n", country);
        }
    }

    g_hash_table_destroy(cities);
}

// ----------------------------------------------------------------------------

int main(void)
{
    printf("---- GList ----\n");
    test_list();
    printf("\n");

    printf("---- GList with g_list_free_full() ----\n");
    test_list_malloc();
    printf("\n");

    printf("---- GHashTable ----\n");
    test_hash();

    return 0;
}

// ----------------------------------------------------------------------------
