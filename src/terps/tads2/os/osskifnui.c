/* 
 *   Copyright (c) 1987, 2002 by Michael J. Roberts.  All Rights Reserved.
 *   
 *   Please see the accompanying license file, LICENSE.TXT, for information
 *   on using and copying this software.  
 */
/*
Name
  osdosnui.c - DOS/Win-specific functions with no user-interface component
Function
  Provides DOS/Windows implementations of a few miscellaneous osifc
  functions that have no user-interface component.  These are useful for
  linking versions of programs with alternative UI's outside of the set
  implemented in osdos.c and the like.
Notes

Modified
  04/05/02 MJRoberts  - Creation (from osdos.c)
*/

#ifdef T_WIN32
#include <windows.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "std.h"
#include "os.h"

extern void canonicalize_path(char *);
extern void safe_strcpy(char *dst, size_t dstlen, const char *src);
extern void safe_strcpyl(char *dst, size_t dstl, const char *src, size_t srcl);
extern unsigned long oss_get_file_attrs(const char *fname);

/*
 *   Open a file.  We'll open the low-level handle with the 'omode' mode,
 *   then open a FILE stream on the handle with the 'mode' string.  
 */
osfildef *os_fsopen(const char *fname, const char *mode, int omode, int sh)
{
    int fd;
    
    /* add the non-inheritable flag to the open mode */
    omode |= _O_NOINHERIT;

    /* try opening the file handle */
    fd = _sopen(fname, omode, sh, _S_IREAD | _S_IWRITE);

    /* if we didn't get a valid file, return null */
    if (fd == -1)
        return 0;

    /* return a FILE stream on the handle */
    return _fdopen(fd, mode);
}

/* ------------------------------------------------------------------------ */
/*
 *   Windows-specific implementation 
 */

/* Windows crypto context */
static HCRYPTPROV oswin_hcp = 0;

/* internal - get a windows crypto context if we don't have one already */
static void init_crypto_ctx()
{
    /* set up a crypto context */
    if (oswin_hcp == 0)
        CryptAcquireContext(&oswin_hcp, 0, 0, PROV_RSA_FULL,
                            CRYPT_VERIFYCONTEXT);
}

/*
 *   initialize 
 */
int osnoui_init(int *argc, char *argv[], const char *prompt,
            char *buf, int bufsiz)
{
    /* initialize the windows crypto context */
    init_crypto_ctx();

    /* success */
    return 0;
}

/*
 *   uninitialize 
 */
void osnoui_uninit()
{
    /* done with our crypto context */
    if (oswin_hcp != 0)
    {
        CryptReleaseContext(oswin_hcp, 0);
        oswin_hcp = 0;
    }

}

/* ------------------------------------------------------------------------ */
/*
 *   Get the system clock in milliseconds.  This is extremely easy for Win32
 *   console apps, since there's an API that does exactly what we want.  
 */
long os_get_sys_clock_ms(void)
{
    return (long)GetTickCount();
}


/* ------------------------------------------------------------------------ */
/*
 *   Random numbers 
 */
void os_gen_rand_bytes(unsigned char *buf, size_t len)
{
    /* make sure we have a crypto context */
    init_crypto_ctx();

    /* generate bytes via the system crypto API */
    CryptGenRandom(oswin_hcp, len, buf);
}

/* ------------------------------------------------------------------------ */
/*
 *   Get the absolute path for the given filename 
 */
