#ifdef RCSID
static char RCSid[] =
"$Header: d:/cvsroot/tads/TADS2/TADSRSC.C,v 1.2 1999/05/17 02:52:13 MJRoberts Exp $";
#endif

/* 
 *   Copyright (c) 1992, 1998 by Michael J. Roberts.  All Rights Reserved.
 *   
 *   Please see the accompanying license file, LICENSE.TXT, for information
 *   on using and copying this software.  
 */
/*
Name
  tadsrsc.c - TADS v2 resource manager
Function
  resource manager for TADS v2
Notes
  Somewhat improved over TADS v1 resource manager thanks to new file
  format.  This version can list, add, replace, and remove resources.
Modified
  04/30/92 MJRoberts     - creation
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "std.h"
#include "fio.h"
#include "resmap.h"

struct TadsResourseMap resmap;

void resmap_init()
{
	resmap.total_res = 0;
	resmap.file_memory = 0;
}

/* printf */
#define rscptf printf

static void errexit(char *msg, long arg)
{
    rscptf(msg, arg);
    rscptf("\n");
    os_term(OSEXFAIL);
}


static void listexit(osfildef *fp, char *msg)
{
    if (fp != 0)
        osfcls(fp);
    errexit(msg, 1);
}

typedef struct opdef opdef;
struct opdef
{
    opdef  *opnxt;                                /* next operation in list */
    char   *opres;                                         /* resource name */
    char   *opfile;                                          /* file to add */
    int     opflag;               /* indication of what to do with resource */
    int     oprestype;                                     /* resource type */
#   define  OPFADD   0x01                                   /* add resource */
#   define  OPFDEL   0x02                                /* delete resource */
#   define  OPFDONE  0x04                  /* this entry has been processed */
#   define  OPFADDDIR 0x08                       /* add an entire directory */
};

/* resource types */
#define RESTYPE_DFLT  0x00     /* default - infer type from filename suffix */
#define RESTYPE_XFCN  0x01
#define RESTYPE_HTML  0x02

osfildef *global_fpin=0;


/* copy a block of bytes from the input file to the output file */
char copybuf[16 * 1024];
static void copybytes(osfildef *fpin, osfildef *fpout, ulong siz)
{
    uint  cursiz;

    /* copy bytes until we run out */
    while (siz != 0)
    {
        /* we can copy up to one full buffer at a time */
        cursiz = (siz > sizeof(copybuf) ? sizeof(copybuf) : siz);

        /* deduct the amount we're copying from the total */
        siz -= cursiz;

        /* read from input, copy to output */
        if (osfrb(fpin, copybuf, cursiz)
            || osfwb(fpout, copybuf, cursiz))
        {
            /* error - close files and display an error */
            osfcls(fpin);
            osfcls(fpout);
            errexit("error copying resource", 1);
        }
    }
}

/* display a list item */
static void show_list_item(int *showed_heading, char *type_name, ulong offs,  ulong res_size, char *res_name, size_t name_len)
{
    /* if we haven't displayed a header yet, do so now */
    if (!*showed_heading)
    {
        /* show the header */
        rscptf("Type       Offs      Size       Name\n"
               "----------------------------------------------------------\n");
        
        /* note that we've displayed the header now */
        *showed_heading = TRUE;
    }

    /* show this item */
    rscptf("%.4s  %10lu   %8lu   %.*s\n", type_name, offs, res_size,
           (int)name_len, res_name);
}

/* HTMLRES index entry */
struct idx_t
{
    /* source of the resource */
    opdef  *src_op;
    struct idx_t *src_idx;

    /* information on the resource in the file */
    ulong   ofs;
    ulong   siz;
    ushort  namlen;
    struct idx_t *nxt;
    char    nam[1];
};

/*
 *   Allocate an index entry 
 */
static struct idx_t *alloc_idx_entry(ulong ofs, ulong siz,
                                     char *nam, ushort namlen,
                                     opdef *src_op, struct idx_t *src_idx)
{
    struct idx_t *entry;

    /* allocate a new entry */
    entry = (struct idx_t *)malloc(sizeof(struct idx_t) + namlen - 1);

    /* fill it in */
    entry->src_op = src_op;
    entry->src_idx = src_idx;
    entry->ofs = ofs;
    entry->siz = siz;
    entry->namlen = namlen;
    memcpy(entry->nam, nam, namlen);
	
    /* return the new entry */
    return entry;
}

