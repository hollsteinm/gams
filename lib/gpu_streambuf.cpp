#include "gpu_streambuf.h"
#include "igpu_mem_buffer.h"
#include <iostream>
#include <algorithm>

gams::gpu_streambuf::gpu_streambuf(igpu_mem_buffer* gpu_buffer)
	: gpu_buffer(gpu_buffer)
{
	sync();
}

gams::gpu_streambuf::~gpu_streambuf()
{
	sync();
}

std::streambuf::int_type gams::gpu_streambuf::overflow(std::streambuf::int_type value)
{
	if (std::streambuf::traits_type::eq_int_type(value, std::streambuf::traits_type::eof()))
	{
		return sync();
	}

	if (pptr() > epptr()) //initial or reset state
	{
		setp(gpu_buffer->get_wr_first(), gpu_buffer->get_wr_curr(), gpu_buffer->get_wr_last());
	}

	*pptr() = std::streambuf::traits_type::to_char_type(value);
	pbump(sizeof(std::streambuf::traits_type::char_type));
	return std::streambuf::traits_type::not_eof(value);
}

std::streambuf::int_type gams::gpu_streambuf::underflow()
{
	if (gptr() > egptr()) //initial or reset state
	{
		setg(gpu_buffer->get_rd_first(), gpu_buffer->get_rd_curr(), gpu_buffer->get_rd_last());
	}

	if (gptr() == egptr())
	{
		sync();
		return std::streambuf::traits_type::eof();
	}

	return std::streambuf::traits_type::to_int_type(*gptr());
}

std::streambuf::int_type gams::gpu_streambuf::sync()
{
	gpu_buffer->commit(); //unmap the host and device memory
	setg(gpu_buffer->get_rd_first(), gpu_buffer->get_rd_last() + 1, gpu_buffer->get_rd_last());
	setp(gpu_buffer->get_wr_first(), gpu_buffer->get_wr_last() + 1, gpu_buffer->get_wr_last());
	gpu_buffer->prepare(); //remap
	return std::streambuf::traits_type::not_eof(std::streambuf::traits_type::eof());
}
