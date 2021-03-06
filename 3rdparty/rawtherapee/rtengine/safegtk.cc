/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *  Copyright (c) 2010 Sasha Vasko <sasha@aftercode.net>
 *  Copyright (c) 2010 Oliver Duis <www.oliverduis.de>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "safegtk.h"
#include <glib/gstdio.h>
#include <fcntl.h>
#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <cstdio>
#endif
#include <memory>
  /*
  Glib::ustring removeExtension (const Glib::ustring& filename) {

      Glib::ustring bname = Glib::path_get_basename(filename);
      int lastdot = bname.find_last_of ('.');
      if (lastdot!=bname.npos)
	  return filename.substr (0, filename.size()-(bname.size()-lastdot));
      else
	  return filename;
  }

  Glib::RefPtr<Gdk::Pixbuf> safe_create_from_file(const Glib::ustring& filename)
  {
	  Glib::RefPtr<Gdk::Pixbuf> res;
	  Glib::ustring path = RTImage::findIconAbsolutePath(filename);
	  if (path.length()) {
		  try {
			  res = Gdk::Pixbuf::create_from_file (path);
		  }
		  catch (Glib::Exception& ex) {
			  printf ("ERROR: (ustring) File \"%s\" not found.\n", ex.what().c_str());
		  }
	  }
	  return res;
  }*/
/*
Cairo::RefPtr<Cairo::ImageSurface> safe_create_from_png(const Glib::ustring& filename)
{
	Cairo::RefPtr<Cairo::ImageSurface> res;
	Glib::ustring path = RTImage::findIconAbsolutePath(filename);
	if (path.length()) {
		// files_test need a std::string which (as stated in its proto) but will only work if
		// we use the Glib::ustring filename !?
		try {
			// create_from_png need a std::string converted from UTF8 with safe_locale_from_utf8
			res = Cairo::ImageSurface::create_from_png (safe_locale_from_utf8(path));
		} catch (...) {
			printf("ERROR: (ustring) File \"%s\" not found.\n", path.c_str());
		}
	}
	return res;
}
*/
Glib::RefPtr<Gio::FileInfo> safe_query_file_info (Glib::RefPtr<Gio::File> &file)
{
		Glib::RefPtr<Gio::FileInfo> info;
#ifdef GLIBMM_EXCEPTIONS_ENABLED
		try {	info = file->query_info();	}catch (...) {	}
#else
		std::auto_ptr<Glib::Error> error;
		info = file->query_info("*", Gio::FILE_QUERY_INFO_NONE, error);
#endif
		return info;
}

#ifdef GLIBMM_EXCEPTIONS_ENABLED
# define SAFE_ENUMERATOR_CODE_START \
				do{try {	if ((dirList = dir->enumerate_children ())) \
						for (Glib::RefPtr<Gio::FileInfo> info = dirList->next_file(); info; info = dirList->next_file()) {
						
# define SAFE_ENUMERATOR_CODE_END \
				}}	catch (Glib::Exception& ex) {	printf ("%s\n", ex.what().c_str());	}}while(0)
