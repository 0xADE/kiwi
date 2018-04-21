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
#include "kiwi.h"

static kl_sink *top_sink;
/* It adds the new kl_sink to the set if the fd not found in the sinks
 * array. If the fd have found in the array it will be replaced by the
 * new one. */
static int sinks_add(kl_sink *s) {
	kl_sink *cursor = top_sink;
	while(cursor!=NULL) {
		if(cursor->fd==s->fd) {
			cursor->fd         = s->fd;
			cursor->filter_set = s->filter_set;
			return 0;
		}
		cursor = cursor->linked;
	}
	if (top_sink!=NULL) {
		s->linked = top_sink;
	}
	top_sink = s;
	return 0;
}

/* Remove the kl_sink from the set and close associated file. */
static int sinks_remove(kl_sink *s) {
	if (top_sink==NULL) return -1;
	kl_sink *cursor = top_sink;
	kl_sink *prev_cursor;
	while (cursor!=NULL) {
		if (cursor->fd==s->fd) {
			if (prev_cursor!=NULL) {
				prev_cursor->linked = cursor->linked;
			}
			fclose(s->fd);
			free(s);
			return 0;
		}
		prev_cursor = cursor;
		cursor      = cursor->linked;
	}
	return -1;
}

static kl_sink *sinks_find(kl_sink *s){
	kl_sink *cursor = top_sink;
	while (cursor!=NULL) {
		if (cursor->fd==s->fd) {
			return cursor;
		}
		cursor = cursor->linked;
	}
	return NULL;
}

kl_sink *kl_create_sink (FILE *fd, ...){
	va_list options;
	va_start(options, fd);
	kl_option *opt;
	kl_sink    sink = {.fd = fd}; // TODO init with default format
	do
	{
		opt = va_arg(options, kl_option *);
		if (opt==NULL) break;
		if (opt->cond!=NULL) {
			// TODO XXX filter_add(sink.filter_set, opt->cond)
		}
		if (opt->fmt!=NULL) {
			sink.fmt = opt->fmt;
		}
	} while (opt!=NULL);
	va_end(options);
	sinks_add(&sink);
}

void kl_modify_sink(kl_sink *s, ...) {
	va_list options;
	va_start(options, s);
	kl_option *opt;
	do
	{
		opt = va_arg(options, kl_option *);
	} while (opt!=NULL);
	va_end(options);
}

// Reset all filters. Starts logging everything to this kl_sink.
void kl_sink_everything(kl_sink *s) {
	kl_sink *kl_sink;
	if ((kl_sink = sinks_find(s))!=NULL) {
		kl_sink->filter_set = NULL; // FIXME нужно ли здесь освободить память от фильтров?
	};
}

// XXX remove it
static void add_to_record (kl_pair *kv){
	// XXX формировать запись, учесть доп форматы (JSON etc)
	switch (kv->type) {
	case KL_PAIR_STRING:
		printf("%s STR %s\n", kv->key, kv->sval);
		break;
	case KL_PAIR_INT:
		printf("%s INT %i\n", kv->key, kv->ival);
		break;
	case KL_PAIR_FLOAT:
		printf("%s FLOAT %f\n", kv->key, kv->fval);
		break;
	case KL_PAIR_DOUBLE:
		printf("%s DOUBLE %f\n", kv->key, kv->dval);
		break;
	case KL_PAIR_TIME:
		printf("%s TIME %s\n", kv->key, kv->sval);
		break;
	}
}

void kl__sink_this(kl_record *rec) {
	// создается копия со списком синков (1)
	// создается пустой массив под активные выходы (2)
	for (int i = 0; i<rec->used; i++) {
		// прогон по фильтрам, в какой синк подходит, он переносится из 1 в 2
		// далее проверка идет по 1 уже без учета отфильтрованных (уже активированных) синков
		// если попадет в запрещающий фильтр, то синк выкидывается из 1 и 2

		printf("%i ", i); // XXX
		add_to_record(rec->array[i]); // XXX debug time

	}

	// если список активных синков не пустой, подготовка строки из записи
	// цикл по активным синкам, выдача
}
