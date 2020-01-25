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
 * Author: Noriyuki Hosoe, Sony Corporation
 */

#include <stdio.h>
#include "sj3err.h"

sj3error(fp, code)
FILE	*fp;
int	code;
{
	unsigned char	tmp[BUFSIZ];

	switch (code) {
#define	CASE(X, str)		case (X): printout(fp, str); break;
	CASE(SJ3_InternalError,		"�C���^�[�i���G���[");
	CASE(SJ3_NormalEnd,		"����I��");
	CASE(SJ3_ServerDown,		"�T�[�o�[�������Ă��܂�");
	CASE(SJ3_OpenSocket,		"socket �� open �Ɏ��s���܂���");
	CASE(SJ3_ConnectSocket,		"socket �� connect �Ɏ��s���܂���");
	CASE(SJ3_GetHostByName,		"gethostbyname �Ɏ��s���܂���");
	CASE(SJ3_NotOpened,		"�I�[�v������Ă��܂���");
	CASE(SJ3_NotEnoughMemory,	"������������܂���");
	CASE(SJ3_IllegalCommand,	"�R�}���h����`����Ă��܂���");
	CASE(SJ3_DifferentVersion,	"�o�[�W�������Ⴂ�܂�");
	CASE(SJ3_NoHostName,		"�z�X�g��������܂���");
	CASE(SJ3_NoUserName,		"���[�U��������܂���");
	CASE(SJ3_NotAllowedUser,	"�ڑ���������Ă��܂���");
	CASE(SJ3_AlreadyConnected,	"�ڑ��ςł�");
	CASE(SJ3_NotConnected,		"�ڑ�����Ă��܂���");
	CASE(SJ3_TooLongParameter,	"�p�����[�^���������܂�");
	CASE(SJ3_IllegalParameter,	"�p�����[�^���ُ�ł�");
	CASE(SJ3_BadDictID,		"���̂悤�Ȏ����͂���܂���");
	CASE(SJ3_IllegalDictFile,	"�����t�@�C�����ُ�ł�");
	CASE(SJ3_IllegalStdyFile,	"�w�K�t�@�C�����ُ�ł�");
	CASE(SJ3_IncorrectPasswd,	"�p�X���[�h���قȂ�܂�");
	CASE(SJ3_FileNotExist,		"�t�@�C�������݂��܂���");
	CASE(SJ3_CannotAccessFile,	"�t�@�C���ɃA�N�Z�X�ł��܂���");
	CASE(SJ3_CannotOpenFile,	"�t�@�C�����I�[�v���ł��܂���");
	CASE(SJ3_CannotCreateFile,	"�t�@�C�����쐬�ł��܂���");
	CASE(SJ3_FileReadError,		"���[�h�G���[������܂���");
	CASE(SJ3_FileWriteError,	"���C�g�G���[������܂���");
	CASE(SJ3_FileSeekError,		"�V�[�N�G���[������܂���");
	CASE(SJ3_StdyAlreadyOpened,	"�w�K�t�@�C���̓I�[�v������Ă��܂�");
	CASE(SJ3_StdyFileNotOpened,	"�w�K�t�@�C�����I�[�v������Ă��܂���");
	CASE(SJ3_TooSmallStdyArea,	"���ߒ��w�K�̈悪���������܂�");
	CASE(SJ3_LockedByOther,		"���̃N���C�A���g�Ƀ��b�N����Ă܂�");
	CASE(SJ3_NotLocked,		"���b�N����Ă��܂���");
	CASE(SJ3_NoSuchDict,		"���̂悤�Ȏ����͂���܂���");
	CASE(SJ3_ReadOnlyDict,		"�������݂ł��Ȃ������ł�");
	CASE(SJ3_DictLocked,		"���������b�N����Ă��܂�");
	CASE(SJ3_BadYomiString,		"������Ȃ��ǂݕ�����ł�");
	CASE(SJ3_BadKanjiString,	"������Ȃ�����������ł�");
	CASE(SJ3_BadHinsiCode,		"�i���R�[�h�ُ�ł�");
	CASE(SJ3_AddDictFailed,		"�����o�^�����s���܂���");
	CASE(SJ3_AlreadyExistWord,	"���łɑ��݂��Ă��܂�");
	CASE(SJ3_NoMoreDouonWord,	"����ȏ㓯�����o�^�ł��܂���");
	CASE(SJ3_NoMoreUserDict,	"����ȏ㎫���ɓo�^�ł��܂���");
	CASE(SJ3_NoMoreIndexBlock,	"����ȏ�C���f�b�N�X�ɓo�^�ł��܂���");
	CASE(SJ3_DelDictFailed,		"�����폜�����s���܂���");
	CASE(SJ3_NoSuchWord,		"���̂悤�ȏn��͂���܂���");
	CASE(SJ3_DirAlreadyExist,	"���̃f�B���N�g�������݂��Ă��܂�");
	CASE(SJ3_CannotCreateDir,	"�f�B���N�g�����쐬�ł��܂���");
	CASE(SJ3_NoMoreDictData,	"����ȏ㎫���f�[�^������܂���");
	CASE(SJ3_UserConnected,		"�ڑ����Ă��郆�[�U������܂�");
	CASE(SJ3_TooLongPasswd,		"�p�X���[�h���������܂�");
	CASE(SJ3_TooLongComment,	"�R�����g���������܂�");
#undef	CASE
	default:
		sprintf(tmp, "����`�̃G���[(%d)", code);
		printout(fp, tmp);
		break;
	}
	printout(fp, "\r\n");
}
