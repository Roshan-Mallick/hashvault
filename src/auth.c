#include "auth.h"
#include "hash.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Compare strings safely */
static int safe_strcmp(const char *a, const char *b)
{
    int diff = 0;
    size_t i;

    size_t la = strlen(a);
    size_t lb = strlen(b);

    /* Different lengths = not equal */
    if (la != lb) return 1;

    /* Compare every character */
    for (i = 0; i < la; i++) {
        diff |= (unsigned char)a[i] ^ (unsigned char)b[i];
    }

    return diff;
}

/* Detect whitespace characters */
static int contains_whitespace(const char *str)
{
    while (*str) {
        if (isspace((unsigned char)*str)) {
            return 1;
        }
        str++;
    }

    return 0;
}

/* Check if username already exists */
int auth_user_exists(const char *username)
{
    FILE *fp = fopen(USERS_FILE, "r");

    /* No file means no users yet */
    if (!fp) return 0;

    char line[MAX_USERNAME + HASH_HEX_LEN + 2];
    char stored_user[MAX_USERNAME];

    while (fgets(line, sizeof(line), fp)) {

        /* Find ':' separator */
        char *colon = strchr(line, ':');
        if (!colon) continue;

        size_t ulen = (size_t)(colon - line);

        if (ulen >= MAX_USERNAME) continue;

        /* Copy username part */
        strncpy(stored_user, line, ulen);
        stored_user[ulen] = '\0';

        /* Match username */
        if (strcmp(stored_user, username) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* Register new user */
int auth_register(const char *username, const char *password)
{
    /* Reject whitespace */
    if (contains_whitespace(username) ||
        contains_whitespace(password)) {
        return -3;
    }

    /* Prevent duplicate usernames */
    if (auth_user_exists(username)) {
        return -1;
    }

    /* Hash password */
    char hash_hex[HASH_HEX_LEN];
    hash_string(password, hash_hex);

    /* Open file in append mode */
    FILE *fp = fopen(USERS_FILE, "a");
    if (!fp) return -2;

    /* Save username and hash */
    fprintf(fp, "%s:%s\n", username, hash_hex);

    fclose(fp);

    return 0;
}

/* Login user */
int auth_login(const char *username, const char *password)
{
    /* Reject whitespace */
    if (contains_whitespace(username) ||
        contains_whitespace(password)) {
        return -3;
    }

    /* Hash entered password */
    char input_hash[HASH_HEX_LEN];
    hash_string(password, input_hash);

    /* Open users file */
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) return -2;

    char line[MAX_USERNAME + HASH_HEX_LEN + 4];
    char stored_user[MAX_USERNAME];
    char stored_hash[HASH_HEX_LEN];

    int found = 0;

    while (fgets(line, sizeof(line), fp)) {

        /* Remove newline */
        line[strcspn(line, "\r\n")] = '\0';

        /* Find ':' separator */
        char *colon = strchr(line, ':');
        if (!colon) continue;

        size_t ulen = (size_t)(colon - line);

        if (ulen >= MAX_USERNAME) continue;

        /* Extract username */
        strncpy(stored_user, line, ulen);
        stored_user[ulen] = '\0';

        /* Skip if username doesn't match */
        if (strcmp(stored_user, username) != 0) continue;

        /* Copy stored hash */
        strncpy(stored_hash, colon + 1, HASH_HEX_LEN - 1);
        stored_hash[HASH_HEX_LEN - 1] = '\0';

        found = 1;
        break;
    }

    fclose(fp);

    /* User not found */
    if (!found) return -1;

    /* Compare password hashes */
    return (safe_strcmp(input_hash, stored_hash) == 0) ? 1 : 0;
}
