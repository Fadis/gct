#include <gct/mmaped_file.hpp>
#include <stdexcept>
#include <system_error>
#include <boost/scope_exit.hpp>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace gct {
  mmaped_file::mmaped_file( const std::filesystem::path &filename ) :
    file_size( std::filesystem::file_size( filename ) ) {
    int fd = open( filename.c_str(), O_RDONLY );
    if( fd < 0 ) {
      throw std::system_error( errno, std::generic_category(), filename.string() );
    }
    BOOST_SCOPE_EXIT( &fd ) { close( fd ); }
    BOOST_SCOPE_EXIT_END
    unsigned int map_flags = MAP_PRIVATE|MAP_POPULATE;
    constexpr unsigned int kilo_in_binary = 1024u;
    constexpr unsigned int huge_tlb_threshold = 2u * kilo_in_binary * kilo_in_binary;
    if( file_size >= huge_tlb_threshold ) map_flags |= MAP_HUGETLB;
    void *addr = mmap( nullptr, file_size, PROT_READ, map_flags, fd, 0 );
    if( !addr ) {
      throw std::system_error( errno, std::generic_category(), filename.string() );
    }
    head.reset(
      reinterpret_cast<uint8_t *>(addr),
      [file_size=file_size](uint8_t *p) {
        munmap(reinterpret_cast<void *>(p), file_size);
      }
    );
  }
  mmaped_file::reference mmaped_file::at( mmaped_file::size_type i ) const {
    if( i >= file_size ) throw std::out_of_range( "mmaped_file::at : out of range" );
    return head.get()[ i ];
  }
}

