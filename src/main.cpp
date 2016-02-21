/*
 *  Simple LPC decoder
 *  by Jakub Vojvoda [vojvoda@swdeveloper.sk]
 *  2015
 * 
 */

#include "decoder.h"
#include "wavfile.h"

#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
    // Input filenames
    std::string  cb_filename = (argc > 1) ? argv[1] : "cb_lpc.txt";
    std::string gcb_filename = (argc > 2) ? argv[2] : "cb_gain.txt";
    std::string cod_filename = (argc > 3) ? argv[3] : "in.cod";

    // Output filename
    std::string output = (argc > 4) ? argv[4] : "out.wav";

    std::vector<int> asym, gsym, L;

    if (!readCOD(cod_filename, &asym, &gsym, &L)) {

        std::cerr << "codebook file '" << cb_filename 
                  << "' does not exist" << std::endl;
        return 1;
    }

    if (asym.empty() || gsym.empty() || L.empty()) {

        std::cerr << "wrong format of codebook file '" 
                  << cb_filename << "' ('%d %d %d\\n')" << std::endl;
        return 1;
    }

    std::vector<std::vector<double> > cb  = readMAT(cb_filename);
    std::vector<std::vector<double> > gcb = readMAT(gcb_filename);

    if (cb.empty()) {

        std::cerr << "lpc file '" << cb_filename 
                  << "' does not exist" << std::endl;
        return 1;
    }

    if (gcb.empty()) {

        std::cerr << "gain file '" << gcb_filename 
                  << "' does not exist" << std::endl;
        return 1;
    }

    std::vector<std::vector<double> > adec;

    for (unsigned int i = 0; i < asym.size(); i++) {

        int index = asym[i] - 1;
        adec.push_back(cb[index]);
    }

    std::vector<double> gdec;

    for (unsigned int i = 0; i < gsym.size(); i++) {

        int index = gsym[i] - 1;
        gdec.push_back(gcb[index][0]);
    }

    srand(time(NULL));
    std::vector<double> s = synthesize(adec, gdec, L, 10, 160);

    writeWAV(s, output);

    return 0;
}