#else
# define SAFE_ENUMERATOR_CODE_START \
				do{std::auto_ptr<Glib::Error> error;	Glib::RefPtr<Gio::Cancellable> cancellable; \
					if ((dirList = dir->enumerate_children (cancellable, "*", Gio::FILE_QUERY_INFO_NONE, error))) \
						for (Glib::RefPtr<Gio::FileInfo> info = dirList->next_file(cancellable, error); !error.get() && info; info = dirList->next_file(cancellable, error)) {
						
# define SAFE_ENUMERATOR_CODE_END 	} if (error.get())	printf ("%s\n", error->what().c_str());}while (0)
#endif
/*
void safe_build_file_list (Glib::RefPtr<Gio::File> &dir, std::vector<FileMTimeInfo> &flist)
{
	Glib::RefPtr<Gio::FileEnumerator> dirList;
	if (dir) {
		SAFE_ENUMERATOR_CODE_START
				flist.push_back (FileMTimeInfo (removeExtension(info->get_name()), info->modification_time()));
		SAFE_ENUMERATOR_CODE_END;
	}
}

/*
 * safe_build_file_list can now filter out at the source all files that doesn't have the extensions specified (if provided)
 */
/*
void safe_build_file_list (Glib::RefPtr<Gio::File> &dir, std::vector<Glib::ustring> &names, const Glib::ustring &directory, const std::vector<Glib::ustring> *extensions)
{
	Glib::RefPtr<Gio::FileEnumerator> dirList;

	if (dir) {
		if (!extensions) {
			SAFE_ENUMERATOR_CODE_START
				names.push_back (Glib::build_filename (directory, info->get_name()));
			SAFE_ENUMERATOR_CODE_END;
		}
		else {
			// convert extensions to lowercase in a new vector list
			std::vector<Glib::ustring> lcExtensions;
			for (unsigned int i=0; i<extensions->size(); i++)
				lcExtensions.push_back ((*extensions)[i].lowercase());

			SAFE_ENUMERATOR_CODE_START
			// convert the current filename to lowercase in a new ustring
			Glib::ustring fname = Glib::ustring(info->get_name()).lowercase();

			int pos = fname.find_last_of('.');
			if (pos > -1 && pos < (fname.length()-1)) {
				// there is an extension to the filename

				Glib::ustring lcFileExt = fname.substr(pos+1).lowercase();

				// look out if it has one of the retained extensions
				for (unsigned int i=0; i<lcExtensions.size(); i++) {
					if (lcFileExt == lcExtensions[i]) {
						names.push_back (Glib::build_filename (directory, info->get_name()));
						break;
					}
				}
			}
			SAFE_ENUMERATOR_CODE_END;
		}
	}
}

*/
void safe_build_subdir_list (Glib::RefPtr<Gio::File> &dir, std::vector<Glib::ustring> &subDirs, bool add_hidden)
{
	Glib::RefPtr<Gio::FileEnumerator> dirList;
	if (dir)
	{
		// CD-ROMs with no drive inserted are reported, but do not exist, causing RT to crash
		 if (!safe_file_test(dir->get_path(),Glib::FILE_TEST_EXISTS)) return;

				SAFE_ENUMERATOR_CODE_START
						if (info->get_file_type() == Gio::FILE_TYPE_DIRECTORY && (!info->is_hidden() || add_hidden))
								subDirs.push_back (info->get_name());
				SAFE_ENUMERATOR_CODE_END;
	}
}

/*
 * For an unknown reason, Glib::filename_to_utf8 doesn't work on Windows, so we're using
 * Glib::filename_to_utf8 for Linux/Apple and Glib::locale_to_utf8 for Windows
 */
Glib::ustring safe_filename_to_utf8 (const std::string& src)
{
	Glib::ustring utf8_str;
#ifdef WIN32
#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try {
		utf8_str = Glib::locale_to_utf8(src);
	}
	catch (const Glib::ConvertError& e) {
		utf8_str = Glib::convert_with_fallback(src, "UTF8", "LATIN1","?");
	}
#else
	{
		std::auto_ptr<Glib::Error> error;
		utf8_str = locale_to_utf8(src, error);
		if (error.get())
			utf8_str = Glib::convert_with_fallback(src, "UTF8", "LATIN1","?", error);
	}
#endif //GLIBMM_EXCEPTIONS_ENABLED
#else
	utf8_str = Glib::filename_to_utf8(src);
#endif
	return utf8_str;
}

Glib::ustring safe_locale_to_utf8 (const std::string& src)
{
	Glib::ustring utf8_str;
#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try {
		utf8_str = Glib::locale_to_utf8(src);
	}
	catch (const Glib::ConvertError& e) {
		utf8_str = Glib::convert_with_fallback(src, "UTF8", "LATIN1","?");
	}
#else
	{
		std::auto_ptr<Glib::Error> error;
		utf8_str = locale_to_utf8(src, error);
		if (error.get())
			utf8_str = Glib::convert_with_fallback(src, "UTF8", "LATIN1","?", error);
	}
#endif //GLIBMM_EXCEPTIONS_ENABLED
	return utf8_str;
}

std::string safe_locale_from_utf8 (const Glib::ustring& utf8_str)
{
	std::string str;
#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try {
		str = Glib::locale_from_utf8(utf8_str);
	}
	catch (const Glib::ConvertError& e) {
		//str = Glib::convert_with_fallback(utf8_str, "LATIN1", "UTF8", "?");
	}
#else
	{
		std::auto_ptr<Glib::Error> error;
            str = Glib::locale_from_utf8(utf8_str, error);
            /*if (error.get())
                {str = Glib::convert_with_fallback(utf8_str, "LATIN1", "UTF8", "?", error);}*/
	}
#endif //GLIBMM_EXCEPTIONS_ENABLED
	return str;
}

