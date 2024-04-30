//getRawShare
//getRawVideo
#include "ZoomVideoSDKRawDataPipeDelegate.h"
#include "helpers/zoom_video_sdk_user_helper_interface.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace ZOOMVIDEOSDK;
std::vector<ZoomVideoSDKRawDataPipeDelegate*> ZoomVideoSDKRawDataPipeDelegate::list_;
int ZoomVideoSDKRawDataPipeDelegate::instance_count = 0;

//this is for video preview only, doesn't need user
ZoomVideoSDKRawDataPipeDelegate::ZoomVideoSDKRawDataPipeDelegate()
{

}

ZoomVideoSDKRawDataPipeDelegate::ZoomVideoSDKRawDataPipeDelegate(IZoomVideoSDKUser* user)
{
	//getRawVideo
	instance_id_ = instance_count++;
	user_ = user;
	user_->GetVideoPipe()->subscribe(ZoomVideoSDKResolution_360P, this);
	list_.push_back(this);
}


ZoomVideoSDKRawDataPipeDelegate::~ZoomVideoSDKRawDataPipeDelegate()
{
	// finish ffmpeg encoding
	log(L"********** [%d] Finishing encoding, user: %s, %dx%d.\n", instance_id_, user_->getUserName(), in_width, in_height);


	user_->GetVideoPipe()->unSubscribe(this);
	log(L"********** [%d] UnSubscribe, user: %s.\n", instance_id_, user_->getUserName());
	instance_count--;
	user_ = nullptr;
}


ZoomVideoSDKRawDataPipeDelegate* ZoomVideoSDKRawDataPipeDelegate::find_instance(IZoomVideoSDKUser* user)
{
	for (auto iter = list_.begin(); iter != list_.end(); iter++)
	{
		ZoomVideoSDKRawDataPipeDelegate* item = *iter;
		if (item->user_ == user)
		{
			return item;
		}
	}
	return nullptr;
}

void ZoomVideoSDKRawDataPipeDelegate::start_encoding_for(IZoomVideoSDKUser* user)
{
	//getRawVideo
	instance_id_ = instance_count++;
	user_ = user;
	user_->GetVideoPipe()->subscribe(ZoomVideoSDKResolution_360P, this);
	list_.push_back(this);
}

void ZoomVideoSDKRawDataPipeDelegate::stop_encoding_for(IZoomVideoSDKUser* user)
{
	ZoomVideoSDKRawDataPipeDelegate* encoder = ZoomVideoSDKRawDataPipeDelegate::find_instance(user);
	if (encoder)
	{
		encoder->~ZoomVideoSDKRawDataPipeDelegate();
	}
}



void ZoomVideoSDKRawDataPipeDelegate::onRawDataFrameReceived(YUVRawDataI420* data)
{
	//getRawVideo
	//this is the part where callback for raw video data or raw share data happens.
	//you can choose to save the data_ buffer as raw YUV file (warning, this is huge), or convert it to mp4 / avi or other format. 
	//do be mindful of the compute power and memory usage which this callback can utilize

	const zchar_t* userName = user_->getUserName();
	const zchar_t* userID = user_->getUserID();
	const int width = data->GetStreamWidth();
	const int height = data->GetStreamHeight();
	const int bufLen = data->GetBufferLen();
	const int rotation = data->GetRotation();
	const int sourceID = data->GetSourceID();

	if ((sourceID != current_sourceID) && (sourceID == 0 || strlen(userID) > 0) // to skip frames when sourceID comes in but userID is not ready, otherwise create another sepreate file for this moment.
		)
	{
		log(L"********** [%d] Start encoding, user: %s, %dx%d, sourceID: %d.\n", instance_id_, user_->getUserName(), width, height, sourceID);
		if (is_ffmpeg_encoding_on)
		{
			ffmpeg_stop();
		};
		current_sourceID = sourceID;
		in_width = width;
		in_height = height;
		ffmpeg_start(userName, userID, sourceID);
		//printf("sourceIdNotReady: width * height is % d * %d.\n", width, height);
		is_ffmpeg_encoding_on = 1;
	}
	else
	{
		if (is_ffmpeg_encoding_on == 1 && (width != in_width || height != in_height))
		{
			is_ffmpeg_encoding_on = 0;
			log(L"********** [%d] Update scale, user: %s, %dx%d -> %dx%d sourceID: %d.\n", instance_id_, user_->getUserName(), in_width, in_height, width, height, sourceID);
			in_width = width;
			in_height = height;
			// the video source reslution changed, update the ffmpeg filter for scale.
			ffmpeg_filter_init();
			//printf("w!=w: width * height is %d * %d.\n", width, height);
			is_ffmpeg_encoding_on = 1;
		}
	}
	if (is_ffmpeg_encoding_on)
	{
		ffmpeg_filter(
			reinterpret_cast<unsigned char*>(data->GetYBuffer()),
			reinterpret_cast<unsigned char*>(data->GetUBuffer()),
			reinterpret_cast<unsigned char*>(data->GetVBuffer()));
		//printf("width * height is %d * %d.\n",width,height);
		ffmpeg_encode();
	}

}

