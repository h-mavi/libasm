#include "libasm.h"
#include <sys/types.h>

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

#define STR_SIZE 128

static void print_result(int ok) {
    printf("    %s\n", ok ? GREEN "[OK]" RESET : RED "[KO]" RESET);
}

/* ---------- STRLEN ---------- */
static void test_strlen(const char *s)
{
    size_t	a = strlen(s),
			b = ft_strlen(s);
    printf("test '%s'\n    original -> %zu\n    ft_      -> %zu\n", s, a, b);
    print_result(a == b);
}

/* ---------- STRCPY ---------- */
static void test_strcpy(const char *src)
{
    char d1[STR_SIZE], d2[STR_SIZE];

    memset(d1, 'X', sizeof(d1));
    memset(d2, 'X', sizeof(d2));
    printf("test '%s'\n    original -> '%s'\n    ft_      -> '%s'\n", src, strcpy(d1, src), ft_strcpy(d2, src));
    print_result(strcmp(d1, d2) == 0);
}

/* ---------- STRCMP ---------- */

static void test_strcmp(const char *s1, const char *s2)
{
    int	a = strcmp(s1, s2),
		b = ft_strcmp(s1, s2);

    printf("test '%s' - '%s'\n    original -> %d\n    ft_      -> %d\n", s1, s2, a, b);
    print_result(((a > 0) - (a < 0)) == ((b > 0) - (b < 0)));
}

/* ---------- WRITE ---------- */
static void test_write(const char *label, int fd1, int fd2, const void *buf, size_t count)
{
    ssize_t r1, r2;
    int     e1, e2;

    errno = 0;
    r1 = write(fd1, buf, count);
    e1 = errno;
    errno = 0;
    r2 = ft_write(fd2, buf, count);
    e2 = errno;

    printf("test %s\n    original -> ret %zd, errno %d (%s)\n    ft_      -> ret %zd, errno %d (%s)\n",
           label, r1, e1, e1 ? strerror(e1) : "-", r2, e2, e2 ? strerror(e2) : "-");
    print_result(r1 == r2 && e1 == e2);
}

/* ---------- READ ---------- */
static void test_read(const char *label, int fd1, int fd2, int use_null_buf, size_t count)
{
    char    b1[STR_SIZE], b2[STR_SIZE];
    ssize_t r1, r2;
    int     e1, e2;

    memset(b1, 0, sizeof b1);
    memset(b2, 0, sizeof b2);
    errno = 0;
    r1 = read(fd1, use_null_buf ? NULL : b1, count);
    e1 = errno;
    errno = 0;
    r2 = ft_read(fd2, use_null_buf ? NULL : b2, count);
    e2 = errno;

    printf("test %s\n    original -> ret %zd, errno %d (%s), buf '%s'\n"
           "    ft_      -> ret %zd, errno %d (%s), buf '%s'\n",
           label, r1, e1, e1 ? strerror(e1) : "-", b1,
           r2, e2, e2 ? strerror(e2) : "-", b2);
    print_result(r1 == r2 && e1 == e2 && memcmp(b1, b2, sizeof b1) == 0);
}

/* ---------- STRDUP ---------- */
static void test_strdup(const char *s)
{
    char *a = strdup(s), *b = ft_strdup(s);

    printf("test '%s'\n    original -> '%s'\n    ft_      -> '%s'\n", s, a, b);
    print_result(a && b && strcmp(a, b) == 0);
    free(a);
    free(b);
}

int main(void)
{
    const char *a = "Hello", *empty = "";
    const char *d1 = "Hell", *d2 = "Hello world!", *d3 = "Bye";
    const char *long_s = "Una stringa molto lunga per testare bene il ciclo di ft_strlen 0123456789";

    printf("\n========== FT_STRLEN ==========\n");
    test_strlen(a);
    test_strlen(empty);
    test_strlen(long_s);

    printf("\n========== FT_STRCPY ==========\n");
    test_strcpy(a);
    test_strcpy(empty);
    test_strcpy(long_s);

    printf("\n========== FT_STRCMP ==========\n");
    test_strcmp(a, a);
    test_strcmp(a, d1);
    test_strcmp(a, d2);
    test_strcmp(a, d3);
    test_strcmp(a, empty);
    test_strcmp(empty, a);
    test_strcmp(empty, empty);
    test_strcmp("a\xff", "a\x01");   /* confronto come unsigned char */

    printf("\n========== FT_WRITE ==========\n");
    {
        const char *f1 = "file1.txt", *f2 = "file2.txt";
        int   fd1 = open(f1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        int   fd2 = open(f2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        int   rd = open(f1, O_RDONLY);
        int   closed_fd;

        if (fd1 < 0 || fd2 < 0 || rd < 0) { perror("open"); return 1; }

        test_write("write su file: \"Hello\"", fd1, fd2, a, 5);
        test_write("write su file: count 0", fd1, fd2, a, 0);
        test_write("write su file: stringa lunga", fd1, fd2, long_s, strlen(long_s));
        test_write("write su stdout", 1, 1, "stdout ok\n", 10);
        test_write("write fd non valido (-1)", -1, -1, a, 5);
        test_write("write fd di sola lettura", rd, rd, a, 5);
        test_write("write buffer NULL", fd1, fd2, NULL, 5);

        closed_fd = open(f1, O_RDONLY);
        close(closed_fd);
        test_write("write fd chiuso", closed_fd, closed_fd, a, 5);

        close(fd1);
        close(fd2);
        close(rd);
    }

    printf("\n========== FT_READ ==========\n");
    {
        const char *f1 = "file1.txt";
        int fd1 = open(f1, O_RDONLY);
        int fd2 = open(f1, O_RDONLY);
        int wr  = open("file_w.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        int dir = open(".", O_RDONLY);
        int closed_fd;

        if (fd1 < 0 || fd2 < 0 || wr < 0) { perror("open"); return 1; }

        test_read("read 5 byte", fd1, fd2, 0, 5);
        test_read("read 20 byte (continua)", fd1, fd2, 0, 20);
        test_read("read count 0", fd1, fd2, 0, 0);
        test_read("read fino a EOF", fd1, fd2, 0, 100);
        test_read("read a EOF (ritorna 0)", fd1, fd2, 0, 10);
        test_read("read fd non valido (-1)", -1, -1, 0, 5);
        test_read("read fd di sola scrittura", wr, wr, 0, 5);
        if (dir >= 0)
            test_read("read su directory (EISDIR)", dir, dir, 0, 5);
        test_read("read buffer NULL", fd1, fd2, 1, 5);

        closed_fd = open(f1, O_RDONLY);
        close(closed_fd);
        test_read("read fd chiuso", closed_fd, closed_fd, 0, 5);

        close(fd1);
        close(fd2);
        close(wr);
        if (dir >= 0) close(dir);
        unlink("file_w.txt"); // elimina il file dal disco
    }

    printf("\n========== FT_STRDUP ==========\n");
    test_strdup(a);
    test_strdup(empty);
    test_strdup(long_s);
}