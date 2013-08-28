/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_hqx.h"
#include "ext/gd/php_gd.h"

#include <gd.h>
#include <hqx.h>

/* True global resources - no need for thread safety here */
static int le_gd;

/* {{{ hqx_functions[]
 *
 * Every user visible function must have an entry in hqx_functions[].
 */
const zend_function_entry hqx_functions[] = {
	PHP_FE(image_scale_hqx2,	NULL)
	PHP_FE(image_scale_hqx3,	NULL)
	PHP_FE(image_scale_hqx4,	NULL)
	PHP_FE_END	/* Must be the last line in hqx_functions[] */
};
/* }}} */

static const zend_module_dep hqx_deps[] = {
	ZEND_MOD_REQUIRED("gd")
	ZEND_MOD_END
};

/* {{{ hqx_module_entry
 */
zend_module_entry hqx_module_entry = {
//#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER_EX,
//#endif
    NULL,
	hqx_deps,
	"hqx",
	hqx_functions,
	PHP_MINIT(hqx),
	PHP_MSHUTDOWN(hqx),
	PHP_RINIT(hqx),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(hqx),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(hqx),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HQX
ZEND_GET_MODULE(hqx)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(hqx)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	le_gd = phpi_get_le_gd();
	hqxInit();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(hqx)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(hqx)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(hqx)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(hqx)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "hqx support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

void hqx_scale(gdImagePtr srcIm, int factor, gdImagePtr* result)
{
	gdImagePtr dstIm;
	uint32_t *srcBuffer, *dstBuffer;
	size_t srcSize, dstSize;
	uint32_t w, h;
	uint32_t x, y;

	w = gdImageSX(srcIm);
	h = gdImageSY(srcIm);

	srcSize = sizeof(uint32_t) * w * h;
	dstSize = sizeof(uint32_t) * factor * w * factor * h;
	
	// Unfortunately it doesn't work to simply pass the gd buffer from the
	// gdImage struct to hqx, and the gd documentation explicitly says
	// not to access member fields directly. Thus we allocate two buffers
	// and copy the data back and forth as a workaround.

	srcBuffer = (uint32_t*)emalloc(srcSize);
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			srcBuffer[w*y + x] = gdImageGetPixel(srcIm, x, y);
		}
	}

	dstBuffer = (uint32_t*)emalloc(dstSize);
	if (factor == 4) {
		hq4x_32_rb(srcBuffer, sizeof(uint32_t)*w,
				   dstBuffer, sizeof(uint32_t)*factor*w,
				   w, h);
	} else if (factor == 3) {
		hq3x_32_rb(srcBuffer, sizeof(uint32_t)*w,
				   dstBuffer, sizeof(uint32_t)*factor*w,
				   w, h);
	} else if (factor == 2) {
		hq2x_32_rb(srcBuffer, sizeof(uint32_t)*w,
				   dstBuffer, sizeof(uint32_t)*factor*w,
				   w, h);
	}

	dstIm = gdImageCreateTrueColor(factor*w, factor*h);
	gdImageAlphaBlending(dstIm, 0);
	gdImageSaveAlpha(dstIm, 1);

	for (y = 0; y < factor*h; y++) {
		for (x = 0; x < factor*w; x++) {
			gdImageSetPixel(dstIm, x, y, dstBuffer[factor*w*y + x]);
		}
	}

	efree(srcBuffer);
	efree(dstBuffer);

	*result = dstIm;
}

PHP_FUNCTION(image_scale_hqx2)
{
	zval *zSrc;
	gdImagePtr srcIm, dstIm;
	int factor = 2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zSrc) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(srcIm, gdImagePtr, &zSrc, -1, "Image", le_gd);

	hqx_scale(srcIm, factor, &dstIm);

	ZEND_REGISTER_RESOURCE(return_value, dstIm, le_gd);
}

PHP_FUNCTION(image_scale_hqx3)
{
	zval *zSrc;
	gdImagePtr srcIm, dstIm;
	int factor = 3;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zSrc) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(srcIm, gdImagePtr, &zSrc, -1, "Image", le_gd);

	hqx_scale(srcIm, factor, &dstIm);

	ZEND_REGISTER_RESOURCE(return_value, dstIm, le_gd);
}

PHP_FUNCTION(image_scale_hqx4)
{
	zval *zSrc;
	gdImagePtr srcIm, dstIm;
	int factor = 4;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zSrc) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(srcIm, gdImagePtr, &zSrc, -1, "Image", le_gd);

	hqx_scale(srcIm, factor, &dstIm);

	ZEND_REGISTER_RESOURCE(return_value, dstIm, le_gd);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
