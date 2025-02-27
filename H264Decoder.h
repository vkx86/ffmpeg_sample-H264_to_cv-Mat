//
// Created by curi on 17-6-28.
//

#ifndef OPENCVIDEO_MASTER_H264DECODER_H
#define OPENCVIDEO_MASTER_H264DECODER_H

//#define __STDC_CONSTANT_MACROS
#include <stdio.h>
#include <unistd.h>
// Opencv
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>

extern "C" {
//#include "libavutil/avutil.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
//新版里的图像转换结构需要引入的头文件
#include "libswscale/swscale.h"
};

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc  avcodec_alloc_frame
#endif


class H264Decoder {

public :

    H264Decoder();
    ~H264Decoder();
    void init();
    void decode(unsigned char *inputBuff, size_t size);
    void play();
    cv::Mat getMat();
    bool isMatReady() const;

private:

    const AVCodec *codec{};
    AVCodecContext *ctx = nullptr;
    int frame_count{};
    AVFrame *frame{};
    AVPacket avpkt{};

    AVFrame *convFrame{};
    int convSize{};

    uint8_t *out_buffer = nullptr;

    struct SwsContext *img_convert_ctx{};
    cv::Mat pCvMat;
    bool matReady{};
    AVPixelFormat convPixFmt = AV_PIX_FMT_NONE;
    int scaleWidth{};
    int scaleHeight{};

};

#endif //OPENCVIDEO_MASTER_H264DECODER_H
