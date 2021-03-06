#ifndef WAV_PARSER_HH
#define WAV_PARSER_HH

#include <fstream>
#include <iostream>

typedef std::uint8_t byte;

/*!
    \file WavParser.h
    Parse the .wav files and gather its properties.
*/

/*! \brief WavParser class

    Implementation of a .wav file parser that gathers its signal and properties
    so it can later be appropriately processed by #SignalProcessor.
    
    \date September 2014
*/

class WavParser
{
    public:
        /*! Constructor.
            Sets the .wav proprerties (sample rate, number of frames and number of channels) to -1.
        */
        WavParser();

        //! Destructor.
        ~WavParser();

        /*! read the wav file passed in parameter #fileName and returns the bytes corresponding to the actual signal informations. 
        */
        unsigned char* readFileBytes(const char *fileName, size_t *length);


        /*! Converts two bytes to a double digit.
        */        
        double bytesToDouble(byte firstByte, byte secondByte);

        /*! Open the wav file passed in parameter.
        */
        void openWav(char *filename);

        /*! Retrieves all the properties of .wav file.
            \param wavFile The path to the .wav file.
        */
        void getInfos(char *wavFile);

        /*! Print all the properties of the .wav file gathered by the parser.
        */
        void printInfos();

        /*! Parse the .wav file and fills #data with its content.
            \param wavFile The path to the .wav file to be parsed.
        */
        void parse(char *wavFile);

        /*! Returns #data.
            \return The orignal signal of the .wav file once it has been parsed.
        */
        float *getData();

        /*! Returns the data corresponding to the audio stored in the left canal.
        */
        float *getLeft();
        
        /*! Returns the data corresponding to the audio stored in the right canal.
        */
        float *getRight();

        /*! Returns #data_size.
            \return The size of the signal once it has been parsed.
        */
        int getDataSize();

        /*! Returns #window_ms_size.
            \return The size of the signal's window in milliseconds.
        */
        int getWindowMSSize();

        /*! Returns #window_size.
            \return The size of the signal's window in Hertz.
        */
        int getWindowSize();

    private:
        //attributes

        /*! The number of frames in the parsed signal.
        */
        int f;

        /*! The number of channels in the .wav file.
            -   mono = 1
            -   stereo = 2
        */
        int c;

        /*! The sample rate of the signal. Usually equals to 44100 Hertz.
        */
        int sr;

        /*! The total number of frames in the signal #data.
            Corresponds to the number of frames times the number of channels (#f * #c).
        */
        int num_frames;

        /*! The size of each frame in milliseconds.
        */
        int window_ms_size;

        /*! The size of each frame in Hertz.
        */
        int window_size;

        /*! The lentgh of the signal.
        */
        int data_size;

        /*! Holds the original time-based signal from the .wav file
        */
        float *data;

        /*! Holds the original time-based signal from the left canal of the .wav file
        */
        float *left;

        /*! Holds the original time-based signal from the right canal of the .wav file
        */
        float *right;
};

#endif /* WAV_PARSER_HH */
