#include "LogClass.h"
#include "SPIFFS.h"
#include "FS.h"


String test;


void listDir(fs::FS &fs, const char *dirname, uint8_t levels);

void readFile(fs::FS &fs, const char *path);

void writeFile(fs::FS &fs, const char *path, const char *message);

void appendFile(fs::FS &fs, const char *path, const char *message);

void deleteFile(fs::FS &fs, const char *path);

long sizeFile(fs::FS &fs, const char *path);

#define FORMAT_SPIFFS_IF_FAILED true

void LogClass::begin()
{
	if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
	{
		Serial.println("SPIFFS Mount Failed");
		return;
	}
}
void LogClass::list_dir(const char *dir)
{
	listDir(SPIFFS, dir, 0);
}
void LogClass::write_file(const char *file, const char *text)
{
	writeFile(SPIFFS, file, text);
}
void LogClass::delete_file(const char *file)
{
	deleteFile(SPIFFS, file);
}
void LogClass::append_file(const char *file, const char *text)
{
	appendFile(SPIFFS, file, text);
}
long LogClass::size_file(const char *file)
{
	return sizeFile(SPIFFS, file);
}
String LogClass::read_file(const char *file)
{
	readFile(SPIFFS, file);
	return test;
}
void LogClass::delete_buffer(){
	test = "";
}
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
	Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root)
	{
		Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		Serial.println(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if (levels)
			{
				listDir(fs, file.name(), levels - 1);
			}
		}
		else
		{
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("\tSIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}
}
long sizeFile(fs::FS &fs, const char *path)
{

	File file = fs.open(path);
	
	if (!file || file.isDirectory())
	{
		Serial.println("- failed to open file for reading");
	}
	else 
	{
		return file.size();
	}
}
void readFile(fs::FS &fs, const char *path)
{
	char *filetxt = "";
	Serial.printf("Reading file: %s\r\n", path);

	File file = fs.open(path);
	if (!file || file.isDirectory())
	{
		Serial.println("- failed to open file for reading");
	}
	// Serial.println("- read from file:");
	while (file.available())
	{
		test += ((char)file.read());// append string two to string one.
	}
	Serial.println(test);
}
void writeFile(fs::FS &fs, const char *path, const char *message)
{
	Serial.printf("Writing file: %s\r\n", path);

	File file = fs.open(path, FILE_WRITE);
	if (!file)
	{
		Serial.println("- failed to open file for writing");
		return;
	}
	if (file.print(message))
	{
		Serial.println("- file written");
	}
	else
	{
		Serial.println("- frite failed");
	}
}
void appendFile(fs::FS &fs, const char *path, const char *message)
{
	Serial.printf("Appending to file: %s\r\n", path);

	File file = fs.open(path, FILE_APPEND);
	if (!file)
	{
		Serial.println("- failed to open file for appending");
		return;
	}
	if (file.print(message))
	{
		Serial.println("- message appended");
	}
	else
	{
		Serial.println("- append failed");
	}
}
void deleteFile(fs::FS &fs, const char *path)
{
	Serial.printf("Deleting file: %s\r\n", path);
	if (fs.remove(path))
	{
		Serial.println("- file deleted");
	}
	else
	{
		Serial.println("- delete failed");
	}
}
