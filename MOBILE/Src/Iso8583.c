/*
 * Copyright (c) 2000-2004 CompuService SRL/Uruguay, All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
 * IN NO EVENT SHALL COMPUSERVICE SRL. BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */

/*
 * $Id: iso8583.c,v 1.9 2004/11/09 02:03:56 apr Exp $
 */

#include "SDK30.h"
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "iso8583.h"
#include "UI.h"


int Print_Logs;

const unsigned char EBCDIC2ASCII [256]  = {
	'\x00',	'\x01',	'\x02',	'\x03',	'\xdc',	'\x09',	'\xc3',	'\x7f',
	'\xca',	'\xb2',	'\xd5',	'\x0b',	'\x0c',	'\x0d',	'\x0e',	'\x0f',
	'\x10',	'\x11',	'\x12',	'\x13',	'\xdb',	'\xda',	'\x08',	'\xc1',
	'\x18',	'\x19',	'\xc8',	'\xf2',	'\x1c',	'\x1d',	'\x1e',	'\x1f',
	'\xc4',	'\xb3',	'\xc0',	'\xd9',	'\xbf',	'\x0a',	'\x17',	'\x1b',
	'\xb4',	'\xc2',	'\xc5',	'\xb0',	'\xb1',	'\x05',	'\x06',	'\x07',
	'\xcd',	'\xba',	'\x16',	'\xbc',	'\xbb',	'\xc9',	'\xcc',	'\x04',
	'\xb9',	'\xcb',	'\xce',	'\xdf',	'\x14',	'\x15',	'\xfe',	'\x1a',
	' ',	'\xa0',	'\xe2',	'\xe4',	'\xe0',	'\xe1',	'\xe3',	'\xe5',
	'\xe7',	'\xf1',	'\xa2',	'.',	'<',	'(',	'+',	'|',
	'&',	'\xe9',	'\xea',	'\xeb',	'\xe8',	'\xed',	'\xee',	'\xef',
	'\xec',	'\xdf',	'!',	'$',	'*',	')',	';',	'\xac',
	'-',	'/',	'\xc2',	'\xc4',	'\xc0',	'\xc1',	'\xc3',	'\xc5',
	'\xc7',	'\xd1',	'\xa6',	',',	'%',	'_',	'>',	'?',
	'\xf8',	'\xc9',	'\xca',	'\xcb',	'\xc8',	'\xcd',	'\xce',	'\xcf',
	'\xcc',	'`',	':',	'#',	'@',	'\'',	'=',	'"',
	'\xd8',	'a',	'b',	'c',	'd',	'e',	'f',	'g',
	'h',	'i',	'\xab',	'\xbb',	'\xf0',	'\xfd',	'\xfe',	'\xb1',
	'\xb0',	'j',	'k',	'l',	'm',	'n',	'o',	'p',
	'q',	'r',	'\xaa',	'\xba',	'\xe6',	'\xb8',	'\xc6',	'\xa4',
	'\xb5',	'~',	's',	't',	'u',	'v',	'w',	'x',
	'y',	'z',	'\xa1',	'\xbf',	'\xd0',	'\xdd',	'\xde',	'\xae',
	'^',	'\xa3',	'\xa5',	'\xb7',	'\xa9',	'\xa7',	'\xb6',	'\xbc',
	'\xbd',	'\xbe',	'[',	']',	'\xaf',	'\xa8',	'\xb4',	'\xd7',
	'{',	'A',	'B',	'C',	'D',	'E',	'F',	'G',
	'H',	'I',	'\xad',	'\xf4',	'\xf6',	'\xf2',	'\xf3',	'\xf5',
	'}',	'J',	'K',	'L',	'M',	'N',	'O',	'P',
	'Q',	'R',	'\xb9',	'\xfb',	'\xfc',	'\xf9',	'\xfa',	'\xff',
	'\\',	'\xf7',	'S',	'T',	'U',	'V',	'W',	'X',
	'Y',	'Z',	'\xb2',	'\xd4',	'\xd6',	'\xd2',	'\xd3',	'\xd5',
	'0',	'1',	'2',	'3',	'4',	'5',	'6',	'7',
	'8',	'9',	'\xb3',	'\xdb',	'\xdc',	'\xd9',	'\xda',	'\x1a'
};

