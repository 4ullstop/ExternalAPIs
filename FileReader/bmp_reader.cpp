#include "bmp_reader.h"

struct bit_scan_result
{
    bool32 found;
    u32 index;
};

inline bit_scan_result
FindLeastSignificantSetBit(u32 value)
{
    bit_scan_result result = {};
    result.found = false;

#if COMPILER_MSVC
    result.found = _BitScanForward((unsigned long*)&result.index, value);
#else
    for (u32 test = 0; test < 32; ++test)
    {
	if (value & (1 << test))
	{
	    result.index = test;
	    result.found = true;
	    break;
	}
    }
#endif
    return(result);
}

loaded_bitmap
DEBUGLoadBMP(thread_context* thread, debug_platform_read_entire_file* ReadEntireFile, char* filename)
{
    loaded_bitmap result = {};
    debug_read_file_result readResult = ReadEntireFile(thread, filename);

    if (readResult.contents != 0)
    {
	bitmap_header* header = (bitmap_header*)readResult.contents;
	u32* pixels = (u32*)((u8*)readResult.contents + header->bitmapOffset);
	result.pixels = pixels;
	result.width = header->width;
	result.height = header->height;

	Assert(header->compression == 3);

	u32 redMask = header->redMask;
	u32 greenMask = header->greenMask;
	u32 blueMask = header->blueMask;
	u32 alphaMask = ~(redMask | greenMask | blueMask);

	bit_scan_result redShift = FindLeastSignificantSetBit(redMask);
	bit_scan_result greenShift = FindLeastSignificantSetBit(greenMask);
	bit_scan_result blueShift = FindLeastSignificantSetBit(blueMask);
	bit_scan_result alphaShift = FindLeastSignificantSetBit(alphaMask);

	Assert(redShift.found);
	Assert(greenShift.found);
	Assert(blueShift.found);
	Assert(alphaShift.found);

	u32* sourceDest = pixels;
	for (i32 y = 0; y < header->height; ++y)
	{
	    for (i32 x = 0; x < header->width; ++x)
	    {
		u32 c = *sourceDest;
		*sourceDest++ = ((((c >> alphaShift.index) & 0xFF) << 24)|
				 (((c >> redShift.index) & 0xFF) << 16)|
				 (((c >> greenShift.index) & 0xFF) << 8)|
				 (((c >> blueShift.index) & 0xFF) << 0));
	    }
	}
    }

    return(result);
}
