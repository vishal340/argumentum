// Copyright (c) 2018, 2019 Marko Mahnič
// License: MPL2. See LICENSE in the root of the project.

#pragma once

#include "filesystem.h"

#include <ostream>
#include <string>
#include <string_view>

namespace argumentum {

class ParserConfig
{
public:
   struct Data
   {
      std::string program;
      std::string usage;
      std::string description;
      std::string epilog;
      std::ostream* pOutStream = nullptr;
      std::shared_ptr<Filesystem> pFilesystem = std::make_shared<DefaultFilesystem>();
      unsigned maxIncludeDepth = 8;
   };

private:
   Data mData;

public:
   // Used internally to access the configured parameters.
   const Data& data() const;

   // Set the program name to be used in the generated help.
   ParserConfig& program( std::string_view program );

   // Set the usage string to be used in the generated help. This overrides the
   // default generated usage string.
   ParserConfig& usage( std::string_view usage );

   // Set the description of the program to be shown in the generated help.
   ParserConfig& description( std::string_view description );

   // Set the epolog to be shown at the end of the generated help.
   ParserConfig& epilog( std::string_view epilog );

   // Set the stream to which the parser will write messages.
   // NOTE: The @p stream must outlive the parser.
   ParserConfig& cout( std::ostream& stream );

   // Set the filesystem implementation that will be used to open files with
   // additional parameters parameters.  If the filesystem is not set the parser
   // will use the default filesystem implementation.
   ParserConfig& filesystem( const std::shared_ptr<Filesystem> pFilesystem );
};

}   // namespace argumentum
