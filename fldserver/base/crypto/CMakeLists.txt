if (IS_WIN)
	set(WINDOWS_SOURCES
			"capi_util.cc"
			"unexportable_key_win.cc"
			)
endif ()

if (IS_POSIX)
	set(POSIX_SOURCES

			)
endif ()

if (IS_LINUX)
	set(LINUX_SOURCES

			)
endif ()
if (CORE_USING_BORINGSSL)
	fldserver_cc_library(
			NAME
			base_crypto
			HDRS
			"aead.h"
			"capi_util.h"
			# "chaps_support.h"
			"crypto_export.h"
			"ec_private_key.h"
			"ec_signature_creator.h"
			"ec_signature_creator_impl.h"
			"encryptor.h"
			"hkdf.h"
			"hmac.h"
			"mac_security_services_lock.h"
			"nss_crypto_module_delegate.h"
			"nss_key_util.h"
			"nss_util.h"
			"nss_util_internal.h"
			"openssl_util.h"
			"p224_spake.h"
			"random.h"
			"rsa_private_key.h"
			"scoped_capi_types.h"
			"scoped_nss_types.h"
			"secure_hash.h"
			"secure_util.h"
			"sha2.h"
			"signature_creator.h"
			"signature_verifier.h"
			"symmetric_key.h"
			"unexportable_key.h"
			COPTS
			${FLDSERVER_DEFAULT_COPTS}
			SRCS
			"aead.cc"

			# "chaps_support.cc"
			"ec_private_key.cc"
			"ec_signature_creator.cc"
			"ec_signature_creator_impl.cc"
			"encryptor.cc"
			"hkdf.cc"
			"hmac.cc"
			"mac_security_services_lock.cc"
			# "nss_key_util.cc"
			# "nss_util.cc"
			# "nss_util_chromeos.cc"
			"openssl_util.cc"
			"p224_spake.cc"
			"random.cc"
			"rsa_private_key.cc"
			"secure_hash.cc"
			"secure_util.cc"
			"sha2.cc"
			"signature_creator.cc"
			"signature_verifier.cc"
			"symmetric_key.cc"
			"unexportable_key.cc"

			"${WINDOWS_SOURCES}"
			"${POSIX_SOURCES}"
			"${LINUX_SOURCES}"
			DEFINES
			DEPS
			Threads::Threads
			PUBLIC
			OBJECT_LIB
	)
	else()
	fldserver_cc_library(
			NAME
			base_crypto
			HDRS
			"sha2.h"
			"secure_hash.h"
			"openssl_util.h"
			COPTS
			${FLDSERVER_DEFAULT_COPTS}
			SRCS
			"openssl_util.cc"
			"sha2.cc"
			"secure_hash.cc"
			"${WINDOWS_SOURCES}"
			"${POSIX_SOURCES}"
			"${LINUX_SOURCES}"
			DEFINES
			DEPS
			Threads::Threads
			PUBLIC
			OBJECT_LIB
	)
endif ()

