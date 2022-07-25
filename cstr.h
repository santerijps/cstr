#ifndef CSTR_H

  #define CSTR_H

  #ifndef _INC_STDLIB
    #include <stdlib.h>
  #endif

  #ifndef _INC_STRING
    #include <string.h>
  #endif

  #ifndef __ASSERT_H_
    #include <assert.h>
  #endif

  struct String {
    char *value;
    size_t length;
  };

  struct StringVector {
    struct String *values;
    size_t length;
    size_t capacity;
  };

  /**
   * Create a new empty string struct.
   * @return A new struct String with a null terminator value and length 0.
  */
  struct String NewEmptyString()
  {
    struct String s;
    s.value = "\0";
    s.length = 0;
    return s;
  }

  /**
   * Create a new string struct with an initial value.
   * @param initial_value The initial value of the string.
   * @return A new struct String with an initial value and length strlen(initial_value).
  */
  struct String NewString(char *initial_value)
  {
    struct String s;
    s.value = initial_value;
    s.length = initial_value == NULL ? 0 : strlen(initial_value);
    return s;
  }

  /**
   * Concatenate two struct Strings. Does not mutate the parameter values.
   * @param s1 The first part of the string.
   * @param s2 The second part of the string.
   * @return A new struct String with the string values concatenated.
  */
  struct String ConcatString(struct String s1, struct String s2)
  {
    struct String s3;
    s3.length = s1.length + s2.length;
    s3.value = (char*) malloc(sizeof(char) * (s3.length + 1));
    memset(s3.value, 0, s3.length + 1);
    strcat(s3.value, s1.value);
    strcat(s3.value, s2.value);
    return s3;
  }

  /**
   * Concatenate a struct String with a const char *.
   * @param s1 The first part of the string of type struct String.
   * @param s2 The second part of the string of type const char*.
   * @return A new struct String with the string values concatenated.
  */
  struct String ConcatLiteral(struct String s1, const char *s2)
  {
    struct String s3;
    s3.length = s1.length + strlen(s2);
    s3.value = (char*) malloc(sizeof(char) * (s3.length + 1));
    memset(s3.value, 0, s3.length + 1);
    strcat(s3.value, s1.value);
    strcat(s3.value, s2);
    return s3;
  }

  /**
   * Extract a substring between two indeces.
   * @param s The string to extract the substring from.
   * @param start The starting index (inclusive)
   * @param stop The last index (exclusive)
   * @return A new struct String.
  */
  struct String SubString(struct String s, int start, int stop)
  {
    if (start >= 0 && (size_t) start < s.length)
    {
      stop = stop <= (int) s.length ? stop : (int) s.length;
      size_t length = stop - start, i;
      struct String sub;
      sub.length = length;
      sub.value = (char*) malloc(sizeof(char) * (length + 1));
      memset(sub.value, 0, length + 1);
      for (i = 0; i < length; i++)
      {
        sub.value[i] = s.value[i + start];
      }
      return sub;
    }
    else
    {
      return NewEmptyString();
    }
  }

  /**
   * Check if s1 equals to s2.
   * @param s1 The first string.
   * @param s2 The second string.
   * @return 1 if equal, 0 otherwise.
  */
  int EqualsString(struct String s1, struct String s2)
  {
    if (s1.length != s2.length)
    {
      return 0;
    }
    for (size_t i = 0; i < s1.length; i++)
    {
      if (s1.value[i] != s2.value[i])
      {
        return 0;
      }
    }
    return 1;
  }

  /**
   * Check if haystack contains the needle.
   * @param haystack The string to search in.
   * @param needle The string to search for.
   * @return 1 if found, 0 otherwise.
  */
  int ContainsString(struct String haystack, struct String needle)
  {
    if (haystack.length < needle.length)
    {
      return 0;
    }
    for (size_t i = 0; i < haystack.length; i++)
    {
      if (haystack.length - i >= needle.length)
      {
        int found = 1;
        for (size_t j = 0; j < needle.length; j++)
        {
          if (haystack.value[i + j] != needle.value[j])
          {
            found = 0;
            break;
          }
        }
        if (found)
        {
          return 1;
        }
      }
    }
    return 0;
  }

  struct StringVector NewStringVector()
  {
    struct StringVector sv;
    sv.length = 0;
    sv.capacity = 0;
    sv.values = NULL;
    return sv;
  }

  void AppendString(struct StringVector *sv, struct String s)
  {
    if (sv->values == NULL)
    {
      sv->capacity = 128;
      sv->values = (struct String*) malloc(sizeof(struct String) * sv->capacity);
    }
    if (sv->length == sv->capacity)
    {
      sv->capacity *= 2;
      sv->values = realloc(sv->values, sizeof(struct String) * sv->capacity);
    }
    sv->values[sv->length++] = s;
  }

  void AppendLiteral(struct StringVector *sv, char *s)
  {
    if (sv->values == NULL)
    {
      sv->capacity = 128;
      sv->values = (struct String*) malloc(sizeof(struct String) * sv->capacity);
    }
    if (sv->length == sv->capacity)
    {
      sv->capacity *= 2;
      sv->values = realloc(sv->values, sizeof(struct String) * sv->capacity);
    }
    sv->values[sv->length++] = NewString(s);
  }

  inline struct String PopString(struct StringVector *sv)
  {
    return sv->values[--(sv->length)];
  }

  inline char* PopLiteral(struct StringVector *sv)
  {
    return sv->values[--(sv->length)].value;
  }

  inline struct String GetString(struct StringVector *sv, size_t index)
  {
    assert(index < sv->length);
    return sv->values[index];
  }

  inline char* GetValue(struct StringVector *sv, size_t index)
  {
    assert(index < sv->length);
    return sv->values[index].value;
  }

  inline size_t GetLength(struct StringVector *sv, size_t index)
  {
    assert(index < sv->length);
    return sv->values[index].length;
  }

  struct StringVector SplitString(struct String s, struct String sep)
  {
    struct StringVector sv = NewStringVector();
    size_t i, j, start = 0;
    if (sep.length < s.length)
    {
      for (i = 0; i < s.length; i++)
      {
        if (s.length - i >= sep.length)
        {
          int found = 1;
          for (j = 0; j < sep.length; j++)
          {
            if (s.value[i + j] != sep.value[j])
            {
              found = 0;
              break;
            }
          }
          if (found)
          {
            AppendString(&sv, SubString(s, start, i));
            i += sep.length - 1;
            start = i + 1;
          }
        }
      }
      if (start < i && i <= s.length)
      {
        AppendString(&sv, SubString(s, start, i));
      }
    }
    return sv;
  }

  struct StringVector SplitLiteral(struct String s, char *sep)
  {
    return SplitString(s, NewString(sep));
  }

  struct String CopyString(struct String s)
  {
    struct String copy;
    copy.length = s.length;
    copy.value = (char*) malloc(sizeof(char) * (s.length + 1));
    memset(copy.value, 0, copy.length + 1);
    strcpy(copy.value, s.value);
    return copy;
  }

  struct String UpperString(struct String s)
  {
    struct String upper = CopyString(s);
    for (size_t i = 0; i< upper.length; i++)
    {
      if (upper.value[i] >= 97 && upper.value[i] <= 122)
      {
        upper.value[i] -= 32;
      }
    }
    return upper;
  }

  struct String LowerString(struct String s)
  {
    struct String lower = CopyString(s);
    for (size_t i = 0; i< lower.length; i++)
    {
      if (lower.value[i] >= 65 && lower.value[i] <= 90)
      {
        lower.value[i] += 32;
      }
    }
    return lower;
  }

  static inline int CharIsPunctuation(char c)
  {
    return c == ' ' || c == '.' || c == '!' || c == '?';
  }

  struct String TitleString(struct String s)
  {
    struct String title = CopyString(s);
    int should_capitalize = 1;
    for (size_t i = 0; i < title.length; i++)
    {
      if (should_capitalize)
      {
        if (title.value[i] >= 97 && title.value[i] <= 122)
        {
          title.value[i] -= 32;
          should_capitalize = 0;
        }
      }
      else
      {
        if (CharIsPunctuation(title.value[i]))
        {
          should_capitalize = 1;
        }
      }
    }
    return title;
  }

  struct String JoinLiteral(struct StringVector *sv, const char *delim)
  {
    struct String s;
    s.length = (sv->length - 1) * strlen(delim) + 1;
    for (size_t i = 0; i < sv->length; i++)
    {
      s.length += sv->values[i].length;
    }
    s.value = (char*) malloc(sizeof(char) * s.length);
    memset(s.value, 0, s.length);
    for (size_t i = 0; i < sv->length - 1; i++)
    {
      strcat(s.value, sv->values[i].value);
      strcat(s.value, delim);
    }
    strcat(s.value, sv->values[sv->length - 1].value);
    return s;
  }

  struct String JoinString(struct StringVector *sv, struct String s)
  {
    return JoinLiteral(sv, s.value);
  }

  struct String ReplaceString(struct String s, struct String search, struct String replace)
  {
    if (search.length > s.length)
    {
      return s;
    }
    struct StringVector split = SplitString(s, search);
    struct String join = JoinString(&split, replace);
    return join;
  }

  struct String ReplaceLiteral(struct String s, char *search, char *replace)
  {
    if (strlen(search) > s.length)
    {
      return s;
    }
    struct StringVector split = SplitString(s, NewString(search));
    struct String join = JoinString(&split, NewString(replace));
    return join;
  }

  struct StringVector Slice(struct StringVector *sv, int start, int stop)
  {
    struct StringVector slice = NewStringVector();
    if (start >= 0 && (size_t) start < sv->length)
    {
      for (size_t i = start; i < (size_t) stop && i < sv->length; i++)
      {
        AppendString(&slice, sv->values[i]);
      }
    }
    return slice;
  }

  struct String ReverseString(struct String s)
  {
    struct String reversed = CopyString(s);
    for (size_t i = 0; i < s.length; i++)
    {
      reversed.value[s.length - 1 - i] = s.value[i];
    }
    return reversed;
  }

  int FirstIndexOfString(struct String s, struct String search)
  {
    int index = -1;
    if (search.length < s.length) for (size_t i = 0; i < s.length; i++)
    {
      int found = 1;
      for (size_t j = 0; j < search.length; j++)
      {
        if (s.value[i + j] != search.value[j])
        {
          found = 0;
          break;
        }
      }
      if (found)
      {
        index = i;
        break;
      }
    }
    return index;
  }

  int LastIndexOfString(struct String s, struct String search)
  {
    int index = -1;
    if (search.length < s.length) for (size_t i = 0; i < s.length; i++)
    {
      int found = 1;
      for (size_t j = 0; j < search.length; j++)
      {
        if (s.value[i + j] != search.value[j])
        {
          found = 0;
          break;
        }
      }
      if (found)
      {
        index = i;
      }
    }
    return index;
  }

  int StartsWithString(struct String s, struct String search)
  {
    if (search.length > s.length)
    {
      return 0;
    }
    for (size_t i = 0; i < search.length; i++)
    {
      if (s.value[i] != search.value[i])
      {
        return 0;
      }
    }
    return 1;
  }

  int EndsWithString(struct String s, struct String search)
  {
    if (search.length > s.length)
    {
      return 0;
    }
    for (size_t i = 0; i < search.length; i++)
    {
      if (s.value[s.length - search.length + i] != search.value[i])
      {
        return 0;
      }
    }
    return 1;
  }

#endif