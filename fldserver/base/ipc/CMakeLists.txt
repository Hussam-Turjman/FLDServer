# Auto Generated File DO NOT EDIT




if (IS_WIN)
	set(WINDOWS_SOURCES
			"handle_attachment_win.cc"
			"handle_win.cc"
			)
endif ()

if (IS_POSIX)
	set(POSIX_SOURCES
			"ipc_platform_file_attachment_posix.cc"
			)
endif ()

if (IS_LINUX)
	set(LINUX_SOURCES

			)
endif ()

fldserver_cc_library(
		NAME
		ipc
		HDRS
		"handle_attachment_win.h"
		"handle_win.h"
		"ipc_channel.h"
		"ipc_channel_factory.h"
		"ipc_channel_handle.h"
		"ipc_channel_mojo.h"
		"ipc_channel_nacl.h"
		"ipc_channel_proxy.h"
		"ipc_channel_reader.h"
		"ipc_listener.h"
		"ipc_logging.h"
		"ipc_message.h"
		"ipc_message_attachment.h"
		"ipc_message_attachment_set.h"
		"ipc_message_macros.h"
		"ipc_message_null_macros.h"
		"ipc_message_pipe_reader.h"
		"ipc_message_protobuf_utils.h"
		"ipc_message_start.h"
		"ipc_message_support_export.h"
		"ipc_message_templates.h"
		"ipc_message_templates_impl.h"
		"ipc_message_utils.h"
		"ipc_mojo_bootstrap.h"
		"ipc_mojo_handle_attachment.h"
		"ipc_mojo_message_helper.h"
		"ipc_mojo_param_traits.h"
		"ipc_param_traits.h"
		"ipc_platform_file.h"
		"ipc_platform_file_attachment_posix.h"
		"ipc_sender.h"
		"ipc_sync_channel.h"
		"ipc_sync_message.h"
		"ipc_sync_message_filter.h"
		"message_filter.h"
		"message_filter_router.h"
		"message_mojom_traits.h"
		"message_router.h"
		"message_view.h"
		"native_handle_type_converters.h"
		"param_traits_log_macros.h"
		"param_traits_macros.h"
		"param_traits_read_macros.h"
		"param_traits_write_macros.h"
		"struct_constructor_macros.h"
		"trace_ipc_message.h"
		COPTS
		${FLDSERVER_DEFAULT_COPTS}
		SRCS
		"ipc_logging.cc"
		"ipc_message.cc"
		"ipc_platform_file.cc"
		"ipc_sync_message.cc"
		"message_router.cc"
		"message_view.cc"
		"${WINDOWS_SOURCES}"
		"${POSIX_SOURCES}"
		"${LINUX_SOURCES}"
		DEFINES
		DEPS
		Threads::Threads
		PUBLIC
		OBJECT_LIB
)




# Sources       : 31
# Headers       : 46
# Deps          : 0
# Defines       : 0
# Platform      : linux
# OS            : posix
# Architecture  : ('64bit', 'ELF')
