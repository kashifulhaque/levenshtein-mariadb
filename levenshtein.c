#include <stdint.h>
#include <stdlib.h>

int levenshtein(const char *s1, const char *s2) {
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);
  size_t *cache = calloc((len1 + 1) * (len2 + 1), sizeof(size_t));

  for(size_t i = 1; i <= len1; ++i) {
    cache[i] = i;
  }

  for(size_t i = 1; i <= len2; ++i) {
    cache[i * (len1 + 1)] = i;

    for(size_t j = 1; j <= len1; ++j) {
      size_t cost = (s1[j - 1] != s2[i - 1]);
      size_t deletion = cache[(i - 1) * (len1 + 1) + j] + 1;
      size_t insertion = cache[i * (len1 + 1) + (j - 1)] + 1;
      size_t substitution = cache[(i - 1) * (len1 + 1) + (j - 1)] + cost;
      cache[i * (len1 + 1) + j] = MIN(MIN(deletion, insertion), substitution);
    }
  }

  size_t result = cache[len2 * (len1 + 1) + len1];
  free(cache);

  return result;
}
