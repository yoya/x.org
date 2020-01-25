/* 
 * Copyright (C) 1993, 1994, Alain Nissen & Raphael Quinet,
 *                                                University of Liege, Belgium.
 *
 * The X Consortium, and any party obtaining a copy of these files from the
 * X Consortium, directly or indirectly, is granted, free of charge, a full
 * and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and documentation
 * files (the "Software"), including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons who receive copies from any such party
 * to do so.  This license includes without limitation a license to do the
 * foregoing actions under any patents of the party supplying this software
 * to the X Consortium.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * X CONSORTIUM, THE AUTHORS OR THE UNIVERSITY OF LIEGE BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * Authors :
 *    Raphael Quinet <quinet@montefiore.ulg.ac.be>
 *                   <quinet@stud.montefiore.ulg.ac.be>
 *    Alain Nissen   <nissen@montefiore.ulg.ac.be>
 *                   <nissen@stud.montefiore.ulg.ac.be>
 */

#if defined(DGUX) || defined(_CRAY) || defined(hpux) || defined(_IBMR2) || defined(aix) || defined(macII) || (defined(MOTOROLA) && defined(SYSV)) || defined(_SEQUENT_) || defined(sgi) || defined(nec_ews_svr2) || defined(SX) || defined(PC_UX) || (defined(SYSV386) && !defined(SVR4))
#define SystemV
#else
#define NotSystemV
#endif

#if defined(__sxg__) || defined(__osf__) || ((defined(sun) || defined(__sun)) && (defined(__SVR4) || defined(__svr4__)))
#define NoVFork
#else
#ifdef hpux
#define HasVFork
#else
#ifdef SystemV
#define NoVFork
#else
#define HasVFork
#endif
#endif
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__)
#define CppCmd "/usr/libexec/cpp -traditional"
#else
#ifdef WIN32
#define CppCmd "cl -nologo -batch -E"
#else
#ifdef bsdi
#define CppCmd "/usr/bin/cpp"
#else
#if defined(__uxp__) || (defined(MOTOROLA) && defined(SVR4)) || (defined(SYSV386) && defined(SVR4)) || ((defined(sun) || defined(__sun)) && (defined(__SVR4) || defined(__svr4__)))
#define CppCmd "/usr/ccs/lib/cpp"
#else
#ifdef _IBMR2
#define CppCmd "/usr/lpp/X11/Xamples/util/cpp/cpp"
#else
#define CppCmd "/lib/cpp"
#endif
#endif
#endif
#endif
#endif
