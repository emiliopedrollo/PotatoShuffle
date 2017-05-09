#ifndef   UTF8_H
#define   UTF8_H
#include <stdlib.h>
#include <errno.h>

#define   UTF8_MAXLEN 6

static size_t utf8_to_code(const unsigned char *const buffer, unsigned int *const codeptr)
{
    if (!buffer) {
        errno = EINVAL;
        return 0;
    }

    if (*buffer == 0U) {
        errno = 0;
        return 0;
    }

    if (*buffer < 128U) {
        if (codeptr)
            *codeptr = buffer[0];
        return 1;
    }

    if (*buffer < 192U) {
        errno = EILSEQ;
        return 0;
    }

    if (*buffer < 224U) {
        if (buffer[1] >= 128U && buffer[1] < 192U)
            return ((buffer[0] - 192U) << 6U)
                   |  (buffer[1] - 128U);
        errno = EILSEQ;
        return 0;
    }

    if (*buffer < 240U) {
        if (buffer[1] >= 128U && buffer[1] < 192U &&
            buffer[2] >= 128U && buffer[2] < 192U)
            return ((buffer[0] - 224U) << 12U)
                   | ((buffer[1] - 128U) << 6U)
                   |  (buffer[2] - 128U);
        errno = EILSEQ;
        return 0;
    }

    if (*buffer < 248U) {
        if (buffer[1] >= 128U && buffer[1] < 192U &&
            buffer[2] >= 128U && buffer[2] < 192U &&
            buffer[3] >= 128U && buffer[3] < 192U)
            return ((buffer[0] - 240U) << 18U)
                   | ((buffer[1] - 128U) << 12U)
                   | ((buffer[2] - 128U) << 6U)
                   |  (buffer[3] - 128U);
        errno = EILSEQ;
        return 0;
    }

    if (*buffer < 252U) {
        if (buffer[1] >= 128U && buffer[1] < 192U &&
            buffer[2] >= 128U && buffer[2] < 192U &&
            buffer[3] >= 128U && buffer[3] < 192U &&
            buffer[4] >= 128U && buffer[4] < 192U)
            return ((buffer[0] - 248U) << 24U)
                   | ((buffer[1] - 128U) << 18U)
                   | ((buffer[2] - 128U) << 12U)
                   | ((buffer[3] - 128U) << 6U)
                   |  (buffer[4] - 128U);
        errno = EILSEQ;
        return 0;
    }

    if (*buffer < 254U) {
        if (buffer[1] >= 128U && buffer[1] < 192U &&
            buffer[2] >= 128U && buffer[2] < 192U &&
            buffer[3] >= 128U && buffer[3] < 192U &&
            buffer[4] >= 128U && buffer[4] < 192U &&
            buffer[5] >= 128U && buffer[5] < 192U)
            return ((buffer[0] - 252U) << 30U)
                   | ((buffer[1] - 128U) << 24U)
                   | ((buffer[2] - 128U) << 18U)
                   | ((buffer[3] - 128U) << 12U)
                   | ((buffer[4] - 128U) << 6U)
                   |  (buffer[5] - 128U);
        errno = EILSEQ;
        return 0;
    }

    errno = EILSEQ;
    return 0;
}

static size_t code_to_utf8(unsigned char *const buffer, const unsigned int code)
{
    if (code < 128U) {
        buffer[0] = code;
        return 1;
    }
    if (code < 2048U) {
        buffer[0] = 0xC0U | (code >> 6U);
        buffer[1] = 0x80U | (code & 0x3FU);
        return 2;
    }
    if (code < 65536) {
        buffer[0] = 0xE0U | (code >> 12U);
        buffer[1] = 0x80U | ((code >> 6U) & 0x3FU);
        buffer[2] = 0x80U | (code & 0x3FU);
        return 3;
    }
    if (code < 2097152U) {
        buffer[0] = 0xF0U | (code >> 18U);
        buffer[1] = 0x80U | ((code >> 12U) & 0x3FU);
        buffer[2] = 0x80U | ((code >> 6U) & 0x3FU);
        buffer[3] = 0x80U | (code & 0x3FU);
        return 4;
    }
    if (code < 67108864U) {
        buffer[0] = 0xF8U | (code >> 24U);
        buffer[1] = 0x80U | ((code >> 18U) & 0x3FU);
        buffer[2] = 0x80U | ((code >> 12U) & 0x3FU);
        buffer[3] = 0x80U | ((code >> 6U) & 0x3FU);
        buffer[4] = 0x80U | (code & 0x3FU);
        return 5;
    }
    if (code <= 2147483647U) {
        buffer[0] = 0xFCU | (code >> 30U);
        buffer[1] = 0x80U | ((code >> 24U) & 0x3FU);
        buffer[2] = 0x80U | ((code >> 18U) & 0x3FU);
        buffer[3] = 0x80U | ((code >> 12U) & 0x3FU);
        buffer[4] = 0x80U | ((code >> 6U) & 0x3FU);
        buffer[5] = 0x80U | (code & 0x3FU);
        return 6;
    }
    errno = EINVAL;
    return 0;
}

#endif /* UTF8_H */