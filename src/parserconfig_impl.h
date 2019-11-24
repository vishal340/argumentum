// Copyright (c) 2018, 2019 Marko Mahnič
// License: MPL2. See LICENSE in the root of the project.

#pragma once

#include "parserconfig.h"

#include <ostream>
#include <string>
#include <string_view>

namespace argparse {

CPPARGPARSE_INLINE const ParserConfig::Data& ParserConfig::data() const
{
   return mData;
}

CPPARGPARSE_INLINE ParserConfig& ParserConfig::program( std::string_view program )
{
   mData.program = program;
   return *this;
}

CPPARGPARSE_INLINE ParserConfig& ParserConfig::usage( std::string_view usage )
{
   mData.usage = usage;
   return *this;
}

CPPARGPARSE_INLINE ParserConfig& ParserConfig::description( std::string_view description )
{
   mData.description = description;
   return *this;
}

CPPARGPARSE_INLINE ParserConfig& ParserConfig::epilog( std::string_view epilog )
{
   mData.epilog = epilog;
   return *this;
}

CPPARGPARSE_INLINE ParserConfig& ParserConfig::cout( std::ostream& stream )
{
   mData.pOutStream = &stream;
   return *this;
}

CPPARGPARSE_INLINE ParserConfig& ParserConfig::filesystem( const std::shared_ptr<Filesystem> pFilesystem )
{
   if ( pFilesystem )
      mData.pFilesystem = pFilesystem;
   return *this;
}

}   // namespace argparse