void ZoomVideoSDKRawDataPipeDelegate::onRawDataStatusChanged(RawDataStatus status)
{
	log(L"********** [%d] onRawDataStatusChanged, user: %s, %d.\n", instance_id_, user_->getUserName(), status);
	if (status == RawData_On)
	{
	}
	else
	{
	}
}

void ZoomVideoSDKRawDataPipeDelegate::err_msg(int code)
{
	char errbuf[100];

	printf("%s\n", errbuf);
}

void ZoomVideoSDKRawDataPipeDelegate::log(const wchar_t* format, ...)
{
	va_list args;
	va_start(args, format);
	wprintf(format, args);
	va_end(args);
}

int ZoomVideoSDKRawDataPipeDelegate::ffmpeg_start(const char* userName, const char* userID, int sourceID)
{
	int ret = 0;

	// timestamp
	start_time = steady_clock::now();

	if (strlen(userID) == 0)
		userID = "0";
	char fileName[100];
	sprintf_s(fileName, sizeof(fileName), "%s_%d_%s_%dx%d_to_%dx%d", userID, sourceID, userName, in_width, in_height, out_width, out_height);
	char yuvFileName[110];
	sprintf_s(yuvFileName, sizeof(yuvFileName), "../%s.yuv", fileName);
	if (isOutputYUV)
	{
		errno_t err;

		// Use fopen_s instead of fopen
		err = fopen_s(&fp_yuv, yuvFileName, "wb+");
		if (err != 0)
		{
			printf("Error opening output file.\n");
			return -1;
		}
	}


	char outFileName[110];
	sprintf_s(outFileName, sizeof(outFileName), "%s.mkv", fileName);
	sprintf_s(fn_out, sizeof(fn_out), "../%s", outFileName); // Assuming fn_out is declared elsewhere with a known size

	// ffmpeg init
	ffmpeg_filter_init();

	pFormatCtx = avformat_alloc_context();

	// Method1: Guess Format
	fmt = av_guess_format(NULL, fn_out, NULL);
	pFormatCtx->oformat = fmt;
	// Method 2.
	// avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
	// fmt = pFormatCtx->oformat;

	// Open output file
	if (avio_open(&pFormatCtx->pb, fn_out, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("Failed to open output file! \n");
		return -1;
	}

	// init streams & codec
	video_st = avformat_new_stream(pFormatCtx, 0);
	// video_st->time_base.num = 1;
	// video_st->time_base.den = 25;
	if (video_st == NULL)
	{
		return -1;
	}
	// Param that must set
	const AVCodec* codec = avcodec_find_decoder(video_st->codecpar->codec_id);
	pCodecCtx = avcodec_alloc_context3(codec);
	// pCodecCtx->codec_id =AV_CODEC_ID_HEVC;
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
	pCodecCtx->bit_rate = 400000;
	pCodecCtx->gop_size = 250;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;
	// H264
	// pCodecCtx->me_range = 16;
	// pCodecCtx->max_qdiff = 4;
	// pCodecCtx->qcompress = 0.6;
	pCodecCtx->qmin = 10;
	pCodecCtx->qmax = 51;
	// Optional Param
	pCodecCtx->max_b_frames = 3;

	// Show some Information
	av_dump_format(pFormatCtx, 0, fn_out, 1);

	// prepare encoder
	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("Can not find encoder! \n");
		return -1;
	}

	pCodecCtx->width = out_width;
	pCodecCtx->height = out_height;

	AVDictionary* param = 0;
	// H.264
	if (pCodecCtx->codec_id == AV_CODEC_ID_H264)
	{
		av_dict_set(&param, "preset", "slow", 0);
		av_dict_set(&param, "tune", "zerolatency", 0);
		// av_dict_set(&param, "profile", "main", 0);
	}
	if (avcodec_open2(pCodecCtx, pCodec, &param) < 0)
	{
		printf("Failed to open encoder! \n");
		return -1;
	}

	// Write File Header
	if ((ret = avformat_write_header(pFormatCtx, NULL)) < 0)
	{
		printf("Failed to write header! \n");
		return ret;
	}
	return ret;
}