/*
 *   Process an HTML resource list.  If 'old_htmlres' is true, it
 *   indicates that the input file is pointing to an old resource map;
 *   otherwise, we need to construct a brand new one.  
 */
static opdef *prochtmlres(osfildef *fp,
                          int *copyrsc, int *showed_heading, int old_htmlres)
{
    opdef *add_list = 0;
    char   buf[512];
	char   tmp_name[512];
    ulong  in_entry_cnt;
    ulong  in_table_siz;
    ulong  i;
	int    j;
    struct idx_t **in_list = 0, *out_list = 0, **p;
    ulong  in_res_base;
	osfildef *fpout_res;

    /* we'll be handling the resource - tell the caller not to copy it */
    *copyrsc = FALSE;

    /* if there's an old HTMLRES resource, read it */
    if (old_htmlres)
    {
        /* read the index entry count and size */
        if (osfrb(fp, buf, 8))
            listexit(fp, "unable to read HTMLRES header");
        in_entry_cnt = osrp4(buf);
        in_table_siz = osrp4(buf + 4);

        /* allocate space for pointers to all of the entries */
        if (in_entry_cnt != 0)
        {
            in_list = (struct idx_t **)
                      malloc(in_entry_cnt * sizeof(struct idx_t *));
            if (in_list == 0)
                listexit(fp, "unable to allocate space for HTMLRES entries");
        }

        /* read the index table entries */
        for (i = 0, p = in_list ; i < in_entry_cnt ; ++i, ++p)
        {
            ushort name_siz;
            ulong  res_siz;
            ulong  res_ofs;
            
            /* read the entry information */
            if (osfrb(fp, buf, 10))
                listexit(fp,
                         "unable to read HTMLRES index table entry (prefix)");
            
            /* get the resource size */
            res_ofs = osrp4(buf);
            res_siz = osrp4(buf + 4);
            
            /* read the name */
            name_siz = osrp2(buf + 8);
            if (name_siz > sizeof(buf))
                listexit(fp, "name too large in HTMLRES index table entry");
            if (osfrb(fp, buf, name_siz))
                listexit(fp,
                         "unable to read HTMLRES index table entry (name)");
            
            /* build this entry */
            *p = alloc_idx_entry(res_ofs, res_siz, buf, name_siz, 0, 0);
        }
		
		//HERE FIX
		/*
         *   The resources start at the end of the index table - note the
         *   location of the end of the input table, since it's the base
         *   address relative to which the resource offsets are stated.  
         */
        in_res_base = osfpos(fp);
		//END FIX

        /* if we don't have an output file, list the HTMLRES contents */
		/* display all of the entries */
		for (i = 0, p = in_list ; i < in_entry_cnt ; ++i, ++p)
		{
			//Отобразить карту ресурсов, для теста
			//show_list_item(showed_heading, "HTML",
			//			   in_res_base + (*p)->ofs, (*p)->siz,  (*p)->nam, (*p)->namlen);
			//Заполнение карты ресурсов
			strcpy(resmap.res[resmap.total_res].filename,"");
			strncpy(resmap.res[resmap.total_res].filename,(*p)->nam,(*p)->namlen);
			resmap.res[resmap.total_res].offset = in_res_base + (*p)->ofs;
			resmap.res[resmap.total_res].sz = (*p)->siz;
			resmap.total_res++;
			/*
			//Копирование
			strcpy(tmp_name,"res/");
			strncat(tmp_name,(*p)->nam,(*p)->namlen);
			if ((*p)->namlen>0)
			{
				//заменить относительные пути
				for (j=4;j<((*p)->namlen+4);j++){
					if (tmp_name[j]=='\\' || tmp_name[j]=='/') tmp_name[j]='_';
				}
				
				if ((fpout_res = osfopwb(tmp_name, OSFTBIN)) == 0)
					errexit("unable to create res file", 1);
				else {
					//rscptf("alloc index nam=%s, ofs=%d, sz=%d\n", tmp_name,in_res_base + (*p)->ofs, (*p)->siz);
					//Тут и делать копирование в папку res
					osfseek(fp, in_res_base + (*p)->ofs, OSFSK_SET);
					copybytes(fp, fpout_res, (*p)->siz);
					osfcls(fpout_res);
				}
			}
			*/
		}
		
		/* there's no more processing to do */
		goto done;
		//END FIX
	}

done:
    /* if we have an input list, free it */
    if (in_list != 0)
    {
        /* delete all of the entries in the input table */
        for (i = 0, p = in_list ; i < in_entry_cnt ; ++i, ++p)
        {
            /* delete this entry if we haven't already done so */
            if (*p != 0)
                free(*p);
        }

        /* delete the input pointer list itself */
        free(in_list);
    }

    /* return the op list in its current form */
    return 0;
}


