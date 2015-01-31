TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
#    plugin/qwebdav \
    apps/saildav

OTHER_FILES = \
    $$files(rpm/*) \
    LICENSE \
    README.md
