
#ifndef __WAVE_STRUCT_H_
#define __WAVE_STRUCT_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    数据格式为PCM的WAV文件头
    --------------------------------
    | Base_chunk | RIFF |
    ---------------------
    | WAVE              |
    ---------------------
    | Base_chunk | fmt  |   Header
    ---------------------
    | Wave_format|      |
    ---------------------
    | Base_chunk | data |
    --------------------------------
*/

#define FOURCC uint32_t 
#define WAVE_HEAD_SIZE 36

#define MAKE_FOURCC(a,b,c,d) \
( ((uint32_t)a) | ( ((uint32_t)b) << 8 ) | ( ((uint32_t)c) << 16 ) | ( ((uint32_t)d) << 24 ) )

namespace assist
{
	// The basic chunk of RIFF file format
	struct BaseChunk {

		FOURCC fcc;        // FourCC id
		uint32_t cb_size;  // 数据域的大小
	};

	struct STWaveFMT
	{
		uint16_t format_tag;      // WAVE的数据格式，PCM数据该值为1
		uint16_t channels;        // 声道数
		uint32_t sample_per_sec;  // 采样率
		uint32_t bytes_per_sec;   // 码率，channels * sample_per_sec * bits_per_sample / 8
		uint16_t block_align;     // 音频数据块，每次采样处理的数据大小，channels * bits_per_sample / 8
		uint16_t bits_per_sample; // 量化位数，8、16、32等
		//uint16_t ex_size;         // 扩展块的大小，附加块的大小

		STWaveFMT()
		{
			format_tag = 1; // PCM format data
			channels = 0;
			sample_per_sec = 0;
			bytes_per_sec = 0;
			block_align = 0;
			bits_per_sample = 0;
			//ex_size = 0;    // don't use extesion field
		}

		void SetOption(uint16_t nb_channel, uint32_t sample_rate, uint16_t sample_bits)
		{
			channels = nb_channel;
			sample_per_sec = sample_rate;
			bits_per_sample = sample_bits;
			format_tag = 0x01;                                                // PCM format data
			bytes_per_sec = channels * sample_per_sec * bits_per_sample / 8;  // 码率
			block_align = channels * bits_per_sample / 8;
			//ex_size = 0;                                                     // don't use extension field
		}
	};

	struct STWaveHeader 
	{

		BaseChunk riff;
		FOURCC wave_fcc;
		BaseChunk fmt;
		STWaveFMT  fmt_data;
		BaseChunk data;

		STWaveHeader(uint16_t nb_channel, uint32_t sample_rate, uint16_t sample_bits)
		{
			riff.fcc = MAKE_FOURCC('R', 'I', 'F', 'F');
			riff.cb_size = 0;
			wave_fcc = MAKE_FOURCC('W', 'A', 'V', 'E');
			fmt.fcc = MAKE_FOURCC('f', 'm', 't', ' ');
			fmt.cb_size = 16;
			fmt_data.SetOption(nb_channel, sample_rate, sample_bits);
			data.fcc = MAKE_FOURCC('d', 'a', 't', 'a');
			data.cb_size = 0;
		}
	};

	/*
	使用示例
	    assist::STWaveHeader header(nb_channel, sample_rate, sample_bits);
		header.riff.cb_size = audioSize + WAVE_HEAD_SIZE + sizeof(header.data) - sizeof(header.riff);
		header.data.cb_size = audioSize;

		wavData = (uint8_t *)malloc(header.riff.cb_size + sizeof(header.riff));
		if (NULL == wavData)
		{
			return -1;
		}
		memcpy(wavData, &header, sizeof(header));
		memcpy(wavData + sizeof(header), audioData, audioSize);
		wavSize = sizeof(header) + audioSize;
	*/
}

#endif