/* process changes to resource file (or just list it) */
static opdef *rscproc(osfildef *fp)
{
    char   buf[128];
    ulong  siz;
    char   datebuf[27];
    ulong  endpos;
    uchar  nambuf[40];
    int    copyrsc;
    ulong  startpos;
    uint   endpos_ofs;
    ulong  first_xfcn = 0;
    ulong  extcnt_pos = 0;
    int    found_user_rsc = FALSE;
    int    showed_heading = FALSE;
    int    found_htmlres = FALSE;
    char  *file_type;

	/* 
	 *   the input file exists -- check file and version headers, and
	 *   get flags and timestamp 
	 */
	if (osfrb(fp, buf, (int)(sizeof(FIOFILHDR) + sizeof(FIOVSNHDR) + 2)))
		listexit(fp, "error reading file header");

	/* check the file type */
	if (memcmp(buf, FIOFILHDR, sizeof(FIOFILHDR)) == 0)
		file_type = "game";
	else if (memcmp(buf, FIOFILHDRRSC, sizeof(FIOFILHDRRSC)) == 0)
		file_type = "resource";
	else
		listexit(fp, "invalid resource file header");

	/* check the version header */
	if (memcmp(buf + sizeof(FIOFILHDR), FIOVSNHDR, sizeof(FIOVSNHDR))
		&& memcmp(buf + sizeof(FIOFILHDR), FIOVSNHDR2, sizeof(FIOVSNHDR2))
		&& memcmp(buf + sizeof(FIOFILHDR), FIOVSNHDR3, sizeof(FIOVSNHDR3)))
		listexit(fp, "incompatible resource file version");

	/* get the timestamp */
	if (osfrb(fp, datebuf, (size_t)26))
		listexit(fp, "error reading file");
	datebuf[26] = '\0';
    

    /* if listing, show file creation timestamp */
    //    rscptf("\n"
    //          "File type:     TADS %s file\n"
    //           "Date compiled: %s\n", file_type, datebuf);

    /*
     *   Process the input file, if there is one 
     */
    for ( ; fp != 0 ; )
    {
        /* assume this resource will be copied to the output */
        copyrsc = TRUE;

        startpos = osfpos(fp);
        if (osfrb(fp, buf, 1)
            || osfrb(fp, buf + 1, (int)(buf[0] + 4)))
            listexit(fp, "error reading file");

        memcpy(nambuf, buf + 1, (size_t)buf[0]);
        nambuf[buf[0]] = '\0';
        
        endpos_ofs = 1 + buf[0];
        endpos = osrp4(buf + endpos_ofs);
        siz = endpos - startpos;
        
        /* see what kind of resource we have, and do the right thing */
        if (!strcmp((char *)nambuf, "$EOF"))
        {
            /* end of file marker - quit here */
            break;
        }
        else if (!strcmp((char *)nambuf, "HTMLRES"))
        {
            /* if we've already found an HTMLRES list, it's an error */
            if (found_htmlres)
            {
                rscptf("error: multiple HTMLRES maps found in file\n"
                       " -- redundant entries have been deleted.\n");
                copyrsc = FALSE;
            }
            else
            {
                /* go process it */
                prochtmlres(fp, &copyrsc,
                                     &showed_heading, TRUE);
            }

            /* note that we've found a resource */
            found_user_rsc = TRUE;
            found_htmlres = TRUE;
        }

        /* skip to the next resource */
        osfseek(fp, endpos, OSFSK_SET);
    }


    /* if just listing, and we didn't find anything, tell the user so */
    if (!found_user_rsc)
        rscptf("No user resources found.\n");

    /* 
     *   return the final oplist (it may have been changed during
     *   processing) 
     */
    return 0;
}

