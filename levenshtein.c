#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

bool levenshtein_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
long long levenshtein(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

#define min(a, b) ((a) < (b) ? (a) : (b))

bool levenshtein_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  if (args->arg_count != 2)
  {
    strcpy(message, "LEVENSHTEIN() requires two arguments");
    return 1;
  }
  args->arg_type[0] = STRING_RESULT;
  args->arg_type[1] = STRING_RESULT;
  return 0;
}

long long levenshtein(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
  const char *s1 = args->args[0];
  const char *s2 = args->args[1];
  size_t len1 = args->lengths[0];
  size_t len2 = args->lengths[1];

  size_t *cache = (size_t *)calloc((len1 + 1) * (len2 + 1), sizeof(size_t));

  for (size_t i = 0; i <= len1; ++i)
    cache[i] = i;

  for (size_t i = 1; i <= len2; ++i)
  {
    cache[i * (len1 + 1)] = i;
    for (size_t j = 1; j <= len1; ++j)
    {
      size_t cost = (s1[j - 1] != s2[i - 1]);
      size_t deletion = cache[(i - 1) * (len1 + 1) + j] + 1;
      size_t insertion = cache[i * (len1 + 1) + (j - 1)] + 1;
      size_t substitution = cache[(i - 1) * (len1 + 1) + (j - 1)] + cost;
      cache[i * (len1 + 1) + j] = min(min(deletion, insertion), substitution);
    }
  }

  long long result = cache[len2 * (len1 + 1) + len1];
  free(cache);
  return result;
}
