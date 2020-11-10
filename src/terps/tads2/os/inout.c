//#include "osgen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include "run.h"
#include "lib.h"
#include "tio.h"
#include <windows.h>
#include <Wincrypt.h>

/*
 *   os_flush forces output of anything buffered for standard output.  It
 *   is generally used prior to waiting for a key (so the normal flushing
 *   may not occur, as it does when asking for a line of input).  
 */
void os_flush(void)
{
    //fflush( stdout );
}

/* 
 *   update the display - since we're using text mode, there's nothing we
 *   need to do 
 */
void os_update_display(void)
{
}

/* Get a character from the keyboard. For extended characters, return 0,
   then return the extended key at the next call to this function */
int os_getc(void)
{
    return 0;
    //return (oss_getc_from_window(story_win));
}

void os_term(int rc)
{
    //glk_exit();
    //exit flag!
}

/* Display a string in the score area (rightmost) of the status line */
void os_strsc(const char* p)
{
    //if (p == NULL)              /* NULL means simply refresh the status */
    //    oss_draw_status_line();
    //else oss_change_status_right(p);
}

//window clear
void oscls(void)
{
    //glk_window_clear(story_win);
}

/* Set the score. If score == -1, use the last score */
void os_score(int cur, int turncount)
{
    char buf[20];

    if (turncount == -1) {       /* -1 means simply refresh the status */
        //oss_draw_status_line();
    }
    else {
        sprintf(buf, "%d/%d", cur, turncount);
        //oss_change_status_right(buf);
    }
}

/* Get a character from the keyboard, returning low-level, untranslated
   key codes. Since we don't deal with anything but low-level codes,
   this is exactly the same as os_getc. */
int os_getc_raw(void)
{
    return os_getc();
}

/* Wait for a key to be hit */
void os_waitc(void)
{
    os_getc();
}

/*
 *   The default stdio implementation does not support reading a line of
 *   text with timeout.  
 */
int os_gets_timeout(unsigned char *buf, size_t bufl,
                    unsigned long timeout, int resume_editing)
{
    /* tell the caller this operation is not supported */
    return OS_EVT_NOTIMEOUT;
}

/* 
 *   since we don't support os_gets_timeout(), we don't need to do anything
 *   in the cancel routine
 */
void os_gets_cancel(int reset)
{
    /* os_gets_timeout doesn't do anything, so neither do we */
}

/*
 *   Get an event - stdio version.  This version does not accept a timeout
 *   value, and can only get a keystroke.  
 */
int os_get_event(unsigned long timeout, int use_timeout,
                 os_event_info_t *info)
{
    /* if there's a timeout, return an error indicating we don't allow it */
    if (use_timeout)
        return OS_EVT_NOTIMEOUT;

    /* get a key the normal way */
    info->key[0] = os_getc();

    /* if it's an extended key, get the other key */
    if (info->key[0] == 0)
    {
        /* get the extended key code */
        info->key[1] = os_getc();

        /* if it's EOF, return an EOF event rather than a key event */
        if (info->key[1] == CMD_EOF)
            return OS_EVT_EOF;
    }

    /* return the keyboard event */
    return OS_EVT_KEY;
}

/* Call an external function */
int os_excall(int (*extfn)(void*), void* arg) { return 0; }

/* Check for user break */
int os_break(void) { return FALSE; }

/* Get a filename from a startup parameter, if possible. Which it isn't */
int os_paramfile(char* buf) { return FALSE; }

/* Set the terminal into "plain" mode */
void os_plain(void) {}

/* Set the saved game extension. Sha, as if. */
void os_set_save_ext(const char* ext) {}

void os_expause(void)
{
#ifdef USE_EXPAUSE
    os_printz("(Strike any key to exit...)");
    os_flush();
    os_waitc();
#endif /* USE_EXPAUSE */
}

/*
 *   Use the system strlwr() implementation for os_strlwr()
 */
char* os_strlwr(char* s)
{
    return strlwr(s);
}

void os_set_text_attr(int attr)
{
    /* attributes are not supported in non-RUNTIME mode */
}

void os_set_text_color(os_color_t fg, os_color_t bg)
{
    /* colors aren't supported in non-RUNTIME mode - ignore it */
}

void os_set_screen_color(os_color_t color)
{
    /* colors aren't supported in non-RUNTIME mode - ignore it */
}

void os_set_title(const char* title)
{
    /* ignore the information */
}

void os_xlat_html4(unsigned int html4_char,
    char* result, size_t result_buf_len)
{
    // This routine actually translates a Unicode-16 character to an appropriate
    // implementation-defined string.  On BeOS, this just means translating it to
    // UTF-8, which is supported by the sprintf() routine's "wide char" formatting.
    snprintf(result, result_buf_len, "%lc", html4_char);
}

void os_status(int stat)
{
    /* ignore the new status */
}

int os_get_status()
{
    return 0;
}

