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

   ॐ तारे तुत्तारे तुरे स्व
 */

#include <stdio.h>
#include <time.h>
#include "kiwi.h"

#define RECORD_INIT_SIZE 4

enum levels {LEVEL_TRACE, LEVEL_DEBUG, LEVEL_INFO, LEVEL_WARN, LEVEL_ERROR, LEVEL_FATAL};
static const char *level_names[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

/* The timestamp will added to the records when the key is not null.
 */
static char *ts_key;
static char *ts_fmt = "%H:%M:%S";
void kl_with_ts(char *key, ...)
{
	ts_key = key;
	va_list arg;
	va_start(arg, key);
	char *fmt = va_arg(arg, char *);
	va_end(arg);
	if (fmt!=NULL) ts_fmt = fmt;
}

void kl_without_ts()
{
	ts_key = NULL;
}

static void record_stamp(kl_record *rec)
{
	if (ts_key==NULL) return;
	time_t     t  = time(NULL);
	struct tm *lt = localtime(&t);
	char       buf[16];
	buf[strftime(buf, sizeof(buf), ts_fmt, lt)] = '\0';
	kl_pair pair = {.type = KL_PAIR_TIME, .key = ts_key, .sval = buf};
	kl__record_append(rec, &pair);
}

__attribute__((sentinel))
void kl__log_varg(kl_pair *kv, ...)
{
	kl_record rec;
	kl__record_init(&rec, RECORD_INIT_SIZE);
	record_stamp(&rec);
	va_list narg;
	va_start(narg, kv);
	kl_pair *pair = kv;
	do
	{
		kl__record_append(&rec, pair);
		pair = va_arg(narg, kl_pair *);
	} while (pair!=NULL);
	va_end(narg);
	kl__sink_this(&rec);
	kl__record_free(&rec);
}

/* kl_logs() allows use pairs with both string key and value without
 * wrapping them with kl_s(). It assumes that only string values
 * passed to the log kl_record.
 */
__attribute__((sentinel))
void kl__logs_varg(char *key, char *val, ...) {
	kl_record rec;
	kl__record_init(&rec, RECORD_INIT_SIZE);
	record_stamp(&rec);
	va_list narg;
	va_start(narg, val);
	kl_pair pair  = {.type = KL_PAIR_STRING, .key = key, .sval = val};
	int     count = 0;
	char *  next;
	do
	{
		kl__record_append(&rec, &pair);
		next = va_arg(narg, char *);
		if (count++%2==0) {
			/* Even argument is key. */
			key = next;
		}
		else
		{
			/* Odd argument is value. */
			pair.key  = key;
			pair.sval = next;
		};
	} while (next!=NULL);
	va_end(narg);
	kl__sink_this(&rec);
	kl__record_free(&rec);
}

/* Helper that accepts value of the pair as string (char*). This
   function allocates memory in the heap so it should be freed later.
 */
kl_pair *kl_s(char *key, char *val) {
	kl_pair *pair;
	pair       = malloc(sizeof(kl_pair));
	pair->type = KL_PAIR_STRING;
	pair->key  = key;
	pair->sval = val;
	return pair;
}

/* Helper that accepts value of the pair as integer. This function
   allocates memory in the heap so it should be freed later.
 */
kl_pair *kl_i(char *key, int val) {
	kl_pair *pair;
	pair       = malloc(sizeof(kl_pair));
	pair->type = KL_PAIR_INT;
	pair->key  = key;
	pair->ival = val;
	return pair;
}

/* Helper that accepts value of the pair as float. This function
   allocates memory in the heap so it should be freed later.
 */
kl_pair *kl_f(char *key, float val) {
	kl_pair *pair;
	pair       = malloc(sizeof(kl_pair));
	pair->type = KL_PAIR_FLOAT;
	pair->key  = key;
	pair->fval = val;
	return pair;
}

/* Helper that accepts value of the pair as double. This function
   allocates memory in the heap so it should be freed later.
 */
kl_pair *kl_d(char *key, double val) {
	kl_pair *pair;
	pair       = malloc(sizeof(kl_pair));
	pair->type = KL_PAIR_DOUBLE;
	pair->key  = key;
	pair->dval = val;
	return pair;
}

