/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/console/lib/RCS/mktrig.h,v 2.5 89/04/24 19:21:49 mmackay Exp $ */
/* $Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/console/lib/RCS/mktrig.h,v $ */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsidmktrig = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/atk/console/lib/RCS/mktrig.h,v 2.5 89/04/24 19:21:49 mmackay Exp $";
#endif /* !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS) */

int SineMult[] = {
	0,
	174,
	348,
	523,
	697,
	871,
	1045,
	1218,
	1391,
	1564,
	1736,
	1908,
	2079,
	2249,
	2419,
	2588,
	2756,
	2923,
	3090,
	3255,
	3420,
	3583,
	3746,
	3907,
	4067,
	4226,
	4383,
	4539,
	4694,
	4848,
	4999,
	5150,
	5299,
	5446,
	5591,
	5735,
	5877,
	6018,
	6156,
	6293,
	6427,
	6560,
	6691,
	6819,
	6946,
	7071,
	7193,
	7313,
	7431,
	7547,
	7660,
	7771,
	7880,
	7986,
	8090,
	8191,
	8290,
	8386,
	8480,
	8571,
	8660,
	8746,
	8829,
	8910,
	8987,
	9063,
	9135,
	9205,
	9271,
	9335,
	9396,
	9455,
	9510,
	9563,
	9612,
	9659,
	9702,
	9743,
	9781,
	9816,
	9848,
	9876,
	9902,
	9925,
	9945,
	9961,
	9975,
	9986,
	9993,
	9998,
	10000,
	9998,
	9993,
	9986,
	9975,
	9961,
	9945,
	9925,
	9902,
	9876,
	9848,
	9816,
	9781,
	9743,
	9702,
	9659,
	9612,
	9563,
	9510,
	9455,
	9396,
	9335,
	9271,
	9205,
	9135,
	9063,
	8987,
	8910,
	8829,
	8746,
	8660,
	8571,
	8480,
	8386,
	8290,
	8191,
	8090,
	7986,
	7880,
	7771,
	7660,
	7547,
	7431,
	7313,
	7193,
	7071,
	6946,
	6819,
	6691,
	6560,
	6427,
	6293,
	6156,
	6018,
	5877,
	5735,
	5591,
	5446,
	5299,
	5150,
	5000,
	4848,
	4694,
	4539,
	4383,
	4226,
	4067,
	3907,
	3746,
	3583,
	3420,
	3255,
	3090,
	2923,
	2756,
	2588,
	2419,
	2249,
	2079,
	1908,
	1736,
	1564,
	1391,
	1218,
	1045,
	871,
	697,
	523,
	348,
	174,
	0,
	-174,
	-348,
	-523,
	-697,
	-871,
	-1045,
	-1218,
	-1391,
	-1564,
	-1736,
	-1908,
	-2079,
	-2249,
	-2419,
	-2588,
	-2756,
	-2923,
	-3090,
	-3255,
	-3420,
	-3583,
	-3746,
	-3907,
	-4067,
	-4226,
	-4383,
	-4539,
	-4694,
	-4848,
	-4999,
	-5150,
	-5299,
	-5446,
	-5591,
	-5735,
	-5877,
	-6018,
	-6156,
	-6293,
	-6427,
	-6560,
	-6691,
	-6819,
	-6946,
	-7071,
	-7193,
	-7313,
	-7431,
	-7547,
	-7660,
	-7771,
	-7880,
	-7986,
	-8090,
	-8191,
	-8290,
	-8386,
	-8480,
	-8571,
	-8660,
	-8746,
	-8829,
	-8910,
	-8987,
	-9063,
	-9135,
	-9205,
	-9271,
	-9335,
	-9396,
	-9455,
	-9510,
	-9563,
	-9612,
	-9659,
	-9702,
	-9743,
	-9781,
	-9816,
	-9848,
	-9876,
	-9902,
	-9925,
	-9945,
	-9961,
	-9975,
	-9986,
	-9993,
	-9998,
	-10000,
	-9998,
	-9993,
	-9986,
	-9975,
	-9961,
	-9945,
	-9925,
	-9902,
	-9876,
	-9848,
	-9816,
	-9781,
	-9743,
	-9702,
	-9659,
	-9612,
	-9563,
	-9510,
	-9455,
	-9396,
	-9335,
	-9271,
	-9205,
	-9135,
	-9063,
	-8987,
	-8910,
	-8829,
	-8746,
	-8660,
	-8571,
	-8480,
	-8386,
	-8290,
	-8191,
	-8090,
	-7986,
	-7880,
	-7771,
	-7660,
	-7547,
	-7431,
	-7313,
	-7193,
	-7071,
	-6946,
	-6819,
	-6691,
	-6560,
	-6427,
	-6293,
	-6156,
	-6018,
	-5877,
	-5735,
	-5591,
	-5446,
	-5299,
	-5150,
	-5000,
	-4848,
	-4694,
	-4539,
	-4383,
	-4226,
	-4067,
	-3907,
	-3746,
	-3583,
	-3420,
	-3255,
	-3090,
	-2923,
	-2756,
	-2588,
	-2419,
	-2249,
	-2079,
	-1908,
	-1736,
	-1564,
	-1391,
	-1218,
	-1045,
	-871,
	-697,
	-523,
	-348,
	-174,
	0,
	174,
	348,
	523,
	697,
	871,
	1045,
	1218,
	1391,
	1564,
	1736,
	1908,
	2079,
	2249,
	2419,
	2588,
	2756,
	2923,
	3090,
	3255,
	3420,
	3583,
	3746,
	3907,
	4067,
	4226,
	4383,
	4539,
	4694,
	4848,
	4999,
	5150,
	5299,
	5446,
	5591,
	5735,
	5877,
	6018,
	6156,
	6293,
	6427,
	6560,
	6691,
	6819,
	6946,
	7071,
	7193,
	7313,
	7431,
	7547,
	7660,
	7771,
	7880,
	7986,
	8090,
	8191,
	8290,
	8386,
	8480,
	8571,
	8660,
	8746,
	8829,
	8910,
	8987,
	9063,
	9135,
	9205,
	9271,
	9335,
};