/*
 *   non-stop mode does nothing in character-mode implementations, since the
 *   portable console layer handles MORE mode
 */
void os_nonstop_mode(int flag)
{
}

void os_gen_charmap_filename(char* filename, char* internal_id, char* argv0)
{
    filename[0] = 0;
}

/* Character map loading */
void os_advise_load_charmap(char* id, char* ldesc, char* sysinfo) {}

int os_vasprintf(char** bufptr, const char* fmt, va_list argp)
{
    /* count the length of the result */
    int len = _vscprintf(fmt, argp);

    /* allocate a buffer */
    if ((*bufptr = (char*)osmalloc(len + 1)) == 0)
        return -1;

    /* format the result */
    return _vsnprintf(*bufptr, len + 1, fmt, argp);
}

/*
 *   Sleep until the specified time.  To avoid locking up our window,
 *   rather than using the system Sleep() API, we'll instead process and
 *   discard events until the delay is finished.  Fortunately, we have the
 *   handy os_get_event() routine to process events with a timeout; we'll
 *   just call this repeatedly, discarding the events, until the timeout
 *   expires.
 */
void os_sleep_ms(long delay_in_milliseconds)
{
    long done_time;

    /* calculate when we'll be done */
    done_time = os_get_sys_clock_ms() + delay_in_milliseconds;

    /* discard events until the timeout expires */
    for (;;)
    {
        long cur_time;
        os_event_info_t info;

        /* get the current time */
        cur_time = GetTickCount();

        /* if we've reached the expiration point, delay no longer */
        if (cur_time >= done_time)
            break;

        /*
         *   Read an event, timing out after what's left of our delay
         *   interval.  If we get an end-of-file event, abort the wait.
         */
        if (os_get_event(done_time - cur_time, TRUE, &info) == OS_EVT_EOF)
            break;
    }
}

int os_get_sysinfo(int code, void* parm, long* result)
{
    switch (code) {
    case SYSINFO_TEXT_HILITE:
        /* we do support text highlighting */
        *result = 1;
        return TRUE;

    case SYSINFO_HTML:
    case SYSINFO_JPEG:
    case SYSINFO_PNG:
    case SYSINFO_WAV:
    case SYSINFO_MIDI:
    case SYSINFO_WAV_MIDI_OVL:
    case SYSINFO_WAV_OVL:
    case SYSINFO_PREF_IMAGES:
    case SYSINFO_PREF_SOUNDS:
    case SYSINFO_PREF_MUSIC:
    case SYSINFO_PREF_LINKS:
    case SYSINFO_MPEG:
    case SYSINFO_MPEG1:
    case SYSINFO_MPEG2:
    case SYSINFO_MPEG3:
    case SYSINFO_LINKS_HTTP:
    case SYSINFO_LINKS_FTP:
    case SYSINFO_LINKS_NEWS:
    case SYSINFO_LINKS_MAILTO:
    case SYSINFO_LINKS_TELNET:
    case SYSINFO_PNG_TRANS:
    case SYSINFO_PNG_ALPHA:
    case SYSINFO_OGG:
    case SYSINFO_BANNERS:
        /* Since we support none of these, set result to 0 */
        *result = 0;
        return TRUE;                              /* We recognized the code */
    case SYSINFO_INTERP_CLASS:
        /* we're a text-only character-mode interpreter */
        /*
         *   $$$ we might want to be more specific: if it's possible to
         *   determine whether we're running on a character-mode or GUI
         *   platform, we should indicate type TEXT or TEXTGUI as
         *   appropriate.  There's no practical difference between these
         *   classes at the moment, though, so it's not very important to
         *   distinguish them.
         */
        *result = SYSINFO_ICLASS_TEXT;
        return TRUE;
    default:
        return FALSE;
    }
}

/* Ask the user for a filename. Return 0 if successful, non-zero otherwise */
int os_askfile(const char* prompt, char* fname_buf, int fname_buf_len,
    int prompt_type, int file_type)
{
    //frefid_t fileref;
    //glui32 glk_prompt_type, glk_file_type;
    //
    //glk_prompt_type = oss_convert_prompt_type(prompt_type);
    //glk_file_type = oss_convert_file_type(file_type) |
    //    fileusage_TextMode;
    //
    //fileref = glk_fileref_create_by_prompt(glk_file_type, glk_prompt_type, 0);
    //if (fileref == NULL)
    //    return OS_AFE_CANCEL;
    //return (oss_convert_fileref_to_string(fileref, fname_buf, fname_buf_len)
    //    == FALSE ? OS_AFE_FAILURE : OS_AFE_SUCCESS);
    //????
    return -1;
}


/*
 *   show the dialog
 */
int os_input_dialog(int icon_id, const char* prompt, int standard_button_set,
    const char** buttons, int button_count,
    int default_index, int cancel_index)
{
    //????
    return -1;
}

/* Show a [MORE] prompt */
void os_more_prompt(void)
{
}