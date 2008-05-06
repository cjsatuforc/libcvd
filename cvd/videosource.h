#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include <cvd/config.h>

#include <cvd/readaheadvideobuffer.h>
#include <cvd/colourspaces.h>

#include <cvd/diskbuffer2.h>

#if CVD_HAVE_FFMPEG
#include <cvd/videofilebuffer.h>
#endif

#if CVD_HAVE_V4L2BUFFER
#include <cvd/Linux/v4lbuffer.h>
#endif

#if CVD_HAVE_DVBUFFER
#include <cvd/Linux/dvbuffer.h>
#endif

#if CVD_HAVE_QTBUFFER
#include <cvd/OSX/qtbuffer.h>
#endif

namespace CVD {
    struct ParseException : public Exceptions::All
    {
	ParseException(const std::string& what_) { what = what_; }
    };
    
    struct VideoSourceException : public Exceptions::All
    {
	VideoSourceException(const std::string& what_) { what = what_; }
    };

    struct VideoSource 
    {
	std::string protocol;
	std::string identifier;
	typedef std::vector<std::pair<std::string,std::string> > option_list;
	option_list options;
    };

    std::ostream& operator<<(std::ostream& out, const VideoSource& vs);

    void parse(std::istream& in, VideoSource& vs);
   	
#ifdef CVD_HAVE_GLOB
    template <class T> VideoBuffer<T>* makeDiskBuffer2(const std::vector<std::string>& files, double fps, VideoBufferFlags::OnEndOfBuffer eob)
    {
	return new DiskBuffer2<T>(files, fps, eob);    
    }
    template <> inline VideoBuffer<vuy422> * makeDiskBuffer2(const std::vector<std::string>& , double , VideoBufferFlags::OnEndOfBuffer )
    {
	throw VideoSourceException("DiskBuffer2 cannot handle type vuy422");
    }
    template <> inline VideoBuffer<yuv422> * makeDiskBuffer2(const std::vector<std::string>& , double , VideoBufferFlags::OnEndOfBuffer )
    {
	throw VideoSourceException("DiskBuffer2 cannot handle type yuv422");
    }
#endif

    void get_files_options(const VideoSource& vs, int& fps, int& ra_frames, VideoBufferFlags::OnEndOfBuffer& eob);
    
#if CVD_HAVE_V4L2BUFFER
    template <class T> VideoBuffer<T>* makeV4LBuffer(const std::string& dev, const ImageRef& size, int input, bool interlaced)
    {
	throw VideoSourceException("V4LBuffer cannot handle types other than byte, bayer, yuv422, Rgb<byte>");
    }

    template <> VideoBuffer<byte>* makeV4LBuffer(const std::string& dev, const ImageRef& size, int input, bool interlaced);
    template <> VideoBuffer<bayer>* makeV4LBuffer(const std::string& dev, const ImageRef& size, int input, bool interlaced);
    template <> VideoBuffer<yuv422>* makeV4LBuffer(const std::string& dev, const ImageRef& size, int input, bool interlaced);
    template <> VideoBuffer<Rgb<byte> >* makeV4LBuffer(const std::string& dev, const ImageRef& size, int input, bool interlaced);

    void get_v4l2_options(const VideoSource& vs, ImageRef& size, int& input, bool& interlaced);

#endif


#if CVD_HAVE_FFMPEG    
    template <class T> VideoBuffer<T>* makeVideoFileBuffer(const std::string& file, VideoBufferFlags::OnEndOfBuffer eob)
    {
	throw VideoSourceException("VideoFileBuffer cannot handle types other than byte, Rgb<byte>");
    }
    
    template <> VideoBuffer<byte>* makeVideoFileBuffer(const std::string& file, VideoBufferFlags::OnEndOfBuffer eob);
    template <> VideoBuffer<Rgb<byte> >* makeVideoFileBuffer(const std::string& file, VideoBufferFlags::OnEndOfBuffer eob);

    void get_file_options(const VideoSource& vs, int& ra_frames, VideoBufferFlags::OnEndOfBuffer& eob);

#endif

#if CVD_HAVE_DVBUFFER
    template <class T> VideoBuffer<T>* makeDVBuffer2(int cam, int dmabufs, int bright, int exposure, int fps)
    {
	throw VideoSourceException("DVBuffer2 cannot handle types other than byte, Rgb<byte>");
    }
    
    template <> VideoBuffer<byte>* makeDVBuffer2(int cam, int dmabufs, int bright, int exposure, int fps);
    template <> VideoBuffer<Rgb<byte> >* makeDVBuffer2(int cam, int dmabufs, int bright, int exposure, int fps);

    void get_dc1394_options(const VideoSource& vs, int& dma_bufs, int& bright, int& exposure, int& fps);

#endif

#if CVD_HAVE_QTBUFFER
    template <class T> VideoBuffer<T> * makeQTBuffer( const ImageRef & size, int input, bool showsettings)
    {
	throw VideoSourceException("QTBuffer cannot handle types other than vuy422");
    }
    template <> VideoBuffer<vuy422> * makeQTBuffer( const ImageRef & size, int input, bool showsettings);
    template <> VideoBuffer<yuv422> * makeQTBuffer( const ImageRef & size, int input, bool showsettings);
    
    void get_qt_options(const VideoSource & vs, ImageRef & size, bool & showsettings);
#endif

