/*-
 * Copyright 2016 Vsevolod Stakhov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config.h"
#include "rspamd.h"
#include "heap.h"
#include "ottery.h"

static const niter = 100500;
static const nrem = 100;

static inline
struct rspamd_min_heap_elt *
new_elt (guint pri)
{
	struct rspamd_min_heap_elt *elt;

	elt = g_slice_alloc0 (sizeof (*elt));
	elt->pri = pri;

	return elt;
}

void
rspamd_heap_test_func (void)
{
	struct rspamd_min_heap *heap;
	struct rspamd_min_heap_elt *elt;
	gint i;
	guint prev;

	heap = rspamd_min_heap_create (32);

	for (i = 100; i >= 0; i --) {
		elt = new_elt (i);
		rspamd_min_heap_push (heap, elt);
	}

	for (i = 0; i <= 100; i ++) {
		elt = rspamd_min_heap_pop (heap);
		g_assert (elt->pri == i);
	}

	rspamd_min_heap_destroy (heap);
	heap = rspamd_min_heap_create (128);

	for (i = 0; i < niter; i ++) {
		elt = new_elt (ottery_rand_uint32 () % G_MAXINT32 + 1);
		rspamd_min_heap_push (heap, elt);
	}

	for (i = 0; i < nrem; i ++) {
		elt = rspamd_min_heap_index (heap, ottery_rand_uint32 () % niter);
		rspamd_min_heap_remove_elt (heap, elt);
	}

	prev = 0;

	for (i = 0; i < niter - nrem; i ++) {
		elt = rspamd_min_heap_pop (heap);

		if (prev != 0) {
			g_assert (elt->pri >= prev);
		}

		prev = elt->pri;
	}

	rspamd_min_heap_destroy (heap);

	heap = rspamd_min_heap_create (32);

	elt = new_elt (2);
	elt->data = GINT_TO_POINTER (1);
	rspamd_min_heap_push (heap, elt);
	elt = new_elt (3);
	elt->data = GINT_TO_POINTER (2);
	rspamd_min_heap_push (heap, elt);
	elt = new_elt (4);
	elt->data = GINT_TO_POINTER (3);
	rspamd_min_heap_push (heap, elt);

	rspamd_min_heap_update_elt (heap, elt, 0);
	elt = rspamd_min_heap_pop (heap);
	g_assert (elt->idx == GINT_TO_POINTER (3));
}