int CosineMult[] = {
	9999,
	9998,
	9993,
	9986,
	9975,
	9961,
	9945,
	9925,
	9902,
	9876,
	9848,
	9816,
	9781,
	9743,
	9702,
	9659,
	9612,
	9563,
	9510,
	9455,
	9396,
	9335,
	9271,
	9205,
	9135,
	9063,
	8987,
	8910,
	8829,
	8746,
	8660,
	8571,
	8480,
	8386,
	8290,
	8191,
	8090,
	7986,
	7880,
	7771,
	7660,
	7547,
	7431,
	7313,
	7193,
	7071,
	6946,
	6819,
	6691,
	6560,
	6427,
	6293,
	6156,
	6018,
	5877,
	5735,
	5591,
	5446,
	5299,
	5150,
	5000,
	4848,
	4694,
	4539,
	4383,
	4226,
	4067,
	3907,
	3746,
	3583,
	3420,
	3255,
	3090,
	2923,
	2756,
	2588,
	2419,
	2249,
	2079,
	1908,
	1736,
	1564,
	1391,
	1218,
	1045,
	871,
	697,
	523,
	348,
	174,
	0,
	-174,
	-348,
	-523,
	-697,
	-871,
	-1045,
	-1218,
	-1391,
	-1564,
	-1736,
	-1908,
	-2079,
	-2249,
	-2419,
	-2588,
	-2756,
	-2923,
	-3090,
	-3255,
	-3420,
	-3583,
	-3746,
	-3907,
	-4067,
	-4226,
	-4383,
	-4539,
	-4694,
	-4848,
	-4999,
	-5150,
	-5299,
	-5446,
	-5591,
	-5735,
	-5877,
	-6018,
	-6156,
	-6293,
	-6427,
	-6560,
	-6691,
	-6819,
	-6946,
	-7071,
	-7193,
	-7313,
	-7431,
	-7547,
	-7660,
	-7771,
	-7880,
	-7986,
	-8090,
	-8191,
	-8290,
	-8386,
	-8480,
	-8571,
	-8660,
	-8746,
	-8829,
	-8910,
	-8987,
	-9063,
	-9135,
	-9205,
	-9271,
	-9335,
	-9396,
	-9455,
	-9510,
	-9563,
	-9612,
	-9659,
	-9702,
	-9743,
	-9781,
	-9816,
	-9848,
	-9876,
	-9902,
	-9925,
	-9945,
	-9961,
	-9975,
	-9986,
	-9993,
	-9998,
	-9999,
	-9998,
	-9993,
	-9986,
	-9975,
	-9961,
	-9945,
	-9925,
	-9902,
	-9876,
	-9848,
	-9816,
	-9781,
	-9743,
	-9702,
	-9659,
	-9612,
	-9563,
	-9510,
	-9455,
	-9396,
	-9335,
	-9271,
	-9205,
	-9135,
	-9063,
	-8987,
	-8910,
	-8829,
	-8746,
	-8660,
	-8571,
	-8480,
	-8386,
	-8290,
	-8191,
	-8090,
	-7986,
	-7880,
	-7771,
	-7660,
	-7547,
	-7431,
	-7313,
	-7193,
	-7071,
	-6946,
	-6819,
	-6691,
	-6560,
	-6427,
	-6293,
	-6156,
	-6018,
	-5877,
	-5735,
	-5591,
	-5446,
	-5299,
	-5150,
	-5000,
	-4848,
	-4694,
	-4539,
	-4383,
	-4226,
	-4067,
	-3907,
	-3746,
	-3583,
	-3420,
	-3255,
	-3090,
	-2923,
	-2756,
	-2588,
	-2419,
	-2249,
	-2079,
	-1908,
	-1736,
	-1564,
	-1391,
	-1218,
	-1045,
	-871,
	-697,
	-523,
	-348,
	-174,
	0,
	174,
	348,
	523,
	697,
	871,
	1045,
	1218,
	1391,
	1564,
	1736,
	1908,
	2079,
	2249,
	2419,
	2588,
	2756,
	2923,
	3090,
	3255,
	3420,
	3583,
	3746,
	3907,
	4067,
	4226,
	4383,
	4539,
	4694,
	4848,
	4999,
	5150,
	5299,
	5446,
	5591,
	5735,
	5877,
	6018,
	6156,
	6293,
	6427,
	6560,
	6691,
	6819,
	6946,
	7071,
	7193,
	7313,
	7431,
	7547,
	7660,
	7771,
	7880,
	7986,
	8090,
	8191,
	8290,
	8386,
	8480,
	8571,
	8660,
	8746,
	8829,
	8910,
	8987,
	9063,
	9135,
	9205,
	9271,
	9335,
	9396,
	9455,
	9510,
	9563,
	9612,
	9659,
	9702,
	9743,
	9781,
	9816,
	9848,
	9876,
	9902,
	9925,
	9945,
	9961,
	9975,
	9986,
	9993,
	9998,
	9999,
	9998,
	9993,
	9986,
	9975,
	9961,
	9945,
	9925,
	9902,
	9876,
	9848,
	9816,
	9781,
	9743,
	9702,
	9659,
	9612,
	9563,
	9510,
	9455,
	9396,
	9335,
	9271,
	9205,
	9135,
	9063,
	8987,
	8910,
	8829,
	8746,
	8660,
	8571,
	8480,
	8386,
	8290,
	8191,
	8090,
	7986,
	7880,
	7771,
	7660,
	7547,
	7431,
	7313,
	7193,
	7071,
	6946,
	6819,
	6691,
	6560,
	6427,
	6293,
	6156,
	6018,
	5877,
	5735,
	5591,
	5446,
	5299,
	5150,
	5000,
	4848,
	4694,
	4539,
	4383,
	4226,
	4067,
	3907,
	3746,
	3583,
};