    template <class T> VideoBuffer<T>* open_video_source(const VideoSource& vs)
    {
	if(0)
	{
	}
#if CVD_HAVE_GLOB
	else if (vs.protocol == "files") {
	    int fps, ra_frames=0;
	    VideoBufferFlags::OnEndOfBuffer eob;
	    get_files_options(vs, fps, ra_frames, eob);
	    VideoBuffer<T>* vb = makeDiskBuffer2<T>(globlist(vs.identifier), fps, eob);
	    if (ra_frames)
		vb = new ReadAheadVideoBuffer<T>(*vb, ra_frames);
	    return vb;
	}
#endif

#if CVD_HAVE_V4L2BUFFER
	else if (vs.protocol == "v4l2") {
	    ImageRef size;
	    int input;
	    bool interlaced;
	    get_v4l2_options(vs, size, input, interlaced);
	    return makeV4LBuffer<T>(vs.identifier, size, input, interlaced);	
	} 
#endif
#if CVD_HAVE_DVBUFFER
	else if (vs.protocol == "dc1394") {
	    int cam_no = atoi(vs.identifier.c_str());
	    int dma_bufs, bright, exposure, fps;
	    get_dc1394_options(vs, dma_bufs, bright, exposure, fps);
	    return makeDVBuffer2<T>(cam_no, dma_bufs, bright, exposure, fps);
	} 
#endif
#if CVD_HAVE_FFMPEG
	else if (vs.protocol == "file") {
	    int ra_frames = 0;
	    VideoBufferFlags::OnEndOfBuffer eob;
	    get_file_options(vs, ra_frames, eob);
	    VideoBuffer<T>* vb = makeVideoFileBuffer<T>(vs.identifier, eob);
	    if (ra_frames)
		vb = new ReadAheadVideoBuffer<T>(*vb, ra_frames);
	    return vb;
	} 
#endif
#if CVD_HAVE_QTBUFFER
    else if (vs.protocol == "qt") {
        ImageRef size;
        bool showsettings;
        int input = atoi(vs.identifier.c_str());
        get_qt_options(vs, size, showsettings);
        return makeQTBuffer<T>(size, input, showsettings);
    }
#endif
	else
	    throw VideoSourceException("undefined video source protocol: '" + vs.protocol + "'\n\t valid protocols: "
#if CVD_HAVE_FFMPEG
				       "file, "
#endif
#if CVD_HAVE_V4L2BUFFER
				       "v4l2, "
#endif
#if CVD_HAVE_DVBUFFER
				       "dc1394, "
#endif
#if CVD_HAVE_QTBUFFER
				       "qt, "
#endif
#ifdef CVD_HAVE_GLOB
				       "files"
#endif 
				       );
    }

/**
opens a video device described by a video source url given from an input stream. See
@ref open_video_source(const std::string &) for details on the url syntax.

@ingroup gVideo
*/
    template <class T> VideoBuffer<T>* open_video_source(std::istream& in)
    {
	VideoSource vs;
	parse(in, vs);
	return open_video_source<T>(vs);
    }

/**
opens a video device described by a video source url. This allows to decide at
runtime what kind of video input your program is using. Basic use is to call
open_video_source<T>(url) to get a VideoBuffer<T>*.

The url syntax is the following:
@verbatim
url      := protocol ':' [ '[' options ']' ] // identifier
protocol := "files" | "file" | "v4l2" | "dc1394" | "qt"
options  := option [ ',' options ]
option   := name [ '=' value ]
@endverbatim

identifier and values can be quoted literals with escapes, all other text is unquoted.
Some Examples:

Open a DiskBuffer2 for *.pgm in /local/capture/:
@verbatim
files:///local/capture/ *.pgm
@endverbatim

Open a DiskBuffer2 that loops and uses a ReadAheadVideoBuffer wrapper with 40 frame buffer, with 30 fps:
@verbatim
files:[read_ahead=40, fps=30, on_end=loop]///local/capture/ *.pgm
@endverbatim

Open a V4L2 device at /dev/video0:
@verbatim
v4l2:///dev/video0
@endverbatim
   
Open a V4L2 device with fields on input 2:
@verbatim
v4l2:[input=2,fields]///dev/video0
@endverbatim

Open firewire camera 1 with 3 dma bufs and default brightness/exposure and fps:
@verbatim
dc1394:[dma_bufs=3]//1
@endverbatim

Open an avi file relative to the current directory:
@verbatim
file://../../stuff/movie.avi
@endverbatim

Open the first QuickTime camera and show the settings dialog
@verbatim
qt:[showsettings=1]//0
@endverbatim

Options supported by the various protocols are:
@verbatim
'files' protocol (DiskBuffer2):  identifier is glob pattern
        fps = <number>
        read_ahead [= <number>] (default is 50 if specified without value)
        on_end = repeat_last | unset_pending | loop (default is repeat_last)

'file' protocol (VideoFileBuffer): identifier is path to file
       read_ahead  [= <number>] (default is 50 if specified without value)
       on_end = repeat_last | unset_pending | loop (default is repeat_last)

'v4l2' protocol (V4LBuffer): identifier is device name
       size = vga | qvga | pal | ntsc | <width>x<height>  (default vga)
       input = <number>
       interlaced | fields [= true | false | yes | no]

'dc1394' protocol (DVBuffer): identifier is camera number
       fps = <number> (default 30)
       dma_bufs | dma_buffers = <number> (default 3)
       brightness | bright = <number> (default -1)
       exposure | exp = <number> (default -1)

'qt' protocol (QTBuffer): identifier is camera number
      size = vga | qvga | <width>x<height>  (default vga)
      showsettings = 0 | 1 (default 0)

@endverbatim

@ingroup gVideo
*/
    template <class T> VideoBuffer<T>* open_video_source(const std::string& src)
    {
	std::istringstream in(src);
	return open_video_source<T>(in);
    }    
}

#endif
