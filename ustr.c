#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

/*
Initializes a new UStr with contents
*/
UStr new_ustr(char* contents) {
	int32_t bytes = strlen(contents);
	int32_t codepoints = utf8_strlen(contents);
	uint8_t all_ascii = bytes == codepoints;
	char* contents_copy = malloc(bytes + 1);
	strcpy(contents_copy, contents);
	UStr s = {
		codepoints,
		bytes,
		all_ascii,
		contents_copy
	};
	return s;
}

/*
Returns length of string
*/
int32_t len(UStr s) {
	return s.codepoints;
}

/*
Returns a substring of str, starting at index start (inclusive) 
and ending at index end (exclusive).

Returns an empty string on invalid range.
*/
UStr substring(UStr s, int32_t start, int32_t end) {
	// TODO: implement this
	// Check for invalid range
    if (start < 0 || end > s.codepoints || start >= end) {
        return new_ustr("");
    }

    // Find byte positions for start and end
    int32_t start_byte = 0, end_byte = 0;
    int32_t current_codepoint = 0;
    int32_t i = 0;
    
    // Find start byte position
    while (current_codepoint < start && i < s.bytes) {
        i += utf8_codepoint_size(s.contents[i]);
        current_codepoint++;
    }
    start_byte = i;
    
    // Find end byte position
    while (current_codepoint < end && i < s.bytes) {
        i += utf8_codepoint_size(s.contents[i]);
        current_codepoint++;
    }
    end_byte = i;
    
    // Allocate and copy substring
    int32_t sub_len = end_byte - start_byte;
    char* sub_contents = malloc(sub_len + 1);
    strncpy(sub_contents, s.contents + start_byte, sub_len);
    sub_contents[sub_len] = '\0';
    
    UStr result = new_ustr(sub_contents);
    free(sub_contents); // new_ustr makes its own copy
    return result;
}

/*
Given 2 strings s1 and s2, returns a string that is the result of 
concatenating s1 and s2. 
*/
UStr concat(UStr s1, UStr s2) {
	// TODO: implement this
	int32_t total_bytes = s1.bytes + s2.bytes;
    char* combined = malloc(total_bytes + 1);
    strcpy(combined, s1.contents);
    strcat(combined, s2.contents);
    
    UStr result = new_ustr(combined);
    free(combined); // new_ustr makes its own copy
    return result;
}

/*
Given a string s and an index, return a string with the character at index 
removed from the original string. 

Returns the original string if index is out of bounds.
*/
UStr removeAt(UStr s, int32_t index) {
	// TODO: implement this
	// Check for invalid index
    if (index < 0 || index >= s.codepoints) {
        return s;
    }
    
    // Find byte position of the character to remove
    int32_t byte_pos = 0;
    int32_t current_codepoint = 0;
    while (current_codepoint < index && byte_pos < s.bytes) {
        byte_pos += utf8_codepoint_size(s.contents[byte_pos]);
        current_codepoint++;
    }
    
    int32_t char_len = utf8_codepoint_size(s.contents[byte_pos]);
    
    // Allocate memory for new string (original bytes minus removed character)
    char* new_contents = malloc(s.bytes - char_len + 1);
    
    // Copy part before the character
    strncpy(new_contents, s.contents, byte_pos);
    
    // Copy part after the character
    strcpy(new_contents + byte_pos, s.contents + byte_pos + char_len);
    
    UStr result = new_ustr(new_contents);
    free(new_contents); // new_ustr makes its own copy
    return result;
}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
	// TODO: implement this
	if (s.codepoints == 0) {
        return new_ustr("");
    }
    
    // Create an array to store pointers to each UTF-8 character
    char** chars = malloc(s.codepoints * sizeof(char*));
    int32_t* char_lens = malloc(s.codepoints * sizeof(int32_t));
    
    // Split the string into individual UTF-8 characters
    int32_t pos = 0;
    for (int32_t i = 0; i < s.codepoints; i++) {
        char_lens[i] = utf8_codepoint_size(s.contents[pos]);
        chars[i] = s.contents + pos;
        pos += char_lens[i];
    }
    
    // Allocate memory for reversed string
    char* reversed = malloc(s.bytes + 1);
    int32_t reversed_pos = 0;
    
    // Copy characters in reverse order
    for (int32_t i = s.codepoints - 1; i >= 0; i--) {
        strncpy(reversed + reversed_pos, chars[i], char_lens[i]);
        reversed_pos += char_lens[i];
    }
    reversed[s.bytes] = '\0';
    
    UStr result = new_ustr(reversed);
    free(chars);
    free(char_lens);
    free(reversed);
    return result;
}


void print_ustr(UStr s) {
	printf("%s [codepoints: %d | bytes: %d]", s.contents, s.codepoints, s.bytes);
}

void free_ustr(UStr s) {
	if (s.contents != NULL) {
		free(s.contents);
		s.contents = NULL;
	}
}
