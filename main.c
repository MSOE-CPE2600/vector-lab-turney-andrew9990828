/******************************************************************** 
* File: main.c
* Author: biebera@msoe.edu <Andrew Bieber>
* Class: CPE 2600
* Date: 9/30/25
*
* Compile: make
* Run: ./minimat
*
* Summary:
*   Interactive “minimat” program. Users can assign values
*   to vectors a, b, c, d and perform operations including
*   addition, subtraction, scalar multiplication, dot product,
*   and cross product. Supports simple command-style input.
*   Extras: 'clear' command, basic help menu, and 2-D input
*   (x y) which fills z = 0 automatically.
*   NOTE: CHATGPT ONLY USED FOR COMMENTS/DOCUMENTATION ALL CODE
*   WRITTEN BY ME AND ONLY ME ANDREW BIEBER
*   CHATGPT WAS USED FOR GUIDANCE IN THE MAIN.C LOGIC
********************************************************************/

#include <stdio.h>
#include <string.h>
#include "math.h"

/* --------- helpers --------- */

static void print_menu(void) {
    printf("usage (spaces required):\n");
    printf("  a = 1 2 3        | a = 1,2,3       | a = 1 2   (z=0)\n");
    printf("  c = a + b        | c = a - b\n");
    printf("  c = a * 2        | c = 2 * a       (either order)\n");
    printf("  a + b            | a - b           -> ans = x y z\n");
    printf("  a * 2            | 2 * a           -> ans = x y z\n");
    printf("  a . b            -> ans = <scalar> (dot product)\n");
    printf("  a x b            -> ans = x y z    (cross product)\n");
    printf("  a                | list | clear | quit\n");
}

static int idx_from_char(char c) {
    if (c >= 'a' && c <= 'j') { return (int)(c - 'a'); }
    if (c >= 'A' && c <= 'J') { return (int)(c - 'A'); }
    return -1;
}

static void print_one_named(char name, vector v, int set) {
    if (set) {
        printf("%c = %g %g %g\n", name, v.x, v.y, v.z);
    } else {
        printf("%c = (unset)\n", name);
    }
}

static void commas_to_spaces(char *dst, const char *src) {
    size_t i = 0;
    while (src[i] != '\0') {
        dst[i] = (src[i] == ',') ? ' ' : src[i];
        i++;
    }
    dst[i] = '\0';
}

