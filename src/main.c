// Copyright 2024 Roy T. Hashimoto. All Rights Reserved.
#include <emscripten.h>
#include <sqlite3.h>

// sqlite-vec extension
int sqlite3_vec_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// Some SQLite API functions take a pointer to a function that frees
// memory. Although we could add a C binding to a JavaScript function
// that calls sqlite3_free(), it is more efficient to pass the sqlite3_free
// function pointer directly. This function provides the C pointer to
// JavaScript.
void* EMSCRIPTEN_KEEPALIVE getSqliteFree() {
  return sqlite3_free;
}

// Function to initialize sqlite-vec extension on a database connection
int EMSCRIPTEN_KEEPALIVE sqlite3_vec_init_wrapper(sqlite3 *db) {
  char *errMsg = NULL;
  int rc = sqlite3_vec_init(db, &errMsg, NULL);
  if (errMsg) {
    sqlite3_free(errMsg);
  }
  return rc;
}

int main() {
  sqlite3_initialize();
  return 0;
}