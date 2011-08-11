/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#include "des_int.h"

static const char *me;

struct test_case {
    char *saltstr;
    int saltlen;
    unsigned char keys[12][8];
};

struct test_case test_cases[] = {
    {
        "Sodium Chloride", -1,
        {
            { 0xa4, 0xd0, 0xd0, 0x9b, 0x86, 0x92, 0xb0, 0xc2, },
            { 0xf1, 0xf2, 0x9e, 0xab, 0xd0, 0xef, 0xdf, 0x73, },
            { 0xd6, 0x85, 0x61, 0xc4, 0xf2, 0x94, 0xf4, 0xa1, },
            { 0xd0, 0xe3, 0xa7, 0x83, 0x94, 0x61, 0xe0, 0xd0, },
            { 0xd5, 0x62, 0xcd, 0x94, 0x61, 0xcb, 0x97, 0xdf, },
            { 0x9e, 0xa2, 0xa2, 0xec, 0xa8, 0x8c, 0x6b, 0x8f, },
            { 0xe3, 0x91, 0x6d, 0xd3, 0x85, 0xf1, 0x67, 0xc4, },
            { 0xf4, 0xc4, 0x73, 0xc8, 0x8a, 0xe9, 0x94, 0x6d, },
            { 0xa1, 0x9e, 0xb3, 0xad, 0x6b, 0xe3, 0xab, 0xd9, },
            { 0xad, 0xa1, 0xce, 0x10, 0x37, 0x83, 0xa7, 0x8c, },
            { 0xd3, 0x01, 0xd0, 0xf7, 0x3e, 0x7a, 0x49, 0x0b, },
            { 0xb6, 0x2a, 0x4a, 0xec, 0x9d, 0x4c, 0x68, 0xdf, },
        }
    },
    {
        "NaCl", 4,
        {
            { 0x61, 0xef, 0xe6, 0x83, 0xe5, 0x8a, 0x6b, 0x98 },
            { 0x68, 0xcd, 0x68, 0xad, 0xc4, 0x86, 0xcd, 0xe5 },
            { 0x83, 0xa1, 0xc8, 0x86, 0x8f, 0x67, 0xd0, 0x62 },
            { 0x9e, 0xc7, 0x8f, 0xa4, 0xa4, 0xb3, 0xe0, 0xd5 },
            { 0xd9, 0x92, 0x86, 0x8f, 0x9d, 0x8c, 0x85, 0xe6 },
            { 0xda, 0xf2, 0x92, 0x83, 0xf4, 0x9b, 0xa7, 0xad },
            { 0x91, 0xcd, 0xad, 0xef, 0x86, 0xdf, 0xd3, 0xa2 },
            { 0x73, 0xd3, 0x67, 0x68, 0x8f, 0x6e, 0xe3, 0x73 },
            { 0xc4, 0x61, 0x85, 0x9d, 0xad, 0xf4, 0xdc, 0xb0 },
            { 0xe9, 0x02, 0x83, 0x16, 0x2c, 0xec, 0xe0, 0x08 },
            { 0x61, 0xc8, 0x26, 0x29, 0xd9, 0x73, 0x6e, 0xb6 },
            { 0x8c, 0xa8, 0x9e, 0xc4, 0xa8, 0xdc, 0x31, 0x73 },
        }
    },
    {
        /* This one intentionally supplies a length shorter
           than the string.  The point of this is to ensure
           that s[len] is not zero, so that anything actually
           relying on that value (i.e., reading out of bounds)
           should generate incorrect results.  */
        "NaCl2", 4,
        {
            { 0x61, 0xef, 0xe6, 0x83, 0xe5, 0x8a, 0x6b, 0x98 },
            { 0x68, 0xcd, 0x68, 0xad, 0xc4, 0x86, 0xcd, 0xe5 },
            { 0x83, 0xa1, 0xc8, 0x86, 0x8f, 0x67, 0xd0, 0x62 },
            { 0x9e, 0xc7, 0x8f, 0xa4, 0xa4, 0xb3, 0xe0, 0xd5 },
            { 0xd9, 0x92, 0x86, 0x8f, 0x9d, 0x8c, 0x85, 0xe6 },
            { 0xda, 0xf2, 0x92, 0x83, 0xf4, 0x9b, 0xa7, 0xad },
            { 0x91, 0xcd, 0xad, 0xef, 0x86, 0xdf, 0xd3, 0xa2 },
            { 0x73, 0xd3, 0x67, 0x68, 0x8f, 0x6e, 0xe3, 0x73 },
            { 0xc4, 0x61, 0x85, 0x9d, 0xad, 0xf4, 0xdc, 0xb0 },
            { 0xe9, 0x02, 0x83, 0x16, 0x2c, 0xec, 0xe0, 0x08 },
            { 0x61, 0xc8, 0x26, 0x29, 0xd9, 0x73, 0x6e, 0xb6 },
            { 0x8c, 0xa8, 0x9e, 0xc4, 0xa8, 0xdc, 0x31, 0x73 },
        }
    },
};

static void do_it (struct test_case *tcase);

int
main (int argc, char *argv[])
{
    int i;

    me = argv[0];
    for (i = 0; i < sizeof (test_cases) / sizeof (struct test_case); i++)
        do_it (&test_cases[i]);
    return 0;
}

static void
do_it (struct test_case *tcase)
{
    unsigned char keydata[8];
    krb5_data salt, passwd;
    krb5_keyblock key;
    krb5_error_code err;
    int i;
    unsigned char longpass[2048];

    key.contents = keydata;
    key.length = sizeof (keydata);

    salt.data = tcase->saltstr;
    if (tcase->saltlen == -1)
        salt.length = strlen (tcase->saltstr);
    else
        salt.length = tcase->saltlen;

    /*
     * Try passwords with lengths equal to, greater than, and less
     * than 8 characters, since the AFS s2k algorithm does
     * interesting stuff depending on the length.
     */
    passwd.data = "My Password";
    for (i = 0; i < 12; i++) {
        passwd.length = i;
        err = mit_afs_string_to_key (&key, &passwd, &salt);
        if (err != 0) {
            com_err (me, err, "");
            exit (1);
        }
        if (memcmp (tcase->keys[i], keydata, 8) != 0)
            abort ();
    }

    /* Run another pass to make sure the characters after the
       password in the buffer aren't influencing the output.  The
       password is *not* required to be null-terminated.  */
    memset (longpass, '!', sizeof (longpass));
    longpass[sizeof (longpass)-1] = '\0';
    memcpy (longpass, "My Password", strlen ("My Password"));
    passwd.data = (char *) longpass;
    for (i = 0; i < 12; i++) {
        passwd.length = i;
        err = mit_afs_string_to_key (&key, &passwd, &salt);
        if (err != 0) {
            com_err (me, err, "");
            exit (1);
        }
        if (memcmp (tcase->keys[i], keydata, 8) != 0)
            abort ();
    }
}
