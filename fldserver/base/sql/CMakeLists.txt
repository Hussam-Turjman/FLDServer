if (IS_WIN)
	set(WINDOWS_SOURCES

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

fldserver_cc_library(
		NAME
		sql
		HDRS
		"database.h"
		"error_delegate_util.h"
		"init_status.h"
		"initialization.h"
		"internal_api_token.h"
		"meta_table.h"
		"recover_module/btree.h"
		"recover_module/cursor.h"
		"recover_module/integers.h"
		"recover_module/module.h"
		"recover_module/pager.h"
		"recover_module/parsing.h"
		"recover_module/payload.h"
		"recover_module/record.h"
		"recover_module/table.h"
		"recovery.h"
		"sandboxed_vfs.h"
		"sandboxed_vfs_file.h"
		"sql_features.h"
		"statement.h"
		"statement_id.h"
		"transaction.h"
		"vfs_wrapper.h"
		COPTS
		${FLDSERVER_DEFAULT_COPTS}
		SRCS
		"database.cc"
		"error_delegate_util.cc"
		"initialization.cc"
		"meta_table.cc"
		"recover_module/btree.cc"
		"recover_module/cursor.cc"
		"recover_module/integers.cc"
		"recover_module/module.cc"
		"recover_module/pager.cc"
		"recover_module/parsing.cc"
		"recover_module/payload.cc"
		"recover_module/record.cc"
		"recover_module/table.cc"
		"recovery.cc"
		"sandboxed_vfs.cc"
		"sandboxed_vfs_file.cc"
		"sql_features.cc"
		"statement.cc"
		"statement_id.cc"
		"transaction.cc"
		"vfs_wrapper.cc"
		"${WINDOWS_SOURCES}"
		"${POSIX_SOURCES}"
		"${LINUX_SOURCES}"
		DEFINES
		DEPS
		Threads::Threads
		PUBLIC
		OBJECT_LIB
)
