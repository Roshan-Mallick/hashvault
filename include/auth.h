#ifndef AUTH_H
#define AUTH_H

#include "hash.h"

/* Limits */
#define MAX_USERNAME  64
#define MAX_PASSWORD  128

/* User database file */
#define USERS_FILE "data/users.txt"

/* Register new user */
int auth_register(const char *username,
                  const char *password);

/* Login existing user */
int auth_login(const char *username,
               const char *password);

/* Check if user exists */
int auth_user_exists(const char *username);

#endif /* AUTH_H */
