/*
 *  Simple LPC decoder
 *  by Jakub Vojvoda [vojvoda@swdeveloper.sk]
 *  2015
 * 
 */

#ifndef DECODER_H
#define DECODER_H

#include <vector>
#include <string>

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

double value(std::vector<double> x, unsigned int index);

bool readCOD(std::string filename, std::vector<int> *a, std::vector<int> *g, std::vector<int> *L);
std::vector<std::vector<double> > readMAT(std::string filename);

std::vector<double> synthesize(std::vector<std::vector<double> > A, std::vector<double> G, std::vector<int> L, int P, int lram);
std::vector<double> filter(std::vector<double> b, std::vector<double> a, std::vector<double> x, std::vector<double> *zi);

std::vector<double> randn(int size);
std::vector<double> randn(int size, float mean, float sd);

std::vector<int> goperator(int from, int step, int to);

#endif // DECODER_H
