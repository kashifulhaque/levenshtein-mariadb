gcc -shared -o levenshtein.so -fPIC $(mysql_config --cflags) levenshtein.c $(mysql_config --libs)