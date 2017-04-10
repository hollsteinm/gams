#include "gpu_streambuf.h"
#include "igpu_mem_buffer.h"
#include <iostream>
#include <algorithm>

gams::gpu_mapped_streambuf::gpu_mapped_streambuf(igpu_mem_buffer* gpu_buffer)
	: gpu_buffer(gpu_buffer)
{
	sync(true);
}

gams::gpu_streambuf::~gpu_streambuf()
{
	sync(false);
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

std::streambuf::int_type gams::gpu_streambuf::sync(bool prepare_after_commit)
{
	gpu_buffer->commit();
	if(prepare_after_commit)
	{
		if(gpu_buffer->prepare()) 
		{
			setg(gpu_buffer->get_rd_first(), gpu_buffer->get_rd_last() + 1, gpu_buffer->get_rd_last());
			setp(gpu_buffer->get_wr_first(), gpu_buffer->get_wr_last() + 1, gpu_buffer->get_wr_last());
			return std::streambuf::traits_type::not_eof(std::streambuf::traits_type::eof());
		} 
		else 
		{
			return std::streambuf::traits_type::eof(); //todo: find a better error to return
		}
	}
	else
	{
		return std::streambuf::traits_type::eof();
	}
}

std::streambuf::int_type gams::gpu_streambuf::sync()
{
	return sync(true);
}
