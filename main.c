/******************************************************************** 
* File: main.c
* Author: biebera@msoe.edu <Andrew Bieber>
* Class: CPE 2600
* Date: 10/27/25
*
* Compile: make
* Run: ./minimat
*
* Summary:
*   Updated interactive “minimat” program.
*   Adds dynamic vector storage (unlimited vectors)
*   and load/save CSV file support.
*
*   Commands:
*     load <file>      load vectors from CSV
*     save <file>      save current vectors to CSV
*     clear            clear all vectors (frees dynamic memory)
*     quit             exit cleanly (no leaks)
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "vector_dynamic.h"

/* -------- helpers -------- */

static void print_menu(void) {
    printf("usage (spaces required):\n");
    printf("  a = 1 2 3 | a = 1,2,3 | a = 1 2 (z=0)\n");
    printf("  c = a + b | c = a - b | c = a x b\n");
    printf("  c = a * 2 | c = 2 * a | a . b\n");
    printf("  list | load <file> | save <file> | clear | quit\n");
}

static int idx_from_char(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= 'A' && c <= 'Z') return c - 'A';
    return -1;
}

static void print_one_named(char name, vector v, int set) {
    if (set)
        printf("%c = %g %g %g\n", name, v.x, v.y, v.z);
    else
        printf("%c = (unset)\n", name);
}

static void commas_to_spaces(char *dst, const char *src) {
    size_t i = 0;
    while (src[i]) {
        dst[i] = (src[i] == ',') ? ' ' : src[i];
        i++;
    }
    dst[i] = '\0';
}

/* -------- CSV I/O -------- */

static void load_csv(VectorStore *vs, const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        perror("load");
        return;
    }
    char buf[128];
    char name;
    double x, y, z;
    int line = 0;
    while (fgets(buf, sizeof buf, fp)) {
        line++;
        if (sscanf(buf, " %c,%lf,%lf,%lf", &name, &x, &y, &z) == 4) {
            int idx = idx_from_char(name);
            if (idx >= 0) {
                set_vector(vs, idx, (vector){x, y, z});
            }
        }
    }
    fclose(fp);
    printf("Loaded vectors from '%s'\n", fname);
}

static void save_csv(VectorStore *vs, const char *fname) {
    FILE *fp = fopen(fname, "w");
    if (!fp) {
        perror("save");
        return;
    }
    for (int i = 0; i < vs->count; i++) {
        if (is_set(vs, i))
            fprintf(fp, "%c,%g,%g,%g\n",
                    'a' + i, vs->data[i].x, vs->data[i].y, vs->data[i].z);
    }
    fclose(fp);
    printf("Saved vectors to '%s'\n", fname);
}

