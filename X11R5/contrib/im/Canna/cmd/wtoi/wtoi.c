/* Copyright 1991 NEC Corporation, Tokyo, Japan.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of NEC Corporation
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.  NEC 
 * Corporation makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * NEC CORPORATION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN 
 * NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE. 
 *
 * Author : Osamu Hata, NEC Corporation  (hata@d1.bs2.mt.nec.co.jp)
 *
 */

#ifndef lint
static char rcsid[]="@(#) 102.1 $Header: /work/nk.R3.1/cmd/wtoi/RCS/wtoi.c,v 1.10 91/09/17 18:56:42 hata Exp $";
#endif
/* wtoi.c  �ƥ����ȷ����μ����ף���ؤ���ʡ٤Τ�Τ��Ѵ����롣
 *	wtoi [-f hinshifile] [wnndic] [irohadic]
 */
#include	<stdio.h>
#include        <ctype.h>
#define		MAXTANGO	256
#define         MAXHINSHI       100

typedef	unsigned char	uchar;

struct hin{
  char *wnn;
  char *iroha;
};

static struct hin table[] = {
  {"̾��","#T35"},{"��̾","#JN"},{"��̾","#CN"},{"��̾&��̾","#JN"},
  {"��ͭ̾��","#KK"},{"����","#NN"},
  {"��ư��","#KS"},
  {"���Ը���","#K5"},{"���Ը���","#G5"},{"���Ը���","#S5"},
  {"���Ը���","#T5"},{"�ʹԸ���","#N5"},{"�йԸ���","#B5"},
  {"�޹Ը���","#M5"},{"��Ը���","#R5"},{"��Ը���","#W5"},
  {"����","#KS"},{"����&̾��","#KSr"},{"����(�Ԥ�)","#C5r"},
  {"����(����)","#SX"},{"����(����)","#ZX"},{"����(����)&̾��","#T30"},
  {"��(��)","#kxo"},{"��(��)","#kxi"},{"��(��)","#kxuru"},
  {"��(��)","#sxi"},{"��(��)","#sxuru"},{"��(��)","#sxe"},{"���(������)","#L5"},
  {"���ƻ�","#KY"},
  {"����ư��","#T05"},{"����ư��&̾��","#T05"},{"����ư��(����)","#F00"},
  {"����","#F14"},{"Ϣ�λ�","#RT"},{"��³��,��ư��","#CJ"},
  {"ñ����","#KJ"},
  {"��Ƭ��","#PRE"},{"������","#SUC"},
  {"��Ƭ����","#NNPRE"},{"������","#JS"},{"��Ƭ������","#JS"},
  {"����������","#JSSUC"},
  {"������̾","#JNSUC"},{"��Ƭ��̾","#CNPRE"},{"������̾","#CNSUC1"},
  {"����ư�첽������","#N2T17"},{"����(����)&̾�첽������","#N2T30"},
  {"����ư��","#D2T35"},{"���ƻ첽����ư��","#D2KY"},
  {"��Ƭ��(��)","#PRE"},{"��Ƭ��(��)","#PRE"},{"����","#T35"}
};

char *salloc(s)
     char *s;
{
  char *new;
  
  if (new = (char *)malloc(strlen( s ) + 1))
    strcpy(new, s);
  else{
    fprintf(stderr, "No more memory\n");
    exit(1);
  }
  return(new);
}

/* �ʻ��б� */
char *chghinshi( hinshi, taiou, fshurui )
     char   *hinshi;
     struct hin   *taiou;
     int    fshurui;  
{
  int   shurui;
  int   i;
  
  shurui = sizeof(table)/sizeof(struct hin);
  for( i = 0 ; i < shurui ; i++) {
    if( !strcmp( hinshi , table[i].wnn ) )
      return( table[i].iroha );
  }
  for( i = 0 ; i <  fshurui ; i++) {
    if( !strcmp( hinshi , taiou[i].wnn ) ) {
      return( taiou[i].iroha );
    }
  }
  return( "#??" );
}

/* �ʻ�ե������ɤ߹��� */
int read_hinshi( fp, taiou )
     FILE    *fp;
     struct  hin  *taiou;
{
  int     shurui;
  char    H[MAXTANGO];
  char    wnn[MAXTANGO],iroha[MAXTANGO];
  
  shurui = 0;
  while( fgets( H, MAXTANGO, fp ) ){
    if( 2 != sscanf( H, "%s %s", wnn, iroha ) )
      continue;
    else {
      taiou[shurui].wnn = salloc(wnn);
      taiou[shurui].iroha = salloc(iroha);
      shurui++;
    }
  }
  return ( shurui );
} 

/* ���� */
wtoi_write( fp, yomi, hinshi, kouho, hindo )
     FILE	*fp;
     uchar   *yomi, *hinshi, *kouho;
     int	hindo;
{
  if( !strcmp(hinshi,"#kxuru") || !strcmp(hinshi,"#sxuru")){
    strcat(yomi,"��");
    strcat(kouho,"��");
  }
  if( hindo == 0 )
    fprintf( fp, "%s %s %s \n", yomi, hinshi, kouho );
  else
    fprintf( fp, "%s %s*%d %s \n", yomi, hinshi, hindo, kouho );
}

int suuji(kazu)
     char *kazu;
{
  int i;

  for(i = 0; i < strlen(kazu); i++ ){
    if(!isdigit(kazu[i]))
      return(0);
  }
  return(atoi(kazu));
}

main(argc,argv)
     int  argc;
     char *argv[]; 
{
  struct hin taiou[MAXHINSHI];
  uchar	S[MAXTANGO],y[MAXTANGO], h[MAXTANGO], k[MAXTANGO],nd[10];
  int	d,option,fshurui;
  FILE	*fph,*fpi,*fpo;
  
  option = 0;
  if( argc <= 5 ) { /* �������������� */ 
    if( argc > 2 && !strcmp(argv[1],"-f") ) { /* �ʻ�ե�������ɤ߹��फ�� */
      if( (fph = fopen( argv[2], "r" ) ) == NULL) { /* �ʻ�ե���������� */
	fprintf(stderr,"%s: cannot open %s\n", argv[0], argv[2] );
	exit(2);
      }
      fshurui = read_hinshi( fph, taiou );
      close( fph );
      option = 1;
    }
    fpi = stdin;
    fpo = stdout;
  }
  else { /* ���������� */
    fprintf(stderr,"Usage: wtoi [-f hinshitaiou] [wnndic] [irohadic]\n",argv[0]);
    exit(2);
  }
  if( argc >= (2 + option*2) ) { /* �ף���򥪡��ץ� */
    if( (fpi = fopen( argv[1 + option*2], "r" ) ) == NULL) {
      fprintf(stderr,"%s: cannot open %s\n", argv[0], argv[1 + option*2] );
      exit(2);
    }
    if( argc == (3 + option*2) ) { /* ����ʼ���򥪡��ץ� */
      if( (fpo = fopen( argv[2 + option*2], "w" ) ) == NULL) {
	fprintf(stderr,"%s: cannot create %s\n", argv[0], argv[2 + option*2] );
	exit(2);
      }
    }
  }
  
  /* ����� */
  while( fgets( (char *)S, sizeof(S), fpi ) ) {
    if( 4 >  sscanf( (char *)S, "%s %s %s %s", y, k, h, nd ) )
      continue;
    else      
      d = suuji(nd);
      wtoi_write( fpo, y, chghinshi( h, taiou, fshurui ), k, d );
  } 
  
  fclose( fpi );
  fclose( fpo );
  exit( 0 );
}