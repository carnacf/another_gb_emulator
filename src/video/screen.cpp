#include "video/screen.h"

#include "memory/memory.h"
#include "utils/utils.h"

namespace video
{
Screen::Screen()
	: m_frameBuffer(new uint8_t[SCREEN_WIDTH * SCREEN_HEIGHT * 4]())
{
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 4; i++)
	{
		m_frameBuffer[i] = 255;
	}
}

Screen::~Screen()
{
	delete[] m_frameBuffer;
}
	
void Screen::tick(uint8_t count)
{
	updateStatusRegister();

	if (m_lcdEnabled)
	{
		m_scanlineCounter += count;
	}
	else
	{
		return;
	}

	if (m_scanlineCounter >= 456)
	{
		m_ly++;
		m_scanlineCounter = 0;

		if (m_ly == 144)
		{
			// Entering VBlank
			uint8_t if_flag = m_memory->read8(0xFF0F);
			if_flag = utils::setBit(if_flag, 0);
			m_memory->write8(0xFF0F, if_flag);
		}
		else if (m_ly > 153)
		{
			m_ly = 0;
		}
		else if (m_ly < 144)
		{
			// Render ScanLine ?
			renderBG(m_ly);
		}
	}
}

void Screen::setMemory(Memory* memory)
{
	m_memory = memory;
}

uint8_t* Screen::getFrameBuffer()
{
	return m_frameBuffer;
}

void Screen::setSCY(uint8_t scy)
{
	m_scy = scy;
}

void Screen::setSCX(uint8_t scx)
{
	m_scx = scx;
}

void Screen::setWY(uint8_t wy)
{
	m_wy = wy;
}

void Screen::setWX(uint8_t wx)
{
	m_wx = wx;
}

uint8_t Screen::getLY() const
{
	return m_ly;
}

void Screen::setLYC(uint8_t lyc)
{
	m_lyc = lyc;
}

void Screen::enableLCD(bool enabled)
{
	m_lcdEnabled = enabled;
}

void Screen::enableWindow(bool enabled)
{
	m_windowEnabled = enabled;
}

void Screen::enableObj(bool enabled)
{
	m_objectEnable = true;
}

void Screen::enablePriority(bool enabled)
{
	m_bgAndWindowPriority = enabled;
}

void Screen::setObjSize(uint8_t size)
{
	m_ObjectSize = size;
}

void Screen::setWindowTileMapAddr(uint16_t addr)
{
	m_windowTileMapAddr = addr;
}

void Screen::setTileDataArea(uint16_t addr)
{
	m_tileDataArea = addr;
}

void Screen::setBgTileMapAddr(uint16_t addr)
{
	m_bgTileMapAddr = addr;
}

uint8_t Screen::getBGPalette() const
{
	return m_bgPalette;
}

void Screen::setBGPalette(uint8_t bgPalette)
{
	m_bgPalette = bgPalette;
}

void Screen::updateStatusRegister()
{
	uint8_t status = m_memory->read8(0xFF41);
	if (!m_lcdEnabled)
	{
		m_scanlineCounter = 0;
		m_ly = 0;

		// Set mode to 1
		status &= 0xFC;
		status |= 1;
		m_memory->write8(0xFF41, status);

		return;
	}

	uint8_t currentMode = status & 0x03;
	bool requestInterrupt = false;

	// VBlank
	if (m_ly >= 144)
	{
		status = utils::resetBit(status, 0);
		status = utils::setBit(status, 1);
		requestInterrupt = utils::testBit(status, 4);
	}
	else
	{
		if (m_scanlineCounter < 80)
		{
			// Mode 2
			status = utils::setBit(status, 1);
			status = utils::resetBit(status, 0);
			requestInterrupt = utils::testBit(status, 5);
		}
		else if (m_scanlineCounter < 172)
		{
			// Mode 3
			status = utils::setBit(status, 0);
			status = utils::setBit(status, 1);
		}
		else
		{
			// Mode 0
			status = utils::resetBit(status, 0);
			status = utils::resetBit(status, 1);
			requestInterrupt = utils::testBit(status, 3);
		}
	}

	if (requestInterrupt && (currentMode != (status & 0x03)))
	{
		uint8_t if_flag = m_memory->read8(0xFF0F);
		if_flag = utils::setBit(if_flag, 1);
		m_memory->write8(0xFF0F, if_flag);
	}

	if (m_ly == m_lyc)
	{
		status = utils::setBit(status, 2);
		if (utils::testBit(status, 6))
		{
			uint8_t if_flag = m_memory->read8(0xFF0F);
			if_flag = utils::setBit(if_flag, 1);
			m_memory->write8(0xFF0F, if_flag);
		}
	}
	else
	{
		status = utils::resetBit(status, 2);
	}

	m_memory->write8(0xFF41, status);
}

void Screen::renderBG(uint8_t line)
{
	uint16_t tileMapY = m_scy / 8 + line / 8;
	uint16_t startAddr = m_bgTileMapAddr + tileMapY * 32;

	for (uint8_t tileY = 0; tileY < 8; tileY++)
	{
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			uint8_t tileMapX = (x + m_scx) / 8;
			uint8_t tilePixel = (x + m_scx) % 8;

			uint16_t tileIdAddress = startAddr + tileMapX;
			uint8_t tileId = m_memory->read8(tileIdAddress);
			uint16_t tileAdress = fromTileIdtoAdress(tileId);

			uint64_t tileLine = getTileLine(tileAdress, tileY);

			uint8_t colorId = ((uint8_t*)&tileLine)[tilePixel];
			m_frameBuffer[line * SCREEN_WIDTH * 4 + x * 4] = 255 - fromColorIdtoColor(colorId);
			m_frameBuffer[line * SCREEN_WIDTH * 4 + x * 4 + 1] = 255 - fromColorIdtoColor(colorId);
			m_frameBuffer[line * SCREEN_WIDTH * 4 + x * 4 + 2] = 255 - fromColorIdtoColor(colorId);
			m_frameBuffer[line * SCREEN_WIDTH * 4 + x * 4 + 3] = 255;
		}
	}
}
uint64_t Screen::getTileLine(uint16_t tileAddr, uint8_t line)
{
	// We pair the two Bytes of a given line into pairs of bits store in a 8 byte value
	// This is an trick to easily get the pixels colors.
	uint64_t a = m_memory->read8(tileAddr + line * 2);
	uint64_t tmpA = a * 0x0100040010004001 & 0x0001000100010001;
	uint64_t tmpB = a * 0x0002000800200080 & 0x0100010001000100;
	a = tmpA | tmpB;

	uint64_t b = m_memory->read8(tileAddr + line * 2 + 1);
	tmpA = b * 0x0100040010004001 & 0x0001000100010001;
	tmpB = b * 0x0002000800200080 & 0x0100010001000100;
	b = (tmpA | tmpB) << 1;

	return a | b;
}

uint16_t Screen::fromTileIdtoAdress(uint8_t tileId)
{
	uint8_t tileMemorySize = 16;
	switch (m_tileDataArea)
	{
	case 0x8800:
		return m_tileDataArea + (tileId + 128) * tileMemorySize;
	case 0x8000:
	default:
		return m_tileDataArea + tileId * tileMemorySize;
	}
}

uint8_t Screen::fromColorIdtoColor(uint8_t colorId)
{
	return  m_bgPalette & (0b0000'0011 << (colorId*2));
}

}
