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

#ifndef PHP_HQX_H
#define PHP_HQX_H

extern zend_module_entry hqx_module_entry;
#define phpext_hqx_ptr &hqx_module_entry

#ifdef PHP_WIN32
#	define PHP_HQX_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_HQX_API __attribute__ ((visibility("default")))
#else
#	define PHP_HQX_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(hqx);
PHP_MSHUTDOWN_FUNCTION(hqx);
PHP_RINIT_FUNCTION(hqx);
PHP_RSHUTDOWN_FUNCTION(hqx);
PHP_MINFO_FUNCTION(hqx);

PHP_FUNCTION(image_scale_hqx2);
PHP_FUNCTION(image_scale_hqx3);
PHP_FUNCTION(image_scale_hqx4);

#ifdef ZTS
#define HQX_G(v) TSRMG(hqx_globals_id, zend_hqx_globals *, v)
#else
#define HQX_G(v) (hqx_globals.v)
#endif

#endif	/* PHP_HQX_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
