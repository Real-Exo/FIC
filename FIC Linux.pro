TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        App/ChecksumFile/ChecksumFile.cpp \
        App/CommandLine/ArgumentBuilder.cpp \
        App/CommandLine/BuildArguments.cpp \
        App/CommandLine/CommandLineInterpreter.cpp \
        App/Comparator/FileComparator.cpp \
        App/Comparator/PlainFileComparator.cpp \
        App/Comparator/ZipFileComparator.cpp \
        App/FIC.cpp \
        App/FatalException.cpp \
        App/Log/Log.cpp \
        App/Master/ChecksumMaster.cpp \
        App/Master/FileMaster.cpp \
        App/Master/Master.cpp \
        App/Master/PlainFileMaster.cpp \
        App/Master/ZipFileMaster.cpp \
        App/Platform/Linux.cpp \
        App/Print.cpp \
        App/Slave/ChecksumSlave.cpp \
        App/Slave/FileSlave.cpp \
        App/Slave/PlainFileSlave.cpp \
        App/Slave/Slave.cpp \
        App/Slave/ZipFileSlave.cpp \
        App/SourceBuilder.cpp \
        Libs/Exception/Exception.cpp \
        Libs/Util/CRC32/CRC32.cpp

HEADERS += \
    App/ChecksumFile/ChecksumFile.h \
    App/CommandLine/ArgumentBuilder.h \
    App/CommandLine/CommandLineInterpreter.h \
    App/Common/CompareResult.h \
    App/Comparator/FileComparator.h \
    App/Comparator/PlainFileComparator.h \
    App/Comparator/ZipFileComparator.h \
    App/FIC.h \
    App/Log/Log.h \
    App/Master/ChecksumMaster.h \
    App/Master/FileMaster.h \
    App/Master/Master.h \
    App/Master/PlainFileMaster.h \
    App/Master/ZipFileMaster.h \
    App/Platform/Platform.h \
    App/Print.h \
    App/Slave/ChecksumSlave.h \
    App/Slave/FileSlave.h \
    App/Slave/PlainFileSlave.h \
    App/Slave/Slave.h \
    App/Slave/ZipFileSlave.h \
    App/SourceBuilder.h \
    Libs/Exception/Exception.h \
    Libs/Util/CRC32/CRC32.h \
    Libs/Util/Narrow.h \
    Libs/Util/UniqueResource.h

LIBS += -lzip
