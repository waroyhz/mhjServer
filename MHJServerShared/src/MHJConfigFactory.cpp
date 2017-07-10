/**
 * @file
 * @brief initialization file read and write API implementation
 * @author Deng Yangjun
 * @date 2007-1-9
 * @version 0.2
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <MHJConfigFactory.h>

#define MAX_FILE_SIZE 1024*16
#define LEFT_BRACE '['
#define RIGHT_BRACE ']'

static int load_ini_file(const char *file, char *buf, int *file_size) {
	FILE *in = NULL;
	int i = 0;
	*file_size = 0;

	assert(file != NULL);
	assert(buf != NULL);

	in = fopen(file, "r");
	if (NULL == in) {
		return 0;
	}

	buf[i] = fgetc(in);

	//load initialization file
	while (buf[i] != (char) EOF) {
		i++;
		assert(i < MAX_FILE_SIZE); //file too big, you can redefine MAX_FILE_SIZE to fit the big file
		buf[i] = fgetc(in);
	}

	buf[i] = '\0';
	*file_size = i;

	fclose(in);
	return 1;
}

static int newline(char c) {
	return ('\n' == c || '\r' == c) ? 1 : 0;
}
static int end_of_string(char c) {
	return '\0' == c ? 1 : 0;
}
static int left_barce(char c) {
	return LEFT_BRACE == c ? 1 : 0;
}
static int isright_brace(char c) {
	return RIGHT_BRACE == c ? 1 : 0;
}
static int parse_file(const char *section, const char *key, const char *buf, int *sec_s, int *sec_e, int *key_s, int *key_e, int *value_s, int *value_e) {
	const char *p = buf;
	int i = 0;

	assert(buf != NULL);
	assert(section != NULL && strlen(section));
	assert(key != NULL && strlen(key));

	*sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

	while (!end_of_string(p[i])) {
		//find the section
		if ((0 == i || newline(p[i - 1])) && left_barce(p[i])) {
			int section_start = i + 1;

			//find the ']'
			do {
				i++;
			} while (!isright_brace(p[i]) && !end_of_string(p[i]));

			if (0 == strncmp(p + section_start, section, i - section_start)) {
				int newline_start = 0;

				i++;

				//Skip over space char after ']'
				while (isspace(p[i])) {
					i++;
				}

				//find the section
				*sec_s = section_start;
				*sec_e = i;

				while (!(newline(p[i - 1]) && left_barce(p[i])) && !end_of_string(p[i])) {
					int j = 0;
					//get a new line
					newline_start = i;

					while (!newline(p[i]) && !end_of_string(p[i])) {
						i++;
					}

					//now i  is equal to end of the line
					j = newline_start;

					if (';' != p[j]) //skip over comment
							{
						while (j < i && p[j] != '=') {
							j++;
							if ('=' == p[j]) {
								if (strncmp(key, p + newline_start, j - newline_start) == 0) {
									//find the key ok
									*key_s = newline_start;
									*key_e = j - 1;

									*value_s = j + 1;
									*value_e = i;

									return 1;
								}
							}
						}
					}

					i++;
				}
			}
		} else {
			i++;
		}
	}
	return 0;
}

/**
 *@brief read string in initialization file\n
 * retrieves a string from the specified section in an initialization file
 *@param section [in] name of the section containing the key name
 *@param key [in] name of the key pairs to value
 *@param value [in] pointer to the buffer that receives the retrieved string
 *@param size [in] size of result's buffer
 *@param default_value [in] default value of result
 *@param file [in] path of the initialization file
 *@return 1 : read success; \n 0 : read fail
 */
int read_profile_string(const char *section, const char *key, char *value, int size, const char *default_value, const char *file) {
	char buf[MAX_FILE_SIZE] = { 0 };
	int file_size;
	int sec_s, sec_e, key_s, key_e, value_s, value_e;

	//check parameters
	assert(section != NULL && strlen(section));
	assert(key != NULL && strlen(key));
	assert(value != NULL);
	assert(size > 0);
	assert(file != NULL && strlen(key));

	if (!load_ini_file(file, buf, &file_size)) {
		if (default_value != NULL) {
			strncpy(value, default_value, size);
		}
		return 0;
	}

	if (!parse_file(section, key, buf, &sec_s, &sec_e, &key_s, &key_e, &value_s, &value_e)) {
		if (default_value != NULL) {
			strncpy(value, default_value, size);
		}
		return 0; //not find the key
	} else {
		int cpcount = value_e - value_s;

		if (size - 1 < cpcount) {
			cpcount = size - 1;
		}

		memset(value, 0, size);
		memcpy(value, buf + value_s, cpcount);
		value[cpcount] = '\0';

		return 1;
	}
}

/**
 *@brief read int value in initialization file\n
 * retrieves int value from the specified section in an initialization file
 *@param section [in] name of the section containing the key name
 *@param key [in] name of the key pairs to value
 *@param default_value [in] default value of result
 *@param file [in] path of the initialization file
 *@return profile int value,if read fail, return default value
 */
int read_profile_int(const char *section, const char *key, int default_value, const char *file) {
	char value[32] = { 0 };
	if (!read_profile_string(section, key, value, sizeof(value), NULL, file)) {
		return default_value;
	} else {
		return atoi(value);
	}
}

/**
 * @brief write a profile string to a ini file
 * @param section [in] name of the section,can't be NULL and empty string
 * @param key [in] name of the key pairs to value, can't be NULL and empty string
 * @param value [in] profile string value
 * @param file [in] path of ini file
 * @return 1 : success\n 0 : failure
 */