const unsigned char ASCII2EBCDIC [256]  = {
	'\x00',	'\x01',	'\x02',	'\x03',	'7',	'-',	'.',	'/',
	'\x16',	'\x05',	'%',	'\x0b',	'\x0c',	'\x0d',	'\x0e',	'\x0f',
	'\x10',	'\x11',	'\x12',	'\x13',	'<',	'=',	'2',	'&',
	'\x18',	'\x19',	'?',	'\'',	'\x1c',	'\x1d',	'\x1e',	'\x1f',
	'@',	'Z',	'\x7f',	'{',	'[',	'l',	'P',	'}',
	'M',	']',	'\\',	'N',	'k',	'`',	'K',	'a',
	'\xf0',	'\xf1',	'\xf2',	'\xf3',	'\xf4',	'\xf5',	'\xf6',	'\xf7',
	'\xf8',	'\xf9',	'z',	'^',	'L',	'~',	'n',	'o',
	'|',	'\xc1',	'\xc2',	'\xc3',	'\xc4',	'\xc5',	'\xc6',	'\xc7',
	'\xc8',	'\xc9',	'\xd1',	'\xd2',	'\xd3',	'\xd4',	'\xd5',	'\xd6',
	'\xd7',	'\xd8',	'\xd9',	'\xe2',	'\xe3',	'\xe4',	'\xe5',	'\xe6',
	'\xe7',	'\xe8',	'\xe9',	'\xba',	'\xe0',	'\xbb',	'\xb0',	'm',
	'y',	'\x81',	'\x82',	'\x83',	'\x84',	'\x85',	'\x86',	'\x87',
	'\x88',	'\x89',	'\x91',	'\x92',	'\x93',	'\x94',	'\x95',	'\x96',
	'\x97',	'\x98',	'\x99',	'\xa2',	'\xa3',	'\xa4',	'\xa5',	'\xa6',
	'\xa7',	'\xa8',	'\xa9',	'\xc0',	'O',	'\xd0',	'\xa1',	'\x07',
	'?',	'?',	'?',	'?',	'?',	'?',	'?',	'?',
	'?',	'?',	'?',	'?',	'?',	'?',	'?',	'?',
	'?',	'?',	'?',	'?',	'?',	'?',	'?',	'?',
	'?',	'?',	'?',	'?',	'?',	'?',	'?',	'?',
	'A',	'\xaa',	'J',	'\xb1',	'\x9f',	'\xb2',	'j',	'\xb5',
	'\xbd',	'\xb4',	'\x9a',	'\x8a',	'_',	'\xca',	'\xaf',	'\xbc',
	'\x90',	'\x8f',	'\xea',	'\xfa',	'\xbe',	'\xa0',	'\xb6',	'\xb3',
	'\x9d',	'\xda',	'\x9b',	'\x8b',	'\xb7',	'\xb8',	'\xb9',	'\xab',
	'd',	'e',	'b',	'f',	'c',	'g',	'\x9e',	'h',
	't',	'q',	'r',	's',	'x',	'u',	'v',	'w',
	'\xac',	'i',	'\xed',	'\xee',	'\xeb',	'\xef',	'\xec',	'\xbf',
	'\x80',	'\xfd',	'\xfe',	'\xfb',	'\xfc',	'\xad',	'\xae',	'Y',
	'D',	'E',	'B',	'F',	'C',	'G',	'\x9c',	'H',
	'T',	'Q',	'R',	'S',	'X',	'U',	'V',	'W',
	'\x8c',	'I',	'\xcd',	'\xce',	'\xcb',	'\xcf',	'\xcc',	'\xe1',
	'p',	'\xdd',	'\xde',	'\xdb',	'\xdc',	'\x8d',	'\x8e',	'\xdf'
};

char *strnncpy (char *d, const char *s, int l) {
    strncpy (d, s, l);
    d[l]='\0';
    return d;
}

char *strpad (char *s, int len) {
    char *d = s;
    while (*s && len--)
        s++;
    while (len-- > 0)
        *(s++) = ' ';
    *s = '\0';
    return d;
}

char *strtrim (char *s) {
    char *s1, *d = s;
    for (s1=NULL; *s; s++) {
        if (*s != ' ')
            s1 = NULL;
        if (*s == ' ' && !s1)
            s1 = s;
    }
    if (s1)
        *s1 = '\0';
    return d;
}

