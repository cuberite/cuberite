#ifndef SQPLUSUTF8_H
#define SQPLUSUTF8_H

#if defined(SQUTF8) || SQPLUS_USE_LATIN1!=1 

// Simple Unicode <-> UTF8 conversion routines
//int sqplus_utf8_len(char lead_byte);
int sqplus_utf8_len_first(const char *str);
int sqplus_utf8_strlen(const char *str);
int sqplus_utf8_to_wchar(int *result, const char *string);
int sqplus_wchar_to_utf8(char *s, int wc, int size);

#endif // defined(SQUTF8) || SQPLUS_USE_LATIN1!=1 

#endif // SQPLUSUTF8_H
