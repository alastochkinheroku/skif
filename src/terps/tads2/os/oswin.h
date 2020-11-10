/* $Header: d:/cvsroot/tads/TADS2/msdos/OSWIN.H,v 1.4 1999/07/11 00:46:37 MJRoberts Exp $ */

/* 
 *   Copyright (c) 1998, 2002 Michael J. Roberts.  All Rights Reserved.
 *   
 *   Please see the accompanying license file, LICENSE.TXT, for information
 *   on using and copying this software.  
 */
/*
Name
  oswin.h - OS definitions for 32-bit Windows (95/98/NT)
Function
  
Notes
  
Modified
  10/17/98 MJRoberts  - Creation
*/

#ifndef OSWIN_H
#define OSWIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <share.h>
#include <fcntl.h>
#include <time.h>
#include <stdarg.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/stat.h>


//заглушки для функций из GLK
/*
** Turn on/off a busy cursor. Under Glk, we don't handle this at all.
*/
#define os_csr_busy(show_as_busy)

/* We don't worry about setting a time zone, so zero this function */
#define os_tzset()

//Вытащил только определенные конструкции из:
/* include general DOS/Windows definitions */
//#include "osdosbas.h"


/* ------------------------------------------------------------------------ */
/*
 *   Define the maintainer for this platform
 */
#define  TADS_OEM_NAME   "Anton Lastochkin <antlas@example.com>"

/* ------------------------------------------------------------------------ */
/*
 *   If TRUE and FALSE aren't defined, define them now
 */
#ifndef TRUE
# define TRUE 1
#endif
#ifndef FALSE
# define FALSE 0
#endif

#if defined(MICROSOFT) || defined(_MSC_VER)
/*
 *   Older Microsoft compilers don't provide <stdint.h> with the ANSI type
 *   definitions, but they do have built-in equivalents of their own.
 */
    typedef __int16 int16_t;
    typedef unsigned __int16 uint16_t;
    typedef __int32 int32_t;
    typedef unsigned __int32 uint32_t;
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;
#endif /* MICROSOFT */

/* ------------------------------------------------------------------------ */
/*
 *   osgen configuration options for this operating system.  These
 *   determine which routines from osgen are included.  Routines from
 *   osgen can be omitted by turning off various of these symbols, if the
 *   OS wants to provide its own more custom definition for the routine.
 */
#define STD_OSCLS
#define STD_OS_HILITE
#define USEDOSCOMMON
#define USE_OVWCHK
#define USE_DOSEXT
#define USE_NULLPAUSE
#define USE_TIMERAND
#define USE_NULLSTYPE
#define USE_PATHSEARCH                  /* use search paths for tads files */
#define USE_STDARG

//#define USE_MORE /* text-only - use formatter-level wrapping and MORE mode */

/* maximum width (in characters) of a line of text */
#define OS_MAXWIDTH  135

# define USE_STDIO
# define USE_NULLSTAT
# define USE_NULLSCORE

/* ------------------------------------------------------------------------ */
/*
 *   DOS-specific interfaces to low-level support routines
 */
void oss_con_init(void);
void oss_con_uninit(void);
void ossgmx(int* max_line, int* max_column);
int ossmon(void);
void oss_win_resize_event(void);

/*
    *   low-level console character reader - DOS-specific interface
    */
char os_getch(void);

/*
    *   Get a character from the keyboard with a timeout.  The timeout is an
    *   interval in milliseconds; for example, a timeout of 1000 specifies
    *   that we should wait no more than one second for a character to be
    *   pressed.
    *
    *   If a key is pressed before the timeout elapses, this should return
    *   OS_GETS_OK and put the character value in *ch. If the timeout elapses
    *   before a key is pressed, this returns OS_GETS_TIMEOUT.  If an error
    *   occurs, this returns OS_GETS_EOF.
    */
int oss_getc_timeout(unsigned char* ch, unsigned long timeout);

    /* status codes for timeout-based input functions */
#define OS_GETS_OK       0
#define OS_GETS_EOF      1
#define OS_GETS_TIMEOUT  2