char *strltrim (char *s) {
    char *d = s;
    while (*s == ' ')
        s++;
    if (d != s)
        strcpy (d, s);
    return d;
}

char *strltrim2 (char *d, char *s, int n) {
    while (*s == ' ' && n--)
        s++;
    strnncpy (d, s, n);
    return d;
}

char *zeropad (char *s, int len) {
    int i;
    char *d = s;
    if ( (i = strlen (s)) >= len)
        return NULL;
    while (*s == ' ') 
        *(s++) = '0';
    s = d;
    s[len--] = '\0';
    while (i--)
        if (!isdigit ((s [len--] = s [i])))
            return NULL;
    while (len >= 0)
        s [len--] = '0';
    return d;
}

int str2bcd (unsigned char *d, const char *s, int len, int padLeft)
{
    int i = ((len & 1) && padLeft)? 1 : 0;
    for (memset (d, 0, (len + 1) >> 1); *s && (len-- > 0); i++)
        d [i >> 1] |= (*(s++) - '0') << ((i & 1)? 0 : 4);
    return (i + 1) >> 1;
}

char *bcd2str (char *d, const unsigned char *s, int len, int padLeft)
{
    int left = ((len & 1) && padLeft)? 0 : 1;
    int i = 0;
    for (i = 0; i < len; i++)
        d [i] = (i % 2 == left)? (*(s++) & 0x0F) + '0'
                               : ((*s >> 4) & 0x0F) + '0';
    d [i] = '\0';
    return d;
}

int str_to_bcd (unsigned char *d, const char *s) {
    return str2bcd (d, s, strlen (s), 1);
}

char *bcd_to_str (char *d, const unsigned char *s, int l) {
    return bcd2str (d, s, l, 1);
}

char *str_to_av2 (char *d, const char *s) {
    int l;
    l = strlen (s);
    if (l > 99)
        return NULL;
    d[0] = l/10 + '0'; 
    d[1] = l%10 + '0';
    strcpy (d+2, s);
    return d;
}

unsigned char *str_to_bv2 (unsigned char *d, const char *s) {
    int l;
    l = strlen (s);
    if (l > 99)
        return NULL;
    d[0] = ((l/10) << 4) | l%10;
    strcpy ((char *)d+1, s);
    return d;
}

int str_to_bcd2 (unsigned char *d, const char *s) {
    int l;
    l = strlen (s);
    if (l > 99)
        return -1;
    d[0] = ((l/10) << 4) | l%10;
    return str2bcd (d+1, s, l, 0) + 1;
}

int str_to_bcd2_ebcdic (char *d, const char *s) {
    int l;
    l = strlen (s);
    if (l > 99)
        return -1;
    l = (l+1)/2;
    d[0] = l/10 | 0xF0;
    d[1] = l%10 | 0xF0;

    return str2bcd (d+2, s, l*2, 0);
}

char *str_to_av3 (char *d, const char *s) {
    int l;
    l = strlen (s);
    if (l > 999)
        return NULL;
    d[0] = l/100 + '0';
    d[1] = (l%100)/10 + '0'; 
    d[2] = l%10 + '0';
    strcpy (d+3, s);
    return d;
}

unsigned char *str_to_bv3 (unsigned char *d, const char *s) {
    int l;
    l = strlen (s);
    if (l > 999)
        return NULL;
    d[0] = l/100;
    d[1] = ((l%100)/10 << 4) | l%10;
    strcpy ((char *)d+2, s);
    return d;
}

unsigned char *str_to_bv3_ebcdic (unsigned char *d, const char *s) {
    int l;
    l = strlen (s);
    if (l > 999)
        return NULL;
    d[0] = l/100 | 0xF0;
    d[1] = (l%100)/10 | 0xF0;
    d[2] = l%10 | 0xF0;
    strcpy (d+3, s);
    return d;
}

int str_to_bcd3 (unsigned char *d, const char *s) {
    int l;
    l = strlen (s);
    if (l > 999)
        return -1;
    d[0] = l/100;
    d[1] = ((l%100)/10 << 4) | l%10;
    return str2bcd (d+2, s, l, 0) + 2;
}

char *av2_to_str (char *d, const char *s) {
    int i;
    if (!isdigit (s[0]) || !isdigit (s[1]))
        return NULL;
    i = ((s[0]-'0')*10) + (s[1]-'0');
    return strnncpy (d, s+2, i);
}