/* -------------------- main -------------------- */

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        print_menu();
        return 0;
    }

    VectorStore store;
    init_store(&store);  // dynamic storage setup

    vector ans = {0, 0, 0};
    char line_raw[128], line[128];
    char lhs, x, y, fname[64];
    double v1, v2, v3, s;

    while (1) {
        printf("minimat> ");
        if (!fgets(line_raw, sizeof line_raw, stdin)) break;
        if (line_raw[0] == '\n' || line_raw[0] == '\r') { print_menu(); continue; }

        commas_to_spaces(line, line_raw);

        /* quit */
        if (strncmp(line, "quit", 4) == 0) break;

        /* clear */
        if (strncmp(line, "clear", 5) == 0) {
            clear_vectors(&store);
            printf("cleared: all vectors unset\n");
            continue;
        }

        /* list */
        if (strncmp(line, "list", 4) == 0) {
            for (int i = 0; i < store.count; i++)
                print_one_named('a' + i, get_vector(&store, i), is_set(&store, i));
            continue;
        }

        /* load/save */
        if (sscanf(line, "load %63s", fname) == 1) { load_csv(&store, fname); continue; }
        if (sscanf(line, "save %63s", fname) == 1) { save_csv(&store, fname); continue; }

        /* assignments */
        if (sscanf(line, " %c = %lf %lf %lf", &lhs, &v1, &v2, &v3) == 4) {
            int L = idx_from_char(lhs);
            set_vector(&store, L, (vector){v1, v2, v3});
            print_one_named(lhs, get_vector(&store, L), 1);
            continue;
        }
        if (sscanf(line, " %c = %lf %lf", &lhs, &v1, &v2) == 3) {
            int L = idx_from_char(lhs);
            set_vector(&store, L, (vector){v1, v2, 0});
            print_one_named(lhs, get_vector(&store, L), 1);
            continue;
        }

        /* binary operations */
        if (sscanf(line, " %c = %c + %c", &lhs, &x, &y) == 3) {
            int L = idx_from_char(lhs), X = idx_from_char(x), Y = idx_from_char(y);
            if (!is_set(&store, X) || !is_set(&store, Y)) { printf("error: unset operand\n"); continue; }
            set_vector(&store, L, add(get_vector(&store, X), get_vector(&store, Y)));
            print_one_named(lhs, get_vector(&store, L), 1);
            continue;
        }

        if (sscanf(line, " %c = %c - %c", &lhs, &x, &y) == 3) {
            int L = idx_from_char(lhs), X = idx_from_char(x), Y = idx_from_char(y);
            if (!is_set(&store, X) || !is_set(&store, Y)) { printf("error: unset operand\n"); continue; }
            set_vector(&store, L, subtract(get_vector(&store, X), get_vector(&store, Y)));
            print_one_named(lhs, get_vector(&store, L), 1);
            continue;
        }

        if (sscanf(line, " %c = %c x %c", &lhs, &x, &y) == 3) {
            int L = idx_from_char(lhs), X = idx_from_char(x), Y = idx_from_char(y);
            if (!is_set(&store, X) || !is_set(&store, Y)) { printf("error: unset operand\n"); continue; }
            set_vector(&store, L, crossproduct(get_vector(&store, X), get_vector(&store, Y)));
            print_one_named(lhs, get_vector(&store, L), 1);
            continue;
        }

        if (sscanf(line, " %c = %c * %lf", &lhs, &x, &s) == 3) {
            int L = idx_from_char(lhs), X = idx_from_char(x);
            set_vector(&store, L, scalarmultiplication(get_vector(&store, X), s));
            print_one_named(lhs, get_vector(&store, L), 1);
            continue;
        }
        if (sscanf(line, " %c = %lf * %c", &lhs, &s, &x) == 3) {
            int L = idx_from_char(lhs), X = idx_from_char(x);
            set_vector(&store, L, scalarmultiplication(get_vector(&store, X), s));
            print_one_named(lhs, get_vector(&store, L), 1);
            continue;
        }

        /* simple expressions (no assignment) */
        if (sscanf(line, " %c + %c", &x, &y) == 2) {
            ans = add(get_vector(&store, idx_from_char(x)), get_vector(&store, idx_from_char(y)));
            printf("ans = %g %g %g\n", ans.x, ans.y, ans.z);
            continue;
        }

        if (sscanf(line, " %c - %c", &x, &y) == 2) {
            ans = subtract(get_vector(&store, idx_from_char(x)), get_vector(&store, idx_from_char(y)));
            printf("ans = %g %g %g\n", ans.x, ans.y, ans.z);
            continue;
        }

        if (sscanf(line, " %c . %c", &x, &y) == 2) {
            double d = dotproduct(get_vector(&store, idx_from_char(x)), get_vector(&store, idx_from_char(y)));
            printf("ans = %g\n", d);
            continue;
        }

        if (sscanf(line, " %c x %c", &x, &y) == 2) {
            ans = crossproduct(get_vector(&store, idx_from_char(x)), get_vector(&store, idx_from_char(y)));
            printf("ans = %g %g %g\n", ans.x, ans.y, ans.z);
            continue;
        }

        /* print single variable */
        char ch;
        if (sscanf(line, " %c", &ch) == 1) {
            int X = idx_from_char(ch);
            print_one_named(ch, get_vector(&store, X), is_set(&store, X));
            continue;
        }

        print_menu();
    }

    free_store(&store);
    return 0;
}
