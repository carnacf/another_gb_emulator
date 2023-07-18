#include "video/screen.h"

namespace video
{
void Screen::tick(uint8_t count)
{

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
}
