fldserver_cc_library(
        NAME
        nix
        HDRS
        "mime_util_xdg.h"
        "xdg_mime/xdgmime.h"
        "xdg_mime/xdgmimealias.h"
        "xdg_mime/xdgmimecache.h"
        "xdg_mime/xdgmimeglob.h"
        "xdg_mime/xdgmimeicon.h"
        "xdg_mime/xdgmimeint.h"
        "xdg_mime/xdgmimemagic.h"
        "xdg_mime/xdgmimeparent.h"
        "xdg_user_dirs/xdg_user_dir_lookup.h"
        "xdg_util.h"
        COPTS
        ${FLDSERVER_DEFAULT_COPTS}
        SRCS
        "mime_util_xdg.cc"
        "xdg_mime/xdgmime.c"
        "xdg_mime/xdgmimealias.c"
        "xdg_mime/xdgmimecache.c"
        "xdg_mime/xdgmimeglob.c"
        "xdg_mime/xdgmimeicon.c"
        "xdg_mime/xdgmimeint.c"
        "xdg_mime/xdgmimemagic.c"
        "xdg_mime/xdgmimeparent.c"
        "xdg_user_dirs/xdg_user_dir_lookup.cc"
        "xdg_util.cc"
        DEFINES
        DEPS
        Threads::Threads
        PUBLIC
        OBJECT_LIB
)
