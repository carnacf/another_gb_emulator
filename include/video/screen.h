#pragma once

#include <cstdint>

class Memory;

namespace video
{
constexpr uint8_t SCREEN_WIDTH = 160;
constexpr uint8_t SCREEN_HEIGHT = 140;

class Screen
{
public:

	Screen();
	~Screen();

	void tick(uint8_t count);

	void setMemory(Memory* memory);

	uint8_t* getFrameBuffer();

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
	
	uint8_t getBGPalette() const;
	void setBGPalette(uint8_t bgPalette);

private:

	void updateStatusRegister();

	void renderBG(uint8_t line);
	void renderWindow(uint8_t line);
	void renderObjects(uint8_t line);

	uint64_t getTileLine(uint16_t tileAddr, uint8_t line);
	uint16_t fromTileIdtoAdress(uint8_t tileId);
	uint8_t fromColorIdtoColor(uint8_t colorId);

private:
	Memory* m_memory;

	uint16_t m_windowTileMapAddr = 0x9800;
	uint16_t m_tileDataArea = 0x8800;
	uint16_t m_bgTileMapAddr = 0x9800;
	
	uint8_t* m_frameBuffer = nullptr;
	
	uint8_t m_ObjectSize = 8;
	
	uint8_t m_ly = 0;
	uint8_t m_lyc = 0;

	uint8_t m_scy = 0;
	uint8_t m_scx = 0;
	uint8_t m_wy = 0;
	uint8_t m_wx = 0;
	
	uint8_t m_bgPalette = 0;

	uint16_t m_scanlineCounter = 0;

	bool m_objectEnable = false;
	bool m_bgAndWindowPriority = false;
	bool m_lcdEnabled = true;
	bool m_windowEnabled = true;
};
}
