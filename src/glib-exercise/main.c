// <#file>

#include <glib.h>

#include <stdio.h>
#include <string.h>

// ----------------------------------------------------------------------------

/* GLib Exercise.
 *
 * Given a random string, count occurrences of each alphanumeric character
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
    char* secrets[3] = {
        "aaa_bbbb_ccc_dddd",
        "1111_222_333_4444",
        "www_xxx_yy_zzzzzz",
    };

    exercise(secrets[g_random_int() % 3]);

    return 0;
}

// ----------------------------------------------------------------------------
