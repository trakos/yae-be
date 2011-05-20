/*
 * base64.h
 *
 *  Created on: 2004-07-03
 *      Author: Sebastian Pawlak
 */

#ifndef BASE64_H_
#define BASE64_H_

#include <string>

void encodeBase64(const unsigned char *s, char *t, unsigned int len);
void decodeBase64(const char *s, unsigned char *t);

std::string encodeBase64(std::string text);
std::string decodeBase64(std::string text);

#endif /* BASE64_H_ */
