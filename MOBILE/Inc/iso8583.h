#ifndef __iso8583_h
#define __iso8583_h	"$Id: iso8583.h,v 1.4 2004/11/09 02:03:56 apr Exp $"

#include <stdarg.h>
#include "SDK30.h"
#define MAX_ISO_FIELD 128
#define LOGS_ENABLE 1
typedef struct {
    char *fld[MAX_ISO_FIELD+1];
} isomsg;

typedef struct {
    int (*packager) (unsigned char *d, void *s, int len, int pack);
    int len;
    char *descrip;
} iso_packager;

void isomsg_init   (isomsg *m);
int  isomsg_pack   (isomsg *m, iso_packager *p, void *packbuf);
int  isomsg_unpack (isomsg *m, iso_packager *p, void *packbuf, int len);
void isomsg_dump   (FILE *fp, isomsg *m);
void isomsg_free   (isomsg *m);
int str2hex(unsigned char *d, const char *s, int len);

#define PACK 1
#define UNPACK 0

/* Field Packagers */
int IF_CHAR     (unsigned char *d, void *s, int len, int pack);
int IFE_CHAR    (unsigned char *d, void *s, int len, int pack);
int IFA_NUMERIC (unsigned char *d, void *s, int len, int pack);
int IFB_NUMERIC (unsigned char *d, void *s, int len, int pack);
int IFB_BITMAP  (unsigned char *d, void *m, int len, int pack);
int IFC_BITMAP  (unsigned char *d, void *m, int len, int pack);
int IFB_LLNUM   (unsigned char *d, void *m, int len, int pack);
int IFEB_LLNUM  (unsigned char *d, void *m, int len, int pack);
int IFB_AMOUNT  (unsigned char *d, void *m, int len, int pack);
int IFB_LLCHAR  (unsigned char *d, void *m, int len, int pack);
int IFA_LLLCHAR (unsigned char *d, void *s, int len, int pack);
int IFB_LLLCHAR (unsigned char *d, void *m, int len, int pack);
int IFE_LLLCHAR (unsigned char *d, void *m, int len, int pack);
int IFB_LLLNUM  (unsigned char *d, void *m, int len, int pack);
int IFB_BINARY  (unsigned char *d, void *m, int len, int pack);
int IF_CHAR1     (unsigned char *d, void *s, int len, int pack);

int IFA_LLNUM   (unsigned char *d, void *m, int len, int pack);
int IFA_LLCHAR  (unsigned char *d, void *m, int len, int pack);

#define tohex(a) (((a)&0x0F)>9?((a)&0x0F)-10+'A':((a)&0x0F)+'0')

char *strnncpy (char *d, const char *s, int l);
char *strpad (char *s, int len);
char *strtrim (char *s);
char *strltrim (char *s);
char *strltrim2 (char *d, char *s, int l);
char *zeropad (char *s, int len);
int str_to_bcd (unsigned char *d, const char *s);
char *bcd_to_str  (char *d, const unsigned char *s, int l);
char *bcd_to_str_padLeft  (char *d, const unsigned char *s, int l);
int str2bcd (unsigned char *d, const char *s, int len, int padLeft);
char *bcd2str (char *d, const unsigned char *s, int len, int padLeft);
char *str_to_av2  (char *d, const char *s);
unsigned char *str_to_bv2  (unsigned char *d, const char *s);
unsigned char *str_to_bv2_ebcdic  (unsigned char *d, const char *s);
char *str_to_av3  (char *d, const char *s);
char *str_to_ebcdic3 (char *d, const char *s);
unsigned char *str_to_bv3  (unsigned char *d, const char *s);
char *av2_to_str  (char *d, const char *s);
char *av3_to_str  (char *d, const char *s);
char *bv2_to_str  (char *d, const unsigned char *s);
char *bv3_to_str  (char *d, const unsigned char *s);
int  str_to_bcd2  (unsigned char *d, const char *s);
int  str_to_bcd2_ebcdic (char *d, const char *s);
int  str_to_bcd3  (unsigned char *d, const char *s);
char *bcd2_to_str (char *d, const unsigned char *s);
char *bcd3_to_str (char *d, const unsigned char *s);
unsigned char *map_clear   (unsigned char *map, int maxfld);
unsigned char *map_set	 (unsigned char *map, int fld);
int map_test	  (const unsigned char *map, int fld);
unsigned char *map_unset   (unsigned char *map, int fld);
char *bin_to_hex  (char *d, const unsigned char *s, int len);
unsigned char *hex_to_bin  (unsigned char *d, const char *s, int len);
char *strlwr (char *s);
int atoni (const char *s, int l);
int all_are_digits (const char *s);
void dumpbuf (FILE *fp, const unsigned char *s, unsigned len);
char *ascii2ebcdic (char *s, int offset, int len);
char *ebcdic2ascii (char *s, int offset, int len);
int WriteLogFile(char *log);
int WriteMaskLog(char *log);
long strtol(const char *__n, char **__end_PTR, int __base);
#endif