char *av3_to_str (char *d, const char *s) {
    int i;
    if (!isdigit (s[0]) || !isdigit (s[1]) || !isdigit (s[2]))
        return NULL;
    i = ((s[0]-'0')*100) + ((s[1]-'0')*10) + (s[2]-'0');
    return strnncpy (d, s+3, i);
}

char *bv2_to_str (char *d, const unsigned char *s) {
    int i;
    if (!isdigit (((s[0]>>4)&0x0F)+'0') || !isdigit ((s[0] & 0x0F)+'0'))
        return NULL;
    i = ((s[0] >> 4) & 0x0F)*10 + (s[0] & 0x0F);
    return strnncpy (d, (const char *)s+1, i);
}

char *bcd2_to_str (char *d, const unsigned char *s) {
    int i;
    if (!isdigit (((s[0]>>4)&0x0F)+'0') || !isdigit ((s[0] & 0x0F)+'0'))
        return NULL;
    i = ((s[0] >> 4) & 0x0F)*10 + (s[0] & 0x0F);
    return bcd2str (d, s+1, i, 0);
}

char *bv3_to_str (char *d, const unsigned char *s) {
    int i;
    if ( s[0] & 0xF0  || !isdigit ((s[0] & 0x0F)+'0')   ||
        !isdigit (((s[1]>>4)&0x0F)+'0') ||
        !isdigit ((s[1] & 0x0F)+'0')) return NULL;
    i = s[0] * 100 + ((s[1] >> 4) & 0x0F)*10 + (s[1] & 0x0F);
    return strnncpy (d, (const char *) s+2, i);
}

char *bcd3_to_str (char *d, const unsigned char *s) {
    int i;
    if ( s[0] & 0xF0  || !isdigit ((s[0] & 0x0F)+'0')   ||
        !isdigit (((s[1]>>4)&0x0F)+'0') ||
        !isdigit ((s[1] & 0x0F)+'0')) return NULL;
    i = s[0] * 100 + ((s[1] >> 4) & 0x0F)*10 + (s[1] & 0x0F);
    return bcd2str(d, s+2, i, 0);
}

unsigned char *map_clear (unsigned char *map, int maxfld) {
    memset (map, 0, maxfld/8);
    return map;
}

unsigned char *map_set (unsigned char *map, int fld) {
    if (fld-- > 64)
        map[0] |= 0x80;
    else if (fld < 0)
        return NULL;
    map [fld/8] |= 0x80 >> (fld%8);
    return map;
}

int map_test (const unsigned char *map, int fld) {
    if (fld-- < 1)
        return 0;
    return map[fld/8] & (0x80 >> (fld%8));
}

unsigned char *map_unset (unsigned char *map, int fld) {
    fld--;
    map [fld/8] &= ~(0x80 >> (fld%8));
    return map;
}

char *bin_to_hex (char *d, const unsigned char *s, int len) {
    int i;
    char *d1;
    d1 = d;
    for (i=0; i < len; i++) {
        *(d++) = tohex ((s[i] >> 4));
        *(d++) = tohex (s[i]);
    }
    *d = '\0';
    return d1;
}

unsigned char *hex_to_bin (unsigned char *d, const char *s, int len) {
    int i, val;
    if (len % 2 || !len)
        return NULL;
    memset (d, 0, len/2);
    for (i=0; i<len; i++) {
        if (s[i] >= '0' && s[i] <= '9')
            val = s[i]-'0';
        else if (s[i] >= 'A' && s[i] <= 'F')
            val = s[i]-'A'+10;
        else if (s[i] >= 'a' && s[i] <= 'f')
            val = s[i]-'a'+10;
        else
            return NULL;

        d[i/2] |= val;
        if (! (i%2))
            d[i/2] <<= 4;
    }
    return d;
}

char *strlwr (char *s) {
    char *d = s;
    for (; *s; s++)
        *s = tolower (*s);
    return d;
}

char *ascii2ebcdic (char *s, int offset, int len) {
    int i;
    char *d = s;
    len += offset;
    for (i=offset; i<len; i++) {
        s[i] = ASCII2EBCDIC[s[i] &0xff];
    }
    return d;
}

char *ebcdic2ascii (char *s, int offset, int len) {
    int i;
    char *d = s;
    len += offset;
    for (i=offset; i<len; i++) {
        s[i] = EBCDIC2ASCII[s[i] & 0xff];
    }
    return d;
}



