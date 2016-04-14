/*
 * Copyright (C) 2016 Cristian Klein.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/lzo.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>

#include "zcomp_null.h"

struct null_backend_private {
	int unused;	// this structure is not yet used
};

static void *null_create(gfp_t flags)
{
	void *ret;

	ret = kmalloc(sizeof(struct null_backend_private), flags);
	if (!ret)
		ret = __vmalloc(sizeof(struct null_backend_private),
				flags | __GFP_HIGHMEM,
				PAGE_KERNEL);
	return ret;
}

static void null_destroy(void *private)
{
	kvfree(private);
}

static int null_compress(const unsigned char *src, unsigned char *dst,
		size_t *dst_len, void *private)
{
	memcpy(dst, src, PAGE_SIZE);
	*dst_len = PAGE_SIZE;
	return 0; // never fails
}

static int null_decompress(const unsigned char *src, size_t src_len,
		unsigned char *dst)
{
	memcpy(dst, src, src_len);
	return 0; // never fails
}

struct zcomp_backend zcomp_null = {
	.compress = null_compress,
	.decompress = null_decompress,
	.create = null_create,
	.destroy = null_destroy,
	.name = "null",
};
