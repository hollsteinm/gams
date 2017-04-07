#pragma once

#include <iosfwd>
#include <vector>
#include <streambuf>

namespace gams {
    class igpu_mem_buffer;

    class gpu_streambuf : public std::streambuf
    {
    public: 
        gpu_streambuf(igpu_mem_buffer* gpu_buffer);
        virtual ~gpu_streambuf();

        virtual std::streambuf::int_type underflow() override;
        virtual std::streambuf::int_type overflow(std::streambuf::int_type value) override;

		gpu_streambuf(const gpu_streambuf&) = delete;
		gpu_streambuf& operator=(const gpu_streambuf&) = delete;

	protected:
		virtual std::streambuf::int_type sync() override;

    private:
		igpu_mem_buffer* gpu_buffer;
    };
}
