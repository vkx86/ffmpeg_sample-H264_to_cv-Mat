//
// Created by curi on 17-6-28.
//

#include "H264Decoder.h"

H264Decoder::H264Decoder() {
    init();
}

H264Decoder::~H264Decoder() {
    av_frame_free(&frame);
    av_frame_free(&pFrameBGR);
}

void H264Decoder::init() {

    matReady = false;

    av_init_packet(&avpkt);

    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    if (avcodec_open2(ctx, codec, nullptr) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    frame_count = 0;

    //存储解码后转换的RGB数据
    pFrameBGR = av_frame_alloc();


}

void H264Decoder::decode(unsigned char *inputBuff, size_t size){

    avpkt.size = size;
    if(avpkt.size == 0)
        return;

    avpkt.data = inputBuff;

    int len, got_frame;


    len =
        avcodec_decode_video2(ctx, frame, &got_frame, &avpkt);
        //avcodec_receive_frame(ctx, frame);

    if (len < 0) {
        matReady = false;
        fprintf(stderr, "Error while decoding frame %d\n", frame_count);
        frame_count++;

        return;
    }

    if(out_buffer == nullptr){

        BGRSize = avpicture_get_size(AV_PIX_FMT_BGR24, ctx->width,
                                     ctx->height);
        out_buffer = (uint8_t *) av_malloc(BGRSize);

        avpicture_fill((AVPicture *) pFrameBGR, out_buffer, AV_PIX_FMT_BGR24,
                       ctx->width, ctx->height);

        img_convert_ctx =
                sws_getContext(ctx->width, ctx->height, ctx->pix_fmt,
                               ctx->width, ctx->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, nullptr, nullptr,
                               nullptr);
        pCvMat.create(cv::Size(ctx->width, ctx->height), CV_8UC3);

    }

    if (got_frame) {

        matReady = true;
        sws_scale(img_convert_ctx, (const uint8_t *const *)frame->data,
                  frame->linesize, 0, ctx->height, pFrameBGR->data, pFrameBGR->linesize);

        memcpy(pCvMat.data, out_buffer, BGRSize);

//        printf("decoding frame: %d\n",frame_count);
        frame_count++;
    }
    else{
        matReady = false;
    }
    if (avpkt.data) {
        avpkt.size -= len;
        avpkt.data += len;
    }

}

void H264Decoder::play() {
    if(matReady){
        cv::imshow("decode",pCvMat);
        cv::waitKey(1);
    }
}

cv::Mat H264Decoder::getMat() {
    if(matReady){
        return pCvMat;
    }
    else{
        return {};
    }
}

int H264Decoder::getBgrSize() const {
    return BGRSize;
}

bool H264Decoder::isMatReady() const {
    return matReady;
}




