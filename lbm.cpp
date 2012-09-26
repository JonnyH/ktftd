/*
 * =====================================================================================
 *
 *       Filename:  lbm.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 09:53:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "lbm.hpp"
#include "palette_image.hpp"

#include <cassert>
#include <endian.h>
#include <memory>
#include <map>
#include <sstream>
#include <cstdint>
#include <cstring>

namespace ktftd
{
namespace img
{

	class IFFChunk
	{
	public:
		IFFChunk(std::string name, uint32_t length, char* data);
		std::string name;
		uint32_t length;
		std::unique_ptr<char[]> data;
	};

	class IFF_BMHD_Chunk : public IFFChunk
	{
	public:
		IFF_BMHD_Chunk(std::string name, uint32_t length, char* data);
		class
		{
		public:
			uint16_t width;
			uint16_t height;
			uint16_t xOrigin;
			uint16_t yOrigin;
			uint8_t numPlanes;
			uint8_t mask;
			uint8_t compression;
			uint8_t padding;
			uint16_t transp;
			uint16_t aspect;
			uint16_t pageWidth;
			uint16_t pageHeight;
		} header;
	};

	class IFF_CMAP_Chunk : public IFFChunk
	{
	public:
		IFF_CMAP_Chunk(std::string name, uint32_t length, char* data);
		std::unique_ptr<RGBColor[]> colorMap;
	};

	class IFF_FORM_Chunk : public IFFChunk
	{
	public:
		IFF_FORM_Chunk(std::string name, uint32_t length, char* data);
		std::string subName;
		std::map<std::string, IFFChunk*> subChunks;

	};

	static std::map<std::string, IFFChunk*> LoadChunks(std::istream &inStream)
	{
		std::map<std::string, IFFChunk*> chunks;
		while (inStream.good() && !inStream.eof())
		{
			char chunkName[5];
			uint32_t chunkSize;
			inStream.read(chunkName, 4);
			chunkName[4] = '\0';
			if (inStream.eof() || !inStream.good())
			{
				break;
			}
			inStream.read((char*)&chunkSize, 4);
			chunkSize = be32toh(chunkSize);
			std::string name(chunkName);
			std::cout << "Found chunk \"" << name << "\" size " << chunkSize << std::endl;

			char* data = new char[chunkSize];
			inStream.read(data, chunkSize);
			if (name == "BMHD")
			{
				std::cout << "Inserting BMHD header\n";
				chunks[name] = new IFF_BMHD_Chunk(name, chunkSize, data);
			}
			else if(name == "CMAP")
			{
			std::cout << "Inserting CMAP chunk\n";
			chunks[name] = new IFF_CMAP_Chunk(name, chunkSize, data);
			}
			else if(name == "FORM")
			{
			std::cout << "Inserting FORM chunk\n";
			chunks[name] = new IFF_FORM_Chunk(name, chunkSize, data);
			}
			else
			{
			std::cout << "Inserting normal chunk\n";
			chunks[name] = new IFFChunk(name, chunkSize, data);
			}
		}
		return chunks;
	}

	IFFChunk::IFFChunk(std::string name, uint32_t length, char* data) : name(name), length(length), data(new char[length])
	{
		memcpy((void*)&this->data[0], data, length);
	}

	IFF_BMHD_Chunk::IFF_BMHD_Chunk(std::string name, uint32_t length, char* data) : IFFChunk(name, length, data)
	{
		assert(length == 20);
		memcpy(&this->header, data, 20);
		this->header.width = be16toh(this->header.width);
		this->header.height = be16toh(this->header.height);
		this->header.xOrigin = be16toh(this->header.xOrigin);
		this->header.yOrigin = be16toh(this->header.yOrigin);
		this->header.transp = be16toh(this->header.transp);
		this->header.aspect = be16toh(this->header.aspect);
		this->header.pageWidth = be16toh(this->header.pageWidth);
		this->header.pageHeight = be16toh(this->header.pageHeight);
		std::cout << "BMHD header:" 
		<< "\twidth\t=\t" << this->header.width << std::endl
		<< "\theight\t=\t" << this->header.height << std::endl
		<< "\txOrigin\t=\t" << this->header.xOrigin << std::endl
		<< "\tyOrigin\t=\t" << this->header.yOrigin << std::endl
		<< "\tnumPlanes\t=\t" << (int)this->header.numPlanes << std::endl
		<< "\tmask\t=\t" << (int)this->header.mask << std::endl
		<< "\tcompression\t=\t" << (int)this->header.compression << std::endl
		<< "\tpadding\t=\t" << (int)this->header.padding << std::endl
		<< "\ttransp\t=\t" << this->header.transp << std::endl
		<< "\taspect\t=\t" << this->header.aspect << std::endl
		<< "\tpageWidth\t=\t" << this->header.pageWidth << std::endl
		<< "\tpageHeight\t=\t" << this->header.pageHeight << std::endl;
	}

	IFF_CMAP_Chunk::IFF_CMAP_Chunk(std::string name, uint32_t length, char* data) : IFFChunk(name, length, data)
	{
		uint32_t colorVals = length/3;
		std::cout << "New CMAP Chunk w/ " << colorVals << " colors\n";
		assert(colorVals == 256);
		this->colorMap = std::unique_ptr<RGBColor[]>(new RGBColor[colorVals]);
		memcpy((void*)&colorMap[0], data, colorVals*3);
	}

	IFF_FORM_Chunk::IFF_FORM_Chunk(std::string name, uint32_t length, char* data) : IFFChunk(name, length, data)
	{
		this->subName = std::string("PBM ");
		std::cout << "New FORM." << subName << " chunk\n";
		std::istringstream stream;
		stream.rdbuf()->pubsetbuf(&data[4], length-4);
		this->subChunks = LoadChunks(stream);
	}

	static char* DecompressByteRun1(char* inData, uint32_t inputLength, uint32_t outputLength)
	{
		char* outputBuffer = new char[outputLength];
		char* outPosition = outputBuffer;
		size_t inPos = 0;
		while (inPos < inputLength)
		{
			if (inData[inPos] >= 0)
			{
				int dataRun = inData[inPos]+1;
				inPos++;
				for (int i = 0; i < dataRun; i++)
				{
					*outPosition = inData[inPos];
					inPos++;
					outPosition++;
				}
			}
			else if (inData[inPos] > -128)
			{
				int dataRun = -inData[inPos]+1;
				inPos++;
				char byte = inData[inPos];
				for (int i = 0; i < dataRun; i++)
				{
					*outPosition = byte;
					outPosition++;
				}
				inPos++;
			}
			else
			{
				assert(0);
			}
		}
		return outputBuffer;
	}

	Image LoadLBMImage(std::istream& inStream)
	{
		auto rootChunks = LoadChunks(inStream);
		assert(rootChunks.count("FORM") == 1);
		IFF_FORM_Chunk* formChunk = static_cast<IFF_FORM_Chunk*>(rootChunks["FORM"]);
		assert(formChunk->subName == "PBM ");
		assert(formChunk->subChunks.count("BMHD") == 1);
		assert(formChunk->subChunks.count("BODY") == 1);
		assert(formChunk->subChunks.count("CMAP") == 1);
		IFF_BMHD_Chunk* bmhdChunk = static_cast<IFF_BMHD_Chunk*>(formChunk->subChunks["BMHD"]);
		IFF_CMAP_Chunk* cmapChunk = static_cast<IFF_CMAP_Chunk*>(formChunk->subChunks["CMAP"]);	
		IFFChunk* bodyChunk = static_cast<IFFChunk*>(formChunk->subChunks["BODY"]);


		std::unique_ptr<char[]> decompressedBody;
		uint32_t decompressedSize = 0;

		switch (bmhdChunk->header.compression)
		{
			case 0:
			{
				std::cout << "No compression\n";
				decompressedSize = bodyChunk->length;
				decompressedBody = std::unique_ptr<char[]>(new char[bodyChunk->length]);
				memcpy((void*)&decompressedBody[0], (void*)&bodyChunk->data[0], bodyChunk->length);
				break;
			}
			case 1:
			{
				std::cout << "ByteRun1 compression\n";
				decompressedSize = bmhdChunk->header.width*bmhdChunk->header.height;
				decompressedBody = std::unique_ptr<char[]>(DecompressByteRun1(bodyChunk->data.get(), bodyChunk->length, decompressedSize));
			}
				
		}

		Palette palette;
		memcpy(palette.palette, cmapChunk->data.get(), 256);
		PaletteImage paletteImage(bmhdChunk->header.width, bmhdChunk->header.height);
		memcpy(paletteImage.data.get(), decompressedBody.get(), decompressedSize);
		return paletteImage.getImage(palette);

	}


}//namespace img
}//namespace ktftd
