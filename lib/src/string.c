#include "string.h"

int strncmp(const char *str1, const char *str2, int n) {
    while (n && *str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
        --n;
    }
    if (n == 0) {
        return 0;
    } else {
        return *(unsigned char *) str1 - *(unsigned char *) str2;
    }
}

int strlen(const char *str) {
    const char *s;
    for (s = str; *s; ++s);
    return (s - str);
}

int vsnprintf(char *buffer, unsigned int buffer_len,
    const char *fmt, va_list va) {
    char *pbuffer = buffer;
    char buf[STR_BUF_SIZE];
    char ch;

    int putc(char ch) {
        if ((unsigned int)((pbuffer - buffer) + 1) >= buffer_len) {
            return 0;
        }
        *(pbuffer++) = ch;
        *(pbuffer) = '\0';
        return 1;
    }

    int puts(char *s, unsigned int len)
    {
        unsigned int i;

        if (buffer_len - (pbuffer - buffer) - 1 < len) {
            len = buffer_len - (pbuffer - buffer) - 1;
        }

        for (i = 0; i < len; i++) {
            *(pbuffer++) = s[i];
        }
        *(pbuffer) = '\0';
        return len;
    }

    while ((ch = *(fmt++))) {
        if ((unsigned int)((pbuffer - buffer) + 1) >= buffer_len) {
            break;
        }
        if (ch != '%') {
            putc(ch);
        } else {
            char zero_pad = 0;
            char *ptr;
            unsigned int len;

            ch = *(fmt++);

            if (ch == '0') {
                ch = *(fmt++);
                if (ch == '\0') {
                    goto end;
                }
                if (ch >= '0' && ch <= '9') {
                    zero_pad = ch - '0';
                }
                ch = *(fmt++);
            }

            switch (ch) {
                case 0:
                    goto end;
                case 'u':
                case 'd':
                    len = itoa(va_arg(va, unsigned int), 10, 0, (ch == 'u'),
                        buf, zero_pad);
                    puts(buf, len);
                    break;
                case 'x':
                case 'X':
                    len = itoa(va_arg(va, unsigned int), 16, (ch == 'X'), 1,
                        buf, zero_pad);
                    puts(buf, len);
                    break;
                case 'c' :
                    putc((char)(va_arg(va, int)));
                    break;
                case 's' :
                    ptr = va_arg(va, char*);
                    puts(ptr, strlen(ptr));
                    break;
                default:
                    putc(ch);
                    break;
            }
        }
    }
    end:
    return pbuffer - buffer;
}

int snprintf(char *buffer, unsigned int buffer_len, const char *fmt, ...) {
    int ret;
    va_list va;
    va_start(va, fmt);
    ret = vsnprintf(buffer, buffer_len, fmt, va);
    va_end(va);

    return ret;
}

unsigned int itoa(int value, unsigned int radix, unsigned int uppercase,
    unsigned int unsig, char *buffer, unsigned int zero_pad) {
    char *pbuffer = buffer;
    int negative = 0;
    unsigned int i, len;

    //! unsupported
    if (radix > 16)
        return 0;

    if (value < 0 && !unsig) {
        negative = 1;
        value = -value;
    }

    do {
        int digit = value % radix;
        *(pbuffer++) =
            (digit < 10 ? '0' + digit : (uppercase ? 'A' : 'a') + digit - 10);
        value /= radix;
    } while (value > 0);

    for (i = (pbuffer - buffer); i < zero_pad; i++) {
        *(pbuffer++) = '0';
    }

    if (negative) {
        *(pbuffer++) = '-';
    }

    *(pbuffer) = '\0';

    len = (pbuffer - buffer);
    for (i = 0; i < len / 2; i++) {
        char j = buffer[i];
        buffer[i] = buffer[len-i-1];
        buffer[len-i-1] = j;
    }

    return len;
}

int atoi(const char *str) {
    int n = 0;
    int sign = 1;

    while (*str == ' ') {
        ++str;
    }
    if (*str == '-') {
        sign = -1;
        ++str;
    } else if (*str == '+') {
        sign = 1;
        ++str;
    }
    while (*str >= '0' && *str <= '9') {
        if (n > INT_MAX/10) {
            break;
        }
        n *= 10;
        int ch = *str - '0';

        if (n > INT_MAX - ch) {
            break;
        }
        n += ch;
        ++str;
    }
    if (*str >= '0' && *str <= '9') {
        return sign == 1 ? INT_MAX : INT_MIN;
    }
    return sign * n;
}

char *strtok(char *str, const char *delim) {
    static char *nxt;
    static int size;

    int i;

    if (str) {
        nxt = str;
        size = strlen(str);
    } else if (size > 0) {
        nxt++;
        size--;
        str = nxt;
    } else {
        str = NULL;
    }

    while (*nxt) {
        i = strspn(nxt, delim);
        while (i > 1) {
            *nxt = '\0';
            nxt++;
            size--;
            i--;
        }
        if (i == 1) {
            *nxt = '\0';
            if (size > 1) {
                nxt--;
                size++;
            }
        }
        nxt++;
        size--;
    }
    return str;
}

int strspn(const char *str1,const char *str2) {
    const char *p;
    const char *a;
    int count = 0;

    for (p = str1; *p != '\0'; ++p) {
        for (a = str2; *a != '\0'; ++a) {
            if (*p == *a) {
                break;
            }
        }
        if (*a == '\0') {
            return count;
        } else {
            ++count;
        }
    }

    return count;
}

char *strcpy(char *dest, const char *src) {
    char *temp = dest;
    while ((bool) (*dest++ = *src++));
    return temp;
}

char* strstr(const char *str, const char *target) {
    if (!*target) {
        return (char*) str;
    }
    char *p1 = (char*) str;
    while (*p1) {
        char *p1_begin = p1;
        char *p2 = (char*) target;
        while (*p1 && *p2 && *p1 == *p2) {
            p1++;
            p2++;
        }
        if (!*p2) {
            return p1_begin;
        }
        p1 = p1_begin + 1;
    }
    return NULL;
}
