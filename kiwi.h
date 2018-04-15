/*
   This file is part of Kiwi Logger (KL) — the library for logging.

   Copyright (c) 2018, Alexander I.Grafov <grafov@gmail.com>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of kvlog nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   Usage example:
     kl_log("key1", kl_s("sample"), "key2", kl_i(123), "key3", kl_f(3.141592));

   It will output something like:
     key1="sample" key2=123 key3=3.141592

   If you would output it in traditional way with levels:
     kl_info("key", "string value")

   Output will look like:
     level=INFO key="string value"

   ॐ तारे तुत्तारे तुरे स्व
 */

#ifndef KIWI_H
#define KIWI_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/** Logger interface **
 */
enum kl_pairval {KL_PAIR_STRING, KL_PAIR_INT, KL_PAIR_FLOAT, KL_PAIR_DOUBLE, KL_PAIR_TIME};
typedef struct {
	enum kl_pairval type;
	char *          key;
	char *          sval;
	int             ival;
	float           fval;
	double          dval;
} kl_pair;

/* For avoid varargs limitations each function that allows variadic
 * parameters redefined with NULL at the end. You should use functions
 * from #defines and don't use `_varg` functions!
 */
void kl_log_varg(kl_pair *kv, ...); // use kl_log() instead!
void kl_logs_varg(char *key, char *val, ...); // use kl_logs() instead!

#if __STRICT_ANSI__ // by recipe from https://snai.pe/c/preprocessor/varargs/
 #define kl_log(pair, ...) kl_log_varg(pair, __VA_ARGS__, (kl_pair *)NULL)
 #define kl_logs(k, v, ...) kl_logs_varg(k, v, __VA_ARGS__, (char *)NULL)
#else
 #define kl_log(pair, pairs ...) kl_log_varg(pair, ## pairs, (kl_pair *)NULL)
 #define kl_logs(k, v, pairs ...) kl_logs_varg(k, v, ## pairs, (char *)NULL)
#endif

/* Helpers for adding key-value pairs to the log record.
 */
kl_pair *kl_s(char *key, char *val);
kl_pair *kl_i(char *key, int val);
kl_pair *kl_f(char *key, float val);
kl_pair *kl_d(char *key, double val);

/* Setup timestamp output as additional context for the log records.
 * Second argument is time format. Default format is %H:%M:%S.
 */
void kl_with_ts(char *key, ...);
void kl_without_ts();

/* Emulate levels for compatibility with traditional loggers.
 */
void kl_level_key(char *key);
void kl_trace(kl_pair *kv, ...);
void kl_debug(kl_pair *kv, ...);
void kl_info(kl_pair *kv, ...);
void kl_warn(kl_pair *kv, ...);
void kl_error(kl_pair *kv, ...);
void kl_fatal(kl_pair *kv, ...);

/*
** Log record
*/
typedef struct {
	kl_pair **array;
	size_t    used;
	size_t    size;
} record;

/* Internal functions. */
int kl_record_init(record *a, size_t initial_size);
int kl_record_append(record *a, kl_pair *pair);
void kl_record_free(record *a);

/*
** Sink interface
*/
enum match_type {FILTER_MATCH_KEY, FILTER_RANGE_INT};

typedef struct {
	enum match_type type;
	char *          name;
	char *          val;
	int             from;
	int             to;
} filter;

typedef struct {
	FILE *  fd;
	filter *filter_set;
} sink;

typedef struct {
	int (*atstart)(sink *, record *);
	int (*format)(sink *, record *);
	int (*atfinish)(sink *, record *);
} format;

typedef struct {
	filter *cond;
	format *fmt;
	time_t *at;
} option;

/* Create a new sink. By default the sink will pass everything until
 * the filters applied. Arbitrary number of options could be passed
 * for modifying default sink settings. It could be filtering options
 * or reference to alternative formatter (all options are references
 * to `option` type instances. */
sink *kl_create_sink(FILE *fd, ...);

/* Change sink settings by adding or removing the filters or changing
 * the output format. Arbitrary number of options could be passed for
 * modifying default sink settings. It could be filtering options or
 * reference to alternative formatter (all options are references to
 * `option` type instances. */
void kl_modify_sink(sink *s, ...);

/* Reset all the filters for the sink. By default the sink will output
 * all passed records. Filtering options will not reset. */
void kl_sink_everything(sink *s);

/* Closes the sink and underlaying file. Return 0 on success or error code. */
int kl_close_sink(sink *s);

/* Internal function. */
void kl__sink_this(record *rec);

/*
** XXX Sink interface
   filter must_key(char* key);
   filter must_sval(char* key, char* val);
   filter must_irange(char* key, int from, int to);

   filter with_key(char* key);
   filter without_key(char* key);
   filter with_sval(char* key, char* val);
   filter without_sval(char* key, char* sval);
   filter with_irange(char* key, int from, int to);
   filter without_irange(char* key, int from, int to);
*/

#endif
