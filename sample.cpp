#include "H264Decoder.h"
#include <fstream>

unsigned char buf[700000];//should be max enough

int main() {

    H264Decoder decoder;

    for (int j = 0; j < 500; j++) {
        std::ifstream fin("/home/developer/CLionProjects/ffmpeg_sample-H264_to_cv-Mat/264rawFrame/outputFrame"
                          + std::to_string(j), std::ios_base::binary);
        fin.seekg(0, std::ios::end);
        int len = fin.tellg();
        fin.seekg(0, std::ios::beg);

        fin.read((char *) buf, len);

        decoder.decode(buf, len);
        if(decoder.isMatReady())
            decoder.play();
    }
}

