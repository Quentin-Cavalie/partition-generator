
#include <iostream>
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include "WavParser.h"

/*!
    \file WavParser.cc
    Parse the .wav files and gather its properties.
*/

WavParser::WavParser()
:f(-1), c(-1), sr(-1), num_frames(-1)
{

}

WavParser::~WavParser()
{

}

float *
WavParser::getData()
{
    return data;
}

int
WavParser::getDataSize()
{
    return data_size;
}

int 
WavParser::getWindowMSSize()
{
    return window_ms_size;
}
int
WavParser::getWindowSize()
{
    return window_size;
}

void
WavParser::getInfos(char *wavFile)
{
	SNDFILE *sf;
	SF_INFO info;

	info.format = 0;
    sf = sf_open(wavFile, SFM_READ, &info);
   
    if (sf == NULL)
    {
        printf("Failed to open the file.\n");
    }

    f = info.frames;
    sr = info.samplerate;
    c = info.channels;

   	num_frames = f*c;

    window_size = num_frames / sr; 
    window_ms_size = (1000 / window_size);

   	sf_close(sf);
}

void
WavParser::printInfos()
{
	if (f == -1 && sr == -1 && c == -1 && num_frames == -1)
		printf("File Informations Incomplete\n");

	else
	{       
		printf("\n----FILE INFOS----\n\n");

		if (f != -1)
			printf("frames = %d\n",f);

		if (sr !=-1)
    		printf("sample rate = %d\n",sr);

    	if (sr != -1)
    		printf("channels = %d\n",c);

        if (num_frames != - 1)
            printf("total frames = %d\n", num_frames);

        printf("wav length = %d sec\n", window_size);

        printf("window size = %d ms\n", window_ms_size);
	}
}

void
WavParser::parse(char *wavFile)
{
    SNDFILE *sf;
    SF_INFO info;

    data_size = f * c;

    info.format = 0;
    sf = sf_open(wavFile, SFM_READ, &info);
   
    if (sf == NULL)
        printf("Failed to open the file.\n");    	

    data = new float[f * c];

    sf_readf_float(sf, data, info.frames);

  //  float sum = 0;

	// for (int i = 0; i < f * c; ++i)
	// {
 //   	 	/* code */
 //   	 	//std::cout << data[i] << std::endl;
 //   		sum += data[i];
	// }

//	std::cout << "sum = " << sum << std::endl;

    sf_close(sf);
}
