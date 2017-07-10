/*
 *
 */

#ifndef INI_FILE_H_
#define INI_FILE_H_

class MHJConfigFactory {
public:
	bool Setfile(const char *filename);
	void SetSection(const char *section);
	int ReadProfile(const char *key, char *valuebuffer, int size, const char *default_value);
private:
	char psection[255];
	static char pfilename[255];
};

int read_profile_string(const char *section, const char *key, char *value, int size, const char *default_value, const char *file);
int write_profile_string(const char *section, const char *key, const char *value, const char *file);

#endif //end of INI_FILE_H_