/*
 *   Operation parsing context.  Certain options affect subsquent
 *   operations; we use this structure to keep track of the current
 *   settings from past arguments.  
 */
typedef struct
{
    /*
     *   Type to use for resources.  If this is RESTYPE_DFLT, it means
     *   that we need to infer the type from the filename suffix.  
     */
    int restype;

    /* 
     *   operations to perform - a combination of OPFADD and OPFDEL 
     */
    int flag;

    /* 
     *   context flag: if true, it means that we just parsed a -type
     *   option, so the next argument is the type name 
     */
    int doing_type;
} opctxdef;

/*
 *   Get the resource type of a file.  If an explicit resource type is
 *   specified in the resource type argument, we'll use that; otherwise
 *   (i.e., the resource type argument is RESTYPE_DFLT), we'll look at the
 *   filename suffix to determine the type.  
 */
static int get_file_restype(int restype, char *fname)
{
    /* see if we have a specified type */
    if (restype == RESTYPE_DFLT)
    {
        char *p;
        char *lastdot;

        /* 
         *   No specified type - infer type from the filename.  If the
         *   filename ends in .bin, .com, or has no '.', we'll assume it's
         *   an XFCN; otherwise, it's an HTML resource.  
         */
        for (p = fname, lastdot = 0 ; *p ; ++p)
        {
            /* if we're at a dot, note it, but keep looking */
            if (*p == '.')
                lastdot = p;
        }

        /* check our suffix */
        if (lastdot == 0
            || !stricmp(lastdot, ".bin") || !stricmp(lastdot, ".com"))
            return RESTYPE_XFCN;
        else
            return RESTYPE_HTML;
    }
    else
    {
        /* 
         *   an explicit resource type has been specified - use it without
         *   regard to the filename 
         */
        return restype;
    }
}


/* get a resource type given a resource name */
static int parse_res_type(char *p)
{
    /* see what we have */
    if (!stricmp(p, "xfcn"))
        return RESTYPE_XFCN;
    else if (!stricmp(p, "html"))
        return RESTYPE_HTML;
    else
    {
        rscptf("invalid resource type specified: %s", p);
        errexit("", 1);
        return 0;
    }
}

void resmap_create(osfildef *fpin, unsigned long start_pos)
{
	long fsize=0;

	resmap_cleanup();
	resmap_init();
	
	fseek(fpin, 0, SEEK_END);
	fsize = ftell(fpin);
	fseek(fpin, start_pos, SEEK_SET);  //same as rewind(f);

	resmap.file_memory = malloc(fsize + 1);
	fread(resmap.file_memory, fsize, 1, fpin);
	fseek(fpin, start_pos, SEEK_SET);  //same as rewind(f);

	resmap.file_memory[fsize] = 0;

	rscproc(fpin);
}

 void resmap_cleanup()
 {
	 if (resmap.file_memory) {
		 free(resmap.file_memory);
		 resmap.file_memory=0;
	 }
 }

/*
int main(int argc, char **argv)
{
    osfildef *fpin;
    char      inbuf[OSFNMAX + 1];
    char     *infile;
    opdef    *oplist = (opdef *)0;
    opctxdef  opctx;
    int       do_create = FALSE;

    rscptf("TADS Resource Unpacker by alastochkin\n");
    rscptf("using: tadssrsc gamfile.gam");
    
    opctx.restype = RESTYPE_DFLT;
    opctx.flag = OPFADD | OPFDEL;
    opctx.doing_type = FALSE;
	
	if (argc != 2) errexit("invalid arguments number", 1);
    
    infile = argv[1];
    strcpy(inbuf, infile);
    os_defext(inbuf, "gam");

    if ((fpin = osfoprb(inbuf, OSFTGAME)) == 0)
    {
        errexit("unable to open resource file", 1);
    }
    if (fpin != 0)
    {
		global_fpin = fpin;
        rscproc(fpin);
        osfcls(fpin);
        os_term(OSEXSUCC);
    }
    return OSEXSUCC;
}
*/