int ZoomVideoSDKRawDataPipeDelegate::ffmpeg_filter_init()
{
	int ret;

	buffersrc = avfilter_get_by_name("buffer");
	buffersink = avfilter_get_by_name("buffersink");
	outputs = avfilter_inout_alloc();
	inputs = avfilter_inout_alloc();
	filter_graph = avfilter_graph_alloc();

	 enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };


	///* buffer video source: the decoded frames from the decoder will be inserted here. */
	char args[512];
	snprintf(args, sizeof(args),
		"video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
		in_width, in_height, AV_PIX_FMT_YUV420P,
		1, 25, 1, 1);

	// Create buffer source filter
	if (avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in", args, NULL, filter_graph) < 0)
	{
		printf("Cannot create buffer source\n");
		return -1;
	}

	// Create buffer sink filter
	if ((ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out", NULL, NULL, filter_graph)) < 0)
	{
		printf("Cannot create buffer sink, code: %d, ", ret);
		err_msg(ret);
		return ret;
	}

	// Set pixel formats directly on the buffer sink context
	if ((ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts, AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN)) < 0) {
		printf("Cannot set output pixel formats\n");
		return ret;
	}

	/* Endpoints for the filter graph. */
	outputs = avfilter_inout_alloc();
	inputs = avfilter_inout_alloc();
	outputs->name = av_strdup("in");
	outputs->filter_ctx = buffersrc_ctx;
	outputs->pad_idx = 0;
	outputs->next = NULL;

	inputs->name = av_strdup("out");
	inputs->filter_ctx = buffersink_ctx;
	inputs->pad_idx = 0;
	inputs->next = NULL;

	char filter_descr[50];
	// Use sprintf_s instead of sprintf
	sprintf_s(filter_descr, sizeof(filter_descr), "scale=w=%d:h=%d", out_width, out_height);

	if (avfilter_graph_parse_ptr(filter_graph, filter_descr,
		&inputs, &outputs, NULL) < 0)
		return -2;

	if (avfilter_graph_config(filter_graph, NULL) < 0)
		return -3;

	frame_in = av_frame_alloc();
	frame_buffer_in = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, in_width,
		in_height, 1));
	av_image_fill_arrays(frame_in->data, frame_in->linesize, frame_buffer_in,
		AV_PIX_FMT_YUV420P, in_width, in_height, 1);

	frame_out = av_frame_alloc();
	frame_buffer_out = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, out_width, out_height, 1));
	av_image_fill_arrays(frame_out->data, frame_out->linesize, frame_buffer_out,
		AV_PIX_FMT_YUV420P, out_width, out_height, 1);

	frame_in->format = AV_PIX_FMT_YUV420P;
	frame_in->width = in_width;
	frame_in->height = in_height;

	return ret;
}

int ZoomVideoSDKRawDataPipeDelegate::ffmpeg_filter(uint8_t* Y, uint8_t* U, uint8_t* V)
{
	// input Y,U,V
	frame_in->data[0] = Y;
	frame_in->data[1] = U;
	frame_in->data[2] = V;

	// output Y,U,V
	if (isOutputYUV && frame_in->format == AV_PIX_FMT_YUV420P)
	{
		for (int i = 0; i < frame_in->height; i++)
		{
			fwrite(frame_in->data[0] + frame_in->linesize[0] * i, 1, frame_in->width, fp_yuv);
		}
		for (int i = 0; i < frame_in->height / 2; i++)
		{
			fwrite(frame_in->data[1] + frame_in->linesize[1] * i, 1, frame_in->width / 2, fp_yuv);
		}
		for (int i = 0; i < frame_in->height / 2; i++)
		{
			fwrite(frame_in->data[2] + frame_in->linesize[2] * i, 1, frame_in->width / 2, fp_yuv);
		}
	}

	// apply filter
	if (av_buffersrc_add_frame(buffersrc_ctx, frame_in) < 0)
	{
		printf("Error while add frame.\n");
		return -1;
	}

	/* pull filtered pictures from the filtergraph */
	if ((av_buffersink_get_frame(buffersink_ctx, frame_out)) < 0)
		return -1;

	return 0;
}

