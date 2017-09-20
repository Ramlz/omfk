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

int strlen(const char *str)
{
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

    while ((ch=*(fmt++))) {
        if ((unsigned int)((pbuffer - buffer) + 1) >= buffer_len) {
            break;
        }
        if (ch!='%') {
            putc(ch);
        } else {
            char zero_pad = 0;
            char *ptr;
            unsigned int len;

            ch=*(fmt++);

            if (ch=='0') {
                ch=*(fmt++);
                if (ch == '\0') {
                    goto end;
                }
                if (ch >= '0' && ch <= '9') {
                    zero_pad = ch - '0';
                }
                ch=*(fmt++);
            }

            switch (ch) {
                case 0:
                    goto end;
                case 'u':
                case 'd':
                    len = itoa(va_arg(va, unsigned int), 10, 0, (ch=='u'),
                        buf, zero_pad);
                    puts(buf, len);
                    break;
                case 'x':
                case 'X':
                    len = itoa(va_arg(va, unsigned int), 16, (ch=='X'), 1,
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

int sscanf(const char* str, const char* fmt, ...) {
    int count;
    va_list va;
    
    va_start (va, fmt);
    count = vsscanf (str, fmt, va);
    va_end (va);
    return (count);
}

int vsscanf(const char* str, const char* fmt, va_list va) {
    int value, tmp;
    int count;
    int pos;
    char neg, fmt_code;
    const char* pf;
    char* sval;

    for (pf = fmt, count = 0; *fmt != 0 && *str != 0; fmt++, str++) {
        while (*fmt == ' ' && *fmt != 0) {
            fmt++;
        }
        if (*fmt == 0) {
            break;
        }
        while (*str == ' ' && *str != 0) {
            str++;
        }
        if (*str == 0) {
            break;
        }
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'n') {
                if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
                    fmt_code = 'x';
                    str += 2;
                } else {
                    fmt_code = 'd';
                }
            } else {
                fmt_code = *fmt;
            }

            switch (fmt_code) {
            case 'x':
            case 'X':
                for (value = 0, pos = 0; *str != 0; str++, pos++) {
                    if ('0' <= *str && *str <= '9') {
                        tmp = *str - '0';
                    } else if ('a' <= *str && *str <= 'f') {
                        tmp = *str - 'a' + 10;
                    } else if ('A' <= *str && *str <= 'F') {
                        tmp = *str - 'A' + 10;
                    } else {
                        break;
                    }
                    value *= 16;
                    value += tmp;
                }
                if (pos == 0) {
                    return count;
                }
                *(va_arg(va, int*)) = value;
                count++;
                break;
            case 'u':
            case 'd':
                if (*str == '-') {
                    neg = 1;
                    str++;
                } else {
                    neg = 0;
                }
                for (value = 0, pos = 0; *str != 0; str++, pos++) {
                    if ('0' <= *str && *str <= '9') {
                        value = value*10 + (int)(*str - '0');
                    } else {
                        break;
                    }
                }
                if (pos == 0) {
                    return count;
                }
                *(va_arg(va, int*)) = neg ? -value : value;
                count++;
                break;
            case 'c':
                *(va_arg(va, char*)) = *str;
                count++;
                break;
            case 's':
                sval = va_arg(va, char*);
                while(*str && *str != ' ') {
                    *sval++ = *str++;
                    count++;
                }
                *sval = 0;
                break;
            default:
                return count;
            }
        } else {
            if (*fmt != *str) {
                break;
            }
        }
    }
    return count;
}

static unsigned int itoa(int value, unsigned int radix, unsigned int uppercase,
    unsigned int unsig, char *buffer, unsigned int zero_pad) {
    char *pbuffer = buffer;
    int negative = 0;
    unsigned int i, len;

    // unsupported
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
