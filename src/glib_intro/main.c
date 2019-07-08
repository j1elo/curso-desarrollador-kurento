// <#file>

#include <glib.h>

#include <stdio.h>
#include <string.h>



// GList ----------------------------------------------------------------------

// https://developer.gnome.org/glib/stable/glib-Doubly-Linked-Lists.html

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



// GHashTable -----------------------------------------------------------------

// https://developer.gnome.org/glib/stable/glib-Hash-Tables.html

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



// Exercise -------------------------------------------------------------------

/* Given a random string, count occurrences of each alphanumeric character
 * (ie. only letters and numbers).
 */

void print_letter(char letter, int count)
{
    printf("--> '%c': %d\n", letter, count);
}

void exercise(char* secret)
{
    GHashTable* letters = g_hash_table_new(NULL, NULL);

    for (size_t i = 0; i < strlen(secret); i++) {
        char letter = secret[i];
        gpointer key = GINT_TO_POINTER(letter);

        if (!g_ascii_isalnum(letter)) {
            continue;
        }

        if (g_hash_table_contains(letters, key)) {
            gpointer value = g_hash_table_lookup(letters, key);
            int count = GPOINTER_TO_INT(value);
            count += 1;
            value = GINT_TO_POINTER(count);
            g_hash_table_replace(letters, key, value);
        }
        else {
            gpointer value = GINT_TO_POINTER(1);
            g_hash_table_insert(letters, key, value);
        }
    }

    printf("Char count:\n");
    g_hash_table_foreach(letters, (GHFunc)print_letter, NULL);

    g_hash_table_destroy(letters);
}



// ----------------------------------------------------------------------------

int main(void)
{
    printf("---- GList ----\n");

    test_list();

    printf("\n\n");

    // ----

    printf("---- GHashTable ----\n");

    test_hash();

    printf("\n\n");

    // ----

    printf("---- Exercise ----\n");

    char* secrets[3] = {
        "aaa_bbbb_ccc_dddd",
        "1111_222_333_4444",
        "www_xxx_yy_zzzzzz",
    };
    exercise(secrets[g_random_int() % 3]);

    printf("\n\n");

    return 0;
}
