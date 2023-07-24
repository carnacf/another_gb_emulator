#pragma once

#include <cstdint>

class Memory;

namespace video
{
class Screen
{
public:

	void tick(uint8_t count);

	void setMemory(Memory* memory);

	void setSCY(uint8_t scy);
	void setSCX(uint8_t scx);
	void setWY(uint8_t wy);
	void setWX(uint8_t wx);

	uint8_t getLY() const;
	void setLYC(uint8_t lyc);

	void enableLCD(bool enabled);
	void enableWindow(bool enabled);
	void enableObj(bool enabled);
	void enablePriority(bool enabled);
	void setObjSize(uint8_t size);
	void setWindowTileMapAddr(uint16_t addr);
	void setTileDataArea(uint16_t addr);
	void setBgTileMapAddr(uint16_t addr);

private:

	void updateStatusRegister();

private:
	Memory* m_memory;

	uint16_t m_windowTileMapAddr = 0x9800;
	uint16_t m_tileDataArea = 0x8800;
	uint16_t m_bgTileMapAddr = 0x9800;
	
	uint8_t m_ObjectSize = 8;
	
	uint8_t m_ly = 0;
	uint8_t m_lyc = 0;

	uint8_t m_scy = 0;
	uint8_t m_scx = 0;
	uint8_t m_wy = 0;
	uint8_t m_wx = 0;
	
	uint16_t m_scanlineCounter = 0;

	bool m_objectEnable = false;
	bool m_bgAndWindowPriority = false;
	bool m_lcdEnabled = true;
	bool m_windowEnabled = true;
};
}
