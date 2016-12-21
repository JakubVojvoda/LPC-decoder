/*
 *  Simple LPC (Linear Predictive Coding) decoder in C++
 *  by Jakub Vojvoda [github.com/JakubVojvoda]
 *  2015
 *
 * Source code is licensed under MIT License
 * (for more details see LICENSE)
 * 
 */
 
#include "wavfile.h"

#include <fstream>
#include <climits>

bool writeWAV(std::vector<double> s, std::string filename)
{
    std::ofstream wav_file(filename.c_str());

    if (!wav_file.is_open())
        return false;

    int file_size = 2 * s.size() + 44;

    // the RIFF chunk descriptor
    char chunk_id[]     = "RIFF";
    uint32_t chunk_size = file_size - 8;
    char format[]       = "WAVE";

    wav_file.write(chunk_id, 4);
    wav_file.write(reinterpret_cast<const char *>(&chunk_size), 4);
    wav_file.write(format, 4);

    // the FMT sub-chunk
    char  subchunk1_id[]    = "fmt ";
    uint32_t subchunk1_size = 16;
    uint16_t audio_format   = 1;
    uint16_t num_channels   = 1;
    uint32_t sample_rate    = 8000;
    uint32_t byte_rate      = 16000;
    uint16_t block_align    = 2;
    uint16_t bit_per_sample = 16;

    wav_file.write(subchunk1_id, 4);
    wav_file.write(reinterpret_cast<const char *>(&subchunk1_size), 4);
    wav_file.write(reinterpret_cast<const char *>(&audio_format), 2);
    wav_file.write(reinterpret_cast<const char *>(&num_channels), 2);
    wav_file.write(reinterpret_cast<const char *>(&sample_rate), 4);
    wav_file.write(reinterpret_cast<const char *>(&byte_rate), 4);
    wav_file.write(reinterpret_cast<const char *>(&block_align), 2);
    wav_file.write(reinterpret_cast<const char *>(&bit_per_sample), 2);

    // the DATA sub-chunk
    char subchunk2_id[]     = "data";
    uint32_t subchunk2_size = file_size - 44;

    wav_file.write(subchunk2_id, 4);
    wav_file.write(reinterpret_cast<const char *>(&subchunk2_size), 4);

    // wav file data
    uint16_t data[s.size()];

    for (unsigned int i = 0; i < s.size(); i++) {

        data[i] = uint16_t(s[i] * SHRT_MAX);
    }

    wav_file.write(reinterpret_cast<const char *>(&data), subchunk2_size);

    wav_file.close();
    return true;
}