/* -------------------- main -------------------- */

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        print_menu();
        return 0;
    }

    vector store[10];   /* a..j vectors */
    int set[10] = {0};  /* track whether each vector is assigned */

    vector ans = (vector){0, 0, 0};

    char line_raw[128];
    char line[128];
    char lhs, x, y;
    double v1, v2, v3, s;

    while (1) {
        printf("minimat> ");
        if (!fgets(line_raw, sizeof line_raw, stdin)) {
            break;
        }

        if (line_raw[0] == '\n' || line_raw[0] == '\r') {
            print_menu();
            continue;
        }

        commas_to_spaces(line, line_raw);

        if (strncmp(line, "quit", 4) == 0) {
            break;
        }

        if (strncmp(line, "clear", 5) == 0) {
            for (int i = 0; i < 10; i++) {
                store[i] = (vector){0, 0, 0};
                set[i] = 0;
            }
            printf("cleared: a..j are now (unset)\n");
            continue;
        }

        if (strncmp(line, "list", 4) == 0) {
            for (int i = 0; i < 10; i++) {
                print_one_named((char)('a' + i), store[i], set[i]);
            }
            continue;
        }

        /* ---- assignments ---- */

        if (sscanf(line, " %c = %lf %lf %lf", &lhs, &v1, &v2, &v3) == 4) {
            int L = idx_from_char(lhs);
            if (L < 0) { printf("error: only a..j allowed\n"); continue; }
            store[L] = (vector){v1, v2, v3};
            set[L] = 1;
            print_one_named((char)('a' + L), store[L], set[L]);
            continue;
        }

        if (sscanf(line, " %c = %lf %lf", &lhs, &v1, &v2) == 3) {
            int L = idx_from_char(lhs);
            if (L < 0) { printf("error: only a..j allowed\n"); continue; }
            store[L] = (vector){v1, v2, 0.0};
            set[L] = 1;
            print_one_named((char)('a' + L), store[L], set[L]);
            continue;
        }

        if (sscanf(line, " %c = %c + %c", &lhs, &x, &y) == 3) {
            int L = idx_from_char(lhs);
            int X = idx_from_char(x);
            int Y = idx_from_char(y);
            if (L < 0 || X < 0 || Y < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X] || !set[Y]) { printf("error: operand unset\n"); continue; }
            store[L] = add(store[X], store[Y]);
            set[L] = 1;
            print_one_named((char)('a' + L), store[L], set[L]);
            continue;
        }

        if (sscanf(line, " %c = %c - %c", &lhs, &x, &y) == 3) {
            int L = idx_from_char(lhs);
            int X = idx_from_char(x);
            int Y = idx_from_char(y);
            if (L < 0 || X < 0 || Y < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X] || !set[Y]) { printf("error: operand unset\n"); continue; }
            store[L] = subtract(store[X], store[Y]);
            set[L] = 1;
            print_one_named((char)('a' + L), store[L], set[L]);
            continue;
        }

        if (sscanf(line, " %c = %c * %lf", &lhs, &x, &s) == 3) {
            int L = idx_from_char(lhs);
            int X = idx_from_char(x);
            if (L < 0 || X < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X]) { printf("error: operand unset\n"); continue; }
            store[L] = scalarmultiplication(store[X], s);
            set[L] = 1;
            print_one_named((char)('a' + L), store[L], set[L]);
            continue;
        }

        if (sscanf(line, " %c = %lf * %c", &lhs, &s, &x) == 3) {
            int L = idx_from_char(lhs);
            int X = idx_from_char(x);
            if (L < 0 || X < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X]) { printf("error: operand unset\n"); continue; }
            store[L] = scalarmultiplication(store[X], s);
            set[L] = 1;
            print_one_named((char)('a' + L), store[L], set[L]);
            continue;
        }

        /* ---- expressions (no assignment) ---- */

        if (sscanf(line, " %c + %c", &x, &y) == 2) {
            int X = idx_from_char(x);
            int Y = idx_from_char(y);
            if (X < 0 || Y < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X] || !set[Y]) { printf("error: operand unset\n"); continue; }
            ans = add(store[X], store[Y]);
            printf("ans = %g %g %g\n", ans.x, ans.y, ans.z);
            continue;
        }

        if (sscanf(line, " %c - %c", &x, &y) == 2) {
            int X = idx_from_char(x);
            int Y = idx_from_char(y);
            if (X < 0 || Y < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X] || !set[Y]) { printf("error: operand unset\n"); continue; }
            ans = subtract(store[X], store[Y]);
            printf("ans = %g %g %g\n", ans.x, ans.y, ans.z);
            continue;
        }

        if (sscanf(line, " %c * %lf", &x, &s) == 2) {
            int X = idx_from_char(x);
            if (X < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X]) { printf("error: operand unset\n"); continue; }
            ans = scalarmultiplication(store[X], s);
            printf("ans = %g %g %g\n", ans.x, ans.y, ans.z);
            continue;
        }

        if (sscanf(line, " %lf * %c", &s, &x) == 2) {
            int X = idx_from_char(x);
            if (X < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X]) { printf("error: operand unset\n"); continue; }
            ans = scalarmultiplication(store[X], s);
            printf("ans = %g %g %g\n", ans.x, ans.y, ans.z);
            continue;
        }

        if (sscanf(line, " %c . %c", &x, &y) == 2) {
            int X = idx_from_char(x);
            int Y = idx_from_char(y);
            if (X < 0 || Y < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X] || !set[Y]) { printf("error: operand unset\n"); continue; }
            double dprod = dotproduct(store[X], store[Y]);
            printf("ans = %g\n", dprod);
            continue;
        }

        if (sscanf(line, " %c x %c", &x, &y) == 2) {
            int X = idx_from_char(x);
            int Y = idx_from_char(y);
            if (X < 0 || Y < 0) { printf("error: only a..j allowed\n"); continue; }
            if (!set[X] || !set[Y]) { printf("error: operand unset\n"); continue; }
            vector cprod = crossproduct(store[X], store[Y]);
            printf("ans = %g %g %g\n", cprod.x, cprod.y, cprod.z);
            continue;
        }

        /* ---- single variable print fix ---- */
        {
            char ch, extra;
            if (sscanf(line, " %c %c", &ch, &extra) == 1) {
                int X = idx_from_char(ch);
                if (X < 0) {
                    printf("error: only a..j allowed\n");
                } else {
                    print_one_named((char)('a' + X), store[X], set[X]);
                }
                continue;
            }
            else if (sscanf(line, " %c %c", &ch, &extra) == 2) {
                printf("error: single variable display only (use a..j)\n");
                print_menu();
                continue;
            }
        }

        print_menu();
    }

    return 0;
}