#define __32BIT__
#define osfar_t

/* if OS_LOADDS isn't defined, define it to nothing */
#ifndef OS_LOADDS
# define OS_LOADDS
#endif

/* ------------------------------------------------------------------------ */
/*
 *   General compiler configuration
 */

 /* void a return value */
#define DISCARD (void)

/* copy a structure - dst and src are structures, not pointers */
#define OSCPYSTRUCT(dst, src) ((dst) = (src))

/* ANSI compiler */
#define OSANSI

/*
 *   For any other case, use the standard library malloc functions
 */
# define osmalloc(siz) malloc(siz)
# define osfree(block) free(block)
# define osrealloc(ptr, siz) realloc(ptr, siz)


 /* main program exit codes */
#define OSEXSUCC 0                                 /* successful completion */
#define OSEXFAIL 1                                        /* error occurred */


/* ------------------------------------------------------------------------ */
/*
 *   File system interface
 */

 /* maximum length of a filename */
#ifdef _MAX_PATH
/* this is the Microsoft macro for the maximum length of a file path */
# define OSFNMAX  _MAX_PATH
#else
/* this is the Borland macro for the maximum length of a file path */
# define OSFNMAX  MAXPATH
#endif

/* normal path separator character */
#define OSPATHCHAR '\\'

/* alternate path separator characters */
#define OSPATHALT "/:"

/*
 *   URL path separators - do not include ":", since we don't want to convert
 *   this to a "/" in a URL
 */
#define OSPATHURL "\\/"

 /* directory separator character for PATH-style environment variables */
#define OSPATHSEP ';'                                        /* ':' on UNIX */

/* current working directory */
#define OSPATHPWD "."


/*
 *   OS file structure type.  All files are manipulated through pointers
 *   to this type.
 */
typedef FILE osfildef;

/*
    *   Directory search handle, for os_open_dir() et al.  The file search
    *   mechanism we use varies by compiler, so we'll define this as an opaque
    *   pointer type; the concrete implementations for the different compilers
    *   use this to point to appropriate structure types.
    */
typedef void* osdirhdl_t;

/*
 *   OS file time structure
 */
struct os_file_time_t
{
    time_t t;
};


    /* get a line of text from a text file (fgets semantics) */
    /* char *osfgets(char *buf, size_t len, osfildef *fp); */
#define osfgets(buf, len, fp) fgets(buf, len, fp)

/* write a line of text to a text file (fputs semantics) */
/* void osfputs(const char *buf, osfildef *fp); */
#define osfputs(buf, fp) fputs(buf, fp)

/* write bytes to file; TRUE ==> error */
/* int osfwb(osfildef *fp, const uchar *buf, int bufl); */
#define osfwb(fp, buf, bufl) (fwrite(buf, bufl, 1, fp) != 1)

/* flush buffers; TRUE ==> error */
/* void osfflush(osfildef *fp); */
#define osfflush(fp) fflush(fp)

/* read bytes from file; TRUE ==> error */
/* int osfrb(osfildef *fp, uchar *buf, int bufl); */
#define osfrb(fp, buf, bufl) (fread(buf, bufl, 1, fp) != 1)

/* read bytes from file and return count; returns # bytes read, 0=error */
/* size_t osfrbc(osfildef *fp, uchar *buf, size_t bufl); */
#define osfrbc(fp, buf, bufl) (fread(buf, 1, bufl, fp))

/* get position in file */
/* long osfpos(osfildef *fp); */
#define osfpos(fp) ftell(fp)

/* seek position in file; TRUE ==> error */
/* int osfseek(osfildef *fp, long pos, int mode); */
#define osfseek(fp, pos, mode) fseek(fp, pos, mode)
#define OSFSK_SET  SEEK_SET
#define OSFSK_CUR  SEEK_CUR
#define OSFSK_END  SEEK_END

/* close a file */
/* void osfcls(osfildef *fp); */
#define osfcls(fp) fclose(fp)

/* delete a file - TRUE if error */
/* int osfdel(const char *fname); */
#define osfdel(fname) remove(fname)