int atoni (const char *s, int l)
{
    char buf [255];
    return atoi (strnncpy (buf, s, l));
}

void dumpbuf (FILE *fp, const unsigned char *buf, unsigned len) {
    //register int i;
/*
    fprintf (fp, "<dump length=\"%d\"/>\n  ", len);
    for (i=0; i<len; ) {
      fprintf (fp, "%02X ", (unsigned int) (buf[i] & 0xFF));
      if (++i % 16 == 0)
         fputs ("\n  ", fp);
    }
    fprintf (fp, "\n</dump>\n");
    fflush (fp);
*/
    LOG_PRINTF("%s",buf);
}

int all_are_digits (const char *s) {
    while (*s)
        if (!isdigit(*(s++)))
            return 0;
    return 1;
}

/* strtoll, idem a strtol pero se le agrega un largo maximo */
long strtoll (const char *s, char **endp, int base, int len)
{
    char buf[9];
    strnncpy (buf, s, len > 8 ? 8 : len);
    return strtol (buf, endp, base);
}

/* Field packagers/unpackagers */

int IF_CHAR (unsigned char *d, void *s, int len, int pack) {
    strnncpy (d, s, len);
    strpad (d, len);
    return len;
}

int IFE_CHAR (unsigned char *d, void *s, int len, int pack) {
    strnncpy (d, s, len);
    if (pack) {
        strpad (d, len);
        ascii2ebcdic (d, 0, len);
    }
    else {
        ebcdic2ascii (d, 0, len);
        strpad (d, len);
    }
    return len;
}

int IFA_NUMERIC (unsigned char *d, void *s, int len, int pack) {
    strnncpy (d, s, len);
    zeropad (d, len);
    return len;
}

int IFB_NUMERIC (unsigned char *d, void *s, int len, int pack) {
    char buf [80];
    if (pack) {
        strcpy (buf, s);
        zeropad (buf, len);
        return str_to_bcd ((unsigned char *)d, buf);
    } else {
        bcd_to_str ((char *)d, s, len);
        return (len+1)/2;
    }
}

int str2hex(unsigned char *d, const char *s, int len)
{
	int slen = len, dlen=0;

	if (slen<1)
		return 0;
	else
	{
		while (slen>0)
		{
			d[dlen] =((s[dlen*2]-55)>9?(s[dlen*2]-55):(s[dlen*2]-48))*16+((s[dlen*2+1]-55)>9?(s[dlen*2+1]-55):(s[dlen*2+1]-48));
			dlen++;
			slen-=2;
		}
		return dlen;
	}
}

int IFB_BITMAP (unsigned char *d, void *s, int len, int pack) {
    register int i;
    char wmap [16];
    isomsg *m = (isomsg *) s;
    if (pack) {
        map_clear ((unsigned char *)wmap, 128);
        for (i=0; i<=MAX_ISO_FIELD; i++) {
            if (m->fld[i] != (char *) 0) 
                map_set ((unsigned char *)wmap, i);
        }
        len = map_test ((unsigned char *)wmap, 1) ? 16 : 8;
        memcpy (d, wmap, len);
    } else {
        len = map_test ((unsigned char *)s, 1) ? 16 : 8;
        memcpy (d, s, len);
    }
    return len;
}

int IFC_BITMAP (unsigned char *d, void *s, int len, int pack) {
    register int i;
    char wmap [16];
    isomsg *m = (isomsg *) s;
    if (pack) {
        map_clear ((unsigned char *)wmap, 128);
        for (i=0; i<=MAX_ISO_FIELD; i++) {
            if (m->fld[i] != (char *) 0) 
                map_set ((unsigned char *)wmap, i);
        }
        len = map_test ((unsigned char *)wmap, 1) ? 16 : 8;

		for (i=0; i<len; i++)
		{
			sprintf((char *)&d[i*2],"%02X",(unsigned char )wmap[i]);
		}
		len*=2;
    } else {
		len = map_test ((unsigned char *)s, 1) ? 16 : 8;
        memcpy (d, s, len);
        if (Print_Logs==LOGS_ENABLE)
	        //dumpbuf (stdout, d, len);
        	LOG_PRINTF("%s",d);
    }
    return len;
}

