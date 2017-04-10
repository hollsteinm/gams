#pragma once

#include <iosfwd>
#include <vector>
#include <streambuf>

namespace gams {
    class igpu_mem_buffer;

    class gpu_mapped_streambuf : public std::streambuf
    {
    public: 
        gpu_mapped_streambuf(igpu_mem_buffer* gpu_buffer);
        virtual ~gpu_mapped_streambuf();

        virtual std::streambuf::int_type underflow() override;
        virtual std::streambuf::int_type overflow(std::streambuf::int_type value) override;

	gpu_mapped_streambuf(const gpu_mapped_streambuf&) = delete;
	gpu_mapped_streambuf& operator=(const gpu_mapped_streambuf&) = delete;

    protected:
        virtual std::streambuf::int_type sync() override;
	std::streambuf::int_type sync(bool prepare_after_commit);

    private:
        igpu_mem_buffer* gpu_buffer;
    };
}
