/*
 * Copyright 1991 Sony Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Sony not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Sony makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SONY DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SONY
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Masaki Takeuchi, Sony Corporation
 */
/*
 *	message.c
 *	Copyright (c) 1987 1988 1989 Sony Corporation.
 *	Written by Masaki Takeuchi.
 *	Sat May 30 14:47:48 JST 1987
 */
/*
 * $Header: message.S,v 1.1 91/09/04 23:08:29 root Locked $ Sony;
 */

char *mess_warning
	="warning: Please copy user dictionary to your home directory.\n\r";
char *mess_error1
	="error: Can't open user dictionary (%s).\n\r";
char *mess_error2
	="error: Can't open main dictionary (%s).\n\r";

char	*SJrun		= "�r�i�w�@�@";

char	*Mode_buff	= "";
char	*Mode_unbuff	= "";

char	*Mode_Zhira	= "�S�Ђ炪��";
char	*Mode_Zkata	= "�S�J�^�J�i";
char	*Mode_Hkata	= "���J�^�J�i";
char	*Mode_Zalpha	= "�S�A���t�@";
char	*Mode_Halpha	= "���A���t�@";

char	*Gmode
		= "  �o�^ ���� �C�� �@�@ code  ���A �S�A ���J �S�J ����";

char	*GEtc		= "���̑��@�@";
char	*Getc
		= "  �o�^ ���� Help sjrc �C��  �@�@ �@�@ �@�@ �@�@ �@�@";

char	*GHelp		= "Help�@Mode";
char	*MHelp		= "  �K�C�h�E���C�� %s �ɂȂ�܂��� ";

/* guide code input	*/

char	*GCshift	= "ShiftJIS�@";
char	*GCeuc		= "�@�d�t�b�@";
char	*GCjis		= "�@�i�h�r�@";
char	*GCkuten	= "�@�@��_�@";

/* guide toroku		*/

char	*Gtoroku	= "�o�^";
char	*Mhani		= " �͈͂��w�肵�Ă������� ";
char	*Myomi		= " ���� [%s] : �ǂݓ��� ";
char	*TOUROKU_OK	= " ���� [%s] : �ǂ� [%s] (y/n)? ";
char	*TOUROKU_FAIL	= " �o�^�ł��܂���ł��� ";
char	*TOUROKU_SUCC	= " �o�^���܂��� ";

/* guide syoukyo	*/

char	*Gsyoukyo	= "����";
char	*SYOUKYO_FAIL	= " �����ł��܂���ł��� ";
char	*SYOUKYO_SUCC	= " �������܂��� ";

/* get sjrc		*/

char	*SUCCsjrc	= "��ǂݍ��݂܂���";
char	*FAILsjrc	= "��ǂݍ��߂܂���ł���";

/* guide hensyuu	*/

char	*Gedit		= "�C�� ";