int ZoomVideoSDKRawDataPipeDelegate::ffmpeg_encode() {
	int ret;

	// timestamp
	steady_clock::time_point current_time = steady_clock::now();

	// Calculate PTS for the frame
	frame_out->pts = duration_cast<std::chrono::milliseconds>(current_time - start_time).count() * (video_st->time_base.den) / (video_st->time_base.num * 1000);

	// Allocate the output packet
	AVPacket* pkt = av_packet_alloc();
	if (!pkt) {
		printf("Could not allocate AVPacket\n");
		return -1;
	}

	// Send the frame to the encoder
	ret = avcodec_send_frame(pCodecCtx, frame_out);
	if (ret < 0) {
		printf("Failed to send frame to encoder, code: %d\n", ret);
		av_packet_free(&pkt); // Clean up the allocated packet
		return -1;
	}

	// Receive the encoded packet from the encoder
	while (ret >= 0) {
		ret = avcodec_receive_packet(pCodecCtx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			break; // No more packets to receive from the encoder
		}
		else if (ret < 0) {
			printf("Failed to encode, code: %d\n", ret);
			av_packet_free(&pkt); // Clean up the allocated packet
			return -1;
		}

		printf("Succeed to encode frame: %5d\tsize:%5d\n", framecnt, pkt->size);
		framecnt++;
		pkt->stream_index = video_st->index;
		av_write_frame(pFormatCtx, pkt);
		av_packet_unref(pkt);
	}

	av_packet_free(&pkt); // Clean up the allocated packet after usage
	av_frame_unref(frame_out);
	printf(".");

	return 0;
}

int ZoomVideoSDKRawDataPipeDelegate::ffmpeg_stop()
{


	AVCodecContext* codec_ctx = NULL;
	const AVCodec* codec = NULL;
	codec = avcodec_find_decoder(video_st->codecpar->codec_id);
	codec_ctx = avcodec_alloc_context3(codec);

	// Flush Encoder
	if ((ZoomVideoSDKRawDataPipeDelegate::ffmpeg_flush(pFormatCtx, 0)) < 0)
	{
		printf("Flushing encoder failed\n");
		return -1;
	}

	// Write file trailer
	av_write_trailer(pFormatCtx);

	// Clean
	if (video_st)
	{
		avcodec_close(codec_ctx);
		av_free(frame_out);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);

	if (isOutputYUV)
	{
		fclose(fp_yuv);
	}



	av_frame_free(&frame_in);
	avfilter_graph_free(&filter_graph);
	return 0;
}


int ZoomVideoSDKRawDataPipeDelegate::ffmpeg_flush(AVFormatContext* fmt_ctx, unsigned int stream_index) {
	int ret;
	AVPacket* enc_pkt;
	AVCodecContext* codec_ctx = avcodec_alloc_context3(NULL);

	if (!codec_ctx) {
		// Handle error
		return AVERROR(ENOMEM);
	}

	// Copy codec parameters from stream to codec context
	ret = avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[stream_index]->codecpar);
	if (ret < 0) {
		avcodec_free_context(&codec_ctx);
		return ret;
	}

	// Open the codec
	ret = avcodec_open2(codec_ctx, avcodec_find_decoder(codec_ctx->codec_id), NULL);
	if (ret < 0) {
		avcodec_free_context(&codec_ctx);
		return ret;
	}

	// Allocate an AVPacket for encoded data
	enc_pkt = av_packet_alloc();
	if (!enc_pkt) {
		avcodec_free_context(&codec_ctx);
		return AVERROR(ENOMEM);
	}

	// Check if the codec supports delayed frames
	if (!(codec_ctx->codec->capabilities & AV_CODEC_CAP_DELAY)) {
		av_packet_free(&enc_pkt);
		avcodec_free_context(&codec_ctx);
		return 0;
	}

	// Send NULL to the encoder to flush remaining frames
	ret = avcodec_send_frame(codec_ctx, NULL);
	if (ret < 0) {
		// Handle error
		av_packet_free(&enc_pkt);
		avcodec_free_context(&codec_ctx);
		return ret;
	}

	while (ret >= 0) {
		ret = avcodec_receive_packet(codec_ctx, enc_pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			ret = 0; // Finished flushing
			break;
		}
		else if (ret < 0) {
			// Handle error
			break;
		}

		// Log success, adjust for your logging system
		printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", enc_pkt->size);

		// Mux encoded frame
		ret = av_write_frame(fmt_ctx, enc_pkt);
		if (ret < 0) {
			break; // Handle error
		}

		av_packet_unref(enc_pkt); // Cleanup packet once it has been consumed
	}

	av_packet_free(&enc_pkt); // Free the packet after use
	avcodec_free_context(&codec_ctx); // Cleanup codec context
	return ret; // Return the result of flushing process
}