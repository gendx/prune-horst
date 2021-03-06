/*
The MIT License (MIT)

Copyright (c) 2016 kste
original Haraka implementations

Copyright (c) 2017 Nagravision S.A.
changes by JP Aumasson, Guillaume Endignoux, 2017: improvements, non-ni versions

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include "haraka.h"
#include <string.h>


void aesenc (uint8_t *s, const uint8_t *rk) {
    uint8_t i, t, u, v[4][4];
    for (i = 0; i < 16; ++i) v[((i / 4) + 4 - (i % 4)) % 4][i % 4] = sbox[s[i]];
    for (i = 0; i < 4; ++i) {
        t = v[i][0];
        u = v[i][0] ^ v[i][1] ^ v[i][2] ^ v[i][3];
        v[i][0] ^= u ^ XT (v[i][0] ^ v[i][1]);
        v[i][1] ^= u ^ XT (v[i][1] ^ v[i][2]);
        v[i][2] ^= u ^ XT (v[i][2] ^ v[i][3]);
        v[i][3] ^= u ^ XT (v[i][3] ^ t);
    }
    for (i = 0; i < 16; ++i) s[i] = v[i / 4][i % 4] ^ rk[i];
}


void haraka256 (uint8_t *out, const uint8_t *in) {

    uint8_t s0[16], s1[16];
    uint32_t tmp[4];
    int i;

    memcpy (s0, in, 16);
    memcpy (s1, in + 16, 16);

    AES2 (0);
    MIX2;
    AES2 (4);
    MIX2;
    AES2 (8);
    MIX2;
    AES2 (12);
    MIX2;
    AES2 (16);
    MIX2;
    AES2 (20);
    MIX2;

    for (i = 0; i < 16; ++i) {
        out[i] = in[i] ^ s0[i];
        out[i + 16] = in[i + 16] ^ s1[i];
    }
}

void haraka512 (uint8_t *out, const uint8_t *in) {

    uint8_t s0[16], s1[16], s2[16], s3[16];
    uint32_t tmp[5];
    int i;

    memcpy (s0, in, 16);
    memcpy (s1, in + 16, 16);
    memcpy (s2, in + 32, 16);
    memcpy (s3, in + 48, 16);

    AES4 (0);
    MIX4;
    AES4 (8);
    MIX4;
    AES4 (16);
    MIX4;
    AES4 (24);
    MIX4;
    AES4 (32);
    MIX4;
    AES4 (40);
    MIX4;

    for (i = 0; i < 16; ++i) {
        s0[i] = in[i] ^ s0[i];
        s1[i] = in[i + 16] ^ s1[i];
        s2[i] = in[i + 32] ^ s2[i];
        s3[i] = in[i + 48] ^ s3[i];
    }

    ((uint64_t *)out)[0] = ((uint64_t *)s0)[1];
    ((uint64_t *)out)[1] = ((uint64_t *)s1)[1];
    ((uint64_t *)out)[2] = ((uint64_t *)s2)[0];
    ((uint64_t *)out)[3] = ((uint64_t *)s3)[0];
}
