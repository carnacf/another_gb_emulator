#include "video/screen.h"

#include "memory/memory.h"
#include "utils/utils.h"

namespace video
{
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
		}
	}
}

void Screen::setMemory(Memory* memory)
{
	m_memory = memory;
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
}