int os_get_abs_filename(char *buf, size_t buflen, const char *filename)
{
    DWORD len;

    /* if the name is empty, use "." */
    if (filename[0] == '\0')
        filename = ".";

    /* check for a relative path on a specific drive (e.g., "c:test") */
    if (isalpha(filename[0]) && filename[1] == ':'
        && filename[2] != '\\' && filename[2] != '/')
    {
        /* 
         *   GetFullPathName doesn't handle relative paths with drive
         *   letters, because Windows doesn't use the DOS scheme of
         *   maintaining a separate working directory per drive.  First,
         *   check to see if we're talking about the working drive; if so,
         *   just remove the drive letter and get the full path as normal,
         *   since that's the drive we'd apply anyway.
         */
        len = GetFullPathName(".", buflen, buf, 0);
        if (len != 0 && len <= buflen
            && toupper(buf[0]) == toupper(filename[0]) && buf[1] == ':')
        {
            /* 
             *   it's on our working drive anyway, so we can just take off
             *   the drive letter and we'll get the right result - e.g.,
             *   "c:test" -> "test" -> "c:\working_dir\test" 
             */
            filename += 2;
        }
        else
        {
            /*
             *   It's on a different drive.  Windows doesn't keep separate
             *   working directories per drive, so we'll simply pretend that
             *   the working directory on a drive that's not the current
             *   drive is the root folder on that drive.  E.g., "k:test" ->
             *   "k:\test".
             */

            /* make sure we have room to add the "\" after the ":" */
            if (buflen < strlen(filename) + 2)
                return FALSE;

            /* rebuild the path as "k:\path" */
            buf[0] = filename[0];
            buf[1] = filename[1];
            buf[2] = '\\';
            strcpy(buf+3, filename+2);

            /* handled */
            return TRUE;
        }
    }

    /* get the full path name */
    len = GetFullPathName(filename, buflen, buf, 0);
    if (len == 0 || len > buflen)
    {
        /* 
         *   failed, or the buffer isn't big enough - copy the filename as-is
         *   and return failure 
         */
        safe_strcpy(buf, buflen, filename);
        return FALSE;
    }

    /* success */
    return TRUE;
}





/* ------------------------------------------------------------------------ */
/*
 *   Compare path strings.  For DOS/Windows, we treat upper and lower case as
 *   interchangeable, and we treat '/' and '\' as interchangeable.
 */
static int patheq(const char *a, const char *b, size_t len)
{
    /* compare character by character */
    for ( ; len != 0 ; ++a, ++b, --len)
    {
        /* convert both characters to lower case */
        char ca = isupper(*a) ? tolower(*a) : *a;
        char cb = isupper(*b) ? tolower(*b) : *b;

        /* convert forward slashes to backslashes */
        if (ca == '/') ca = '\\';
        if (cb == '/') cb = '\\';

        /* if they differ after all of that, we don't have a match */
        if (ca != cb)
            return FALSE;
    }

    /* no mismatches */
    return TRUE;
}

/* ------------------------------------------------------------------------ */
/*
 *   Is the given path a root path?  This returns true for a drive letter
 *   root (C: or C:\) or a UNC root (\\machine\root). 
 */
static int is_root_path(const char *path)
{
    /* note the length */
    size_t len = strlen(path);

    /* check for a drive letter root */
    if (isalpha(path[0])
        && path[1] == ':'
        && (len == 2
            || (len == 3 && (path[2] == '/' || path[2] == '\\'))))
        return TRUE;

    /* check for a UNC root */
    if ((path[0] == '\\' || path[0] == '/')
        && (path[1] == '\\' || path[1] == '/'))
    {
        /* it starts as a UNC path; count path elements */
        int eles;
        const char *p;
        for (p = path + 2, eles = 1 ; *p != '\0' ; ++p)
        {
            /* if it's a non-trailing slash, count a new element */
            if ((*p == '\\' || *p == '/')
                && p[1] != '\0' && p[1] != '\\' && p[1] != '/')
                ++eles;
        }

        /* 
         *   it's a root path if it has one or two elements (\\MACHINE or
         *   \\MACHINE\ROOT) 
         */
        return eles <= 2;
    }

    /* not a root path */
    return FALSE;
}

/* ------------------------------------------------------------------------ */
/*
 *   Is the given file in the given directory?  
 */