/* rename a file - true on success, false on failure */
/* int os_rename_file(const char *oldname, const char *newname); */
#define os_rename_file(oldname, newname) (rename(oldname, newname) == 0)

/* access a file - 0 if file exists */
/* int osfacc(const char *fname) */
#define osfacc(fname) access(fname, 0)

/* file mode */
int osfmode(const char* fname, int follow_links,
    unsigned long* mode, unsigned long* attrs);

/*
 *   file attributes - we define our own bits for these, since the osifc set
 *   doesn't correspond exactly to the DOS/Windows set (DOS/Win has a
 *   READONLY flag, whereas osifc has separate READ and WRITE flags;
 *   furthermore, WRITE isn't merely the inverse of READONLY, but also
 *   conveys information on ACL permissions on NTFS volumes).
 */
# define OSFATTR_HIDDEN   0x0001
# define OSFATTR_SYSTEM   0x0002
# define OSFATTR_READ     0x0004
# define OSFATTR_WRITE    0x0008

 /* get a character from a file */
 /* int osfgetc(osfildef *fp); */
#define osfgetc(fp) fgetc(fp)

//Стандартный oswin.h

/* ------------------------------------------------------------------------ */
/*
 *   Application instance handle global variable.  The code with the
 *   WinMain() entrypoint function must provide a definition of this
 *   variable, and must initialize it to the application instance handle.
 *   
 *   We implement this as a global variable partly for simplicity, but
 *   mostly because it will make it evident at link time if the WinMain
 *   routine forgets to define this.
 *   
 *   (Note that the definition below is commented out so that we avoid
 *   using the windows.h type HINSTANCE in this portable interface file.
 *   The actual extern is in oswin.c; this comment is here for documentary
 *   purposes only.)  
 */
/* extern HINSTANCE oss_G_hinstance; */


/* ------------------------------------------------------------------------ */
/*
 *   System name and long description 
 */
#define OS_SYSTEM_NAME "WIN32"
#define OS_SYSTEM_LDESC "Windows"


/* ------------------------------------------------------------------------ */
/*
 *   Opening Files.  We use the special share-mode version of fopen so
 *   that we can enforce reasonable file-sharing rules: opening a file for
 *   reading locks out writers; opening a file for writing locks out
 *   readers as well as other writers.
 *   
 *   Other than the sharing modes, we'll use normal stdio routines for our
 *   file interfaces.  
 */

/* newline sequence - DOS/Windows use CR-LF */
#define OS_NEWLINE_SEQ  "\r\n"

/* internal file-open routine */
osfildef *os_fsopen(const char *fname, const char *mode, int omode, int sh);

/* open text file for reading; returns NULL on error */
/* osfildef *osfoprt(const char *fname, os_filetype_t typ); */
#define osfoprt(fname, typ) \
    os_fsopen(fname, "r", _O_RDONLY | _O_TEXT, _SH_DENYWR)

/* open text file for 'volatile' reading; returns NULL on error */
/* osfildef *osfoprtv(const char *fname, os_filetype_t typ); */
#define osfoprtv(fname, typ) \
    os_fsopen(fname, "r", _O_RDONLY | _O_TEXT, _SH_DENYNO)

/* open text file for writing; returns NULL on error */
/* osfildef *osfopwt(const char *fname, os_filetype_t typ); */
#define osfopwt(fname, typ) \
    os_fsopen(fname, "w", \
              _O_CREAT | _O_TRUNC | _O_WRONLY | _O_TEXT, _SH_DENYWR)

/* open text file for reading/writing; don't truncate */
#define osfoprwt(fname, typ) \
    os_fsopen(fname, "r+", _O_CREAT | _O_RDWR | _O_TEXT, _SH_DENYRW)

/* open text file for reading/writing; truncate; returns NULL on error */
/* osfildef *osfoprwtt(const char *fname, os_filetype_t typ); */
#define osfoprwtt(fname, typ) \
    os_fsopen(fname, "w+", \
              _O_CREAT | _O_TRUNC | _O_RDWR | _O_TEXT, _SH_DENYWR)

