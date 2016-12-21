/*
 *  Simple LPC (Linear Predictive Coding) decoder in C++
 *  by Jakub Vojvoda [github.com/JakubVojvoda]
 *  2015
 *
 * Source code is licensed under MIT License
 * (for more details see LICENSE)
 * 
 */

#ifndef WAVFILE_H
#define WAVFILE_H

#include <vector>
#include <string>
#include <boost/cstdint.hpp>

bool writeWAV(std::vector<double> s, std::string filename);

#endif // WAVFILE_H
