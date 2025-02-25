/* $OpenBSD: asn1_locl.h,v 1.22 2022/03/13 14:58:14 jsing Exp $ */
/* Written by Dr Stephen N Henson (steve@openssl.org) for the OpenSSL
 * project 2006.
 */
/* ====================================================================
 * Copyright (c) 2006 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#include "bytestring.h"

__BEGIN_HIDDEN_DECLS

/* Internal ASN1 structures and functions: not for application use */

ASN1_TYPE *ASN1_TYPE_pack_sequence(const ASN1_ITEM *it, void *s, ASN1_TYPE **t);
void *ASN1_TYPE_unpack_sequence(const ASN1_ITEM *it, const ASN1_TYPE *t);

/* These are used internally in the ASN1_OBJECT to keep track of
 * whether the names and data need to be free()ed */
#define ASN1_OBJECT_FLAG_DYNAMIC	 0x01	/* internal use */
#define ASN1_OBJECT_FLAG_CRITICAL	 0x02	/* critical x509v3 object id */
#define ASN1_OBJECT_FLAG_DYNAMIC_STRINGS 0x04	/* internal use */
#define ASN1_OBJECT_FLAG_DYNAMIC_DATA	 0x08	/* internal use */
struct asn1_object_st {
	const char *sn, *ln;
	int nid;
	int length;
	const unsigned char *data;	/* data remains const after init */
	int flags;	/* Should we free this one */
} /* ASN1_OBJECT */;

/* ASN1 print context structure */

struct asn1_pctx_st {
	unsigned long flags;
	unsigned long nm_flags;
	unsigned long cert_flags;
	unsigned long oid_flags;
	unsigned long str_flags;
} /* ASN1_PCTX */;

/* ASN1 public key method structure */

struct evp_pkey_asn1_method_st {
	int pkey_id;
	int pkey_base_id;
	unsigned long pkey_flags;

	char *pem_str;
	char *info;

	int (*pub_decode)(EVP_PKEY *pk, X509_PUBKEY *pub);
	int (*pub_encode)(X509_PUBKEY *pub, const EVP_PKEY *pk);
	int (*pub_cmp)(const EVP_PKEY *a, const EVP_PKEY *b);
	int (*pub_print)(BIO *out, const EVP_PKEY *pkey, int indent,
	    ASN1_PCTX *pctx);

	int (*priv_decode)(EVP_PKEY *pk, const PKCS8_PRIV_KEY_INFO *p8inf);
	int (*priv_encode)(PKCS8_PRIV_KEY_INFO *p8, const EVP_PKEY *pk);
	int (*priv_print)(BIO *out, const EVP_PKEY *pkey, int indent,
	    ASN1_PCTX *pctx);

	int (*pkey_size)(const EVP_PKEY *pk);
	int (*pkey_bits)(const EVP_PKEY *pk);

	int (*param_decode)(EVP_PKEY *pkey, const unsigned char **pder,
	    int derlen);
	int (*param_encode)(const EVP_PKEY *pkey, unsigned char **pder);
	int (*param_missing)(const EVP_PKEY *pk);
	int (*param_copy)(EVP_PKEY *to, const EVP_PKEY *from);
	int (*param_cmp)(const EVP_PKEY *a, const EVP_PKEY *b);
	int (*param_print)(BIO *out, const EVP_PKEY *pkey, int indent,
	    ASN1_PCTX *pctx);
	int (*sig_print)(BIO *out, const X509_ALGOR *sigalg,
	    const ASN1_STRING *sig, int indent, ASN1_PCTX *pctx);

	void (*pkey_free)(EVP_PKEY *pkey);
	int (*pkey_ctrl)(EVP_PKEY *pkey, int op, long arg1, void *arg2);

	/* Legacy functions for old PEM */

	int (*old_priv_decode)(EVP_PKEY *pkey, const unsigned char **pder,
	    int derlen);
	int (*old_priv_encode)(const EVP_PKEY *pkey, unsigned char **pder);
	/* Custom ASN1 signature verification */
	int (*item_verify)(EVP_MD_CTX *ctx, const ASN1_ITEM *it, void *asn,
	    X509_ALGOR *a, ASN1_BIT_STRING *sig, EVP_PKEY *pkey);
	int (*item_sign)(EVP_MD_CTX *ctx, const ASN1_ITEM *it, void *asn,
	    X509_ALGOR *alg1, X509_ALGOR *alg2, ASN1_BIT_STRING *sig);

	int (*pkey_check)(const EVP_PKEY *pk);
	int (*pkey_public_check)(const EVP_PKEY *pk);
	int (*pkey_param_check)(const EVP_PKEY *pk);
} /* EVP_PKEY_ASN1_METHOD */;

/* Method to handle CRL access.
 * In general a CRL could be very large (several Mb) and can consume large
 * amounts of resources if stored in memory by multiple processes.
 * This method allows general CRL operations to be redirected to more
 * efficient callbacks: for example a CRL entry database.
 */

#define X509_CRL_METHOD_DYNAMIC		1

struct x509_crl_method_st {
	int flags;
	int (*crl_init)(X509_CRL *crl);
	int (*crl_free)(X509_CRL *crl);
	int (*crl_lookup)(X509_CRL *crl, X509_REVOKED **ret,
	    ASN1_INTEGER *ser, X509_NAME *issuer);
	int (*crl_verify)(X509_CRL *crl, EVP_PKEY *pk);
};

int asn1_get_choice_selector(ASN1_VALUE **pval, const ASN1_ITEM *it);
int asn1_set_choice_selector(ASN1_VALUE **pval, int value, const ASN1_ITEM *it);

ASN1_VALUE ** asn1_get_field_ptr(ASN1_VALUE **pval, const ASN1_TEMPLATE *tt);

const ASN1_TEMPLATE *asn1_do_adb(ASN1_VALUE **pval, const ASN1_TEMPLATE *tt, int nullerr);

int asn1_do_lock(ASN1_VALUE **pval, int op, const ASN1_ITEM *it);

void asn1_enc_init(ASN1_VALUE **pval, const ASN1_ITEM *it);
void asn1_enc_free(ASN1_VALUE **pval, const ASN1_ITEM *it);
int asn1_enc_restore(int *len, unsigned char **out, ASN1_VALUE **pval, const ASN1_ITEM *it);
int asn1_enc_save(ASN1_VALUE **pval, const unsigned char *in, int inlen, const ASN1_ITEM *it);

int i2d_ASN1_BOOLEAN(int a, unsigned char **pp);
int d2i_ASN1_BOOLEAN(int *a, const unsigned char **pp, long length);

/*
 * Unicode codepoint constants
 */
#define	UNICODE_MAX		0x10FFFF
#define	UNICODE_SURROGATE_MIN	0x00D800
#define	UNICODE_SURROGATE_MAX	0x00DFFF

#define	UNICODE_IS_SURROGATE(x) \
	((x) >= UNICODE_SURROGATE_MIN && (x) <= UNICODE_SURROGATE_MAX)

int UTF8_getc(const unsigned char *str, int len, unsigned long *val);
int UTF8_putc(unsigned char *str, int len, unsigned long value);

int asn1_d2i_read_bio(BIO *in, BUF_MEM **pb);

int asn1_get_object_cbs(CBS *cbs, int der_mode, uint8_t *out_class,
    int *out_constructed, uint32_t *out_tag_number, int *out_indefinite,
    uint32_t *out_length);

int asn1_tag2charwidth(int tag);

int i2t_ASN1_OBJECT_internal(const ASN1_OBJECT *aobj, char *buf, int buf_len,
    int no_name);

__END_HIDDEN_DECLS