int IFA_LLNUM (unsigned char *d, void *s, int len, int pack) {
    char *buf = (char *) s;
    if (pack) {
        str_to_av2 ((char *)d, s);
        len = strlen (s);
    } else {
        len = (buf[0]-'0')*10 + (buf[1]-'0');
        strnncpy ((char *)d, s+2, len);
    }
    return 2 + len;
}

int IFA_LLCHAR (unsigned char *d, void *s, int len, int pack) {
    char *buf = (char *) s;
    if (pack) {
        str_to_av2 (d, s);
        len = strlen (s);
    } else {
        len = (buf[0]-'0')*10 + (buf[1]-'0');
        strnncpy (d, s+2, len);
    }
    return 2 + len;
}

int IFA_LLLCHAR (unsigned char *d, void *s, int len, int pack) {
    char *buf = (char *) s;
    if (pack) {
        str_to_av3 (d, s);
        len = strlen (s);
    } else {
        len = (buf[0]-'0')*100 + (buf[1]-'0')*10 + (buf[2]-'0');
        strnncpy (d, s+3, len);
    }
    return 3 + len;
}

int IFB_LLNUM (unsigned char *d, void *s, int len, int pack) {
    unsigned char *buf = (char *) s;
    if (pack)
        len = str_to_bcd2 ((unsigned char *)d, s);
    else {
        len = ((buf[0] >> 4) & 0x0F)*10 + (buf[0] & 0x0F);
        bcd2str (d, (unsigned char *)buf+1, len, 0);
        len = 1 + ((len+1)/2);
    }
    return len;
}

int IFEB_LLNUM (unsigned char *d, void *s, int len, int pack) {
    unsigned char *buf = (char *) s;
    if (pack) {
        len = str_to_bcd2_ebcdic ((unsigned char *)d, s);
    }
    else {
        len = (buf[0] & 0xf)*10 + (buf[1] & 0xf);
        bcd2str (d, (unsigned char *)buf+2, len*2, 0);
    }
    return 2+len;
}

int IFB_LLCHAR (unsigned char *d, void *s, int len, int pack) {
    unsigned char *buf = (char *) s;
    if (pack) {
        str_to_bv2 ((unsigned char *)d, s);
        len = strlen (d+1);
    } else {
        len = ((buf[0] >> 4) & 0x0F)*10 + (buf[0] & 0x0F);
        strnncpy (d, (unsigned char *)buf+1, len);
    }
    return 1 + len;
}

int IFB_LLLNUM (unsigned char *d, void *s, int len, int pack) {
    unsigned char *buf = (char *) s;
    if (pack)
        len = str_to_bcd3 ((unsigned char *)d, s);
    else {
        len = buf[0]*100 + ((buf[1] >> 4) & 0x0F)*10 + (buf[1] & 0x0F);
        bcd2str (d, (unsigned char *)buf+2, len, 0);
        len = 2 + ((len+1)/2);
    }
    return len;
}

int IFB_LLLCHAR (unsigned char *d, void *s, int len, int pack) {
    unsigned char *buf = (char *) s;
    if (pack) {
        str_to_bv3 ((unsigned char *)d, s);
        len = strlen (s);
    } else {
        len = buf[0]*100 + ((buf[1] >> 4) & 0x0F)*10 + (buf[1] & 0x0F);
        strnncpy (d, (unsigned char *)buf+2, len);
    }
    return 2 + len;
}
int IFB_BINARY (unsigned char *d, void *s, int len, int pack) {
    memcpy (d, s, len);
    return len;
}
int IFB_AMOUNT (unsigned char *d, void *s, int len, int pack) {
    char buf [80];
    memset (buf, 0, sizeof (buf));
    if (pack) {
        * d = *((char *)s);
        s++, d++;
        strcpy (buf, s);
        zeropad (buf, len-1);
        len = str_to_bcd ((unsigned char *)d, buf);
    } else {
        *((char *)d++) = *((char *)s++);
        bcd_to_str ((char *)d, s, --len);
        len = ((len+1)/2);
    }
    return 1 + len;
}

