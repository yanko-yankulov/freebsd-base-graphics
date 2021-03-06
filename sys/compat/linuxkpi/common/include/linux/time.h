/*-
 * Copyright (c) 2014-2015 François Tigeot
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */
#ifndef _LINUX_TIME_H_
#define	_LINUX_TIME_H_

#include <linux/time64.h>

#define tsc_khz tsc_freq/1000L

#include <sys/time.h>
#include <machine/clock.h>
#include <sys/stdint.h>
# include <linux/math64.h>

typedef unsigned long cycles_t;
#define USEC_PER_SEC	1000000L
extern int hz;

static inline u64 nsecs_to_jiffies64(u64 n)
{
	if (NSEC_PER_SEC % hz == 0)
		return div_u64(n, NSEC_PER_SEC / hz);
	else 
		return div_u64(n * 9, (9ull * NSEC_PER_SEC + hz / 2) / hz);
}

static inline unsigned int
jiffies_to_usecs(const unsigned long j)
{
	return (USEC_PER_SEC / hz) * j;
}

static inline unsigned long
jiffies_to_nsecs(const unsigned long j)
{
	return (NSEC_PER_SEC / hz) * j;
}

static inline struct timeval
ns_to_timeval(const int64_t nsec)
{
	struct timeval tv;
	long rem;

	if (nsec == 0) {
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		return (tv);
	}

	tv.tv_sec = nsec / NSEC_PER_SEC;
	rem = nsec % NSEC_PER_SEC;
	if (rem < 0) {
		tv.tv_sec--;
		rem += NSEC_PER_SEC;
	}
	tv.tv_usec = rem / 1000;
	return (tv);
}

static inline int64_t
timeval_to_ns(const struct timeval *tv)
{
	return ((int64_t)tv->tv_sec * NSEC_PER_SEC) +
		tv->tv_usec * NSEC_PER_USEC;
}

#define getrawmonotonic(ts)	nanouptime(ts)

static inline struct timespec
timespec_sub(struct timespec lhs, struct timespec rhs)
{
	struct timespec ts;

	ts.tv_sec = lhs.tv_sec;
	ts.tv_nsec = lhs.tv_nsec;
	timespecsub(&ts, &rhs);

	return ts;
}

static inline void
set_normalized_timespec(struct timespec *ts, time_t sec, int64_t nsec)
{
	/* XXX: this doesn't actually normalize anything */
	ts->tv_sec = sec;
	ts->tv_nsec = nsec;
}

static inline int64_t
timespec_to_ns(const struct timespec *ts)
{
	return ((ts->tv_sec * NSEC_PER_SEC) + ts->tv_nsec);
}

static inline struct timespec
ns_to_timespec(const int64_t nsec)
{
	struct timespec ts;
	int32_t rem;

	if (nsec == 0) {
		ts.tv_sec = 0;
		ts.tv_nsec = 0;
		return (ts);
	}

	ts.tv_sec = nsec / NSEC_PER_SEC;
	rem = nsec % NSEC_PER_SEC;
	if (rem < 0) {
		ts.tv_sec--;
		rem += NSEC_PER_SEC;
	}
	ts.tv_nsec = rem;
	return (ts);
}

static inline int
timespec_valid(const struct timespec *ts)
{
	if (ts->tv_sec < 0 || ts->tv_sec > 100000000 ||
	    ts->tv_nsec < 0 || ts->tv_nsec >= 1000000000)
		return (0);
	return (1);
}

static inline unsigned long
get_seconds(void)
{
	return time_second;
}

#endif /* _LINUX_TIME_H_ */
