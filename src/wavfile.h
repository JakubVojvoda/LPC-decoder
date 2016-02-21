/*
 *  Simple LPC decoder
 *  by Jakub Vojvoda [vojvoda@swdeveloper.sk]
 *  2015
 * 
 */

#ifndef WAVFILE_H
#define WAVFILE_H

#include <vector>
#include <string>
#include <boost/cstdint.hpp>

bool writeWAV(std::vector<double> s, std::string filename);

#endif // WAVFILE_H
