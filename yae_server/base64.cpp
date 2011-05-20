/*
 * base64.cpp
 *
 *  Created on: 2004-07-03
 *      Author: Sebastian Pawlak
 */

#include <base64.h>

#include <string.h>
#include <string>

const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                      "abcdefghijklmnopqrstuvwxyz"
                      "0123456789"
                      "+/";

/* encodeBlock: koduje blok trzech bajtow na cztery znaki wg. tablicy base64
 */
void encodeBlock(const unsigned char in[3], char out[4], int len)
{
    out[0] = base64[in[0] >> 2];
    out[1] = base64[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    out[2] = (len > 1) ? (base64[((in[1] & 0x0f) << 2) | (in[2] >> 6)]) : '=';
    out[3] = (len > 2) ? (base64[in[2] & 0x3f]) : '=';
}


/* encodeBase64: koduje zadany lancuch bajtow do base64
 */
void encodeBase64(const unsigned char *s, char *t, unsigned int len)
{
    int i;

    for (i = 0; i < len / 3; i++)
        encodeBlock(&s[i * 3], &t[i * 4], 3);

    if (len - i * 3 > 0) {
        encodeBlock(&s[i * 3], &t[i * 4], len - i * 3);
        i++;
    }

    t[i * 4] = '\0';
}

/* decodeBlock: odkodowuje blok czterech znakow w base64 na trzy bajty
 */
void decodeBlock(const char in[4], unsigned char out[3])
{
    char inTmp[4] = { 0 };
    int i;
    char *p;

    for (i = 0; i < 4; i++)
        if ((p = strchr(base64, in[i])) != NULL)
            inTmp[i] = p - base64;

    out[0] = (inTmp[0] << 2) | (inTmp[1] >> 4);

    out[1] = (inTmp[1] << 4) | (inTmp[2] >> 2);
    if (out[1] == '\0')
        return;

    out[2] = (inTmp[2] << 6) | (inTmp[3]);
}


/* decodeBase64: odkodowuje zadany lancuch znakow w base64;
 *               napotkanie blednego bajtu, konczy odkodowywanie;
 *               dlugosc wejsciowego lancucha znakow musi dzielic sie przez 4
 */
void decodeBase64(const char *s, unsigned char *t)
{
    int i;

    for (i = 0; i < strlen(s) / 4; i++)
        decodeBlock(&s[i * 4], &t[i * 3]);

    if (s[i * 4 - 1] != '=')
        t[i * 3] = '\0';
}

std::string encodeBase64(std::string text)
{
	unsigned int length = text.size();
	char* buffer = new char[(length+1)*4];
	encodeBase64((const unsigned char*)text.c_str(), buffer, length);
	std::string returnString = buffer;
	delete[] buffer;
	return returnString;
}

std::string decodeBase64(std::string text)
{
	unsigned int length = text.size();
	char* buffer = new char[length+1];
	decodeBase64(text.c_str(),(unsigned char*)buffer);
	std::string returnString = buffer;
	delete[] buffer;
}