int write_profile_string(const char *section, const char *key, const char *value, const char *file) {
	char buf[MAX_FILE_SIZE] = { 0 };
	char w_buf[MAX_FILE_SIZE] = { 0 };
	int sec_s, sec_e, key_s, key_e, value_s, value_e;
	int value_len = (int) strlen(value);
	int file_size;
	FILE *out;

	//check parameters
	assert(section != NULL && strlen(section));
	assert(key != NULL && strlen(key));
	assert(value != NULL);
	assert(file != NULL && strlen(key));

	if (!load_ini_file(file, buf, &file_size)) {
		sec_s = -1;
	} else {
		parse_file(section, key, buf, &sec_s, &sec_e, &key_s, &key_e, &value_s, &value_e);
	}

	if (-1 == sec_s) {
		if (0 == file_size) {
			sprintf(w_buf + file_size, "[%s]\n%s=%s\n", section, key, value);
		} else {
			//not find the section, then add the new section at end of the file
			memcpy(w_buf, buf, file_size);
			sprintf(w_buf + file_size, "\n[%s]\n%s=%s\n", section, key, value);
		}
	} else if (-1 == key_s) {
		//not find the key, then add the new key=value at end of the section
		memcpy(w_buf, buf, sec_e);
		sprintf(w_buf + sec_e, "%s=%s\n", key, value);
		sprintf(w_buf + sec_e + strlen(key) + strlen(value) + 2, buf + sec_e, file_size - sec_e);
	} else {
		//update value with new value
		memcpy(w_buf, buf, value_s);
		memcpy(w_buf + value_s, value, value_len);
		memcpy(w_buf + value_s + value_len, buf + value_e, file_size - value_e);
	}

	out = fopen(file, "w");
	if (NULL == out) {
		return 0;
	}

	if (-1 == fputs(w_buf, out)) {
		fclose(out);
		return 0;
	}

	fclose(out);
	return 1;
}

int CheckInitFile(FILE *fp, char *g_szConfigName) {
	if ((fp = fopen(g_szConfigName, "r")) == NULL) {
//		printf("The Init file is not exit, please creat it!\n");
		return -1;
	} else {
//		printf("Init file is there!\n");
		fclose(fp);
		return 0;
	}
}

int CreatInitFile(FILE *fp, char *g_szConfigName) {
	if ((fp = fopen(g_szConfigName, "a+")) == NULL) {
//		printf("failed to create Initfile!\n");
		return -1;
	} else {
//		printf(" Initfile Created!\n");
		fclose(fp);
		return 0;
	}
}

int getCurIPaddr(char *outAddress) {
	struct ifaddrs * ifAddrStruct = NULL;
	void * tmpAddrPtr = NULL;

	getifaddrs(&ifAddrStruct);

	while (ifAddrStruct != NULL) {
		if (ifAddrStruct->ifa_addr->sa_family == AF_INET) { // check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr = &((struct sockaddr_in *) ifAddrStruct->ifa_addr)->sin_addr;
//	            char addressBuffer[INET_ADDRSTRLEN];

			inet_ntop(AF_INET, tmpAddrPtr, outAddress, INET_ADDRSTRLEN);
//			printf("%s\t", outAddress);
			if (!strncmp(outAddress, "192", 3)) {
//					*((in_addr_t*)(outAddress+retsize))= inet_addr( addressBuffer );
//						retsize+=sizeof(in_addr_t);
//						if (retsize+sizeof(in_addr_t)>insize)
//				printf("%s IP4 Address: %s\n", ifAddrStruct->ifa_name, outAddress);
				break;
			}
		} else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6) { // check it is IP6
			// is a valid IP6 Address
//	            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
//	            char addressBuffer[INET6_ADDRSTRLEN];
//	            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
//	            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
		}
		ifAddrStruct = ifAddrStruct->ifa_next;
	}
	return 0;
}

void str_invert(char *str) {
	int i, n;
	char temp;

	n = strlen(str);
	for (i = 0; i < n + 1; i++) {
		if (i < n - i) {
			temp = str[i];
			str[i] = str[n - i - 1];
			str[n - i - 1] = temp;
		}
	}
}

void itoa(int num, char *str) {
	int i, temp;

	if (num == 0) {
		str[0] = '0';
		str[1] = '\0';
	} else {
		for (i = 0; num > 0; i++) {
			temp = num % 10;
			num /= 10;
			str[i] = temp + '0';
		}
		str[i] = '\0';
	}
	str_invert(str);
}

char MHJConfigFactory::pfilename[255];

bool MHJConfigFactory::Setfile(const char *filename) {
//	FILE *fp = NULL;
//	if (CheckInitFile(fp, (char*) filename) == -1) {
//		if (CreatInitFile(fp, (char*) filename) == -1) {
//			printf("Creat Init file failed, Use Default value!\n");  //Use default key value/
////			return false;
//		}
//	}
	strcpy(pfilename, filename);
	return true;
}

void MHJConfigFactory::SetSection(const char* section) {
	strcpy(psection, section);
}

int MHJConfigFactory::ReadProfile(const char *key, char *valuebuffer, int size, const char *default_value) {

	//read profile string from string, if read null, use default and write default to file
	if (!read_profile_string(psection, key, valuebuffer, size, default_value, pfilename)) {
//			printf("Failed to read ['%s']  from file [%s],  Use default value [%s] \n", key,pfilename,default_value);
		if (!write_profile_string(psection, key, default_value, pfilename)) {
//			printf("Write inifile fail\n");
		}
	}
	return 0;
}

