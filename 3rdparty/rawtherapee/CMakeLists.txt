if (WIN32)
  cmake_minimum_required(VERSION 2.8.4)
  cmake_policy(SET CMP0015 OLD)
else (WIN32)
  cmake_minimum_required(VERSION 2.6)
endif (WIN32)

# the default target is 'Debug'
if (CMAKE_BUILD_TYPE STREQUAL "")
  set (CMAKE_BUILD_TYPE Debug CACHE STRING "One of: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

string (TOUPPER ${CMAKE_BUILD_TYPE} UPPER_CMAKE_BUILD_TYPE)

if (UPPER_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  add_definitions (-D_DEBUG)
endif ()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set (CACHE_NAME_SUFFIX "" CACHE STRING "RawTherapee's cache folder suffix (leave empty to use the default suffix, i.e. latesttag)")

# By default, we don't use specific processor target, so  PROC_TARGET_NUMBER is set to 0. If can specify other values to select specific
# processor targets, which list can be found in ProcessorTargets.cmake.
set (PROC_TARGET_NUMBER 0 CACHE STRING "Selected target processor from the list above (taken from ProcessorTargets.cmake)")

# The following line set special compilation flags for RTEngine, and will be added to CMAKE_CXX_FLAGS
# It were moved away from rtengine/CMakefiles.txt, because some users may want to remove -ffast_math :
# this flag speeds up the floating-point operations, but with a little bite less precisions. This default value
# gives the same result/behaviour as before.
set (RTENGINE_CXX_FLAGS "-ffast-math -funroll-loops" CACHE STRING "Special compilation flags for RTEngine")

#loading the processor targets list
include (ProcessorTargets.cmake)
set (PROC_LABEL "undefined" CACHE STRING "Target processor label, unused if PROC_TARGET_NUMBER = 0 or 2")
set (PROC_FLAGS "" CACHE STRING "Target processor related build/link flags")
if ((NOT(PROC_TARGET_NUMBER EQUAL 0)) AND (NOT(PROC_TARGET_NUMBER EQUAL 2)))
	set (PROC_LABEL ${PROC_TARGET_${PROC_TARGET_NUMBER}_LABEL})
endif ()
if (NOT(PROC_TARGET_NUMBER EQUAL 0))
	set (PROC_FLAGS ${PROC_TARGET_${PROC_TARGET_NUMBER}_FLAGS})
endif ()

# if it exists, the PROC_FORCED_LABEL value is loaded in PROC_LABEL to override the one of ProcessorTargets
if (DEFINED PROC_FORCED_LABEL)
  set (PROC_LABEL ${PROC_FORCED_LABEL})
endif (DEFINED PROC_FORCED_LABEL)

# adding the proc flags to the build flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROC_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PROC_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PROC_FLAGS}")

if (WIN32)
    # we additionnally look in the MinGW path first then in the Gtkmm path,
    # so if you wish to build some dependant library, you have to install them in MinGW to use them
    set(CMAKE_PREFIX_PATH $ENV{MINGW_BASEPATH} $ENV{GTKMM_BASEPATH} CACHE STRING "Additional search paths")
endif (WIN32)

if (APPLE)
#    SET (CMAKE_OSX_ARCHITECTURES "i386;x86_64;" )
#    SET (CMAKE_TRY_COMPILE_OSX_ARCHITECTURES "i386;x86_64;" )
    SET (CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.5.sdk")
    SET (CMAKE_OSX_DEPLOYMENT_TARGET "10.5")
endif (APPLE)

option(USE_CLANG "Build rawtherapee with clang" OFF)
option (AUTOMATED_BUILD_SYSTEM "TRUE if built by an automate" OFF)

option (BUILD_SHARED_LIBS "shared libs" ON)
option (BUILD_SHARED "Build rawtherapee with shared libraries" ON)
option (WITH_BZIP "Build with Bzip2 support" ON)
option (WITH_MYFILE_MMAP "Build using memory mapped file" ON)
option (OPTION_OMP "Build with OpenMP support" ON)
option (BUILD_BUNDLE "Self-contained build" OFF)

# set install directories
if (WIN32 OR APPLE)
    set (BUILD_BUNDLE ON FORCE)
endif(WIN32 OR APPLE)

if (NOT DEFINED BUNDLE_BASE_INSTALL_DIR)
    set (BUNDLE_BASE_INSTALL_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
endif (NOT DEFINED BUNDLE_BASE_INSTALL_DIR)

if (BUILD_BUNDLE)
    set (BINDIR .)
    set (CMAKE_INSTALL_PREFIX ${BUNDLE_BASE_INSTALL_DIR})
endif (BUILD_BUNDLE)

if (NOT DEFINED BINDIR)
    set (BINDIR  ${CMAKE_INSTALL_PREFIX}/bin)
endif (NOT DEFINED BINDIR)

if (NOT DEFINED DATADIR)
    if (BUILD_BUNDLE)
        set (DATADIR .)
    else (BUILD_BUNDLE)
        set (DATADIR ${CMAKE_INSTALL_PREFIX}/share/rawtherapee)
    endif (BUILD_BUNDLE)
endif (NOT DEFINED DATADIR)

if (NOT DEFINED LIBDIR)
    if (BUILD_BUNDLE)
        set (LIBDIR .)
    else (BUILD_BUNDLE)
        # Respect CMAKE_INSTALL_LIBDIR if set
        if (DEFINED CMAKE_INSTALL_LIBDIR)
	    if (IS_ABSOLUTE ${LIBDIR})
            	set (LIBDIR ${CMAKE_INSTALL_LIBDIR})
	    else (IS_ABSOLUTE ${LIBDIR})
		set (LIBDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
	    endif (IS_ABSOLUTE ${LIBDIR})
        else (DEFINED CMAKE_INSTALL_LIBDIR)
            set (LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
        endif (DEFINED CMAKE_INSTALL_LIBDIR)
    endif (BUILD_BUNDLE)
endif (NOT DEFINED LIBDIR)

if (NOT DEFINED DOCDIR)
    if (BUILD_BUNDLE)
        set (DOCDIR ./doc)
    else (BUILD_BUNDLE)
        set (DOCDIR ${CMAKE_INSTALL_PREFIX}/share/doc/rawtherapee)
    endif (BUILD_BUNDLE)
endif (NOT DEFINED DOCDIR)

if (NOT DEFINED CREDITSDIR)
    if (BUILD_BUNDLE)
        set (CREDITSDIR .)
    else (BUILD_BUNDLE)
        set (CREDITSDIR ${CMAKE_INSTALL_PREFIX}/share/doc/rawtherapee)
    endif (BUILD_BUNDLE)
endif (NOT DEFINED CREDITSDIR)

if (NOT DEFINED LICENCEDIR)
    if (BUILD_BUNDLE)
        set (LICENCEDIR .)
    else (BUILD_BUNDLE)
        set (LICENCEDIR ${CMAKE_INSTALL_PREFIX}/share/doc/rawtherapee)
    endif (BUILD_BUNDLE)
endif (NOT DEFINED LICENCEDIR)

if (NOT DEFINED DESKTOPDIR)
    if (UNIX)
        set (DESKTOPDIR ${CMAKE_INSTALL_PREFIX}/share/applications)
    endif (UNIX)
endif (NOT DEFINED DESKTOPDIR)

if (NOT DEFINED ICONSDIR)
    if (UNIX)
        set (ICONSDIR ${CMAKE_INSTALL_PREFIX}/share/icons)
    endif (UNIX)
endif (NOT DEFINED ICONSDIR)

# non-bundle builds has to use absolute paths
if (NOT BUILD_BUNDLE AND
    NOT (IS_ABSOLUTE ${BINDIR} AND IS_ABSOLUTE ${DATADIR} AND IS_ABSOLUTE ${LIBDIR} AND
    IS_ABSOLUTE ${DOCDIR} AND IS_ABSOLUTE ${CREDITSDIR} AND IS_ABSOLUTE ${LICENCEDIR}))
    message (FATAL_ERROR "The paths has to be absolute or use -DBUILD_BUNDLE=ON")
endif ()

# check for libraries
find_package(PkgConfig)
pkg_check_modules (GTK     REQUIRED gtk+-2.0>=2.12)
pkg_check_modules (GLIB2   REQUIRED glib-2.0>=2.16)
pkg_check_modules (GLIBMM  REQUIRED glibmm-2.4>=2.16)
pkg_check_modules (GTKMM   REQUIRED gtkmm-2.4>=2.12)
pkg_check_modules (GIO     REQUIRED gio-2.0>=2.16)
pkg_check_modules (GIOMM   REQUIRED giomm-2.4>=2.12)
pkg_check_modules (GTHREAD REQUIRED gthread-2.0>=2.16)
pkg_check_modules (GOBJECT REQUIRED gobject-2.0>=2.16)
pkg_check_modules (SIGC    REQUIRED sigc++-2.0)
# NOTE: The new mechanism has been tested with BUILD_SHARED = OFF
if (WIN32)
    add_definitions (-DWIN32)
    add_definitions (-D_WIN32)
    if (MINGW)
        add_definitions (-D__MINGW32__)
    endif (MINGW)

    set (EXTRA_LIB "-lws2_32")
endif (WIN32)
# you may need lcms v1.xx for older version : pkg_check_modules (LCMS REQUIRED lcms<=1.99)
pkg_check_modules (LCMS REQUIRED lcms2)
pkg_check_modules (IPTCDATA REQUIRED libiptcdata)
find_package (JPEG REQUIRED)
find_package (PNG REQUIRED)
find_package (TIFF REQUIRED)
find_package (ZLIB REQUIRED)



# link witz bzip
if (WITH_BZIP)
  find_package(BZip2)
  if (BZIP2_FOUND)
    add_definitions (-DBZIP_SUPPORT)
    set (EXTRA_INCDIR ${EXTRA_LIB} ${BZIP2_INCLUDE_DIR})
    set (EXTRA_LIB ${EXTRA_LIB} ${BZIP2_LIBRARIES})
  endif (BZIP2_FOUND)
endif (WITH_BZIP)

if (WITH_MYFILE_MMAP)
	add_definitions (-DMYFILE_MMAP)
endif (WITH_MYFILE_MMAP)

if (OPTION_OMP)
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif (OPENMP_FOUND)
endif (OPTION_OMP)

if(USE_CLANG)
    find_file(CLANG_CMD clang)
    find_file(CLANGPP_CMD clang++)

    if ((CLANG_CMD STREQUAL CLANG_CMD-NOTFOUND) OR (CLANGPP_CMD STREQUAL CLANGPP_CMD-NOTFOUND))
      message(FATAL_ERROR "clang not found! auto-disabling option USE_CLANG")
    else ((CLANG_CMD STREQUAL CLANG_CMD-NOTFOUND) OR (CLANGPP_CMD STREQUAL CLANGPP_CMD-NOTFOUND))
      message(STATUS "clang found: ${CLANG_CMD}")
      message(STATUS "clang++ found: ${CLANGPP_CMD}")

      SET (CMAKE_C_COMPILER             ${CLANG_CMD})
      SET (CMAKE_C_FLAGS                "") #-Wall
      SET (CMAKE_C_FLAGS_DEBUG          "-O0 -g")
      SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
      SET (CMAKE_C_FLAGS_RELEASE        "-O3 -DNDEBUG")
      SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

      SET (CMAKE_CXX_COMPILER             ${CLANGPP_CMD})
      SET (CMAKE_CXX_FLAGS                "") #-Wall
      SET (CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
      SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
      SET (CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
      SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    endif ((CLANG_CMD STREQUAL CLANG_CMD-NOTFOUND) OR (CLANGPP_CMD STREQUAL CLANGPP_CMD-NOTFOUND))

    find_file(LDGOLD_CMD ld.gold)
    if (LDGOLD_CMD STREQUAL LDGOLD_CMD-NOTFOUND)
      message(FATAL_ERROR "ld.gold not found! using standard linker")
    else (LDGOLD_CMD STREQUAL LDGOLD_CMD-NOTFOUND)
      message(STATUS "ld.gold found: ${LDGOLD_CMD}")
    SET (CMAKE_LINKER  LDGOLD_CMD)
    endif (LDGOLD_CMD STREQUAL LDGOLD_CMD-NOTFOUND)
endif(USE_CLANG)

install (FILES AUTHORS.txt DESTINATION ${CREDITSDIR})
install (FILES LICENSE.txt DESTINATION ${LICENCEDIR})
install (FILES AboutThisBuild.txt DESTINATION ${CREDITSDIR})
install (FILES RELEASE_NOTES.txt DESTINATION ${CREDITSDIR} OPTIONAL)

add_subdirectory (rtexif)
add_subdirectory (rtengine)
