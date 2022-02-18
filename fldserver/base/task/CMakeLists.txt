if (IS_WIN)
    set(WINDOWS_SOURCES
            "thread_pool/thread_group_native_win.cc"
            )
endif ()

if (IS_POSIX)
    set(POSIX_SOURCES
            "thread_pool/task_tracker_posix.cc"


            )
endif ()

if (IS_LINUX)
    set(LINUX_SOURCES

            )
endif ()

fldserver_cc_library(
        NAME
        task
        HDRS
        "cancelable_task_tracker.h"
        "common/checked_lock.h"
        "common/checked_lock_impl.h"
        "common/intrusive_heap.h"
        "common/operations_controller.h"
        "common/scoped_defer_task_posting.h"
        "common/task_annotator.h"
        "current_thread.h"
        "lazy_thread_pool_task_runner.h"
        "post_job.h"
        "post_task.h"
        "scoped_set_task_priority_for_current_thread.h"
        "sequence_manager/associated_thread_id.h"
        "sequence_manager/atomic_flag_set.h"
        "sequence_manager/enqueue_order.h"
        "sequence_manager/enqueue_order_generator.h"
        "sequence_manager/lazily_deallocated_deque.h"
        "sequence_manager/lazy_now.h"
        "sequence_manager/real_time_domain.h"
        "sequence_manager/sequence_manager.h"
        "sequence_manager/sequence_manager_impl.h"
        "sequence_manager/sequenced_task_source.h"
        "sequence_manager/task_queue.h"
        "sequence_manager/task_queue_impl.h"
        "sequence_manager/task_queue_selector.h"
        "sequence_manager/task_queue_selector_logic.h"
        "sequence_manager/task_time_observer.h"
        "sequence_manager/tasks.h"
        "sequence_manager/thread_controller.h"
        "sequence_manager/thread_controller_impl.h"
        "sequence_manager/thread_controller_power_monitor.h"
        "sequence_manager/thread_controller_with_message_pump_impl.h"
        "sequence_manager/time_domain.h"
        "sequence_manager/work_deduplicator.h"
        "sequence_manager/work_queue.h"
        "sequence_manager/work_queue_sets.h"
        "simple_task_executor.h"
        "single_thread_task_executor.h"
        "single_thread_task_runner_thread_mode.h"
        "task_executor.h"
        "task_features.h"
        "task_observer.h"
        "task_traits.h"
        "task_traits_extension.h"
        "thread_pool.h"
        "thread_pool/delayed_task_manager.h"
        "thread_pool/environment_config.h"
        "thread_pool/initialization_util.h"
        "thread_pool/job_task_source.h"
        "thread_pool/pooled_parallel_task_runner.h"
        "thread_pool/pooled_sequenced_task_runner.h"
        "thread_pool/pooled_single_thread_task_runner_manager.h"
        "thread_pool/pooled_task_runner_delegate.h"
        "thread_pool/priority_queue.h"
        "thread_pool/sequence.h"
        "thread_pool/service_thread.h"
        "thread_pool/task.h"
        "thread_pool/task_source.h"
        "thread_pool/task_source_sort_key.h"
        "thread_pool/task_tracker.h"
        "thread_pool/task_tracker_posix.h"
        "thread_pool/thread_group.h"
        "thread_pool/thread_group_impl.h"
        "thread_pool/thread_group_native.h"
        "thread_pool/thread_group_native_win.h"
        "thread_pool/thread_pool_impl.h"
        "thread_pool/thread_pool_instance.h"
        "thread_pool/tracked_ref.h"
        "thread_pool/worker_thread.h"
        "thread_pool/worker_thread_observer.h"
        "thread_pool/worker_thread_stack.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "cancelable_task_tracker.cc"
        "common/checked_lock_impl.cc"
        "common/operations_controller.cc"
        "common/scoped_defer_task_posting.cc"
        "common/task_annotator.cc"
        "current_thread.cc"
        "lazy_thread_pool_task_runner.cc"
        "post_job.cc"
        "post_task.cc"
        "scoped_set_task_priority_for_current_thread.cc"
        "sequence_manager/associated_thread_id.cc"
        "sequence_manager/atomic_flag_set.cc"
        "sequence_manager/enqueue_order_generator.cc"
        "sequence_manager/lazy_now.cc"
        "sequence_manager/real_time_domain.cc"
        "sequence_manager/sequence_manager.cc"
        "sequence_manager/sequence_manager_impl.cc"
        "sequence_manager/task_queue.cc"
        "sequence_manager/task_queue_impl.cc"
        "sequence_manager/task_queue_selector.cc"
        "sequence_manager/tasks.cc"
        "sequence_manager/thread_controller.cc"
        "sequence_manager/thread_controller_impl.cc"
        "sequence_manager/thread_controller_power_monitor.cc"
        "sequence_manager/thread_controller_with_message_pump_impl.cc"
        "sequence_manager/time_domain.cc"
        "sequence_manager/work_deduplicator.cc"
        "sequence_manager/work_queue.cc"
        "sequence_manager/work_queue_sets.cc"
        "simple_task_executor.cc"
        "single_thread_task_executor.cc"
        "task_executor.cc"
        "task_features.cc"
        "task_traits.cc"
        "thread_pool/delayed_task_manager.cc"
        "thread_pool/environment_config.cc"
        "thread_pool/initialization_util.cc"
        "thread_pool/job_task_source.cc"
        "thread_pool/pooled_parallel_task_runner.cc"
        "thread_pool/pooled_sequenced_task_runner.cc"
        "thread_pool/pooled_single_thread_task_runner_manager.cc"
        "thread_pool/pooled_task_runner_delegate.cc"
        "thread_pool/priority_queue.cc"
        "thread_pool/sequence.cc"
        "thread_pool/service_thread.cc"
        "thread_pool/task.cc"
        "thread_pool/task_source.cc"
        "thread_pool/task_source_sort_key.cc"
        "thread_pool/thread_group.cc"
        "thread_pool/thread_group_impl.cc"
        "thread_pool/thread_group_native.cc"
        "thread_pool/thread_pool_impl.cc"
        "thread_pool/thread_pool_instance.cc"
        "thread_pool/worker_thread.cc"
        "thread_pool/worker_thread_stack.cc"
        # error C2338: tuple index out of bounds on windows
        "thread_pool.cc"
        # error C2338: tuple index out of bounds on windows
        "thread_pool/task_tracker.cc"
        "${WINDOWS_SOURCES}"
        "${LINUX_SOURCES}"
        "${POSIX_SOURCES}"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)


