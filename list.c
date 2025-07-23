#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "list.h"

/*
Returns an empty list of initial_capacity
*/
List new_list(int32_t initial_capacity) {
    List list;
    list.size = 0;
    list.capacity = initial_capacity;
    list.data = malloc(initial_capacity * sizeof(struct UStr));
    return list;
}

/*
Initializes an list of length size with elements from array
*/
List new_list_from_array(struct UStr* array, int32_t size) {
    List list = new_list(size);
    for (int32_t i = 0; i < size; i++) {
        list.data[i] = array[i];
    }
    list.size = size;
    return list;
}


/*
Given a list of strings and a separator string, returns a single string 
containing all the strings in list joined by the separator.
*/
UStr join(List* list, UStr separator) {
    if (list->size == 0) {
        return new_ustr("");
    }
    int32_t n = list->size;
    size_t total_bytes = 0;
    for (int32_t i = 0; i < n; i++) {
        total_bytes += strlen(list->data[i].contents);
        if (i + 1 < n) {
            total_bytes += strlen(separator.contents);
        }
    }
    char* buf = malloc(total_bytes + 1);
    char* p = buf;
    for (int32_t i = 0; i < n; i++) {
        size_t len = strlen(list->data[i].contents);
        memcpy(p, list->data[i].contents, len);
        p += len;
        if (i + 1 < n) {
            size_t slen = strlen(separator.contents);
            memcpy(p, separator.contents, slen);
            p += slen;
        }
    }
    *p = '\0';

    UStr result = new_ustr(buf);
    free(buf);
    return result;
}

/*
Inserts string s into list at index s, shifting elements to the right.
Expands the list's capacity if necessary (double the capacity, or set to 1 if 0).

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t insert(List* list, UStr s, int32_t index) {
    if (index < 0 || index > list->size) {
        return 0;
    }
    if (list->size == list->capacity) {
        int32_t newCap = list->capacity == 0 ? 1 : list->capacity * 2;
        UStr* newData = malloc(newCap * sizeof(UStr));
        for (int32_t i = 0; i < list->size; i++) {
            newData[i] = list->data[i];
        }
        free(list->data);
        list->data = newData;
        list->capacity = newCap;
    }
    for (int32_t i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = s;
    list->size++;
    return 1;
}

/*
Removes the element at the given index and shifts all subsequent elements left.

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t listRemoveAt(List* list, int32_t index) {
    if (index < 0 || index >= list->size) {
        return 0;
    }
    free_ustr(list->data[index]);
    for (int32_t i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
    return 1;

}

/*
Splits the given string s into substrings separated by the given delimiter string.

Returns a List of String objects, each containing a segment between delimiters.

If the delimiter is the empty string, return a list containing the original 
string as a single element.

If the input string ends with the delimiter, include an empty string at the 
end of the result.

Note that the delimiter could be of a length of more than 1 character
*/
List split(UStr s, UStr separator) {
    List result = new_list(1);
    char* str = s.contents;
    char* sep = separator.contents;
    size_t sepLen = strlen(sep);
    if (sepLen == 0) {
        insert(&result, new_ustr(str), result.size);
        return result;
    }
    char* start = str;
    char* match;
    while ((match = strstr(start, sep)) != NULL) {
        size_t len = match - start;
        char* buf = malloc(len + 1);
        memcpy(buf, start, len);
        buf[len] = '\0';
        insert(&result, new_ustr(buf), result.size);
        free(buf);
        start = match + sepLen;
    }
    size_t rem = strlen(start);
    char* buf = malloc(rem + 1);
    memcpy(buf, start, rem);
    buf[rem] = '\0';
    insert(&result, new_ustr(buf), result.size);
    free(buf);
    return result;

}
