#include(common.pri)

TEMPLATE = subdirs

SUBDIRS += \
    apps/saildav

OTHER_FILES = \
    $$files(rpm/*) \
    LICENSE
