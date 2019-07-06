// <#file>

#include <cjson/cJSON.h>
#include <stdio.h>
// <#mode standard|cpp>
int main(void)
{
    char* json_str = "{ \"name\": \"My Name\", \"Number\": 112233 }";

    cJSON* json = cJSON_Parse(json_str);
    cJSON* name = cJSON_GetObjectItemCaseSensitive(json, "name");

    printf("JSON name: '%s'\n", name->valuestring);

    return 0;
}
