#pragma once

class Graphics;

class BufferResource
{
public:
	virtual ~BufferResource() = default;
	virtual void BindAsBuffer( Graphics& ) noexcept(!_DEBUG) = 0;
	virtual void Resize(Graphics&, unsigned int width, unsigned int height) {};
	virtual void BindAsBuffer( Graphics&,BufferResource* ) noexcept(!_DEBUG) = 0;
	virtual void Clear( Graphics& ) noexcept(!_DEBUG) = 0;
};
