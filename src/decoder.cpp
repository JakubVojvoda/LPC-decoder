/*
 *  Simple LPC decoder
 *  by Jakub Vojvoda [vojvoda@swdeveloper.sk]
 *  2015
 * 
 */

#include "decoder.h"

#include <fstream>
#include <string>
#include <sstream>

double value(std::vector<double> x, unsigned int index)
{
    if (index >= x.size()) {
        return 0;
    }

    return x[index];
}

bool readCOD(std::string filename, std::vector<int> *a, std::vector<int> *g, std::vector<int> *L)
{
    std::ifstream file(filename.c_str());

    if (!file.is_open())
        return false;

    std::string line;

    while (getline(file, line)) {

        std::stringstream nums(line);

        double as, gs, l;
        nums >> as >> gs >> l;

        a->push_back(as);
        g->push_back(gs);
        L->push_back(l);
    }

    file.close();
    return true;
}

std::vector<std::vector<double> > readMAT(std::string filename)
{
    std::ifstream file(filename.c_str());
    std::vector<double> data;
    std::string line;

    if (!file.is_open())
        return std::vector<std::vector<double> >();

    int width  = 0;
    int height = 0;

    while (getline(file, line)) {

        std::stringstream str(line);
        double n;

        while (str >> n) {

            data.push_back(n);
            if (height == 0) width += 1;
        }

        height += 1;
    }

    file.close();

    std::vector<std::vector<double> > matrix(width, std::vector<double>(height));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            matrix[x][y] = data[x + y * width];
        }
    }

    return matrix;
}

std::vector<double> synthesize(std::vector<std::vector<double> > A, std::vector<double> G, std::vector<int> L, int P, int lram)
{
    int Nram = G.size();

    std::vector<double> init(P);
    std::vector<double> ss(Nram * lram);

    int nextvoiced = 1;

    int from = 1;
    int to = from + lram - 1;

    for (int index = 0; index < Nram; index++) {

        std::vector<double> a;
        a.push_back(1.0);

        for (unsigned int j = 0; j < A[index].size(); j++)
            a.push_back(A[index][j]);

        double g = G[index];
        double l = L[index];

        std::vector<double> excit;

        if (l == 0) {

            excit = randn(lram);
        }

        else {

            std::vector<int> where = goperator(nextvoiced, l, lram);
            nextvoiced = *max_element(where.begin(), where.end()) + l - lram;

            excit = std::vector<double>(lram);

            for (unsigned int i = 0; i < where.size(); i++) {

                int index = where[i] - 1;
                excit[index] = 1;
            }
        }

        double sum = 0.0;

        for (unsigned int i = 0; i < excit.size(); i++)
            sum += pow(excit[i], 2);

        double norm = 1.0 / sqrt(sum / lram);

        for (unsigned int i = 0; i < excit.size(); i++) {
            excit[i] = excit[i] * norm;
        }

        std::vector<double> b(1);
        b[0] = g;

        std::vector<double> synt = filter(b, a, excit, &init);

        for (int i = from - 1; i < to; i++) {

            ss[i] = synt[i - from + 1];
        }

        a.clear();
        b.clear();
        excit.clear();

        from += lram;
        to = from + lram - 1;
    }

    return ss;
}


std::vector<double> randn(int size)
{
    return randn(size, 0, 1);
}

std::vector<double> randn(int size, float mean, float sd)
{
    std::vector<double> vrandom(size);

    boost::mt19937 rnd = boost::mt19937();
    rnd.seed(rand());

    boost::normal_distribution<> d(mean, sd);
    boost::variate_generator<boost::mt19937, boost::normal_distribution<> > dist(rnd, d);

    for (int i = 0; i < size; i++) {

        vrandom[i] = dist();
    }

    return vrandom;
}

std::vector<int> goperator(int from, int step, int to)
{
    std::vector<int> result;

    if (step < 1)
        return result;

    for (int i = from; i <= to; i += step) {

        result.push_back(i);
    }

    return result;
}

std::vector<double> filter(std::vector<double> b, std::vector<double> a, std::vector<double> x, std::vector<double> *zi)
{
    std::vector<double> y(x.size());
    std::vector<std::vector<double> > state(zi->size() + 1, std::vector<double>(x.size() + 1));

    for (unsigned int i = 0; i < zi->size() + 1; i++)
        state[i][0] = value((*zi), i);

    for (unsigned int i = 0; i < y.size(); i++) {

        y[i] = (value(state[0],i) + value(b,0) * value(x,i));

        for (unsigned int n = 0; n < zi->size(); n++) {

            state[n][i+1] = value(state[n+1],i) + value(b,n+1) * value(x,i) - value(a,n+1) * value(y,i);
        }
    }

    for (unsigned int n = 0; n < zi->size(); n++)
        (*zi)[n] = value(state[n], x.size());

    return y;
}