bool safe_spawn_command_line_async (const Glib::ustring& cmd_utf8)
{
		std::string cmd;
		bool success = false;
#ifdef GLIBMM_EXCEPTIONS_ENABLED
		try {
				cmd = Glib::filename_from_utf8(cmd_utf8);
				printf ("command line: %s\n", cmd.c_str());
				Glib::spawn_command_line_async (cmd.c_str());
				success = true;
		} catch (Glib::Exception& ex) {
				printf ("%s\n", ex.what().c_str());
		}
#else
		std::auto_ptr<Glib::Error> error;
		cmd = Glib::filename_from_utf8(cmd_utf8, error);
		if (!error.get())	{
				printf ("command line: %s\n", cmd.c_str());
				Glib::spawn_command_line_async (cmd, error);
			}
		if (error.get())
				printf ("%s\n", error->what().c_str());
		else
				success = true;
#endif
		return success;
}

bool safe_spawn_command_line_sync (const Glib::ustring& cmd_utf8)
{
	int exitStatus=-1;
	try {
		//cmd = Glib::filename_from_utf8(cmd_utf8);
		printf ("command line: %s\n", cmd_utf8.c_str());

		// if it crashes here on windows, make sure you have the GTK runtime files gspawn-win32-helper*.exe files in RT directory
		Glib::spawn_command_line_sync (cmd_utf8, NULL, NULL, &exitStatus);
	} catch (Glib::Exception& ex) {
			printf ("%s\n", ex.what().c_str());
	}
	return (exitStatus==0);
}

// Opens a file for binary writing and request exclusive lock (cases were you need "wb" mode plus locking)
// (Important on Windows to prevent Explorer to crash RT when parallel scanning e.g. a currently written image file)
FILE * safe_g_fopen_WriteBinLock(const Glib::ustring& fname) {
	FILE* f=NULL;

#ifdef WIN32
	// g_fopen just uses _wfopen internally on Windows, does not lock access and has no options to set this
	// so use a native function to work around this problem
	wchar_t *wFname = (wchar_t*)g_utf8_to_utf16 (fname.c_str(), -1, NULL, NULL, NULL);
	HANDLE hFile = CreateFileW(wFname, GENERIC_READ | GENERIC_WRITE, 0 /* no sharing allowed */, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	g_free(wFname);

	if (hFile==INVALID_HANDLE_VALUE)
		f=NULL;
	else
		f=_fdopen( _open_osfhandle((intptr_t)hFile, 0) , "wb");
#else
	f = safe_g_fopen(fname, "wb");
#endif

	return f;
}

// Covers old UNIX ::open, which expects ANSI instead of UTF8 on Windows
int safe_open_ReadOnly(const char *fname) {
	int fd=-1;

#ifdef WIN32
	// First convert UTF8 to UTF16, then use Windows function to open
	wchar_t *wFname = (wchar_t*)g_utf8_to_utf16 (fname, -1, NULL, NULL, NULL);
	HANDLE hFile = CreateFileW(wFname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	g_free(wFname);

	// convert back to old file descriptor format
	if (hFile!=INVALID_HANDLE_VALUE) fd = _open_osfhandle((intptr_t)hFile, 0);
#else
	fd = ::open(fname, O_RDONLY);
#endif

	return fd;
}


FILE * safe_g_fopen(const Glib::ustring& src,const gchar *mode)
{
	return g_fopen(src.c_str(),mode);
}

bool safe_file_test (const Glib::ustring& filename, Glib::FileTest test)
{
	return Glib::file_test (filename, test);
}

int safe_g_remove(const Glib::ustring& filename)
{
	return ::g_remove(filename.c_str());
}

int safe_g_rename(const Glib::ustring& oldFilename, const Glib::ustring& newFilename)
{
	return ::g_rename(oldFilename.c_str(), newFilename.c_str());
}

int safe_g_mkdir_with_parents(const Glib::ustring& dirName, int mode)
{
	return ::g_mkdir_with_parents(dirName.c_str(), mode);
}

Glib::ustring safe_get_user_picture_dir() {
    // TODO: MINGW32 has a problem with header files
    #ifdef __MINGW64_VERSION_MAJOR

    // get_user_special_dir/pictures crashes on some Windows configurations.
    // so we use the safe native functions here
    WCHAR pathW[MAX_PATH]={0};
	if (SHGetSpecialFolderPathW(NULL,pathW,CSIDL_MYPICTURES,false)) {
        char pathA[MAX_PATH];
        WideCharToMultiByte(CP_UTF8,0,pathW,-1,pathA,MAX_PATH,0,0);
        return Glib::ustring(pathA);
    } else return Glib::ustring("C:\\");

    #else

    return Glib::get_user_special_dir (G_USER_DIRECTORY_PICTURES);

    #endif
}