/* open binary file for writing; returns NULL on error */
/* osfildef *osfopwb(const char *fname, os_filetype_t typ); */
#define osfopwb(fname, typ) \
    os_fsopen(fname, "wb", \
              _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY, _SH_DENYWR)

/* open SOURCE file for reading - use appropriate text/binary mode */
/* osfildef *osfoprs(const char *fname, os_filetype_t typ); */
#define osfoprs(fname, typ) \
    os_fsopen(fname, "rb", _O_RDONLY | _O_BINARY, _SH_DENYWR)

/* open binary file for reading; returns NULL on erorr */
/* osfildef *osfoprb(const char *fname, os_filetype_t typ); */
#define osfoprb(fname, typ) \
    os_fsopen(fname, "rb", _O_RDONLY | _O_BINARY, _SH_DENYWR)

/* open binary file for 'volatile' reading; returns NULL on erorr */
/* osfildef *osfoprbv(const char *fname, os_filetype_t typ); */
#define osfoprbv(fname, typ) \
    os_fsopen(fname, "rb", _O_RDONLY | _O_BINARY, _SH_DENYNO)

/* open binary file for reading/writing; don't truncate */
#define osfoprwb(fname, typ) \
    os_fsopen(fname, "r+b", _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYRW)

/* open binary file for reading/writing; truncate; returns NULL on error */
/* osfildef *osfoprwtb(const char *fname, os_filetype_t typ); */
#define osfoprwtb(fname, typ) \
    os_fsopen(fname, "w+b", \
              _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _SH_DENYWR)


/* ------------------------------------------------------------------------ */
/*
 *   sprintf equivalents with buffer allocation 
 */
int os_asprintf(char **bufptr, const char *fmt, ...);
int os_vasprintf(char **bufptr, const char *fmt, va_list ap);


/* ------------------------------------------------------------------------ */
/*
 *   Set the initial directory for os_askfile dialogs 
 */
void oss_set_open_file_dir(const char *dir);


/* ------------------------------------------------------------------------ */
/*
 *   If error messages are to be included in the executable, define
 *   ERR_LINK_MESSAGES.  Otherwise, they'll be read from an external
 *   file that is to be opened with oserrop().
 */
/* #define ERR_LINK_MESSAGES */


/* ------------------------------------------------------------------------ */
/* 
 *   Update progress display with current info, if appropriate.  This can
 *   be used to provide a status display during compilation.  Most
 *   command-line implementations will just ignore this notification; this
 *   can be used for GUI compiler implementations to provide regular
 *   display updates during compilation to show the progress so far.  
 */
#define os_progress(fname, linenum)


/* ------------------------------------------------------------------------ */
/*
 *   Single/double quote matching macros.  Used to allow systems with
 *   extended character codes with weird quote characters (such as Mac) to
 *   match the weird characters. 
 */
#define os_squote(c) ((c) == '\'')
#define os_dquote(c) ((c) == '"')
#define os_qmatch(a, b) ((a) == (b))

/* ------------------------------------------------------------------------ */
/* 
 *   theoretical maximum osmalloc size is all of memory 
 */
#define OSMALMAX 0xffffffffL


/* ------------------------------------------------------------------------ */
/* 
 *   usage lines for 32-bit command-line tools 
 */
# define OS_TC_USAGE   "usage: tc32 [options] file"
# define OS_TR_USAGE   "usage: t2r32 [options] file"
# define OS_TDB_USAGE  "usage: tdb32 [options] file"

/* add the special console-mode options messages, if appropriate */
# ifndef HTMLTADS
#  define ERR_TRUS_OS_FIRST    ERR_TRUS_DOS32_1
#  define ERR_TRUS_OS_LAST     ERR_TRUS_DOS32_L
# endif


/* ------------------------------------------------------------------------ */
/*
 *   Make default buffer sizes huge, since we can be fairly liberal with
 *   memory on win95/nt 
 */
#include "osbigmem.h"

#ifdef __cplusplus
}
#endif

#endif /* OSWIN_H */

