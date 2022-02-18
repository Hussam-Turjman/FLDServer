# Copyright (c) 2021 The Core project Authors. All rights reserved. Use of this
# source code is governed by a BSD-style license that can be found in the LICENSE file.

add_subdirectory(dynamic_annotations)
add_subdirectory(modp_b64)
add_subdirectory(threading)
add_subdirectory(containers)
add_subdirectory(nspr)
add_subdirectory(time)
add_subdirectory(files)
add_subdirectory(system)
add_subdirectory(ced)
add_subdirectory(cityhash)
add_subdirectory(cityhash_v103)
add_subdirectory(hash)
add_subdirectory(i18n)
add_subdirectory(json)
add_subdirectory(memory)
add_subdirectory(message_loop)
add_subdirectory(metrics)
add_subdirectory(process)
add_subdirectory(double_conversion)
add_subdirectory(strings)
add_subdirectory(timer)
add_subdirectory(util)
add_subdirectory(power_monitor)
add_subdirectory(task)
add_subdirectory(debug)
add_subdirectory(synchronization)
add_subdirectory(crypto)
add_subdirectory(sql)
add_subdirectory(ipc)

set(EXTRA_INTERNAL_LIBS)

if (IS_POSIX)
	list(APPEND EXTRA_INTERNAL_LIBS fldserver::posix)
	list(APPEND EXTRA_INTERNAL_LIBS fldserver::nix)
	add_subdirectory(posix)
	add_subdirectory(nix)
endif ()

if (IS_WIN)
	add_subdirectory(win)
	list(APPEND EXTRA_INTERNAL_LIBS fldserver::win)
endif ()


if (IS_WIN)
	set(WINDOWS_SOURCES
			"base_paths_win.cc"
			"enterprise_util_win.cc"
			"file_version_info_win.cc"
			"logging_win.cc"
			"native_library_win.cc"
			"rand_util_win.cc"
			"scoped_clear_last_error_win.cc"
			"sync_socket_win.cc"
			)
endif ()

if (IS_POSIX)
	set(POSIX_SOURCES
			"base_paths_posix.cc"
			"cpu_affinity_posix.cc"
			"native_library_posix.cc"
			"rand_util_posix.cc"
			"sync_socket_posix.cc"
			"file_descriptor_store.cc"

			)
endif ()

if (IS_LINUX)
	set(LINUX_SOURCES
			"linux_util.cc"
			)
endif ()


