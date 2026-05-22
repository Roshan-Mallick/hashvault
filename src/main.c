#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "auth.h"

/* Read input safely */
static void read_line(const char *prompt, char *buf, size_t size)
{
    printf("%s", prompt);
    fflush(stdout);

    if (!fgets(buf, (int)size, stdin)) {
        buf[0] = '\0';
        return;
    }

    /* Remove newline */
    buf[strcspn(buf, "\r\n")] = '\0';
}

/* Hash a string */
static void mode_hash_explorer(void)
{
    char input[512];
    char hex[HASH_HEX_LEN];

    printf("\n╔══════════════════════════╗\n");
    printf("║   Hash Explorer Mode     ║\n");
    printf("╚══════════════════════════╝\n");

    read_line("Enter string to hash: ", input, sizeof(input));

    /* Show ASCII values */
    printf("\n[ASCII Breakdown]\n");

    for (size_t i = 0; input[i] != '\0'; i++) {
        printf("  '%c' = %3d (0x%02x)\n",
               input[i],
               (unsigned char)input[i],
               (unsigned char)input[i]);
    }

    /* Show hash changes */
    HashState hs;
    hash_init(&hs);

    printf("\n[Hash State Evolution - h1 only]\n");
    printf("  INIT: h1 = %llu\n",
           (unsigned long long)hs.h1);

    for (size_t i = 0; input[i] != '\0'; i++) {
        hash_update(&hs, (unsigned char)input[i]);

        printf("  '%c': h1 = %llu\n",
               input[i],
               (unsigned long long)hs.h1);
    }

    hash_finalize(&hs, hex);

    printf("\n[Final Hash]\n");
    printf("  Input  : \"%s\"\n", input);
    printf("  Hash   : %s\n", hex);
    printf("  Length : %zu bits\n\n", strlen(hex) * 4);
}

/* Compare two hashes */
static void mode_avalanche(void)
{
    char a[512], b[512];
    char ha[HASH_HEX_LEN], hb[HASH_HEX_LEN];

    printf("\n╔══════════════════════════╗\n");
    printf("║   Avalanche Effect Test  ║\n");
    printf("╚══════════════════════════╝\n");

    read_line("Enter string 1: ", a, sizeof(a));
    read_line("Enter string 2: ", b, sizeof(b));

    hash_string(a, ha);
    hash_string(b, hb);

    printf("\n  String 1: \"%s\"\n", a);
    printf("  Hash    : %s\n\n", ha);

    printf("  String 2: \"%s\"\n", b);
    printf("  Hash    : %s\n\n", hb);

    /* Count different characters */
    int diff = 0;

    for (int i = 0; i < (int)strlen(ha); i++) {
        if (ha[i] != hb[i]) {
            diff++;
        }
    }

    printf("  Different positions: %d / %d (%.1f%%)\n\n",
           diff,
           (int)strlen(ha),
           (double)diff / strlen(ha) * 100.0);
}

/* Register new user */
static void mode_register(void)
{
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char hex[HASH_HEX_LEN];

    printf("\n╔══════════════════════════╗\n");
    printf("║       Register User      ║\n");
    printf("╚══════════════════════════╝\n");

    read_line("Username: ", username, sizeof(username));
    read_line("Password: ", password, sizeof(password));

    /* Show stored hash */
    hash_string(password, hex);

    printf("\n  Stored password hash:\n");
    printf("  %s\n\n", hex);

    int result = auth_register(username, password);

    switch (result) {
        case 0:
            printf("  ✓ User '%s' registered!\n\n", username);
            break;

        case -1:
            printf("  ✗ Username already exists.\n\n");
            break;

        case -2:
            printf("  ✗ File error.\n\n");
            break;
    }
}

/* Login user */
static void mode_login(void)
{
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];

    printf("\n╔══════════════════════════╗\n");
    printf("║         Login            ║\n");
    printf("╚══════════════════════════╝\n");

    read_line("Username: ", username, sizeof(username));
    read_line("Password: ", password, sizeof(password));

    int result = auth_login(username, password);

    switch (result) {
        case 1:
            printf("\n  ✓ Login successful! Welcome, %s.\n\n",
                   username);
            break;

        case 0:
            printf("\n  ✗ Wrong password.\n\n");
            break;

        case -1:
            printf("\n  ✗ User not found.\n\n");
            break;

        case -2:
            printf("\n  ✗ File error.\n\n");
            break;
    }
}

/* Main menu */
int main(void)
{
    printf("\n");

    printf("  ██╗  ██╗ █████╗ ███████╗██╗  ██╗██╗   ██╗ █████╗ ██╗   ██╗██╗  ████████╗\n");
    printf("  ██║  ██║██╔══██╗██╔════╝██║  ██║██║   ██║██╔══██╗██║   ██║██║  ╚══██╔══╝\n");
    printf("  ███████║███████║███████╗███████║╚██╗ ██╔╝███████║██║   ██║██║     ██║\n");
    printf("  ██╔══██║██╔══██║╚════██║██╔══██║ ╚████╔╝ ██╔══██║██║   ██║██║     ██║\n");
    printf("  ██║  ██║██║  ██║███████║██║  ██║  ╚██╔╝  ██║  ██║╚██████╔╝███████╗██║\n");
    printf("  ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝\n");

    printf("\n  Beginner-friendly hashing & auth in C\n\n");

    char choice[8];
    int running = 1;

    while (running) {

        printf("┌─────────────────────────┐\n");
        printf("│  1. Hash a string       │\n");
        printf("│  2. Avalanche tester    │\n");
        printf("│  3. Register user       │\n");
        printf("│  4. Login               │\n");
        printf("│  5. Exit                │\n");
        printf("└─────────────────────────┘\n");

        read_line("Choice: ", choice, sizeof(choice));

        switch (choice[0]) {

            case '1':
                mode_hash_explorer();
                break;

            case '2':
                mode_avalanche();
                break;

            case '3':
                mode_register();
                break;

            case '4':
                mode_login();
                break;

            case '5':
                running = 0;
                break;

            default:
                printf("  Invalid choice.\n\n");
                break;
        }
    }

    printf("  Goodbye.\n\n");

    return 0;
}