int IFE_LLLCHAR (unsigned char *d, void *s, int len, int pack) {
    unsigned char *buf = (char *) s;
    if (pack) {
        str_to_bv3_ebcdic ((unsigned char *)d, s);
        len = strlen (s);
        ascii2ebcdic (d, 3, len);
    } else {
        len = (buf[0]&0x0F)*100 + (buf[1]&0x0F)*10 + (buf[2]&0x0F);
        strnncpy (d, (unsigned char *)buf+3, len);
        ebcdic2ascii (d, 0, len);
    }
    return 3 + len;
}

void isomsg_init (isomsg *m) {
    register int i;
    for (i=0; i<=MAX_ISO_FIELD; i++)
        m->fld[i] = (char *) 0;
}

void isomsg_dump (FILE *fp, isomsg *m) {
    register int i;
    fprintf (fp, "<isomsg>\n");
    for (i=0; i<=MAX_ISO_FIELD; i++)
    {
    	if (m->fld[i] != (char *) 0)
        {
        	if((i==2)||(i==35)||(i==36)||(i==45))		//PY 26Feb09
        	{
				fprintf (fp, "  <field id=\"%d\" value=\"", i);
        		WriteMaskLog(m->fld[i]);
    			fprintf(fp, "\"/>\n");
        	}
        	else
        		fprintf (fp, "  <field id=\"%d\" value=\"%s\"/>\n", i, m->fld[i]);
        }
    }
    fprintf (fp, "</isomsg>\n");
}

int isomsg_pack (isomsg *m, iso_packager *p, void *packbuf) {
    register int i, offset;

    offset  = (p[0].packager) (packbuf, m->fld[0], p[0].len, PACK);
    offset += (p[1].packager) (packbuf+offset, m, p[0].len, PACK);

    for (i=2; i<=MAX_ISO_FIELD; i++) {
        if (m->fld[i] != (char *) 0) 
            offset += (p[i].packager) 
                (packbuf + offset, m->fld[i], p[i].len, PACK);
    }
    return offset;
}

void isomsg_set (isomsg *m, int i, void *buf, int len) {
    char *b = (char *) malloc (len+1);
    memset (b, 0, len+1);
    memcpy (b, buf, len);
    m->fld[i] = b;
}

void isomsg_free (isomsg *m) {
    register int i;
    for (i=0; i<=MAX_ISO_FIELD; i++) {
        if (m->fld[i] != 0)
            free (m->fld[i]);
    }
}

int isomsg_unpack (isomsg *m, iso_packager *p, void *packbuf, int len) {
    int offset = 0, i;
    unsigned char wmap [16], wbuf [1000], wbuf1 [1000];
    isomsg_init (m);
    map_clear ((unsigned char *)wmap, 128);

    offset  = (p[0].packager) (wbuf, packbuf, p[0].len, UNPACK);
    isomsg_set (m, 0, wbuf, p[0].len);

    offset += (p[1].packager) (wmap, packbuf+offset, p[1].len, UNPACK);
    for (i=2; i<=MAX_ISO_FIELD; i++) 
    {
        if (map_test (wmap, i)) 
        {
            len = (p[i].packager) (wbuf, packbuf+offset, p[i].len, UNPACK);
            isomsg_set (m, i, wbuf, p[i].len);
            offset += len;
	        
	        /************ logging the dump values ***************/   //PY 27Feb09
	        if((i==2)||(i==35)||(i==36)||(i==45))
	        {
		       	sprintf((char *)wbuf1,"%d - ",i);
		       	WriteLogFile((char *)wbuf1);
		       	WriteMaskLog((char *)&wbuf);
		       	WriteLogFile("\n");
	        }
	        else
	        {
	            sprintf((char *)wbuf1,"%d - %s\n",i,(char *)&wbuf);
	            WriteLogFile((char *)wbuf1);
	        }
	        /************ logging the dump values ***************/   //PY 27Feb09
        }
    }
    return offset;
}

int IF_CHAR1 (unsigned char *d, void *s, int len, int pack) {
char length[3];
int len1=0;
	strnncpy ((char *)length, s,2);	
	length[2] ='\0'; 
	len1=atoi(length);
//    strnncpy (d, s, len); nilesh
    strnncpy ((char *)d, s,len1+2); 
//   strpad (d, len); nilesh
//	strpad ((char *)d, len);
//    return strlen(d);
	return len1+2;
}

int WriteLogFile(char *log)
{
	LOG_PRINTF("%s",log);
	return TRUE;
}

int WriteMaskLog(char *log)
{
	LOG_PRINTF("%s",log);
	return TRUE;
}