fldserver_cc_library(
		NAME
		base
		HDRS
		"as_const.h"
		"at_exit.h"
		"atomic_ref_count.h"
		"atomic_sequence_num.h"
		"atomicops.h"
		"atomicops_internals_atomicword_compat.h"
		"atomicops_internals_portable.h"
		"auto_reset.h"
		"barrier_closure.h"
		"base64.h"
		"base64url.h"
		"base_paths.h"
		"base_paths_posix.h"
		"base_paths_win.h"
		"base_switches.h"
		"big_endian.h"
		"bind.h"
		"bind_internal.h"
		"bind_post_task.h"
		"bind_post_task_internal.h"
		"bit_cast.h"
		"bits.h"
		"build_time.h"
		"callback.h"
		"callback_forward.h"
		"callback_helpers.h"
		"callback_internal.h"
		"callback_list.h"
		"cancelable_callback.h"
		"check.h"
		"check_op.h"
		"command_line.h"
		"compiler_specific.h"
		"component_export.h"
		"cpu.h"
		"cpu_affinity_posix.h"
		"critical_closure.h"
		"dcheck_is_on.h"
		"deferred_sequenced_task_runner.h"
		"enterprise_util.h"
		"environment.h"
		"export_template.h"
		"feature_list.h"
		"file_descriptor_posix.h"
		"file_descriptor_store.h"
		"file_version_info.h"
		"file_version_info_win.h"
		"format_macros.h"
		"guid.h"
		"immediate_crash.h"
		"integral_types.h"
		"lazy_instance.h"
		"lazy_instance_helpers.h"
		"linux_util.h"
		"location.h"
		"logging.h"
		"logging_win.h"
		"macros.h"
		"native_library.h"
		"no_destructor.h"
		"notreached.h"
		"observer_list.h"
		"observer_list_internal.h"
		"observer_list_threadsafe.h"
		"observer_list_types.h"
		"one_shot_event.h"
		"optional.h"
		"parameter_pack.h"
		"path_service.h"
		"pending_task.h"
		"pickle.h"
		"post_task_and_reply_with_result_internal.h"
		"rand_util.h"
		"run_loop.h"
		"scoped_clear_last_error.h"
		"scoped_generic.h"
		"scoped_multi_source_observation.h"
		"scoped_native_library.h"
		"scoped_observation.h"
		"scoped_observer.h"
		"sequence_checker.h"
		"sequence_checker_impl.h"
		"sequence_token.h"
		"sequenced_task_runner.h"
		"sequenced_task_runner_helpers.h"
		"single_thread_task_runner.h"
		"stl_util.h"
		"supports_user_data.h"
		"sync_socket.h"
		"sys_byteorder.h"
		"syslog_logging.h"
		"task_runner.h"
		"task_runner_util.h"
		"template_util.h"
		"thread_annotations.h"
		"token.h"
		"traits_bag.h"
		"tuple.h"
		"unguessable_token.h"
		"updateable_sequenced_task_runner.h"
		"value_iterators.h"
		"values.h"
		"version.h"
		"vlog.h"
		COPTS
		${FLDSERVER_DEFAULT_COPTS}
		SRCS
		"superfasthash/superfasthash.c"
		"at_exit.cc"
		"barrier_closure.cc"
		"base64.cc"
		"base64url.cc"
		"base_paths.cc"
		"base_switches.cc"
		"big_endian.cc"
		"build_time.cc"
		"callback_helpers.cc"
		"callback_internal.cc"
		"callback_list.cc"
		"check.cc"
		# "check_example.cc"
		"check_op.cc"
		"command_line.cc"
		"cpu.cc"
		"deferred_sequenced_task_runner.cc"
		"environment.cc"
		"feature_list.cc"

		"guid.cc"
		"lazy_instance_helpers.cc"
		"location.cc"
		"logging.cc"
		"native_library.cc"
		"notreached.cc"
		"observer_list_internal.cc"
		"observer_list_threadsafe.cc"
		"observer_list_types.cc"
		"one_shot_event.cc"
		"path_service.cc"
		"pending_task.cc"
		"pickle.cc"
		"rand_util.cc"
		"run_loop.cc"
		"scoped_native_library.cc"
		"sequence_checker_impl.cc"
		"sequence_token.cc"
		"sequenced_task_runner.cc"
		"supports_user_data.cc"
		"sync_socket.cc"
		"syslog_logging.cc"
		"task_runner.cc"
		"token.cc"
		"unguessable_token.cc"
		"value_iterators.cc"
		"values.cc"
		"version.cc"
		"vlog.cc"
		"bits.cc"
		"${POSIX_SOURCES}"
		"${WINDOWS_SOURCES}"
		"${LINUX_SOURCES}"
		DEFINES
		DEPS
		# Third Party
		Threads::Threads
		${ICU_LIBRARIES}
		${SQLite3_LIBRARIES}
		libevent::core
		absl::strings
		absl::container
		absl::hash
		absl::memory
		absl::meta
		absl::numeric
		absl::status
		absl::synchronization
		absl::time
		OpenSSL::SSL
		OpenSSL::Crypto
		${ZLIB_LIBRARIES}
		${LIBXML2_LIBRARIES}
		${Protobuf_LIBRARIES}
		# Internal
		OBJECT_DEPS
		fldserver::base_crypto
		fldserver::ced
		fldserver::dynamic_annotations
		fldserver::modp_b64
		fldserver::threading
		fldserver::containers
		fldserver::nspr
		fldserver::base_time
		fldserver::files
		fldserver::base_system
		fldserver::cityhash
		fldserver::cityhash_v103
		fldserver::hash
		fldserver::i18n
		fldserver::json
		fldserver::base_memory
		fldserver::message_loop
		fldserver::metrics
		fldserver::process
		fldserver::double_conversion
		fldserver::base_strings
		fldserver::timer
		fldserver::util
		fldserver::power_monitor
		fldserver::task
		fldserver::debug
		fldserver::sql
		fldserver::synchronization
		fldserver::ipc
		${EXTRA_INTERNAL_LIBS}
		PUBLIC
)


target_link_libraries(base PUBLIC ${CMAKE_DL_LIBS})