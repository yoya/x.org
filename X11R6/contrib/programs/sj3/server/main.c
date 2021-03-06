/*
 * Copyright (c) 1991-1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: main.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:53 $
 */





#include "sj_sysvdef.h"
#include <locale.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#ifdef SVR4
#include <sys/fcntl.h>
#include <sys/termios.h>
#endif
#include "Const.h"

#ifndef lint
static char rcsid_sony[] = "$Header: /export/work/contrib/sj3/server/RCS/main.c,v 1.12 1994/06/03 07:41:30 notanaka Exp $ SONY;";
#endif

#ifdef SVR4
#define signal sigset
#endif

void	_exit();

extern	int	fork_flag;
extern	char	*lock_file;



static	void	opening()
{
	extern	char	*version_number;
	extern	char	*time_stamp;

	printf("Kana-Kanji Conversion Server Version %s\r\n", version_number);
	printf("Copyright (c) 1990-1994 Sony Corporation\r\n");
	printf("Created at %s\r\n", time_stamp);
}

#ifdef	LOCK_FILE


int	make_lockfile()
{
	int	fd;

	fd = open(lock_file, O_CREAT | O_EXCL, 0600);
	if (fd < 0) return ERROR;
	close(fd);
	return 0;
}
int	erase_lockfile()
{
	return unlink(lock_file);
}
#endif




#if !defined(__sony_news) || defined(SVR4)
static int signal_handler(sig)
int	sig;
{
	warning_out("signal %d catched.", sig);
}

static	int	terminate_handler(sig)
int	sig;
{
	close_socket();
	sj_closeall();
#ifdef	LOCK_FILE
	erase_lockfile();
#endif
	exit(0);
}
#else
static int signal_handler(sig, code, scp)
int	sig;
int	code;
struct sigcontext *scp;
{
	warning_out("signal %d, code %d catched.", sig, code);
}

static	int	terminate_handler(sig, code, scp)
int	sig;
int	code;
struct sigcontext *scp;
{
	close_socket();
	sj_closeall();
#ifdef	LOCK_FILE
	erase_lockfile();
#endif
	exit(0);
}
#endif
server_terminate()
{
	close_socket();
	sj_closeall();
#ifdef	LOCK_FILE
	erase_lockfile();
#endif
	exit(0);
}



static	void	exec_fork()
{
	int	tmp;

	if (fork_flag && (tmp = fork())) {
		if (tmp < 0) {
			fprintf(stderr, "Can't fork child process\r\n");
			fflush(stderr);
		}
		else {
			signal(SIGCHLD, _exit);
			signal(SIGHUP , SIG_IGN);
			signal(SIGINT , SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			signal(SIGTSTP, SIG_IGN);
			signal(SIGTERM, _exit);
			wait(0);
			_exit(0);
		}
	}

	signal(SIGHUP,  (void (*)())signal_handler);


	signal(SIGINT,  (void (*)())terminate_handler);
	signal(SIGQUIT, (void (*)())signal_handler);
	signal(SIGTERM, (void (*)())terminate_handler);
	if (fork_flag)
		signal(SIGTSTP, SIG_IGN);
}




static	void	leave_tty()
{
	int	tmp;

	tmp  = open_error();
	tmp |= open_log();
	tmp |= open_debug();
	kill(getppid(), SIGTERM);
	fclose(stdin);
	fclose(stdout);
	if (!tmp) fclose(stderr);

	if (fork_flag) {
#ifdef SVR4
	        (void) setsid();
#else
		if ((tmp = open("/dev/tty", O_RDWR)) >= 0) {
			ioctl(tmp, TIOCNOTTY, 0);
			close(tmp);
		}
#endif
	}
}



int	main(argc, argv)
int	argc;
char	**argv;
{
#ifdef __sony_news
	(void) setlocale(LC_CTYPE, "ja_JP.EUC");
#endif
	opening();
	if (setuid(geteuid())) {
		fprintf(stderr, "error at setuid.\r\n"); exit(1);
	}

	parse_arg(argc, argv);		
	read_runcmd();			
	set_default();			

#ifdef	LOCK_FILE
	if (make_lockfile() == ERROR) {
		fprintf(stderr, "Already running...\r\n"); exit(1);
	}
#endif

	exec_fork();  

	socket_init();
	open_socket();

        leave_tty(); 

	preload_dict();
	communicate();

	close_socket();
	sj_closeall();
#ifdef	LOCK_FILE
	erase_lockfile();
#endif
	exit(0);
}
