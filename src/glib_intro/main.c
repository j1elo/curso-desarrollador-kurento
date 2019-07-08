// <#file>

#include <glib.h>
#include <stdio.h>

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

int main(void)
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

        // Pass data and state as gpointers
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

    return 0;
}