int os_is_file_in_dir(const char *filename, const char *path,
                      int allow_subdirs, int match_self)
{
    char filename_buf[OSFNMAX], path_buf[OSFNMAX];
    size_t flen, plen;
    int endsep = FALSE;

    /* absolute-ize the filename, if necessary */
    if (!os_is_file_absolute(filename))
    {
        os_get_abs_filename(filename_buf, sizeof(filename_buf), filename);
        filename = filename_buf;
    }

    /* absolute-ize the path, if necessary */
    if (!os_is_file_absolute(path))
    {
        os_get_abs_filename(path_buf, sizeof(path_buf), path);
        path = path_buf;
    }

    /* 
     *   canonicalize the paths, to remove .. and . elements - this will make
     *   it possible to directly compare the path strings 
     */
    safe_strcpy(filename_buf, sizeof(filename_buf), filename);
    canonicalize_path(filename_buf);
    filename = filename_buf;

    safe_strcpy(path_buf, sizeof(path_buf), path);
    canonicalize_path(path_buf);
    path = path_buf;

    /* get the length of the filename and the length of the path */
    flen = strlen(filename);
    plen = strlen(path);

    /* 
     *   If the path ends in a separator character, ignore that.  Exception:
     *   if the path is a root path, leave the separator intact. 
     */
    if (plen > 0 && (path[plen-1] == '\\' || path[plen-1] == '/')
        && !is_root_path(path))
        --plen;

    /* if the path still ends in a path separator, so note */
    endsep = (plen > 0 && (path[plen-1] == '\\' || path[plen-1] == '/'));

    /* 
     *   if the names match, return true if and only if the caller wants
     *   us to match the directory to itself
     */
    if (flen == plen && memicmp(filename, path, flen) == 0)
        return match_self;

    /* 
     *   Check that the filename has 'path' as its path prefix.  First, check
     *   that the leading substring of the filename matches 'path', ignoring
     *   case.  Note that we need the filename to be at least two characters
     *   longer than the path: it must have a path separator after the path
     *   name, and at least one character for a filename past that.
     *   Exception: if the path already ends in a path separator, we don't
     *   need to add another one, so the filename just needs to be one
     *   character longer.
     */
    if (flen < plen + (endsep ? 1 : 2) || !patheq(filename, path, plen))
        return FALSE;

    /* 
     *   Okay, 'path' is the leading substring of 'filename'; next make sure
     *   that this prefix actually ends at a path separator character in the
     *   filename.  (This is necessary so that we don't confuse "c:\a\b.txt"
     *   as matching "c:\abc\d.txt" - if we only matched the "c:\a" prefix,
     *   we'd miss the fact that the file is actually in directory "c:\abc",
     *   not "c:\a".)  If the path itself ends in a path separator, we've
     *   made this check already simply by the substring match.
     */
    if (!endsep && (filename[plen] != '\\' && filename[plen] != '/'))
        return FALSE;

    /*
     *   We're good on the path prefix - we definitely have a file that's
     *   within the 'path' directory or one of its subdirectories.  If we're
     *   allowed to match on subdirectories, we already have our answer
     *   (true).  If we're not allowed to match subdirectories, we still have
     *   one more check, which is that the rest of the filename is free of
     *   path separator charactres.  If it is, we have a file that's directly
     *   in the 'path' directory; otherwise it's in a subdirectory of 'path'
     *   and thus isn't a match.  
     */
    if (allow_subdirs)
    {
        /* 
         *   filename is in the 'path' directory or one of its
         *   subdirectories, and we're allowed to match on subdirectories, so
         *   we have a match 
         */
        return TRUE;
    }
    else
    {
        const char *p;

        /* 
         *   We're not allowed to match subdirectories, so scan the rest of
         *   the filename for path separators.  If we find any, the file is
         *   in a subdirectory of 'path' rather than directly in 'path'
         *   itself, so it's not a match.  If we don't find any separators,
         *   we have a file directly in 'path', so it's a match. 
         */
        for (p = filename + plen + (endsep ? 1 : 0) ;
             *p != '\0' && *p != '/' && *p != '\\' ; ++p) ;

        /* 
         *   if we reached the end of the string without finding a path
         *   separator character, it's a match 
         */
        return (*p == '\0');
    }
}

/* ------------------------------------------------------------------------ */
/*
 *   Match a string to a DOS device name.  If the reference name contains a
 *   trailing '#', we'll match any digit from 1 to 9 to the '#'.
 */
static int devname_eq(const char *str, const char *ref)
{
    /* compare each character */
    for ( ; *str != '\0' && *ref != '\0' ; ++str, ++ref)
    {
        /* get both characters in lower case */
        char strch = isupper(*str) ? tolower(*str) : *str;
        char refch = isupper(*ref) ? tolower(*ref) : *ref;

        /* if they match exactly, proceed to the next character */
        if (refch == strch)
            continue;

        /* 
         *   if the reference character is a trailing '#', match to any digit
         *   1-9 
         */
        if (refch == '#' && ref[1] == '\0'
            && (strch >= '1' && strch <= '9'))
            continue;

        /* no match */
        return FALSE;
    }

    /* if they didn't run out at the same time, we don't have a match */
    return *str == *ref;
}